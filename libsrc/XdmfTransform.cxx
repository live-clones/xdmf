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
#include "XdmfTransform.h"

#include "XdmfFormatMulti.h"
#include "XdmfExpression.h"
#include "XdmfArray.h"
#include "XdmfXNode.h"
#include "XdmfDOM.h"

XdmfTransform::XdmfTransform() {
  strcpy( this->DataTransform, "XML" );
}

XdmfTransform::~XdmfTransform() {
}

XdmfXNode *
XdmfTransform::ArrayToElement( XdmfArray *Array,
      XdmfString HeavyDataName,
      XdmfString HeavyArrayName,
      XdmfXNode *Element ) {

Element = this->DataDescToElement( Array, HeavyDataName, Element );
if( Element ){
  XdmfFormatMulti  Formatter;
  XdmfXNode    *ArrayElement = new XdmfXNode;

  Formatter.SetFormat( this->DataTransform );
  ArrayElement->Set( "NodeType", "DataStructure");
  this->DOM->Insert( Element, ArrayElement );
  Formatter.SetDOM( this->DOM );
  Formatter.ArrayToElement( Array, HeavyArrayName, ArrayElement );
  }
return( Element );
}

XdmfXNode *
XdmfTransform::DataDescToElement( XdmfDataDesc *Desc,
      XdmfString HeavyDataName,  XdmfXNode *Element ) {

XdmfInt32  Rank, SelectionType;
char    Attribute[ XDMF_MAX_STRING_LENGTH  ];
XdmfInt64  *Coordinates, Dimensions[ XDMF_MAX_DIMENSION ];
XdmfFormatMulti  Formatter;
XdmfArray  Selection;
XdmfXNode    *SelectionElement;
ostrstream  Value( Attribute, XDMF_MAX_STRING_LENGTH );

Attribute[0] = '\0';
if( Element == NULL ) {
  Element = new XdmfXNode;
  }
if( Desc == NULL ) {
  XdmfErrorMessage("Data Desc is NULL");
  return( NULL );
  }
Element->Set("NodeType", "DataTransform");
SelectionType = Desc->GetSelectionType();
Selection.SetNumberType( XDMF_INT64_TYPE );
switch (SelectionType) {
  case XDMF_HYPERSLAB :
    // Rank is 2 :  3 (Start, Stride, Count ) x Rank of 
    //  Underlying Data
    Element->Set("Type", "HyperSlab");
    Rank = 2;
    Dimensions[0] = 3;
    Dimensions[1] = Desc->GetRank();
    Selection.SetShape( Rank, Dimensions );
    Selection.SetValues( 0, Desc->GetHyperSlabAsString() );
    break;
  case XDMF_COORDINATES :
    // Rank is 2 : Number of Selected Elements x Rank of
    //    Undelying Data
    Element->Set("Type", "Coordinates");
    Rank = 2;
    Dimensions[0] = Desc->GetSelectionSize();
    Dimensions[1] = Desc->GetRank();
    Selection.SetShape( Rank, Dimensions );
    Coordinates = Desc->GetCoordinates();
    Selection.SetValues( 0, Coordinates, Dimensions[0] * Dimensions[1] );
    delete Coordinates;
    break;
  default :
    Element->Set("Type", "All");
    Rank = Desc->GetShape( Dimensions );
    return( Element );
    break;
  }
Formatter.SetDOM( this->DOM );
Formatter.SetFormat( this->DataTransform );
SelectionElement = Formatter.ArrayToElement( &Selection, HeavyDataName  );
if( this->DOM->IsChild( Element ) == XDMF_FAIL ) {
  XdmfXNode  *Root;
  Root = this->DOM->FindElement("Xdmf");
  if( !Root ){
    XdmfErrorMessage("Can't Find Root Node for Insert");
    return( NULL );
    }
  this->DOM->Insert( Root , Element );
  }
this->DOM->Insert( Element, SelectionElement );
return( Element );
}

XdmfArray *
XdmfTransform::ElementToArray( XdmfXNode *Element ) {

XdmfString  Attribute;
XdmfDataDesc  *Desc;
XdmfFormatMulti  Formatter;
XdmfXNode    *ArrayElement;
XdmfArray  *Array;

Attribute = this->DOM->Get( Element, "NodeType" );
XdmfDebug("NodeType = " << Attribute );
if( XDMF_WORD_CMP( Attribute, "DataStructure" ) ){
  XdmfDebug("Getting Data Directly From DataStructure");
  Formatter.SetDOM( this->DOM );
  Formatter.SetFormat( this->DataTransform );
  Array = Formatter.ElementToArray( Element );
  return( Array );
}
Attribute = this->DOM->Get( Element, "Type" );
XdmfDebug("Type = " << Attribute );
if( XDMF_WORD_CMP( Attribute, "Function" ) ){
  XdmfString  CData;
  XdmfArray  *ReturnArray, *ArrayToDelete[100];
  ostrstream  Function;
  XdmfInt32  Id, NTmp = 0;;
  char    c, *StreamString;

  CData = this->DOM->Get( Element, "Function" );
  XdmfDebug("Transform is Function = " << CData);
  while( (c = *CData++) ) {
    if( c == '$' ) {
      XdmfXNode  *Argument;
      XdmfArray  *TmpArray;
      XdmfTransform  TmpTransform;
      istrstream  CDataStream(CData);
      CDataStream >> Id;
      while( (c = *CData++) ) {
        if( c > ' ') break;
        }
      Argument = this->DOM->FindElement( NULL, Id, Element );
      TmpTransform.SetDOM( this->DOM );
      TmpArray = TmpTransform.ElementToArray( Argument );
      if( TmpArray->GetNumberOfElements() == 1 ){
        XdmfDebug("Using Scalar = " << TmpArray->GetValueAsFloat64( 0 ) );
        Function << " " << TmpArray->GetValueAsFloat64( 0 ) << " ";
        delete TmpArray;
      } else {
        ArrayToDelete[ NTmp++ ] = TmpArray;
        Function << " " << TmpArray->GetTagName() << " ";
      }
    } else {
      Function << c;
      }
    }
  Function << '\0';
  StreamString = Function.str();
  XdmfDebug("Transform Function = " << StreamString );
  ReturnArray = XdmfExpr(  StreamString );
 // Reset Dimensions if Necessary
  CData = this->DOM->Get( Element, "Dimensions" );
  if(CData && ReturnArray){
    ReturnArray->ReformFromString(CData);
   }

  delete StreamString;
  while( NTmp ){
    NTmp--;
    XdmfDebug("Deleteing TmpArray " << ArrayToDelete[ NTmp ]->GetTagName() );
    delete ArrayToDelete[ NTmp ];
    }
  return( ReturnArray );
}

Desc = this->ElementToDataDesc( Element );
if( Desc ){
  // Find the Second Element .. which contains the data
  Attribute = this->DOM->Get( Element, "NodeType");
  if( XDMF_WORD_CMP( Attribute, "DataTransform" ) == 0 ){
    Element = this->DOM->FindElement( "DataTransform", 0, Element );
  }
  ArrayElement = this->DOM->FindElement( NULL, 1, Element );
  Formatter.SetDOM( this->DOM );
  Formatter.SetFormat( this->DataTransform );
  Array = Formatter.ElementToArray( ArrayElement, Desc );
  delete Desc;
  return( Array );
  }
return( NULL );
}

XdmfDataDesc *
XdmfTransform::ElementToDataDesc( XdmfXNode *Element ) {

XdmfDataDesc  *Desc;
XdmfArray  *Selection;
XdmfXNode    *Child;
XdmfString  Attribute;
XdmfInt32  Rank = 1;
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
if( XDMF_WORD_CMP(Attribute, "DataTransform") == 0 ){
  Element = this->DOM->FindElement( "DataTransform", 0, Element );
  if( Element == NULL ) {
    XdmfErrorMessage("No DataTransformElement Found");
    return( NULL );
    }
  }
XdmfDebug("Building DataDesc for Node Named " << this->DOM->Get( Element, "Name") );
// Get Desc for Child
Child = this->DOM->FindElement( NULL , 0 , Element );
Attribute = this->DOM->Get( Child, "NodeType" );
XdmfDebug("1st Child Node Type = " << Attribute );
if( XDMF_WORD_CMP( Attribute, "DataTransform") ) {
  // Recursive Transform
  Desc = this->ElementToDataDesc( Child );
} else {
  // This is the Data
  XdmfFormatMulti    Formatter;

  Formatter.SetDOM( this->DOM );
  Selection = Formatter.ElementToArray( Child );
  XdmfDebug("Selection Values = " << Selection->GetValues() );
  Child = this->DOM->FindElement( NULL , 1 , Element );
  Attribute = this->DOM->Get( Child, "NodeType" );
  XdmfDebug("2nd Child Node Type = " << Attribute );
  if( XDMF_WORD_CMP( Attribute, "DataTransform") ) {
    Desc = this->ElementToDataDesc( Child );
  } else {
    Desc = Formatter.ElementToDataDesc( Child );
    }
  if( !Desc ){
    XdmfErrorMessage("Error Getting Taget Desc");
    return(NULL);
    }
  Attribute = this->DOM->Get( Element, "Type" );
  if( Attribute == NULL ){
    Attribute = "HyperSlab";
    }
  if( XDMF_WORD_CMP( Attribute, "HyperSlab" ) ){
    XdmfInt64  Start[ XDMF_MAX_DIMENSION ];
    XdmfInt64  Stride[ XDMF_MAX_DIMENSION ];
    XdmfInt64  Count[ XDMF_MAX_DIMENSION ];
    XdmfInt32  Index;

    XdmfDebug("Getting HyperSlab of Desc Rank = " << Desc->GetRank() );
    Rank = Desc->GetShape( Dimensions );
    Index = 0;
    Selection->GetValues(Index, Start, Rank);
    Index += Rank;
    Selection->GetValues(Index, Stride, Rank);
    Index += Rank;
    Selection->GetValues(Index, Count, Rank);
    Desc->SelectHyperSlab( Start, Stride, Count );
    }
  if( XDMF_WORD_CMP( Attribute, "Coordinates" ) ){
    XdmfInt64  NCoordinates, *Coordinates;
    XdmfDebug("Getting Coordinates of Desc Rank = " << Desc->GetRank() );
    NCoordinates = Selection->GetNumberOfElements();
    Coordinates = new XdmfInt64[ NCoordinates ];
    Selection->GetValues( 0, Coordinates, NCoordinates );
    NCoordinates /= Desc->GetRank();
    Desc->SelectCoordinates( NCoordinates, Coordinates );
    delete Coordinates;
    }
  delete Selection;
}
return( Desc );
}
