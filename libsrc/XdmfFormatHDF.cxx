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
#include "XdmfFormatHDF.h"

#include "XdmfDOM.h"
#include "XdmfHDF.h"
#include "XdmfArray.h"
#include "XdmfDataDesc.h"

#ifndef _WIN32
#include <unistd.h>
#endif

XdmfFormatHDF::XdmfFormatHDF() {
  strcpy( this->DataFormat, "HDF" );
}

XdmfFormatHDF::~XdmfFormatHDF() {
}

XdmfArray *
XdmfFormatHDF::ElementToArray( XdmfXNode *Element, XdmfDataDesc *Desc, XdmfArray *Array  ) {

XdmfInt32  NumberType;
XdmfString  Data;
XdmfArray  *NewArray = NULL;
XdmfDataDesc  *NewDesc = NULL;

if( !Element ) {
  XdmfErrorMessage("Element is NULL");
  return( NULL );
  }
XdmfDebug("Getting array from element");
NumberType = this->DOM->GetNumberType( Element );
Data = this->DOM->Get(Element, "CData");
if( Data && strlen( Data ) > 1 ){
  if( Array == NULL ){
    NewArray = Array = new XdmfArray( NumberType );
    }
  if( Desc == NULL ){
    NewDesc = Desc = this->ElementToDataDesc( Element );
    }
  if( Desc ){
    XdmfHDF  H5;
    // (Added by Andy)
    H5.SetWorkingDirectory(this->DOM->GetWorkingDirectory());
    if( NewArray ){
      Array->CopyType( Desc );
      Array->CopyShape( Desc );
      // JIC
      Array->CopySelection( Desc );
      Array->Allocate();
    }
    XDMF_WORD_TRIM( Data );
    XdmfDebug("Opening HDF data from " << Data );
    if( H5.Open( Data, "r" ) == XDMF_FAIL ) {
      XdmfErrorMessage("Can't Open Dataset " << Data );
      if( NewArray ){
        delete NewArray;
        }
      return( NULL );
      }
    if( Desc->GetSelectionSize() != H5.GetNumberOfElements() ){
      // We're not reading the entire dataset
      if( Desc->GetSelectionType() == XDMF_HYPERSLAB ){
        XdmfInt32  Rank;
        XdmfInt64  Start[ XDMF_MAX_DIMENSION ];
        XdmfInt64  Stride[ XDMF_MAX_DIMENSION ];
        XdmfInt64  Count[ XDMF_MAX_DIMENSION ];
        
        Rank = Desc->GetHyperSlab( Start, Stride, Count );
        H5.SelectHyperSlab( Start, Stride, Count );
        if( NewArray ){
          Array->SetShape( Rank, Count );
          Array->SelectAll();
          }
      } else {
        XdmfInt64  NumberOfCoordinates;
        XdmfInt64  *Coordinates;


        NumberOfCoordinates = Desc->GetSelectionSize();
        Coordinates = Desc->GetCoordinates();
        if( NewArray ){
          Array->SetNumberOfElements( NumberOfCoordinates );
          }
        H5.SelectCoordinates( NumberOfCoordinates, Coordinates );
        delete Coordinates;
        }
      }
    XdmfDebug("Readind Data");
    if( H5.Read( Array ) == NULL ){
      XdmfErrorMessage("Error Reading Array");
      if( NewArray ){
        delete NewArray;
        }
      H5.Close();
      return( NULL );
    }
    XdmfDebug("Closing Dataset");
    H5.Close();
    if( NewDesc ) {
      delete Desc;
      }
  }else{
    XdmfErrorMessage("Node has Invalid Data Desc");
    if( NewArray ){
      delete NewArray;
      }
    return( NULL );
    }
  
} else {
  XdmfErrorMessage("Node has no CData");
  return( NULL );
}
return( Array );
}

XdmfXNode *
XdmfFormatHDF::ArrayToElement( XdmfArray *Array,
      XdmfString HeavyDataset,
      XdmfXNode *Element,
      XdmfDataDesc *Desc ) {

char    Attribute[ XDMF_MAX_STRING_LENGTH  ];
ostrstream  Value( Attribute, XDMF_MAX_STRING_LENGTH );
XdmfHDF    H5;

if( Desc == NULL ) {
  Desc = Array;
  }
if( HeavyDataset == NULL ){
  HeavyDataset = "Xdmf.h5:/Data";
  }
Element = this->DataDescToElement( Desc, Element );
if( Element == NULL ) {
  XdmfErrorMessage("DataDescToElement returned NULL Element");
  return( NULL );
  }
Element->Set("CData", HeavyDataset );
XDMF_WORD_TRIM( HeavyDataset );
if( H5.Open( HeavyDataset, "rw" ) == XDMF_FAIL ){
  // Create it
  H5.CopyType( Desc );
  H5.CopyShape( Desc );
  if( H5.CreateDataset( HeavyDataset ) == XDMF_FAIL ){
    XdmfErrorMessage("Can't Open or Create Dataset " << HeavyDataset );
    return( NULL );
    }
  }
H5.CopySelection( Desc );
if( H5.Write( Array ) == NULL ) {
  XdmfErrorMessage("Can't Write Dataset " << HeavyDataset );
  H5.Close();
  return( NULL );
  }
H5.Close();
return( Element );
}

