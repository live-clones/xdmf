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
#include "XdmfTopology.h"
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
  this->Geometry = new XdmfGeometry;
  this->GeometryIsMine = 1;
  this->Topology = new XdmfTopology;
  this->TopologyIsMine = 1;
  this->Attribute = (XdmfAttribute **)calloc(1, sizeof( XdmfAttribute * ));
  this->Children = (XdmfGrid **)calloc(1, sizeof( XdmfGrid * ));
  this->AssignedAttribute = NULL;
  this->NumberOfAttributes = 0;
  this->GridType = XDMF_GRID_UNSET;
  this->NumberOfChildren = 0;
  }

XdmfGrid::~XdmfGrid() {
  XdmfInt32  Index;
  if( this->GeometryIsMine && this->Geometry ) delete this->Geometry;
  if( this->TopologyIsMine && this->Topology ) delete this->Topology;
  for ( Index = 0; Index < this->NumberOfAttributes; Index ++ )
    {
    delete this->Attribute[Index];
    }
  free(this->Attribute);
  }

XdmfConstString
XdmfGrid::GetGridTypeAsString(){
    if(this->GridType & XDMF_GRID_MASK){
        switch(this->GridType & XDMF_GRID_MASK) {
            case XDMF_GRID_UNIFORM :
                return("Uniform");
            case XDMF_GRID_COLLECTION :
                return("Collection");
            case XDMF_GRID_TREE :
                return("Tree");
            default :
                XdmfErrorMessage("Unknown Grid Type");
                return(0);
        }
    }else{
        return(this->Topology->GetTopologyTypeAsString());
    }
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
XdmfGrid::UpdateInformation() {

XdmfXmlNode Element;
XdmfInt32  Status = XDMF_FAIL;
XdmfConstString  attribute;


if(XdmfElement::UpdateInformation() != XDMF_SUCCESS) return(XDMF_FAIL);
if( XDMF_WORD_CMP(this->GetElementType(), "Grid") == 0){
    XdmfErrorMessage("Element type" << this->GetElementType() << " is not of type 'Grid'");
    return(XDMF_FAIL);
}

// Allow for "GridType" or "Type"
attribute = this->Get("GridType");
if(!attribute) attribute = this->Get("Type");
if( XDMF_WORD_CMP(attribute, "Collection") ){
    this->GridType = XDMF_GRID_COLLECTION;
}else if( XDMF_WORD_CMP(attribute, "Tree") ){
    this->GridType = XDMF_GRID_TREE;
}else if( XDMF_WORD_CMP(attribute, "Uniform") ){
    this->GridType = XDMF_GRID_UNIFORM;
}else{
    if(attribute){
        XdmfErrorMessage("Unknown Grid Type " << attribute);
        return(XDMF_FAIL);
    }
    // If Type is NULL use default
    this->GridType = XDMF_GRID_UNIFORM;
}
if( this->GridType |= XDMF_GRID_UNIFORM ){
    // Tree or Collection
    XdmfInt32  i, nchild;
    XdmfXmlNode node;

    if (this->Children){
        for(i=0 ; i < nchild ; i++){
            delete this->Children[i];
        }
    }
    nchild = this->DOM->FindNumberOfElements("Grid", this->Element);
    this->NumberOfChildren = nchild;
    this->Children = (XdmfGrid **)realloc(this->Children, nchild * sizeof(XdmfGrid *));
    for(i=0 ; i < nchild ; i++){
        node = this->DOM->FindElement("Grid", i, this->Element);
        if(!node) {
            XdmfErrorMessage("Can't find Child Grid #" << i);
            return(XDMF_FAIL);
        }
        this->Children[i] = new XdmfGrid;
        if(this->Children[i]->SetDOM(this->DOM) == XDMF_FAIL) return(XDMF_FAIL);
        if(this->Children[i]->SetElement(node) == XDMF_FAIL) return(XDMF_FAIL);
        if(this->Children[i]->UpdateInformation() == XDMF_FAIL) return(XDMF_FAIL);
    }
    return(XDMF_SUCCESS);
}
// Handle Uniform Grid
Element = this->DOM->FindElement("Topology", 0, this->Element);
if(Element){
    if(this->Topology->SetDOM( this->DOM ) == XDMF_FAIL) return(XDMF_FAIL);
    if(this->Topology->SetElement(Element) == XDMF_FAIL) return(XDMF_FAIL);
    Status = this->Topology->UpdateInformation();
    if( Status == XDMF_FAIL ){
        XdmfErrorMessage("Error Reading Topology");
        return( XDMF_FAIL );
    }
}
Element = this->DOM->FindElement("Geometry", 0, this->Element);
if(Element){
    if(this->Geometry->SetDOM( this->DOM ) == XDMF_FAIL) return(XDMF_FAIL);
    if(this->Geometry->SetElement(Element) == XDMF_FAIL) return(XDMF_FAIL);
    Status = this->Geometry->UpdateInformation();
    if( Status == XDMF_FAIL ){
        XdmfErrorMessage("Error Reading Geometry");
        return( XDMF_FAIL );
    }
}
if(!this->Name) this->SetName( GetUnique("Grid_" ) );
XdmfInt32 OldNumberOfAttributes = this->NumberOfAttributes;
this->NumberOfAttributes = this->DOM->FindNumberOfElements("Attribute", this->Element );
if( this->NumberOfAttributes > 0 ){
  XdmfInt32  Index;
  XdmfAttribute  *iattribute;
  XdmfXmlNode    AttributeElement;

  for ( Index = 0; Index < OldNumberOfAttributes; Index ++ )
    {
    delete this->Attribute[Index];
    }
  this->Attribute = ( XdmfAttribute **)realloc( this->Attribute,
      this->NumberOfAttributes * sizeof( XdmfAttribute * ));
  for( Index = 0 ; Index < this->NumberOfAttributes ; Index++ ){
    iattribute = new XdmfAttribute;

    this->Attribute[Index] = iattribute;
    AttributeElement = this->DOM->FindElement( "Attribute", Index, Element );
    iattribute->SetDOM( this->DOM );    
    iattribute->SetElement( AttributeElement );
    iattribute->UpdateInformation();
    }
}
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfGrid::Update() {
XdmfInt32  Status = XDMF_FAIL;
XdmfConstString  attribute;

if(XdmfElement::Update() != XDMF_SUCCESS) return(XDMF_FAIL);
if(this->GridType == XDMF_GRID_UNSET) {
    if(this->UpdateInformation() == XDMF_FAIL){
        XdmfErrorMessage("Error Initializing Grid");
        return(XDMF_FAIL);
    }
}
if(this->GridType != XDMF_GRID_UNIFORM){
    XdmfInt32   i;
    // Tree or Collection
    for(i=0; i < this->NumberOfChildren ; i++){
        if(this->Children[i]->Update() == XDMF_FAIL){
            XdmfErrorMessage("Error in Update() of Child Grid " << i);
            return(XDMF_FAIL);
        }
    }
    return(XDMF_SUCCESS);
}
if(this->Topology->Update() == XDMF_FAIL){
    XdmfErrorMessage("Error in Update() of Topology");
    return(XDMF_FAIL);
}
if(this->Geometry->Update() == XDMF_FAIL){
    XdmfErrorMessage("Error in Update() of Geometry");
    return(XDMF_FAIL);
}
return( XDMF_SUCCESS );
}

XdmfGrid *
XdmfGrid::GetChild(XdmfInt32 Index){
    if(this->GridType & XDMF_GRID_MASK){
        if(Index < this->NumberOfChildren){
            return(this->Children[Index]);
        }else{
            XdmfErrorMessage("Grid has " << this->NumberOfChildren << " children. Index " << Index << " is out of range");
        }
    }else{
        XdmfErrorMessage("Grid is Uniform so it has no children");
    }
return(NULL);
}

XdmfInt32
XdmfGrid::IsUniform(){
    if(this->GridType & XDMF_GRID_MASK) return(XDMF_FALSE);
    return(XDMF_TRUE);
}
