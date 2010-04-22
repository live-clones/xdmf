/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : $Id: vtkXdmfDataArray.cxx,v 1.9 2009-07-23 21:23:22 kwleiter Exp $  */
/*  Date : $Date: 2009-07-23 21:23:22 $ */
/*  Version : $Revision: 1.9 $ */
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
#include <vtkXdmfDataArray.h>

#include <vtkObjectFactory.h>
#include <vtkCommand.h>

#include <vtkUnsignedCharArray.h>
#include <vtkCharArray.h>
#include <vtkIntArray.h>
#include <vtkLongArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkUnsignedIntArray.h>
#include "vtkShortArray.h"
#include "vtkUnsignedShortArray.h"

#include <XdmfArray.h>
#include <XdmfDataItem.h>
#include <XdmfDOM.h>
//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkXdmfDataArray);

//----------------------------------------------------------------------------
vtkXdmfDataArray::vtkXdmfDataArray()
{
  this->Array = NULL;
  this->vtkArray = NULL;
}

//----------------------------------------------------------------------------
vtkDataArray *vtkXdmfDataArray::FromXdmfArray( char *ArrayName, int numVTKComponents,
  int numXDMFComponents, int xdmfComponentStartIndex, int MakeCopy ){
  XdmfArray *array = this->Array;
 // XdmfInt64 components = 1;
  //XdmfInt64 tuples = 0;
  if ( ArrayName != NULL ) {
    array = TagNameToArray( ArrayName );
  }
  if( array == NULL ){
    XdmfErrorMessage("Array is NULL");
    return(NULL);
  }
  if ( this->vtkArray )
    {
    this->vtkArray->Delete();
    this->vtkArray = 0;
    }
  switch( array->GetNumberType() ){
  case XDMF_INT8_TYPE :
    if( this->vtkArray == NULL ) {
      this->vtkArray = vtkCharArray::New();
    }
    break;
  case XDMF_UINT8_TYPE :
    if( this->vtkArray == NULL ) {
      this->vtkArray = vtkUnsignedCharArray::New();
    }
    break;
  case XDMF_INT16_TYPE :
    if( this->vtkArray == NULL ) {
      this->vtkArray = vtkShortArray::New();
    }
    break;
  case XDMF_UINT16_TYPE :
    if( this->vtkArray == NULL ) {
      this->vtkArray = vtkUnsignedShortArray::New();
    }
    break;
  case XDMF_UINT32_TYPE :
    if( this->vtkArray == NULL ) {
      this->vtkArray = vtkUnsignedIntArray::New();
    }
    break;
  case XDMF_INT32_TYPE :
    if( this->vtkArray == NULL ) {
      this->vtkArray = vtkIntArray::New();
    }
    break;
  case XDMF_INT64_TYPE :
    if( this->vtkArray == NULL ) {
      this->vtkArray = vtkLongArray::New();
    }
    break;
  case XDMF_FLOAT32_TYPE :
    if( this->vtkArray == NULL ) {
      this->vtkArray = vtkFloatArray::New();
    }
    break;
  case XDMF_FLOAT64_TYPE :
    if( this->vtkArray == NULL ) {
      this->vtkArray = vtkDoubleArray::New();
    }
    break;
  default:
    vtkErrorMacro("Cannot create VTK data array: " << array->GetNumberType());
    return 0;
  }

  this->vtkArray->SetNumberOfComponents(numVTKComponents);
  int numComponentsToCopy = numVTKComponents;
  if(numVTKComponents > numXDMFComponents)
  {
    this->vtkArray->SetNumberOfTuples(array->GetNumberOfElements() / numXDMFComponents);
    for(int i=numXDMFComponents; i<numVTKComponents; ++i)
    {
      this->vtkArray->FillComponent(i, 0);
    }
    numComponentsToCopy = numXDMFComponents;
  }
  else
  {
    this->vtkArray->SetNumberOfTuples(array->GetNumberOfElements() / numXDMFComponents);
  }

  int numVTKValues = this->vtkArray->GetNumberOfComponents() * this->vtkArray->GetNumberOfTuples();
  int xdmfStartIndex = xdmfComponentStartIndex;
  int xdmfStride = numXDMFComponents;

  // We can't take the pointer --- some rearranging must be done.
  if(numVTKComponents != numXDMFComponents)
  {
    MakeCopy = 1;
  }

  if(MakeCopy){
  switch( array->GetNumberType() ){
  case XDMF_INT8_TYPE :
    this->CopyXdmfArray<XdmfInt8>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
  case XDMF_UINT8_TYPE :
    this->CopyXdmfArray<XdmfUInt8>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
  case XDMF_INT16_TYPE :
    this->CopyXdmfArray<XdmfInt16>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
  case XDMF_UINT16_TYPE :
    this->CopyXdmfArray<XdmfUInt16>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
  case XDMF_INT32_TYPE :
    this->CopyXdmfArray<XdmfInt32>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
  case XDMF_UINT32_TYPE :
    this->CopyXdmfArray<XdmfUInt32>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
  case XDMF_INT64_TYPE :
    this->CopyXdmfArray<XdmfInt64>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
  case XDMF_FLOAT32_TYPE :
    this->CopyXdmfArray<XdmfFloat32>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
  case XDMF_FLOAT64_TYPE :
    this->CopyXdmfArray<XdmfFloat64>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
  default :
    this->CopyXdmfArray<XdmfFloat64>(vtkArray, array, xdmfStartIndex, xdmfStride, numComponentsToCopy);
    break;
    }
  }
  else
  {
  switch( array->GetNumberType() ){
  case XDMF_INT8_TYPE :
    {
    vtkCharArray *chara = vtkCharArray::SafeDownCast(this->vtkArray);
    if(!chara){
        XdmfErrorMessage("Cannot downcast data array");
        return(0);
    }
    chara->SetArray((char *)array->GetDataPointer(), numVTKValues, 0);
    }
    break;
  case XDMF_UINT8_TYPE :
    {
    vtkUnsignedCharArray *uchara = vtkUnsignedCharArray::SafeDownCast(this->vtkArray);
    if(!uchara){
        XdmfErrorMessage("Cannot downcast ucharata array");
        return(0);
    }
    uchara->SetArray((unsigned char *)array->GetDataPointer(), numVTKValues, 0);
    }
    break;
  case XDMF_INT16_TYPE :
    {
    vtkShortArray *shorta = vtkShortArray::SafeDownCast(this->vtkArray);
    if(!shorta){
        XdmfErrorMessage("Cannot downcast data array");
        return(0);
    }
    shorta->SetArray((short *)array->GetDataPointer(), numVTKValues, 0);
    }
    break;
  case XDMF_UINT16_TYPE :
    {
    vtkUnsignedCharArray *uchara = vtkUnsignedCharArray::SafeDownCast(this->vtkArray);
    if(!uchara){
        XdmfErrorMessage("Cannot downcast ucharata array");
        return(0);
    }
    uchara->SetArray((unsigned char *)array->GetDataPointer(), numVTKValues, 0);
    }
    break;
  case XDMF_INT32_TYPE :
    {
    vtkIntArray *inta = vtkIntArray::SafeDownCast(this->vtkArray);
    if(!inta){
        XdmfErrorMessage("Cannot downcast intata array");
        return(0);
    }
    inta->SetArray((int *)array->GetDataPointer(), numVTKValues, 0);
    }
    break;
  case XDMF_UINT32_TYPE :
    {
    vtkUnsignedIntArray *uinta = vtkUnsignedIntArray::SafeDownCast(this->vtkArray);
    if(!uinta){
        XdmfErrorMessage("Cannot downcast uintata array");
        return(0);
    }
    uinta->SetArray((unsigned int *)array->GetDataPointer(), numVTKValues, 0);
    }
    break;
  case XDMF_INT64_TYPE :
    {
    vtkLongArray *longa = vtkLongArray::SafeDownCast(this->vtkArray);
    if(!longa){
        XdmfErrorMessage("Cannot downcast longa array");
        return(0);
    }
    longa->SetArray((long *)array->GetDataPointer(), numVTKValues, 0);
    }
    break;
  case XDMF_FLOAT32_TYPE :
    {
    vtkFloatArray *floata = vtkFloatArray::SafeDownCast(this->vtkArray);
    if(!floata){
        XdmfErrorMessage("Cannot downcast floatata array");
        return(0);
    }
    floata->SetArray((float *)array->GetDataPointer(), numVTKValues, 0);
    }
    break;
  case XDMF_FLOAT64_TYPE :
    {
    vtkDoubleArray *doublea = vtkDoubleArray::SafeDownCast(this->vtkArray);
    if(!doublea){
        XdmfErrorMessage("Cannot downcast doubleata array");
        return(0);
    }
    doublea->SetArray((double *)array->GetDataPointer(), numVTKValues, 0);
    }
    break;
  default :
    XdmfErrorMessage("Can't handle number type");
    return(0);
    break;
  }
  array->Reset();
  }
  return( this->vtkArray );
}

//----------------------------------------------------------------------------
template <typename T>
void vtkXdmfDataArray::CopyXdmfArray(vtkDataArray * vtkArray, XdmfArray * xdmfArray, int xdmfStartIndex, int xdmfStride, int numComponentsToCopy)
{
  int idx = xdmfStartIndex; 
  for ( vtkIdType jj = 0; jj < vtkArray->GetNumberOfTuples(); jj ++ )
    {
    for ( vtkIdType kk = 0; kk < numComponentsToCopy; kk ++ )
      {
      T val;
      xdmfArray->GetValues(idx+kk, &val, 1);
      //cout << "Value: " << val << endl;
      vtkArray->SetComponent(jj, kk, val);
      }
      idx+=xdmfStride;
    }
}

//----------------------------------------------------------------------------
char *vtkXdmfDataArray::ToXdmfArray( vtkDataArray *DataArray, int CopyShape ){
  XdmfArray *array;
  if ( DataArray  == NULL )  {
    DataArray = this->vtkArray;
  }
  if ( DataArray  == NULL )  {
    vtkDebugMacro(<< "Array is NULL");
    return(NULL);
  }
  if ( this->Array == NULL ){
    this->Array = new XdmfArray();
    switch( DataArray->GetDataType() ){
    case VTK_CHAR :
    case VTK_UNSIGNED_CHAR :
      this->Array->SetNumberType( XDMF_INT8_TYPE );
      break;
    case VTK_SHORT :
    case VTK_UNSIGNED_SHORT :
    case VTK_INT :
    case VTK_UNSIGNED_INT :
    case VTK_LONG :
    case VTK_UNSIGNED_LONG :
      this->Array->SetNumberType( XDMF_INT32_TYPE );
      break;
    case VTK_FLOAT :
      this->Array->SetNumberType( XDMF_FLOAT32_TYPE );
      break;
    case VTK_DOUBLE :
      this->Array->SetNumberType( XDMF_FLOAT64_TYPE );
      break;
    default :
      XdmfErrorMessage("Can't handle Data Type");
      return( NULL );
    }
  }
  array = this->Array;
  if( CopyShape ) {
    XdmfInt64 Shape[3];

    Shape[0] = DataArray->GetNumberOfTuples();
    Shape[1] = DataArray->GetNumberOfComponents();
    if( Shape[1] == 1 ) {
      array->SetShape( 1, Shape );
    } else {
      array->SetShape( 2, Shape );
    }

  }
  switch( array->GetNumberType() ){
  case XDMF_INT8_TYPE :
    array->SetValues( 0,
      ( unsigned char  *)DataArray->GetVoidPointer( 0 ),
      array->GetNumberOfElements() );  
    break;
  case XDMF_INT32_TYPE :
  case XDMF_INT64_TYPE :
    array->SetValues( 0,
      ( int *)DataArray->GetVoidPointer( 0 ),
      array->GetNumberOfElements() );  
    break;
  case XDMF_FLOAT32_TYPE :
    array->SetValues( 0,
      ( float *)DataArray->GetVoidPointer( 0 ),
      array->GetNumberOfElements() );  
    break;
  default :
    array->SetValues( 0,
      ( double *)DataArray->GetVoidPointer( 0 ),
      array->GetNumberOfElements() );  
    break;
  }
  return( array->GetTagName() );
}
