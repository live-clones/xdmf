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

XdmfGrid *HandleToXdmfGrid( char *Source ){
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
  }

XdmfGrid::~XdmfGrid() {
  if( this->GeometryIsMine && this->Geometry ) delete this->Geometry;
  }

XdmfInt32
XdmfGrid::AssignAttribute( XdmfAttribute *Attribute ){
XdmfInt32 Status;

if( Attribute ){
  Attribute->Update();
  // Status = Attribute->SetBaseAttribute( this, this->BaseGrid );
  this->AssignedAttribute = Attribute;
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
XdmfGrid::AssignAttributeByName( XdmfString Name ){
XdmfInt64 i;
XdmfInt32 Status = XDMF_FAIL;

for( i = 0 ; i < this->NumberOfAttributes ; i++ ){
  if( XDMF_WORD_CMP( this->Attribute[i]->GetName(), Name ) ){
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
XdmfString  Attribute;

Attribute = this->DOM->Get( Element, "NodeType");
if( XDMF_WORD_CMP( Attribute, "Grid" ) == 0 ){
  Element = this->DOM->FindElement("Grid", 0, Element );
  Attribute = this->DOM->Get( Element, "NodeType");
  if( XDMF_WORD_CMP( Attribute, "Grid" ) == 0 ){
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
Attribute = this->DOM->Get( Element, "Name" );
if( Attribute ) {
  this->SetName( Attribute );
} else {
  this->SetName( GetUnique("Grid_" ) );
}
this->NumberOfAttributes = this->DOM->FindNumberOfElements("Attribute", Element );
if( this->NumberOfAttributes > 0 ){
  XdmfInt32  Index;
  XdmfAttribute  *Attribute;
  XdmfXNode    *AttributeElement;

  this->Attribute = ( XdmfAttribute **)realloc( this->Attribute,
      this->NumberOfAttributes * sizeof( XdmfAttribute * ));
  for( Index = 0 ; Index < this->NumberOfAttributes ; Index++ ){
    Attribute = new XdmfAttribute;

    this->Attribute[Index] = Attribute;
    Attribute->SetDOM( this->DOM );    
    AttributeElement = this->DOM->FindElement( "Attribute", Index, Element );
    Attribute->InitAttributeFromElement( AttributeElement );
    }
}
this->CurrentElement = Element;
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfGrid::SetGridFromElement( XdmfXNode *Element ) {

XdmfInt32  Status = XDMF_FAIL;
XdmfString  Attribute;

Attribute = this->DOM->Get( Element, "NodeType");
if( XDMF_WORD_CMP( Attribute, "Grid" ) == 0 ){
  Element = this->DOM->FindElement("Grid", 0, Element );
  Attribute = this->DOM->Get( Element, "NodeType");
  if( XDMF_WORD_CMP( Attribute, "Grid" ) == 0 ){
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
XdmfDebug("Setting Base Grid");
/*
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
  XdmfAttribute  *Attribute;
  XdmfXNode    *AttributeElement;

  for( Index = 0 ; Index < this->NumberOfAttributes ; Index++ ){
    XdmfDebug("Setting Attribute #" << Index + 1);
    AttributeElement = this->DOM->FindElement( "Attribute", Index, Element );
    Attribute = this->Attribute[ Index ];
    Attribute->SetDOM( this->DOM );    
    Attribute->SetAttributeFromElement( AttributeElement );
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
