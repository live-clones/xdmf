/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Jerry A. Clarke                                             */
/*     clarke@arl.army.mil                                         */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2002 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#include "XdmfFormat.h"

#include "XdmfDataDesc.h"
#include "XdmfArray.h"
#include "XdmfXNode.h"
#include "XdmfDOM.h"

XdmfFormat::XdmfFormat() {
  strcpy( this->DataFormat, "XML" );
  this->LocalDataDesc = 0;
}

XdmfFormat::~XdmfFormat() {
  if ( this->LocalDataDesc )
    {
    cout << "Delete: " << this->LocalDataDesc << endl;
    delete this->LocalDataDesc;
    this->LocalDataDesc = 0;
    }
}

XdmfXNode *
XdmfFormat::DataDescToElement( XdmfDataDesc *Desc, XdmfXNode *Element ) {

XdmfInt32  j, i, Rank;
XdmfConstString  Type;
char    Attribute[ XDMF_MAX_STRING_LENGTH  ];
XdmfInt64  Dimensions[ XDMF_MAX_DIMENSION ];
ostrstream  Value( Attribute, XDMF_MAX_STRING_LENGTH );

Attribute[0] = '\0';
if( Element == NULL ) {
  Element = new XdmfXNode;
  }
if( Desc == NULL ) {
  XdmfErrorMessage("Data Desc is NULL");
  return( NULL );
  }
Element->Set("NodeType", "DataStructure");
Element->Set("Format", this->DataFormat );
Value.seekp(0);
Type = XdmfTypeToClassString( Desc->GetNumberType() );
Element->Set("DataType", Type );
Value << ICE_64BIT_CAST(Desc->GetElementSize()) << ends;
Value.seekp(0);
Element->Set("Precision", Attribute );
Rank =  Desc->GetRank();
Value << ICE_64BIT_CAST(Rank) << ends;
Element->Set("Rank", Attribute );
Value.seekp(0);
Value << ICE_64BIT_CAST(Desc->GetDimension( 0 ));
for( i = 1 ; i < Rank ; i++ ){
  Value << " " << ICE_64BIT_CAST(Desc->GetDimension( i ));
  }
Value << ends;
Element->Set("Dimensions", Attribute );
if( Desc->GetNumberType() == XDMF_COMPOUND_TYPE ){
  XdmfXNode    *Child;
  XdmfInt64  ii, NumberOfMembers;
  
  if( this->DOM->IsChild( Element ) == XDMF_FAIL ) {
    XdmfErrorMessage("Array is COMPOUND but Element can't have Child Element");
    XdmfErrorMessage("Element is Needs to be Inserted Into DOM");
    return( NULL );
    }
  NumberOfMembers = Desc->GetNumberOfMembers();
  for( ii = 0 ; ii < NumberOfMembers ; ii++ ){
    Child = new XdmfXNode;
    Child->Set("NodeType", "StructureMember");
    Value.seekp(0);
    Type = XdmfTypeToClassString( Desc->GetMemberType(ii) );
    Child->Set("DataType", Type );
    Value << ICE_64BIT_CAST(Desc->GetMemberSize(ii) /  Desc->GetMemberLength(ii)) << ends;
    Value.seekp(0);
    Child->Set("Precision", Attribute );
    Rank =  Desc->GetMemberShape( ii, Dimensions );
    Value << Rank << ends;
    Child->Set("Rank", Attribute );
    Value.seekp(0);
    Value << " " << ICE_64BIT_CAST(Dimensions[0]);
    for( j = 1 ; j < Rank ; j++ ){
      Value << " " << ICE_64BIT_CAST(Dimensions[j]);
      }
    Value << ends;
    Child->Set("Dimensions", Attribute );
    this->DOM->Insert( Element, Child );
    }
  }
return( Element );
}

XdmfDataDesc *
XdmfFormat::ElementToCompoundDataDesc( XdmfXNode *Element ) {

XdmfDataDesc  *Desc;
//XdmfString  Attribute;
XdmfInt32  i, NumberOfMembers, Member, Rank = 1;
XdmfInt64  TotalSize, Dimensions[ XDMF_MAX_DIMENSION ];
if( !this->DOM ){
  XdmfErrorMessage("Object has no DOM");
  return( NULL );
  }
if( !Element ) {
  XdmfErrorMessage("Element is NULL");
  return( NULL );
  }
XdmfDebug("Building Compound Desc");
Desc = new XdmfDataDesc;
NumberOfMembers = Member = this->DOM->FindNumberOfElements( "StructureMember", Element );
if( Member <= 1 ){
  XdmfErrorMessage("Element has no Children Members");
  return( NULL );
  }
XdmfDebug("Structure has " << NumberOfMembers << " Members");
// First Find The Total Size
TotalSize = 0;
for( i = 0 ; i < NumberOfMembers ; i++ ){
  XdmfXNode    *Child;
  XdmfDataDesc  *ChildDesc;

  Child = this->DOM->FindElement( "StructureMember", i, Element );
  if( Child != Element ){
    XdmfDebug("Checking Size for Node Named " << this->DOM->Get( Child, "Name") );
    ChildDesc = this->ElementToDataDesc( Child );
    TotalSize += ChildDesc->GetElementSize() * ChildDesc->GetNumberOfElements();
    XdmfDebug("Running Total = " << ICE_64BIT_CAST(TotalSize) );
    delete ChildDesc;
    }
  }
XdmfDebug("Total = " << ICE_64BIT_CAST(TotalSize) );
Desc->SetNumberType( XDMF_COMPOUND_TYPE, TotalSize );
// Now Fill in the Members
for( i = 0 ; i < NumberOfMembers ; i++ ){
  XdmfConstString  name;
  XdmfXNode    *Child;
  XdmfDataDesc  *ChildDesc;

  Child = this->DOM->FindElement( "StructureMember", i, Element );
  if( Child != Element ){
    XdmfDebug("Adding Member for Node Named " << this->DOM->Get( Child, "Name") );
    ChildDesc = this->ElementToDataDesc( Child );
    Rank = ChildDesc->GetShape( Dimensions );
    name = this->DOM->Get( Child, "Name" );
    if( !name ) {
      name = GetUnique("XdmfMember");
      }
    XdmfString sname = 0;
    XDMF_STRING_DUPLICATE(sname, name);
    Desc->AddCompoundMember( sname,
        ChildDesc->GetNumberType(),
        Rank,
        Dimensions );
    delete [] sname;
    delete ChildDesc;
    }
  }
return( Desc );
}

XdmfDataDesc *
XdmfFormat::ElementToDataDesc( XdmfXNode *Element, int store /* = 1 */ ) {

XdmfDataDesc  *Desc;
XdmfConstString  Attribute;
XdmfInt32  Rank = 1, NumberType;
XdmfInt64  Dimensions[ XDMF_MAX_DIMENSION ];

if( !this->DOM ){
  XdmfErrorMessage("Object has no DOM");
  return( NULL );
  }
if( !Element ) {
  XdmfErrorMessage("Element is NULL");
  return( NULL );
  }
Attribute = this->DOM->Get( Element, "NodeType" );
if( ( XDMF_WORD_CMP(Attribute, "DataStructure") == 0 ) &&
  ( XDMF_WORD_CMP(Attribute, "StructureMember") == 0 ) ){
  Element = this->DOM->FindElement( "DataStructure", 0, Element );
  if( Element == NULL ) {
    XdmfErrorMessage("No DataStructure Element Found");
    return( NULL );
    }
  }
// XdmfDebug("Building DataDesc for Node Named " << this->DOM->Get( Element, "Name") );
// This Needs to do More For Compound Types
NumberType = this->DOM->GetNumberType( Element );
if( NumberType == XDMF_COMPOUND_TYPE ){
  XdmfDebug("NumberType is Compound");
  Desc = this->ElementToCompoundDataDesc( Element );
} else {
  XdmfDebug("NumberType is Simple");
  Desc = new XdmfDataDesc;
  Desc->SetNumberType( NumberType );
  if ( store )
    {
    if ( this->LocalDataDesc )
      {
      delete this->LocalDataDesc;
      }
    this->LocalDataDesc = Desc;
    }
  }
//
Attribute = this->DOM->Get(Element, "Rank");
if( Attribute ){
  Rank = strtol( Attribute, (XdmfString *)NULL, 0 );
  }
Attribute = this->DOM->Get(Element, "Dimensions");
if( Attribute ){
/*
  XdmfArray  *Array = new XdmfArray( XDMF_INT64_TYPE );

  XdmfDebug("Setting Shape with Rank = " << Rank );
  Array->SetValues( 0, Attribute );

  for( i = 0 ; i < Rank ; i++ ){
    Dimensions[i] = Array->GetValueAsInt64( i );
    XdmfDebug("Dimension[" << i << "] = " << Dimensions[i] );
    }
  delete Array;
*/
  Desc->SetShapeFromString( Attribute );
} else {
  XdmfConstString  Values;
  XdmfArray  *Array = new XdmfArray( XDMF_FLOAT32_TYPE );

  Values = this->DOM->Get(Element, "CData");
  Array->SetValues( 0, Values );
  Rank = 1;
  Dimensions[0] = Array->GetNumberOfElements();
  XdmfDebug("Setting Shape to Linear Length = " << ICE_64BIT_CAST(Dimensions[0]) );
  delete Array;
  Desc->SetShape( Rank, Dimensions );
}

return( Desc );
}
