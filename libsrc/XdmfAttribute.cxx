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
#include "XdmfAttribute.h"
#include "XdmfTransform.h"
#include "XdmfFormatMulti.h"

#include <strstream>

XdmfAttribute::XdmfAttribute() {
  this->AttributeType = XDMF_ATTRIBUTE_TYPE_NONE;
  this->ValuesAreMine = 1;
  this->Values = NULL;
  }

XdmfAttribute::~XdmfAttribute() {
  if( this->ValuesAreMine && this->Values )  delete this->Values;
  }

XdmfString
XdmfAttribute::GetAttributeTypeAsString( void ){
  switch ( this->AttributeType ){
    case XDMF_ATTRIBUTE_TYPE_SCALAR :
      return("Scalar");
    case XDMF_ATTRIBUTE_TYPE_VECTOR :
      return("Vector");
    case XDMF_ATTRIBUTE_TYPE_TENSOR :
      return("Tensor");
    case XDMF_ATTRIBUTE_TYPE_MATRIX :
      return("Matrix");
    default :
      break;
    }
return("None");
}

XdmfInt32
XdmfAttribute::SetAttributeTypeFromString( XdmfString AttributeType ){
XdmfInt64  Dimensions[3];

XdmfDebug("Setting Type to " << AttributeType );
if( XDMF_WORD_CMP( AttributeType, "Scalar" ) ) {
  this->AttributeType = XDMF_ATTRIBUTE_TYPE_SCALAR;
  Dimensions[0] = 1;
  this->ShapeDesc.SetShape( 1, Dimensions );
} else if( XDMF_WORD_CMP( AttributeType, "Vector" ) ) {
  this->AttributeType = XDMF_ATTRIBUTE_TYPE_VECTOR;
  Dimensions[0] = 3;
  this->ShapeDesc.SetShape( 1, Dimensions );
} else if( XDMF_WORD_CMP( AttributeType, "Tensor" ) ) {
  this->AttributeType = XDMF_ATTRIBUTE_TYPE_TENSOR;
  Dimensions[0] = 3;
  Dimensions[1] = 3;
  this->ShapeDesc.SetShape( 2, Dimensions );
} else if( XDMF_WORD_CMP( AttributeType, "Matrix" ) ) {
  this->AttributeType = XDMF_ATTRIBUTE_TYPE_MATRIX;
} else {
  XdmfErrorMessage("Unknown Attribute Type " << AttributeType );
  return( XDMF_FAIL );
  }
return( XDMF_SUCCESS );
}

XdmfString
XdmfAttribute::GetAttributeCenterAsString( void ){
  switch ( this->AttributeCenter ){
    case XDMF_ATTRIBUTE_CENTER_GRID :
      return( "Grid" );
    case XDMF_ATTRIBUTE_CENTER_CELL :
      return( "Cell" );
    case XDMF_ATTRIBUTE_CENTER_FACE :
      return( "Face" );
    case XDMF_ATTRIBUTE_CENTER_EDGE :
      return( "Edge" );
    case XDMF_ATTRIBUTE_CENTER_NODE :
      return( "Node" );
    default :
      break;
    }
return( "Node" );
}

XdmfInt32
XdmfAttribute::SetAttributeCenterFromString( XdmfString AttributeCenter ){

if( XDMF_WORD_CMP( AttributeCenter, "Grid" ) ) {
  this->AttributeCenter = XDMF_ATTRIBUTE_CENTER_GRID;
} else if( XDMF_WORD_CMP( AttributeCenter, "Cell" ) ) {
  this->AttributeCenter = XDMF_ATTRIBUTE_CENTER_CELL;
} else if( XDMF_WORD_CMP( AttributeCenter, "Face" ) ) {
  this->AttributeCenter = XDMF_ATTRIBUTE_CENTER_FACE;
} else if( XDMF_WORD_CMP( AttributeCenter, "Edge" ) ) {
  this->AttributeCenter = XDMF_ATTRIBUTE_CENTER_EDGE;
} else if( XDMF_WORD_CMP( AttributeCenter, "Node" ) ) {
  this->AttributeCenter = XDMF_ATTRIBUTE_CENTER_NODE;
} else {
  XdmfErrorMessage("Unknown Attribute Center " << AttributeCenter );
  return( XDMF_FAIL );
  }
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfAttribute::InitAttributeFromElement( XdmfXNode *Element ) {

XdmfString  Attribute;

if( !Element ){
  XdmfErrorMessage("Element is NULL");
  return( XDMF_FAIL );
  }
this->CurrentElement = Element;
Attribute = this->DOM->Get( Element, "NodeType" );
if( XDMF_WORD_CMP( Attribute, "Attribute" ) == 0 ){
  XdmfErrorMessage("Element Type " << Attribute << " is not an Attribute");
  return( XDMF_FAIL );
  }
Attribute = this->DOM->Get( Element, "Type" );
if( Attribute ){
  this->SetAttributeTypeFromString( Attribute );
} else {
  this->AttributeType = XDMF_ATTRIBUTE_TYPE_SCALAR;
}

Attribute = this->DOM->Get( Element, "Center" );
if( Attribute ){
  this->SetAttributeCenterFromString( Attribute );
} else {
  this->AttributeCenter = XDMF_ATTRIBUTE_CENTER_NODE;
}
Attribute = this->DOM->Get( Element, "Dimensions" );
if( Attribute ){
  this->ShapeDesc.SetShapeFromString( Attribute );
}
Attribute = this->DOM->Get( Element, "Name" );
if( Attribute ) {
  this->SetName( Attribute );
} else {
  this->SetName( GetUnique("Attribute_" ) );
}
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfAttribute::SetAttributeFromElement( XdmfXNode *Element ) {

XdmfInt32  Status;
XdmfXNode   *ValuesNode;
XdmfTransform  ValueReader;

XdmfString  Attribute;

if( !Element ){
  XdmfErrorMessage("Element is NULL");
  return( XDMF_FAIL );
  }
Attribute = this->DOM->Get( Element, "NodeType" );
if( XDMF_WORD_CMP( Attribute, "Attribute" ) == 0 ){
  XdmfErrorMessage("Element Type " << Attribute << " is not an Attribute");
  return( XDMF_FAIL );
  }
if( this->AttributeType == XDMF_ATTRIBUTE_TYPE_NONE ){
  Status = this->InitAttributeFromElement( Element );
  if( Status == XDMF_FAIL ) {
    XdmfErrorMessage("Can't Initialize");
    return( XDMF_FAIL );
    }
  }

ValuesNode = this->DOM->FindElement( NULL, 0, Element );
if( ValuesNode ){
  ValueReader.SetDOM( this->DOM );
  if( this->ValuesAreMine && this->Values ){
    delete this->Values;
    this->Values = NULL;
  } else {
  }
  this->Values = ValueReader.ElementToArray( ValuesNode );
  if( !this->Values ) {
    XdmfErrorMessage("Error Retriving Data Values");
    return( XDMF_FAIL );
    }
} else {
  XdmfErrorMessage("Element has no Data");
  return( XDMF_FAIL );
  }
return( XDMF_SUCCESS );
}


XdmfInt32
XdmfAttribute::Update( void ){

if( this->DOM && this->CurrentElement ){
  return( this->SetAttributeFromElement( this->CurrentElement ) );
}
XdmfErrorMessage("No Current Element set for DOM");
return( XDMF_FAIL );
}
