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
#include "XdmfArray.h"

#include "XdmfArrayCopyMacro.h"

#ifndef _WIN32
#include <unistd.h>
#endif

#if defined(IRIX64) || defined(IRIXN32)
#else
#endif

static XdmfLength  GlobalTimeCntr = 0;
// static  ostrstream  *StringOutput = NULL;

class XdmfArrayListClass
{
public:
  XdmfArrayListClass()
    {
    this->ListLength = 0;
    this->ListIndex = 0;
    this->List = 0;
    }
  ~XdmfArrayListClass();

  XdmfArrayList* AddArray();
  void RemoveArray(XdmfArray*);

  XdmfLength GetNumberOfElements() 
    {
    return this->ListIndex;
    }

  XdmfLength  ListLength;
  XdmfArrayList  *List;

private:
  XdmfLength  ListIndex;
};

XdmfArrayListClass::~XdmfArrayListClass()
{
  if ( this->List )
    {
    while( this->ListIndex )
      {
      delete this->List[this->ListIndex-1].Array;
      }
    delete [] this->List;
    }
}

XdmfArrayList* XdmfArrayListClass::AddArray()
{
XdmfArrayList* res = 0;
if( this->ListIndex >= this->ListLength ){
  XdmfArrayList  *NewList = new XdmfArrayList[ this->ListLength + 32];
  if( this->List != NULL ){
    memcpy( NewList, this->List, this->ListLength * sizeof( XdmfArrayList ));
    delete [] this->List;
    }
  this->List = NewList;
  this->ListLength += 32;
  }
res = this->List + this->ListIndex;
this->ListIndex++;
return res;
}

void XdmfArrayListClass::RemoveArray(XdmfArray* array)
{
  XdmfLength   i;
for( i = 0 ; i < this->ListLength ; i++ ){
  if ( this->List[ i ].Array == array ){
    memcpy( &this->List[ i ],
      &this->List[ i + 1 ],
      (this->ListIndex - i) * sizeof( XdmfArrayList ) );      
    this->ListIndex--;
    break;
    }
  }
}

static XdmfArrayListClass XDMFArrayList;

XdmfArray *
TagNameToArray( XdmfString TagName ) {

char    c;
XdmfInt64  i, Id;
istrstream   Tag(TagName, strlen(TagName));

Tag >> c;
if( c != '_' ) {
  XdmfErrorMessage("Invalid Array Tag Name: " << TagName );
  return( NULL );
  }
#ifdef ICE_HAVE_64BIT_STREAMS
Tag >> Id;
#else
{
  double d;
  Tag >> d;
  Id = d;
}
#endif

//cerr << "List = " << XDMFArrayList.List << endl;
//cerr << "ListLength = " << XDMFArrayList.ListLength << endl;
//cerr << "List[ 0 ]  = " << XDMFArrayList.List[ 0 ].timecntr << endl;
// PrintAllXdmfArrays();
for( i = 0 ; i < XDMFArrayList.ListLength ; i++ ){
//  cerr << "XDMFArrayList.List[ i ].timecntr = " << XDMFArrayList.List[ i ].timecntr << endl;
//  cerr << "Id = " << Id << endl;
  if ( XDMFArrayList.List[ i ].timecntr == Id ){
    return( XDMFArrayList.List[ i ].Array );
    }
  }
XdmfErrorMessage("No Array found with Tag Name: " << TagName );
return( NULL );
}

XdmfArray *
GetNextOlderArray( XdmfLength Age, XdmfLength *AgeOfArray ) {
XdmfLength i;

for( i = 0 ; i < XDMFArrayList.GetNumberOfElements(); i++ ){
  if( XDMFArrayList.List[ i ].timecntr > Age ) {
    if( AgeOfArray != NULL ){
      *AgeOfArray = XDMFArrayList.List[ i ].timecntr;
      }
    return( XDMFArrayList.List[ i ].Array );
    }
}
return( NULL );
}

XdmfLength
GetCurrentArrayTime( void ) {

return (GlobalTimeCntr);
}

extern void
PrintAllXdmfArrays() {
XdmfLength i;

for( i = 0 ; i < XDMFArrayList.GetNumberOfElements(); i++ ){
  cerr << "XdmfArray " << ICE_64BIT_CAST i << '\n';
  cerr << "   NumberType " << XDMFArrayList.List[ i ].Array->GetNumberTypeAsString() << '\n';
  cerr << "   Time = " << ICE_64BIT_CAST XDMFArrayList.List[ i ].timecntr << '\n';
  cerr << "   Size = " << ICE_64BIT_CAST XDMFArrayList.List[ i ].Array->GetNumberOfElements() << '\n';
  }
}

void 
XdmfArray::AddArrayToList( void ) {

ostrstream   Tag(this->TagName, XDMF_ARRAY_TAG_LENGTH);
GlobalTimeCntr++;
XdmfArrayList* array = XDMFArrayList.AddArray();
array->name = NULL;
array->timecntr = GlobalTimeCntr;
array->Array = this;
Tag << "_" << ICE_64BIT_CAST GlobalTimeCntr << "_XdmfArray" << ends;
}

XdmfArray::XdmfArray() {
  XdmfDebug("XdmfArray Constructor");
  this->DataPointer = NULL;
  this->DataIsMine = 1;
  this->AddArrayToList();
 }

XdmfArray::XdmfArray( XdmfInt32 NumberType ) {
  XdmfDebug("XdmfArray Constructor");
  this->DataPointer = NULL;
  this->DataIsMine = 1;
  this->SetNumberType( NumberType );
  this->AddArrayToList();
 }

XdmfArray::XdmfArray( XdmfInt32 NumberType, XdmfLength Length ) {
  XdmfDebug("XdmfArray Constructor");
  this->DataPointer = NULL;
  this->DataIsMine = 1;
  this->SetNumberType( NumberType );
  this->SetNumberOfElements( Length );
  this->AddArrayToList();
 }

XdmfArray::~XdmfArray() {
  XdmfDebug("XdmfArray Destructor");
  if( ( this->DataIsMine ) && ( this->DataPointer != NULL ) ) {
    XdmfDebug(" Deleteing Data Array" );
    // delete [] this->DataPointer;
    free( this->DataPointer );
    this->DataPointer = NULL;
  } else {
    XdmfDebug("Can't Delete Array : Data Pointer is not mine");
  }
  XDMFArrayList.RemoveArray(this);
}

XdmfString
XdmfArray::GetTagName( void ){
return( this->TagName );
}


/*
XdmfPointer XdmfArray::MemCopy( XdmfLength StartOffset,
    XdmfLength NumberOfElemnts,
    XdmfPointer DataPointer,
    XdmfLength Stride ) {
  XdmfPointer RetVal;

if( Stride == 1 ) {
  RetVal = memcpy( this->GetVoidPointer( StartOffset ),
      DataPointer,
      this->Precision * NumberOfElemnts );
} else {
  XdmfLength   i;
  XdmfInt8  *ptr;

  RetVal = this->GetVoidPointer( StartOffset );
  ptr = ( XdmfInt8 *)DataPointer;
  for( i = 0 ; i < NumberOfElemnts ; i++ ){
    memcpy( this->GetVoidPointer( StartOffset ),
      ptr,
      this->Precision );
    StartOffset += Stride;
    ptr += this->Precision;
    }
}

return( RetVal );
}
*/

XdmfInt32 XdmfArray::Allocate( void ){
  XdmfDebug("Allocating " <<
    ICE_64BIT_CAST (this->GetNumberOfElements() *  this->GetElementSize()) <<
    " Bytes");
  if( this->DataIsMine ) {
    XdmfDebug("Data  " << ICE_64BIT_CAST this->DataPointer << " is Mine");
    if( this->DataPointer ) {
      this->DataPointer = (XdmfInt8 *)realloc( this->DataPointer, this->GetNumberOfElements() *  this->GetElementSize());
    } else {
      this->DataPointer = (XdmfInt8 *)malloc( this->GetNumberOfElements() *  this->GetElementSize());
    }
    if( this->DataPointer == NULL ) {
      XdmfDebug("Allocation Failed");
      perror(" Alloc :" );
#ifndef _WIN32      
      XdmfDebug("End == " << sbrk(0)  );
#endif
      }
  }
  XdmfDebug("Data Pointer = " << this->DataPointer );
  if( this->DataPointer == NULL ) {
    XdmfDebug("Allocation Failed");
    return( XDMF_FAIL );
    }
  XdmfDebug("Allocation Succeeded");
  return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::Reform( XdmfInt32 Rank, XdmfInt64 *Dimensions ) {

  XdmfDebug("Reform Shape");
  XdmfDataDesc::SetShape( Rank, Dimensions );  
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetShape( XdmfInt32 Rank, XdmfInt64 *Dimensions ) {

  XdmfDebug("Setting Shape and Allocating Memory");
  XdmfDataDesc::SetShape( Rank, Dimensions );  
  this->Allocate();

return( XDMF_SUCCESS );
}

XdmfInt32
XdmfArray::CopyShape( hid_t DataSpace ){
  XdmfDebug("Setting Shape and Allocating Memory");
  XdmfDataDesc::CopyShape( DataSpace );  
  this->Allocate();
  return( XDMF_SUCCESS );
}

XdmfInt32
XdmfArray::CopyShape( XdmfDataDesc *DataDesc ){
  XdmfDebug("Setting Shape and Allocating Memory");
  XdmfDataDesc::CopyShape( DataDesc );  
  this->Allocate();
  return( XDMF_SUCCESS );
}

XdmfInt32
XdmfArray::Reform( XdmfDataDesc *DataDesc ){
  XdmfDebug("Setting Shape");
  XdmfDataDesc::CopyShape( DataDesc );  
  return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetShapeFromString( XdmfString Dimensions ) {

  XdmfDebug("Setting Shape and Allocating Memory");
  XdmfDataDesc::SetShapeFromString( Dimensions );
  this->Allocate();

return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::ReformFromString( XdmfConstString Dimensions ) {

  XdmfDebug("Setting Shape and Allocating Memory");
  XdmfDataDesc::SetShapeFromString( Dimensions );

return( XDMF_SUCCESS );
}


XdmfPointer XdmfArray::GetDataPointer( XdmfInt64 Index ) {

XdmfByte  *Pointer;

Pointer = ( XdmfByte *)this->DataPointer;
Pointer += (this->GetElementSize() * Index );
return( (XdmfPointer)Pointer );
}

XdmfInt32 XdmfArray::SetValueFromInt64( XdmfInt64 Index, XdmfInt64 Value ) {
return( this->SetValue( Index, Value ) );
}

XdmfInt32 XdmfArray::SetValueFromFloat64( XdmfInt64 Index, XdmfFloat64 Value ) {
return( this->SetValue( Index, Value ) );
}

XdmfInt32 XdmfArray::CopyCompound( XdmfPointer ArrayPointer,
      XdmfInt32 /*ArrayType*/,
      XdmfInt64 /*ArrayStride*/,
      XdmfPointer ValuePointer,
      XdmfInt32  ValueType,
      XdmfInt64 ValueStride,
      XdmfInt32 Direction,
      XdmfInt64 NumberOfValues ) {

XdmfFloat64  *TmpPtr, *TmpArray;
XdmfInt64  Length, NumberOfMembers, MemberIndex, MemberLength;
XdmfInt32  MemberType;
XdmfByte  *Ptr;


XdmfDebug("Coping " << ICE_64BIT_CAST NumberOfValues << " Direction = " << Direction );
if( Direction == XDMF_ARRAY_IN ){

  TmpArray = new XdmfFloat64[ NumberOfValues ];

  // Copy Values To TmpArray
  TmpPtr = TmpArray;
  Ptr = (XdmfByte *)ValuePointer;
  XDMF_ARRAY_COPY( Ptr, ValueType, ValueStride,
      TmpPtr, XDMF_FLOAT64_TYPE, 1,
       XDMF_ARRAY_OUT, NumberOfValues );        

  // Copy TmpArray to Array
  Ptr = (XdmfByte *)ArrayPointer;
  TmpPtr = TmpArray;
  MemberIndex = 0;
  NumberOfMembers = this->GetNumberOfMembers();
  while( NumberOfValues ){
    MemberType = this->GetMemberType( MemberIndex );
    MemberLength = this->GetMemberLength( MemberIndex );
    XDMF_ARRAY_COPY( Ptr, MemberType, 1,
      TmpPtr, XDMF_FLOAT64_TYPE, 1,
      XDMF_ARRAY_IN, MemberLength);
    Ptr += this->GetMemberSize( MemberIndex );
    NumberOfValues -= MemberLength;
    MemberIndex++;
    if( MemberIndex >= NumberOfMembers ) {
      MemberIndex = 0;
      }
  }
} else {
  TmpArray = new XdmfFloat64[ NumberOfValues ];
  // Copy Array to TmpArray
  Ptr = (XdmfByte *)ArrayPointer;
  TmpPtr = TmpArray;
  MemberIndex = 0;
  NumberOfMembers = this->GetNumberOfMembers();
  Length = NumberOfValues;
  XdmfDebug("Copying " << ICE_64BIT_CAST NumberOfValues << " Out");
  while( NumberOfValues ){
    MemberType = this->GetMemberType( MemberIndex );
    MemberLength = this->GetMemberLength( MemberIndex );
    XDMF_ARRAY_COPY( Ptr, MemberType, 1,
      TmpPtr, XDMF_FLOAT64_TYPE, 1,
      XDMF_ARRAY_OUT, MemberLength);
    Ptr += this->GetMemberSize( MemberIndex );
    NumberOfValues -= MemberLength;
    MemberIndex++;
    if( MemberIndex >= NumberOfMembers ) {
      MemberIndex = 0;
      }
  }
  // Copy TmpArray to Values
  TmpPtr = TmpArray;
  Ptr = (XdmfByte *)ValuePointer;
  XDMF_ARRAY_COPY( Ptr, ValueType, ValueStride,
      TmpPtr, XDMF_FLOAT64_TYPE, 1,
       XDMF_ARRAY_IN, Length);        

}

delete TmpArray;
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetValue( XdmfInt64 Index, XdmfInt64 Value ) {

XdmfPointer  ArrayPointer;
XdmfInt64  *vp;

ArrayPointer = this->GetDataPointer(Index);
vp = &Value;
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), 1,
    vp, XDMF_INT64_TYPE, 1,
    XDMF_ARRAY_IN, 1);
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetValue( XdmfInt64 Index, XdmfFloat64 Value ) {

XdmfPointer  ArrayPointer;
XdmfFloat64  *vp;

ArrayPointer = this->GetDataPointer(Index);
vp = &Value;
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), 1,
    vp, XDMF_FLOAT64_TYPE, 1,
    XDMF_ARRAY_IN, 1);
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetValues( XdmfInt64 Index, XdmfConstString Values,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;
XdmfInt64    count = 0, i = 0, NumberOfValues;
istrstream   ist(Values, strlen( Values) );
istrstream   counter(Values, strlen( Values) );
XdmfFloat64  dummy, *ValueArray, *vp;

while( counter >> dummy ) count++;
NumberOfValues = count;
vp = ValueArray = new XdmfFloat64[ count + 1 ];
while( ist >> dummy ) ValueArray[i++] = dummy;
ArrayPointer = this->GetDataPointer(Index);
if( ArrayPointer == NULL ){
  this->SetNumberOfElements( NumberOfValues + Index );
  ArrayPointer = this->GetDataPointer(Index);
  }
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    vp, XDMF_FLOAT64_TYPE, ValuesStride,
    XDMF_ARRAY_IN, NumberOfValues );
delete [] ValueArray;
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetValues( XdmfInt64 Index, XdmfInt8 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
if( ArrayPointer == NULL ){
  this->SetNumberOfElements( NumberOfValues + Index );
  ArrayPointer = this->GetDataPointer(Index);
  }
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_INT8_TYPE, ValuesStride,
    XDMF_ARRAY_IN, NumberOfValues );
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetValues( XdmfInt64 Index, XdmfInt32 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
if( ArrayPointer == NULL ){
  this->SetNumberOfElements( NumberOfValues + Index );
  ArrayPointer = this->GetDataPointer(Index);
  }
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_INT32_TYPE, ValuesStride,
    XDMF_ARRAY_IN, NumberOfValues );
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetValues( XdmfInt64 Index, XdmfInt64 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
if( ArrayPointer == NULL ){
  this->SetNumberOfElements( NumberOfValues + Index );
  ArrayPointer = this->GetDataPointer(Index);
  }
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_INT64_TYPE, ValuesStride,
    XDMF_ARRAY_IN, NumberOfValues );
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetValues( XdmfInt64 Index, XdmfFloat32 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
if( ArrayPointer == NULL ){
  this->SetNumberOfElements( NumberOfValues + Index );
  ArrayPointer = this->GetDataPointer(Index);
  }
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_FLOAT32_TYPE, ValuesStride,
    XDMF_ARRAY_IN, NumberOfValues );
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::SetValues( XdmfInt64 Index, XdmfFloat64 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
if( ArrayPointer == NULL ){
  this->SetNumberOfElements( NumberOfValues + Index );
  ArrayPointer = this->GetDataPointer(Index);
  }
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_FLOAT64_TYPE, ValuesStride,
    XDMF_ARRAY_IN, NumberOfValues );
return( XDMF_SUCCESS );
}


XdmfFloat64 XdmfArray::GetValueAsFloat64( XdmfInt64 Index ) {

XdmfPointer  ArrayPointer;
XdmfFloat64  Value, *vp = &Value;

ArrayPointer = this->GetDataPointer(Index);
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), 1,
    vp, XDMF_FLOAT64_TYPE, 1,
    XDMF_ARRAY_OUT, 1);
return( Value );
}

XdmfFloat32 XdmfArray::GetValueAsFloat32( XdmfInt64 Index ) {

XdmfPointer  ArrayPointer;
XdmfFloat32  Value, *vp = &Value;

ArrayPointer = this->GetDataPointer(Index);
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), 1,
    vp, XDMF_FLOAT32_TYPE, 1,
    XDMF_ARRAY_OUT, 1);
return( Value );
}

XdmfInt64 XdmfArray::GetValueAsInt64( XdmfInt64 Index ) {

XdmfPointer  ArrayPointer;
XdmfInt64  Value, *vp = &Value;

ArrayPointer = this->GetDataPointer(Index);
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), 1,
    vp, XDMF_INT64_TYPE, 1,
    XDMF_ARRAY_OUT, 1);
return( Value );
}

XdmfInt32 XdmfArray::GetValueAsInt32( XdmfInt64 Index ) {

XdmfPointer  ArrayPointer;
XdmfInt32  Value, *vp = &Value;

ArrayPointer = this->GetDataPointer(Index);
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), 1,
    vp, XDMF_INT32_TYPE, 1,
    XDMF_ARRAY_OUT, 1);
return( Value );
}

XdmfInt8 XdmfArray::GetValueAsInt8( XdmfInt64 Index ) {

XdmfPointer  ArrayPointer;
XdmfInt8  Value, *vp = &Value;

ArrayPointer = this->GetDataPointer(Index);
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), 1,
    vp, XDMF_INT8_TYPE, 1,
    XDMF_ARRAY_OUT, 1);
return( Value );
}


XdmfInt32 XdmfArray::GetValues( XdmfInt64 Index, XdmfInt8 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_INT8_TYPE, ValuesStride,
    XDMF_ARRAY_OUT, NumberOfValues );
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::GetValues( XdmfInt64 Index, XdmfInt32 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_INT32_TYPE, ValuesStride,
    XDMF_ARRAY_OUT, NumberOfValues );
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::GetValues( XdmfInt64 Index, XdmfInt64 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_INT64_TYPE, ValuesStride,
    XDMF_ARRAY_OUT, NumberOfValues );
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::GetValues( XdmfInt64 Index, XdmfFloat32 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_FLOAT32_TYPE, ValuesStride,
    XDMF_ARRAY_OUT, NumberOfValues );
return( XDMF_SUCCESS );
}

XdmfInt32 XdmfArray::GetValues( XdmfInt64 Index, XdmfFloat64 *Values,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride,
        XdmfInt64 ValuesStride ) {
XdmfPointer  ArrayPointer;

ArrayPointer = this->GetDataPointer(Index);
XdmfDebug("Getting " << ICE_64BIT_CAST NumberOfValues << " From Pointer = " << ArrayPointer << " to " << Values );
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), ArrayStride,
    Values, XDMF_FLOAT64_TYPE, ValuesStride,
    XDMF_ARRAY_OUT, NumberOfValues );
return( XDMF_SUCCESS );
}

XdmfString  XdmfArray::GetValues( XdmfInt64 Index,
        XdmfInt64 NumberOfValues,
        XdmfInt64 ArrayStride ) {

XdmfInt64  i = 0, MemberLength;
XdmfFloat64  *Values;
ostrstream      StringOutput;
XdmfString Ptr;
static XdmfString ReturnString = NULL;



if( NumberOfValues == 0 ){
  NumberOfValues = this->GetNumberOfElements() - Index;
  }
// NumberOfValues -= 1;
if( this->GetNumberType() == XDMF_COMPOUND_TYPE ){
  XdmfDebug("Array is Compound, increasing value of NumberOfValues " << ICE_64BIT_CAST NumberOfValues );
  MemberLength = 0;
  for( i = 0 ; i < this->GetNumberOfMembers() ; i++ ){
    MemberLength += this->GetMemberLength(i);
    }
  NumberOfValues *= MemberLength;
  XdmfDebug("New NumberOfValues  = " << ICE_64BIT_CAST NumberOfValues );
}
Values = new XdmfFloat64[ NumberOfValues + 10 ];
this->GetValues( Index, Values, NumberOfValues, ArrayStride, 1 );
i = 0;
while( NumberOfValues-- ) {
  StringOutput << ICE_64BIT_CAST Values[i++] << " ";
  }
StringOutput << ends;
Ptr = StringOutput.str();
if ( ReturnString != NULL ) delete [] ReturnString;
ReturnString = new char[ strlen( Ptr ) + 2 ];
strcpy( ReturnString, Ptr );
delete [] Ptr;
return( ReturnString );
}

XdmfInt32  XdmfArray::Generate( XdmfFloat64 StartValue,
      XdmfFloat64 EndValue,
      XdmfInt64 StartIndex,
      XdmfInt64 EndIndex ){

XdmfFloat64  *Values, *vp, Current, Delta;
XdmfInt64  i, Length;

if( EndIndex == StartIndex ) EndIndex = this->GetNumberOfElements() - 1;
Length = EndIndex - StartIndex;
vp = Values = new XdmfFloat64[ Length  + 1];
Current = StartValue;
Delta = ( EndValue - StartValue ) / Length;
Length++;
for( i = 0 ; i < Length ; i++ ) {
  *vp++ = Current;
  Current += Delta;
  }
this->SetValues( StartIndex, Values, Length, 1, 1);
delete [] Values;
return( XDMF_SUCCESS );
}

XdmfArray &
XdmfArray::operator=( XdmfArray &Array ){

XdmfInt64  Length;
XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp, *Values;

Length = MIN( this->GetNumberOfElements(), Array.GetNumberOfElements() );
Vp = Values = new XdmfFloat64[ Length + 10 ];
Array.GetValues( 0, Values, Length );
ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 1,
    XDMF_ARRAY_IN, Length );
delete [] Values;
return( *this );
}

XdmfArray &
XdmfArray::operator=( XdmfFloat64 Value ){

XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp = &Value;

ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_COPY( ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 0,
    XDMF_ARRAY_IN, this->GetNumberOfElements());
return( *this );
}

XdmfArray &
XdmfArray::operator+( XdmfArray &Array ){
XdmfInt64  Length;
XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp, *Values;

Length = MIN( this->GetNumberOfElements(), Array.GetNumberOfElements() );
Vp = Values = new XdmfFloat64[ Length + 10 ];
Array.GetValues( 0, Values, Length );
ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_OPERATE( +=, ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 1,
    XDMF_ARRAY_IN, Length );
delete [] Values;
return( *this );
}

XdmfArray &
XdmfArray::operator+( XdmfFloat64 Value ){
XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp = &Value;

ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_OPERATE( +=, ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 0,
    XDMF_ARRAY_IN, this->GetNumberOfElements());
return( *this );
}

XdmfArray &
XdmfArray::operator-( XdmfArray &Array ){
XdmfInt64  Length;
XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp, *Values;

Length = MIN( this->GetNumberOfElements(), Array.GetNumberOfElements() );
Vp = Values = new XdmfFloat64[ Length + 10 ];
Array.GetValues( 0, Values, Length );
ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_OPERATE( -=, ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 1,
    XDMF_ARRAY_IN, Length );
delete [] Values;
return( *this );
}

XdmfArray &
XdmfArray::operator-( XdmfFloat64 Value ){
XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp = &Value;

ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_OPERATE( -=, ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 0,
    XDMF_ARRAY_IN, this->GetNumberOfElements());
return( *this );
}

XdmfArray &
XdmfArray::operator*( XdmfArray &Array ){
XdmfInt64  Length;
XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp, *Values;

Length = MIN( this->GetNumberOfElements(), Array.GetNumberOfElements() );
Vp = Values = new XdmfFloat64[ Length + 10 ];
Array.GetValues( 0, Values, Length );
ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_OPERATE( *=, ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 1,
    XDMF_ARRAY_IN, Length );
delete [] Values;
return( *this );
}

XdmfArray &
XdmfArray::operator*( XdmfFloat64 Value ){
XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp = &Value;

ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_OPERATE( *=, ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 0,
    XDMF_ARRAY_IN, this->GetNumberOfElements());
return( *this );
}

XdmfArray &
XdmfArray::operator/( XdmfArray &Array ){
XdmfInt64  Length;
XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp, *Values;

Length = MIN( this->GetNumberOfElements(), Array.GetNumberOfElements() );
Vp = Values = new XdmfFloat64[ Length + 10 ];
Array.GetValues( 0, Values, Length );
ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_OPERATE( /=, ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 1,
    XDMF_ARRAY_IN, Length );
delete [] Values;
return( *this );
}

XdmfArray &
XdmfArray::operator/( XdmfFloat64 Value ){
XdmfPointer  ArrayPointer;
XdmfFloat64  *Vp = &Value;

ArrayPointer = this->GetDataPointer();
XDMF_ARRAY_OPERATE( /=, ArrayPointer, this->GetNumberType(), 1,
    Vp, XDMF_FLOAT64_TYPE, 0,
    XDMF_ARRAY_IN, this->GetNumberOfElements());
return( *this );
}

XdmfArray *
XdmfArray::Clone( XdmfArray *Indexes ){

XdmfLength  i, Length, Size;
XdmfByte  *Ptr, *Source, *Target;
XdmfArray  *NewArray = new XdmfArray();
XdmfInt64  *IVals;


Length = Indexes->GetNumberOfElements();
IVals = new XdmfInt64[ Length + 10 ];
Indexes->GetValues( 0, IVals, Length );
NewArray->SetNumberType( this->GetNumberType() );
NewArray->SetNumberOfElements( Length );
Source = ( XdmfByte *)this->GetDataPointer();
Target = ( XdmfByte *)NewArray->GetDataPointer(0);
Size = this->GetElementSize();
for( i = 0 ; i < Length ; i++ ){
  Ptr = Source + ( Size * IVals[ i ] );
  memcpy( Target, Ptr, Size ); 
  Target += Size;
}
delete [] IVals;
return( NewArray );
}

XdmfArray *
XdmfArray::Clone( XdmfLength Start, XdmfLength End ) {

XdmfLength  Length;
XdmfArray  *NewArray = new XdmfArray();


if( ( Start == 0 ) && ( End == 0 )){
  NewArray->CopyType( this );
  NewArray->CopyShape( this );
  Length = this->GetNumberOfElements();
  End = Length - 1;
} else {
  if( End <= Start ) End = this->GetNumberOfElements() - Start - 1;
  Length = End - Start + 1;
  NewArray->SetNumberType( this->GetNumberType() );
  NewArray->SetNumberOfElements( Length );
}
memcpy( NewArray->GetDataPointer(0),
  this->GetDataPointer( Start ),
  Length * this->GetElementSize() );
return( NewArray );
}

XdmfArray *
XdmfArray::Reference( XdmfLength Start, XdmfLength End ) {

XdmfLength  Length;
XdmfArray  *NewArray = new XdmfArray();


if( End <= Start ) End = this->GetNumberOfElements() - Start - 1;
Length = End - Start + 1;
NewArray->SetNumberType( this->GetNumberType() );
NewArray->SetDataPointer( this->GetDataPointer( Start ) );
NewArray->SetNumberOfElements( Length );
return( NewArray );
}


XdmfFloat64
XdmfArray::GetMean( void ) {
XdmfLength  i, Length;
XdmfFloat64  Value, *Data;

Length = this->GetNumberOfElements();
Data = new XdmfFloat64[ Length + 10 ];
this->GetValues( 0, Data, Length);
Value = 0;
for( i = 0 ; i < Length ; i++ ){
  Value += Data[i];
  }
delete [] Data;
Value /= Length;
return( Value );
}

XdmfFloat64
XdmfArray::GetMaxAsFloat64( void ) {
XdmfLength  i, Length;
XdmfFloat64  Value, *Data;

Length = this->GetNumberOfElements();
Data = new XdmfFloat64[ Length + 10 ];
this->GetValues( 0, Data, Length);
Value = Data[0];
for( i = 0 ; i < Length ; i++ ){
  if ( Data[i] > Value ) {
    Value =  Data[i];
    }
  }
delete [] Data;
return( Value );
}

XdmfFloat64
XdmfArray::GetMinAsFloat64( void ) {
XdmfLength  i, Length;
XdmfFloat64  Value, *Data;

Length = this->GetNumberOfElements();
Data = new XdmfFloat64[ Length + 10 ];
this->GetValues( 0, Data, Length);
Value = Data[0];
for( i = 0 ; i < Length ; i++ ){
  if ( Data[i] < Value ) {
    Value =  Data[i];
    }
  }
delete [] Data;
return( Value );
}

XdmfInt64
XdmfArray::GetMinAsInt64( void ) {
return( static_cast<XdmfInt64>(this->GetMinAsFloat64()) );
}

XdmfInt64
XdmfArray::GetMaxAsInt64( void ) {
return( static_cast<XdmfInt64>(this->GetMaxAsFloat64()) );
}

XdmfInt32 XdmfArray::SetValue( XdmfInt64 Index, XdmfInt8 Value ) {
return(this->SetValueFromInt64( Index, Value ));
}
XdmfInt32 XdmfArray::SetValue( XdmfInt64 Index, XdmfInt32 Value ) {
return(this->SetValueFromInt64( Index, Value ));
}

XdmfInt32 XdmfArray::SetValue( XdmfInt64 Index, XdmfFloat32 Value ) {
return(this->SetValueFromFloat64( Index, Value ));
}

istrstream& ICE_READ_STREAM64(istrstream& istr, ICE_64_INT& i)
{
#ifdef ICE_HAVE_64BIT_STREAMS
istr >>i;
#else
{
double d; 
istr >> d;
i = d;
}
#endif
return istr;
}
