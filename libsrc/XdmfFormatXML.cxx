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
#include "XdmfFormatXML.h"

#include "XdmfDOM.h"
#include "XdmfArray.h"
#include "XdmfHDF.h"

XdmfFormatXML::XdmfFormatXML() {
  strcpy( this->DataFormat, "XML" );
}

XdmfFormatXML::~XdmfFormatXML() {
}

XdmfArray *
XdmfFormatXML::ElementToArray( XdmfXNode *Element, XdmfDataDesc *Desc, XdmfArray *Array  ) {

XdmfInt32  NumberType;
XdmfConstString  Data;
XdmfArray  *NewArray = NULL;
XdmfDataDesc  *NewDesc = NULL;

if( !this->DOM ){
  XdmfErrorMessage("Object has no DOM");
  return( NULL );
  }
if( !Element ) {
  XdmfErrorMessage("Element is NULL");
  return( NULL );
  }
NumberType = this->DOM->GetNumberType( Element );
Data = this->DOM->Get(Element, "CData");
if( Data && strlen( Data ) > 1 ){
  XdmfString NewData = 0;
  XDMF_STRING_DUPLICATE(NewData, Data);
  if( Array == NULL ){
    NewArray = Array = new XdmfArray( NumberType );
    }
  if( Desc == NULL ){
    NewDesc = Desc = this->ElementToDataDesc( Element, 0 );
    }
  if( Desc ){
    if( NewArray ){
      Array->CopyType( Desc );
      Array->CopyShape( Desc );
      Array->CopySelection( Desc );
      Array->Allocate();
    }
    if( Desc->GetSelectionSize() != Array->GetNumberOfElements() ){
      XdmfInt64  SelectionSize = Desc->GetSelectionSize();
      XdmfArray  *TempArray = new XdmfArray();

      TempArray->CopyType( Desc );
      TempArray->CopyShape( Desc );
      TempArray->CopySelection( Desc );
      TempArray->Allocate();
      TempArray->SetValues(0, NewData );
      if( NewArray ){
        NewArray->SetShape( 1, &SelectionSize );
        NewArray->SelectAll();
        }
      CopyArray( TempArray, Array );
      delete TempArray;
    } else {
      Array->SetValues(0, NewData );
    }
    if( NewDesc ){
      delete Desc;
      }
  }else{
    XdmfErrorMessage("Node has Invalid Data Desc");
    if( NewArray ){
      delete NewArray;
      }
    delete [] NewData;
    return( NULL );
    }
  
  delete [] NewData;
} else {
  XdmfErrorMessage("Node has no CData");
  return( NULL );
}
return( Array );
}

XdmfXNode *
XdmfFormatXML::ArrayToElement( XdmfArray *Array,
      XdmfString /*HeavyDataset*/,
      XdmfXNode *Element,
      XdmfDataDesc *Desc ) {

char    Attribute[ XDMF_MAX_STRING_LENGTH  ];
ostrstream  Value( Attribute, XDMF_MAX_STRING_LENGTH );

if( Desc == NULL ) {
  Desc = Array;
  }
Element = this->DataDescToElement( Desc, Element );
if( Element == NULL ) {
  XdmfErrorMessage("DataDescToElement returned NULL Element");
  return( NULL );
  }
if( Desc->GetNumberOfElements() == Array->GetSelectionSize() ){
  Element->Set("CData", Array->GetValues( 0 ) );
} else {
  XdmfArray  *NewArray = new XdmfArray;

//  Element->Set("CData", Array->GetValues( 0 ) );
  NewArray->CopyType( Array );
  NewArray->CopyShape( Desc );  
  NewArray->CopySelection( Desc );
  NewArray->Generate( 0.0, 0.0 );
  CopyArray( Array, NewArray );
  Element->Set("CData", NewArray->GetValues( 0 ) );
  delete NewArray;
  }
return( Element );
}

