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
#include "XdmfVtkSupport.h"

VtkScriptObject
XdmfVtkAddressToCommand( XdmfPointer VtkObject) {

return(( VtkScriptObject )VtkObject);
}

XdmfInt32
XdmfArrayCopyFromVtkDataArray( XdmfArray *Array, XdmfPointer VtkArray, XdmfInt64 Start, XdmfInt64 Length ){

vtkDataArray  *VtkData;
XdmfInt64  VtkLength, XdmfLength;
void    *DataPointer;

if( Array == NULL ){
  XdmfErrorMessage("Array == NULL");
  return( XDMF_FAIL );
  }
if( VtkArray == NULL ){
  XdmfErrorMessage("VtkArray == NULL");
  return( XDMF_FAIL );
  }
VtkData = ( vtkDataArray *)VtkArray;
VtkLength = VtkData->GetNumberOfTuples();
VtkLength = MIN( Length, VtkLength - Start );
XdmfLength = Array->GetNumberOfElements();

if( VtkLength != XdmfLength ){
  XdmfErrorMessage("Length of XdmfArray and VtkDataArray not equal");
  XdmfErrorMessage("VtkLength = " << VtkLength );
  XdmfErrorMessage("XdmfLength = " << XdmfLength );
  return( XDMF_FAIL );
  }
DataPointer = VtkData->GetVoidPointer( 0 );
/*
cout << "Array->SetValues() Length = " << VtkLength << " of " << XdmfLength << " Type = " << VtkData->GetDataType() << endl;
*/
switch( VtkData->GetDataType() ){
  case VTK_UNSIGNED_CHAR :
  case VTK_CHAR :
    {
      XdmfInt8  *Data = ( XdmfInt8 *)DataPointer;
      Array->SetValues( Start, Data, VtkLength );
    }
    break;
  case VTK_INT :
    {
      XdmfInt32  *Data = ( XdmfInt32 *)DataPointer;
      Array->SetValues( Start, Data, VtkLength );
    }
    break;
  case VTK_LONG :
    if( sizeof(long) == sizeof( XdmfInt64 ) ){
      XdmfInt64  *Data = ( XdmfInt64 *)DataPointer;
      Array->SetValues( Start, Data, VtkLength );
    } else {
      XdmfInt32  *Data = ( XdmfInt32 *)DataPointer;
      Array->SetValues( Start, Data, VtkLength );
    }
    break;
  case VTK_FLOAT :
    {
      XdmfFloat32  *Data = ( XdmfFloat32 *)DataPointer;
      Array->SetValues( Start, Data, VtkLength );
    }
    break;
  case VTK_DOUBLE :
    {
      XdmfFloat64  *Data = ( XdmfFloat64 *)DataPointer;
      Array->SetValues( Start, Data, VtkLength );
    }
    break;
  default :
    XdmfErrorMessage("Can't Handle Data Type = " << VtkData->GetDataType() );
    return( XDMF_FAIL );
  }
  
/*
cerr << "IN XdmfArrayCopyToVtkDataArray" << endl;
cerr << "VtkArray has " << Target->GetNumberOfTuples() << " Elements" << endl;
cerr << "XdmfArray has " << Array->GetNumberOfElements() << " Elements " << endl;
*/
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfArrayCopyToVtkDataArray( XdmfArray *Array, XdmfPointer VtkArray, XdmfInt64 Start, XdmfInt64 Length ){

vtkDataArray  *VtkData;
XdmfInt64  VtkLength, XdmfLength;
void    *DataPointer;

if( Array == NULL ){
  XdmfErrorMessage("Array == NULL");
  return( XDMF_FAIL );
  }
if( VtkArray == NULL ){
  XdmfErrorMessage("VtkArray == NULL");
  return( XDMF_FAIL );
  }
VtkData = ( vtkDataArray *)VtkArray;
VtkLength = VtkData->GetNumberOfTuples();
VtkLength = MIN( Length, VtkLength - Start );
XdmfLength = Array->GetNumberOfElements();

if( VtkLength != XdmfLength ){
  XdmfErrorMessage("Length of XdmfArray and VtkDataArray not equal");
  XdmfErrorMessage("VtkLength = " << VtkLength );
  XdmfErrorMessage("XdmfLength = " << XdmfLength );
  return( XDMF_FAIL );
  }
DataPointer = VtkData->GetVoidPointer( 0 );
/*
cout << "Array->GetValues() Length = " << VtkLength << " of " << XdmfLength << " Type = " << VtkData->GetDataType() << endl;
*/
switch( VtkData->GetDataType() ){
  case VTK_UNSIGNED_CHAR :
  case VTK_CHAR :
    {
      XdmfInt8  *Data = ( XdmfInt8 *)DataPointer;
      Array->GetValues( Start, Data, VtkLength );
    }
    break;
  case VTK_INT :
    {
      XdmfInt32  *Data = ( XdmfInt32 *)DataPointer;
      Array->GetValues( Start, Data, VtkLength );
    }
    break;
  case VTK_LONG :
    if( sizeof(long) == sizeof(XdmfInt64) ){
      XdmfInt64  *Data = ( XdmfInt64 *)DataPointer;
      Array->GetValues( Start, Data, VtkLength );
    } else {
      XdmfInt32  *Data = ( XdmfInt32 *)DataPointer;
      Array->GetValues( Start, Data, VtkLength );
      }
    break;
  case VTK_FLOAT :
    {
      XdmfFloat32  *Data = ( XdmfFloat32 *)DataPointer;
      Array->GetValues( Start, Data, VtkLength );
    }
    break;
  case VTK_DOUBLE :
    {
      XdmfFloat64  *Data = ( XdmfFloat64 *)DataPointer;
      Array->GetValues( Start, Data, VtkLength );
    }
    break;
  default :
    XdmfErrorMessage("Can't Handle Data Type = " << VtkData->GetDataType() );
    return( XDMF_FAIL );
  }
  
/*
cerr << "IN XdmfArrayCopyToVtkDataArray" << endl;
cerr << "VtkArray has " << Target->GetNumberOfTuples() << " Elements" << endl;
cerr << "XdmfArray has " << Array->GetNumberOfElements() << " Elements " << endl;
*/
return( XDMF_SUCCESS );
}

/*
XdmfInt32
XdmfWriteDataSet( XdmfString FileName, vtkUnstructuredGrid *DataSet){


cerr << "Writing Dataset to " << FileName << endl;
if( !DataSet ) {
  return( XDMF_FAIL );
  }
cerr << "# Points = " << DataSet->GetNumberOfPoints() << endl;
cerr << "# Cells = " << DataSet->GetNumberOfCells() << endl;
return( XDMF_SUCCESS );
}
*/

