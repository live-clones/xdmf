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
#include "XdmfGrid.h"

#include "XdmfDOM.h"
#include "XdmfGeometry.h"
#include "XdmfAttribute.h"

XdmfGrid *HandleToXdmfGrid( XdmfString Source ){
  XdmfObject  *TempObj;
  XdmfGrid   *Grid;

  TempObj = HandleToXdmfObject( Source );
  Grid = (XdmfGrid *)TempObj;
//  XdmfErrorMessage("Pointer = " << Source);
//  XdmfErrorMessage("Grid = " << Grid );
//  XdmfErrorMessage("Name " << Grid->GetClassName() );
//  XdmfErrorMessage("Type " << Grid->GetTopologyTypeAsString() );
  return( Grid );
  }

XdmfGrid::XdmfGrid() {
  this->BaseGrid = NULL;
  this->Geometry = new XdmfGeometry;
  this->GeometryIsMine = 1;
  this->BaseGridIsMine = 1;
  this->Attribute = (XdmfAttribute **)calloc(1, sizeof( XdmfAttribute * ));
  this->AssignedAttribute = NULL;
  this->NumberOfAttributes = 0;
  }

XdmfGrid::~XdmfGrid() {
  XdmfInt32  Index;
  if( this->GeometryIsMine && this->Geometry ) delete this->Geometry;
  for ( Index = 0; Index < this->NumberOfAttributes; Index ++ )
    {
    delete this->Attribute[Index];
    }
  free(this->Attribute);
  }

XdmfInt32
XdmfGrid::AssignAttribute( XdmfAttribute *attribute ){
XdmfInt32 Status = 0;

if( attribute ){
  attribute->Update();
  // Status = attribute->SetBaseAttribute( this, this->BaseGrid );
  this->AssignedAttribute = attribute;
} else {
  XdmfErrorMessage("Attribute is NULL");
  return( XDMF_FAIL );
}
return( Status );
}

XdmfInt32
XdmfGrid::AssignAttribute( XdmfInt64 Index ){
XdmfInt32 Status;

Status = this->AssignAttribute( this->Attribute[ Index ] );
return( Status );
}

XdmfInt32
XdmfGrid::AssignAttributeByIndex( XdmfInt64 Index ){
XdmfInt32 Status;

Status = this->AssignAttribute( this->Attribute[ Index ] );
return( Status );
}

XdmfInt32
XdmfGrid::AssignAttributeByName( XdmfString name ){
XdmfInt64 i;
XdmfInt32 Status = XDMF_FAIL;

for( i = 0 ; i < this->NumberOfAttributes ; i++ ){
  if( XDMF_WORD_CMP( this->Attribute[i]->GetName(), name ) ){
    Status = this->AssignAttribute( this->Attribute[ i ] );
    break;
  }
}
return( Status );
}

XdmfInt64
XdmfGrid::GetAssignedAttributeIndex( void ){
XdmfInt64 i;

for( i = 0 ; i < this->NumberOfAttributes ; i++ ){
  if( this->AssignedAttribute ==  this->Attribute[ i ] ){
    return( i );
    }
  }
return(0);
}

XdmfInt32
XdmfGrid::InitGridFromElement( XdmfXNode *Element ) {

XdmfInt32  Status = XDMF_FAIL;
XdmfConstString  attribute;

attribute = this->DOM->Get( Element, "NodeType");
if( XDMF_WORD_CMP( attribute, "Grid" ) == 0 ){
  Element = this->DOM->FindElement("Grid", 0, Element );
  attribute = this->DOM->Get( Element, "NodeType");
  if( XDMF_WORD_CMP( attribute, "Grid" ) == 0 ){
    XdmfErrorMessage("Can't Find Grid Element");
    return( XDMF_FAIL );
    }
  }
Status = this->InitTopologyFromElement( Element );
if( Status == XDMF_FAIL ){
  XdmfErrorMessage("Error Reading Topology");
  return( XDMF_FAIL );
  }
this->Geometry->SetDOM( this->DOM );
this->Geometry->InitGeometryFromElement( Element );
if( Status == XDMF_FAIL ){
  XdmfErrorMessage("Error Reading Geometry");
  return( XDMF_FAIL );
  }
attribute = this->DOM->Get( Element, "Name" );
if( attribute ) {
  this->SetName( attribute );
} else {
  this->SetName( GetUnique("Grid_" ) );
}
XdmfInt32 OldNumberOfAttributes = this->NumberOfAttributes;
this->NumberOfAttributes = this->DOM->FindNumberOfElements("Attribute", Element );
if( this->NumberOfAttributes > 0 ){
  XdmfInt32  Index;
  XdmfAttribute  *iattribute;
  XdmfXNode    *AttributeElement;

  for ( Index = 0; Index < OldNumberOfAttributes; Index ++ )
    {
    delete this->Attribute[Index];
    }
  this->Attribute = ( XdmfAttribute **)realloc( this->Attribute,
      this->NumberOfAttributes * sizeof( XdmfAttribute * ));
  for( Index = 0 ; Index < this->NumberOfAttributes ; Index++ ){
    iattribute = new XdmfAttribute;

    this->Attribute[Index] = iattribute;
    iattribute->SetDOM( this->DOM );    
    AttributeElement = this->DOM->FindElement( "Attribute", Index, Element );
    iattribute->InitAttributeFromElement( AttributeElement );
    }
}
this->CurrentElement = Element;
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfGrid::SetGridFromElement( XdmfXNode *Element ) {

XdmfInt32  Status = XDMF_FAIL;
XdmfConstString  attribute;

attribute = this->DOM->Get( Element, "NodeType");
if( XDMF_WORD_CMP( attribute, "Grid" ) == 0 ){
  Element = this->DOM->FindElement("Grid", 0, Element );
  attribute = this->DOM->Get( Element, "NodeType");
  if( XDMF_WORD_CMP( attribute, "Grid" ) == 0 ){
    XdmfErrorMessage("Can't Find Grid Element");
    return( XDMF_FAIL );
    }
  }
XdmfDebug("Setting Topology");
if( this->TopologyType == XDMF_NOTOPOLOGY ){
  Status = this->InitGridFromElement( Element );
  }
Status = this->SetTopologyFromElement( Element );
if( Status == XDMF_FAIL ){
  XdmfErrorMessage("Error Reading Topology");
  return( XDMF_FAIL );
  }
XdmfDebug("Setting Geometry");
this->Geometry->SetDOM( this->DOM );
this->Geometry->SetGeometryFromElement( Element );
if( Status == XDMF_FAIL ){
  XdmfErrorMessage("Error Reading Geometry");
  return( XDMF_FAIL );
  }
/*
XdmfDebug("Setting Base Grid");
Status = this->SetBaseGrid();
if( Status == XDMF_FAIL ){
  XdmfErrorMessage("Error Setting Base Grid");
  return( XDMF_FAIL );
  }
*/
/*
XdmfDebug("Setting Attributes");
if( this->NumberOfAttributes > 0 ){
  XdmfInt32  Index;
  XdmfAttribute  *attribute;
  XdmfXNode    *AttributeElement;

  for( Index = 0 ; Index < this->NumberOfAttributes ; Index++ ){
    XdmfDebug("Setting Attribute #" << Index + 1);
    AttributeElement = this->DOM->FindElement( "Attribute", Index, Element );
    attribute = this->Attribute[ Index ];
    attribute->SetDOM( this->DOM );    
    attribute->SetAttributeFromElement( AttributeElement );
    }
}
*/
this->CurrentElement = Element;
return( XDMF_SUCCESS );
}



XdmfInt32
XdmfGrid::Update( void ){

XdmfInt32 Status;

if( ( this->DOM == NULL )  || ( this->CurrentElement == NULL ) ){
  XdmfErrorMessage("No Current Element Set for this DOM");
  return( XDMF_FAIL );
  }
Status = XdmfTopology::Update();
if( Status == XDMF_FAIL ){
  XdmfErrorMessage("Error Updating Topology");
  return( XDMF_FAIL );
  }
Status = this->Geometry->Update();
if( Status == XDMF_FAIL ){
  XdmfErrorMessage("Error Updating Geometry");
  return( XDMF_FAIL );
  }
/*
Status = this->SetBaseGrid( this->BaseGrid );
if( Status == XDMF_FAIL ){
  XdmfErrorMessage("Error Settgin Base Grid");
  return( XDMF_FAIL );
  }
*/
return( XDMF_SUCCESS );
}
