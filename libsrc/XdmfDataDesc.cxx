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
#include "XdmfDataDesc.h"

XdmfDataDesc::XdmfDataDesc() {
  H5dont_atexit();
  this->DataSpace = H5I_BADID;
  this->DataType = H5Tcopy(H5T_NATIVE_FLOAT);
  this->SelectionType = XDMF_SELECTALL;
  this->NextOffset = 0;
  this->Rank = 1;
  }

XdmfDataDesc::~XdmfDataDesc() {
  H5Tclose( this->DataType );
  if(( this->DataSpace != H5S_ALL ) &&
      ( this->DataSpace != H5I_BADID )){
           H5Sclose( this->DataSpace );
    this->DataSpace = H5I_BADID;
        }
  }

XdmfInt32
XdmfDataDesc::GetHyperSlab( XdmfInt64 *Start, XdmfInt64 *Stride, XdmfInt64 *Count ) {

XdmfInt32  i, Rank = this->Rank;

if( this->SelectionType != XDMF_HYPERSLAB ){
  return( XDMF_FAIL );
  }
for( i = 0 ; i < Rank ; i++ ){
  if( Start ) {
    *Start++ = this->Start[i];
    }
  if( Stride ) {
    *Stride++ = this->Stride[i];
    }
  if( Count ) {
    *Count++ = this->Count[i];
    }
  }
return( Rank );
}

XdmfString
XdmfDataDesc::GetHyperSlabAsString( void ){
ostrstream   StringOutput;
char    *Ptr;
static char   *Result = NULL;
XdmfInt32  i, Rank;
XdmfInt64  Start[ XDMF_MAX_DIMENSION ];
XdmfInt64  Stride[ XDMF_MAX_DIMENSION ];
XdmfInt64  Count[ XDMF_MAX_DIMENSION ];

Rank = this->GetHyperSlab( Start, Stride, Count );
if( Rank == XDMF_FAIL ) {
  return( NULL );
  }
for( i = 0 ; i < Rank ; i++ ){
  StringOutput << ICE_64BIT_CAST Start[i] << " ";
  }
for( i = 0 ; i < Rank ; i++ ){
  StringOutput << ICE_64BIT_CAST Stride[i] << " ";
  }
for( i = 0 ; i < Rank ; i++ ){
  StringOutput << ICE_64BIT_CAST Count[i] << " ";
  }
StringOutput << ends;
Ptr = StringOutput.str();
if( Result != NULL ) delete [] Result;
Result = new char[ strlen( Ptr ) + 2 ];
strcpy( Result, Ptr );
delete [] Ptr;
return( Result );
return(NULL);
}

XdmfInt32
XdmfDataDesc::CopySelection( XdmfDataDesc *Desc ){

if( Desc->SelectionType == XDMF_SELECTALL ) { 
  return( XDMF_SUCCESS );
  }
if( Desc->GetSelectionType() == XDMF_HYPERSLAB ){
  XdmfInt64  Start[ XDMF_MAX_DIMENSION ];
  XdmfInt64  Stride[ XDMF_MAX_DIMENSION ];
  XdmfInt64  Count[ XDMF_MAX_DIMENSION ];
  
  Rank = Desc->GetHyperSlab( Start, Stride, Count );
  this->SelectHyperSlab( Start, Stride, Count );
} else {
  XdmfInt64  NumberOfCoordinates;
  XdmfInt64  *Coordinates;


  NumberOfCoordinates = Desc->GetSelectionSize();
  Coordinates = Desc->GetCoordinates();
  this->SelectCoordinates( NumberOfCoordinates, Coordinates );
  delete Coordinates;
  }
return( XDMF_SUCCESS );
}


XdmfInt32
XdmfDataDesc::CopyShape( XdmfDataDesc *DataDesc ) {
XdmfInt32  Status;

Status = this->CopyShape( DataDesc->GetDataSpace() );
// Do This Manually
// if( Status == XDMF_SUCCESS ){
//   Status = this->CopySelection( DataDesc );
//    }
return( Status );
};

XdmfInt32
XdmfDataDesc::CopyType( hid_t DataType ) {
if( this->DataType != H5I_BADID ) {
  H5Tclose( this->DataType );
  }
this->DataType = H5Tcopy( DataType );
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfDataDesc::CopyShape( hid_t DataSpace ) {

hsize_t      HRank;
hsize_t      HDimension[ XDMF_MAX_DIMENSION ];
XdmfInt64    i, Dimensions[ XDMF_MAX_DIMENSION ];
XdmfInt32    Status;

HRank = H5Sget_simple_extent_ndims( DataSpace );
H5Sget_simple_extent_dims( DataSpace, HDimension, NULL );
for( i = 0 ; i < HRank ; i++ ){
  Dimensions[i] = HDimension[i];
  }
Status =  this->SetShape( HRank, Dimensions );
if( Status > 0 ){
  return( XDMF_SUCCESS );
  }
return( XDMF_FAIL );
}


XdmfInt32
XdmfDataDesc::GetShape( XdmfInt64 *Dimensions ) {
hsize_t      HRank;
hsize_t      HDimension[ XDMF_MAX_DIMENSION ];
XdmfInt64    i;

HRank = H5Sget_simple_extent_ndims( DataSpace );
H5Sget_simple_extent_dims( DataSpace, HDimension, NULL );
for( i = 0 ; i < HRank ; i++ ){
  Dimensions[i] = HDimension[i];
  }
return( HRank );
}

XdmfString
XdmfDataDesc::GetShapeAsString( void ) {
ostrstream   StringOutput;
char    *Ptr;
static char   *Result = NULL;
XdmfInt64  i, Rank, Dimensions[ XDMF_MAX_DIMENSION ];

Rank = this->GetShape( Dimensions );
for( i = 0 ; i < Rank ; i++ ){
  StringOutput << ICE_64BIT_CAST Dimensions[i] << " ";
  }
StringOutput << ends;
Ptr = StringOutput.str();
if( Result != NULL ) delete [] Result;
Result = new char[ strlen( Ptr ) + 2 ];
strcpy( Result, Ptr );
delete [] Ptr;
return( Result );
}

XdmfInt32
XdmfDataDesc::SetShape( XdmfInt32 Rank, XdmfInt64 *Dimensions ) {
XdmfInt32    i;
hsize_t      HRank;
hsize_t      HDimension[ XDMF_MAX_DIMENSION ];

if( this->Rank <= 0 ) {
  return( XDMF_FAIL );
  }


if( ( this->DataSpace == H5I_BADID )  || ( this->DataSpace == H5S_ALL ) ) {
  XdmfDebug("Createing new HDF5 DataSpace");
  this->DataSpace = H5Screate( H5S_SIMPLE );
  if( this->DataSpace < 0 ) {
    XdmfErrorMessage( "Can't Create  DataSpace" );
    return( H5I_BADID );
    }
} else {
  HRank = H5Sget_simple_extent_ndims( this->DataSpace );
  if( HRank != Rank ){
    XdmfDebug( "Current Rank " << (int)HRank << " Requested Rank " << Rank );
    XdmfDebug( "Data Space Rank Change After Creation" );
  // Work around for ?bug? in HDF
        // when you increase rank
  if(( this->DataSpace != H5S_ALL ) &&
      ( this->DataSpace != H5I_BADID )){
  XdmfDebug("Closing Space");
           H5Sclose( this->DataSpace );
    this->DataSpace = H5I_BADID;
        }
  this->DataSpace = H5Screate( H5S_SIMPLE );
  if( this->DataSpace < 0 ) {
    XdmfErrorMessage( "Can't Create  DataSpace" );
    return( H5I_BADID );
    }
  }
}

this->Rank = HRank = Rank;
XdmfDebug("Shape : Rank = " << (int)HRank);
for( i = 0 ; i < Rank ; i++ ) {
  XdmfDebug("  Dimension[" << i << "] = " << ICE_64BIT_CAST Dimensions[i] );
  this->Count[i] = this->Dimension[i] = HDimension[i] = Dimensions[i];
  this->Start[i] = 0;
  this->Stride[i] = 1;
  }

H5Sset_extent_simple( this->DataSpace, HRank, HDimension, NULL );
H5Sselect_all( this->DataSpace );

XdmfDebug("Finished Setting Shape");
return( this->DataSpace );
}

XdmfInt32
XdmfDataDesc::SelectAll( void ) {
XdmfInt32 i;

  // Update Rank and Dimensions
  this->GetNumberOfElements();

for( i = 0 ; i < this->Rank ; i++ ) {
  this->Start[i] = 0;
  this->Stride[i] = 1;
  this->Count[i] = this->Dimension[i];
  }
  
  H5Sselect_all( this->DataSpace );
  this->SelectionType = XDMF_SELECTALL;
  return( XDMF_SUCCESS );
}

XdmfInt32
XdmfDataDesc::SelectHyperSlab(  XdmfInt64 *Start, XdmfInt64 *Stride, XdmfInt64 *Count ) {

XdmfInt32  i;
XdmfInt64  Dimensions[ XDMF_MAX_DIMENSION ];
herr_t    status;

this->GetShape( Dimensions );
for( i = 0 ; i < this->Rank ; i++ ) {
  if( Start ) {
    this->Start[i] = Start[ i ];  
  } else {
    this->Start[i] = 0;
  }
  if( Stride ){
    this->Stride[i] = Stride[ i ];  
  } else {
    this->Stride[i] = 1;
  }
  if( Count ) {
    this->Count[i] = Count[ i ];  
  } else {
    this->Count[i] = (( Dimensions[i] - this->Start[i] - 1 ) / this->Stride[i]) + 1;
  }
  XdmfDebug("Dim[" << i << "] = " << ICE_64BIT_CAST this->Dimension[i]  << 
    " Start Stride Count = " <<
    ICE_64BIT_CAST this->Start[i] << " " <<
    ICE_64BIT_CAST this->Stride[i] << " " <<
    ICE_64BIT_CAST this->Count[i] );
  }
this->SelectionType = XDMF_HYPERSLAB;
status = H5Sselect_hyperslab( this->DataSpace,
       H5S_SELECT_SET,
       this->Start, this->Stride, this->Count,
       NULL);
if( status >= 0 ) {
  return( XDMF_SUCCESS );
}
return( XDMF_FAIL );
}

XdmfInt32
XdmfDataDesc::SelectCoordinates(  XdmfInt64 NumberOfElements, XdmfInt64 *Coordinates ){
hssize_t  *HCoordinates;
XdmfInt32  status;
XdmfInt64  i, Rank = this->Rank;
hssize_t  Length = NumberOfElements * Rank;
hssize_t  NElements = Length / MAX( 1, Rank );


if( this->Rank <= 0 ) {
        return( XDMF_FAIL );
        }

this->SelectionType = XDMF_COORDINATES;
XdmfDebug(" Selecting " << (int)NElements << " elements" );
HCoordinates = new hssize_t[ Length ];
for( i = 0 ; i < Length ; i++ ){
  HCoordinates[i] = Coordinates[i];
  }
status = H5Sselect_elements( this->DataSpace,
        H5S_SELECT_SET,
         NElements,
         ( const hssize_t **)HCoordinates);
if( status < 0 ) {
  return( XDMF_FAIL );
}
return ( XDMF_SUCCESS );
}

XdmfInt64
XdmfDataDesc::GetNumberOfElements( void ) {
hsize_t    i, Dimensions[ XDMF_MAX_DIMENSION ];
XdmfInt64  Nelements = 0;
XdmfInt32  Rank = H5Sget_simple_extent_ndims(this->DataSpace );

this->Rank = Rank;
H5Sget_simple_extent_dims( this->DataSpace, Dimensions, NULL );
if(Rank) {
  Nelements = this->Dimension[0] = Dimensions[0];
  for( i = 1 ; i < Rank ; i++ ){
    this->Dimension[i] = Dimensions[i];
    Nelements *= Dimensions[i];
    }
}
return( Nelements );
}

XdmfInt64
XdmfDataDesc::GetElementSize( void ) {
  return( H5Tget_size( this->DataType ) );
}

XdmfInt64
XdmfDataDesc::GetSelectionSize( void ) {
  return( H5Sget_select_npoints( this->DataSpace ) );
}

XdmfInt32
XdmfDataDesc::SelectHyperSlabFromString(  XdmfString Start, XdmfString Stride, XdmfString Count ) {
XdmfInt64  i, HStart[XDMF_MAX_DIMENSION], HStride[XDMF_MAX_DIMENSION], HCount[XDMF_MAX_DIMENSION];
istrstream   Start_ist(Start, strlen( Start ) );
istrstream   Stride_ist(Stride, strlen( Stride ) );
istrstream   Count_ist(Count, strlen( Count ) );

for( i = 0 ; i < this->Rank ; i++ ){
  if( Start ){
    ICE_READ_STREAM64(Start_ist, HStart[i]);
  } else {
    HStart[i] = 0;
    }
  if( Stride ){
    ICE_READ_STREAM64(Stride_ist, HStride[i]);
  } else {
    HStride[i] = 1;
  }
  if( Count ){
    ICE_READ_STREAM64(Count_ist, HCount[i]);
  } else {
    HCount[i] = (this->Dimension[i] - HStart[i]) / HStride[i];
  }
}
return( this->SelectHyperSlab( HStart, HStride, HCount ) );
}

XdmfInt32
XdmfDataDesc::SetShapeFromString( const XdmfString String ) {
  XdmfLength      i = 0, count = 0;
  istrstream   ist(String, strlen( String ) );
  istrstream   counter(String, strlen( String ) );
  XdmfInt64  dummy;

  while( ICE_READ_STREAM64(counter, dummy) ) count++;
  this->Rank = count;
  while( ICE_READ_STREAM64(ist,dummy) ){
          this->Dimension[i] = dummy;
          i++;
          }
  XdmfDebug("String Contains " << this->Rank << " Dimensions" );
  return( this->SetShape( this->Rank, this->Dimension ) );

}

XdmfInt32
XdmfDataDesc::SelectCoordinatesFromString( XdmfString String ) {
  XdmfInt32  Status;
  XdmfLength      i = 0, count = 0;
  istrstream   ist(String, strlen( String ) );
  istrstream   counter(String, strlen( String ) );
  XdmfInt64  dummy, *Coordinates;

  while( ICE_READ_STREAM64(counter, dummy) ) count++;
  Coordinates = new XdmfInt64[ count + 1 ];
  while( ICE_READ_STREAM64(ist, dummy) ){
   Coordinates[i] = dummy;
          i++;
          }
  XdmfDebug("String Contains " << ICE_64BIT_CAST count << " Coordinates" );
  Status = this->SelectCoordinates( count / this->Rank, Coordinates );
  delete [] Coordinates;
  return( Status );

}

XdmfInt32
XdmfDataDesc::SetNumberType( XdmfInt32 NumberType, XdmfInt64 CompoundSize ) {

if( this->DataType != H5I_BADID ) {
  H5Tclose( this->DataType );
  }
if( NumberType == XDMF_COMPOUND_TYPE ) {
   this->DataType = H5Tcreate( H5T_COMPOUND, CompoundSize );
} else {
  this->DataType =  H5Tcopy( XdmfTypeToHDF5Type( NumberType ) );
}
if ( this->DataType < 0 ) {
  XdmfErrorMessage("Error Creating Data Type");
  this->DataType = H5I_BADID;
  return( XDMF_FAIL );
  }
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfDataDesc::SetNumberTypeFromString( XdmfString NumberTypeString, XdmfInt64 CompoundSize ) {
XdmfInt32 NumberType;

NumberType = StringToXdmfType( NumberTypeString );
return( this->SetNumberType( NumberType, CompoundSize ) );
}

XdmfInt32
XdmfDataDesc::GetNumberType( void ) {
  return( HDF5TypeToXdmfType( this->DataType ) );
}

XdmfString
XdmfDataDesc::GetNumberTypeAsString() {

XdmfInt32  Type;

Type = HDF5TypeToXdmfType( this->DataType );
if( Type > 0 ){
  return( XdmfTypeToString( Type ) );
}
return("UNKNOWN");
}

XdmfInt64
XdmfDataDesc::GetNumberOfMembers( void ) {

return( H5Tget_nmembers( this->DataType ) );
}

XdmfInt32
XdmfDataDesc::GetMemberType( XdmfInt64 Index ) {
XdmfInt32  RetVal;
hid_t    DataType;

if( Index >  ( H5Tget_nmembers( this->DataType ) - 1 ) ){
  XdmfErrorMessage("Index is Greater than Number of Members");
  return( NULL );
  }
DataType =  H5Tget_member_type( this->DataType, Index );
RetVal = HDF5TypeToXdmfType( DataType ); 
if( RetVal == XDMF_COMPOUND_TYPE ) {
  hid_t  Super;

  Super = H5Tget_super( DataType );
  RetVal = HDF5TypeToXdmfType( Super );
}
H5Tclose( DataType );
return( RetVal );
}

XdmfString
XdmfDataDesc::GetMemberTypeAsString( XdmfInt64 Index ) {

return( XdmfTypeToString( this->GetMemberType( Index ) ) );
}

XdmfInt64
XdmfDataDesc::GetMemberOffset( XdmfInt64 Index ) {
XdmfInt64  RetVal;

if( Index >  ( H5Tget_nmembers( this->DataType ) - 1 ) ){
  XdmfErrorMessage("Index is Greater than Number of Members");
  return( NULL );
  }
RetVal =  H5Tget_member_offset( this->DataType, Index );
return( RetVal );
}

XdmfInt32
XdmfDataDesc::GetMemberShape( XdmfInt64 Index, XdmfInt64 *Dimensions ) {
XdmfInt32  i, Rank, DataType;
hsize_t     Dims[XDMF_MAX_DIMENSION];

if( Index >  ( H5Tget_nmembers( this->DataType ) - 1 ) ){
  XdmfErrorMessage("Index is Greater than Number of Members");
  return( NULL );
  }
DataType =  H5Tget_member_type( this->DataType, Index );
if( HDF5TypeToXdmfType(DataType) == XDMF_COMPOUND_TYPE ) {
  Rank = H5Tget_array_ndims( DataType );
  if( Rank <= 0 ){
    return( XDMF_FAIL );
    }
  H5Tget_array_dims(DataType, Dims, NULL );
  for( i = 0 ; i < Rank ; i++ ){
    Dimensions[i] = Dims[i];
  }
} else {
  Rank =  1;
  Dimensions[0] = 1;
}
return( Rank );
}

XdmfString
XdmfDataDesc::GetMemberShapeAsString( XdmfInt64 Index ){

static char    ReturnString[ 80 ];
ostrstream  ReturnStream( ReturnString, 80 );
XdmfInt64  Dimensions[XDMF_MAX_DIMENSION];
XdmfInt32  i, Rank;

Rank = this->GetMemberShape( Index, Dimensions );
if( Rank == XDMF_FAIL ) {
  XdmfErrorMessage("Error Getting Member Shape");
  return( NULL );
  }
ReturnString[0] = '0';
for( i = 0 ; i < Rank ; i++ ){
  ReturnStream << ICE_64BIT_CAST Dimensions[i] << " ";
  }
ReturnStream << ends;
return( ReturnString );
}

XdmfInt64
XdmfDataDesc::GetMemberLength( XdmfInt64 Index ) {
XdmfInt64  Length, Dimensions[XDMF_MAX_DIMENSION];
XdmfInt32  i, Rank;

Rank = this->GetMemberShape( Index, Dimensions );
if( Rank == XDMF_FAIL ) {
  XdmfErrorMessage("Error Getting Member Shape");
  return( 0 );
  }
Length = 1;
for( i = 0 ; i < Rank ; i++ ){
  Length *= Dimensions[i];
  }
return( Length );
}

XdmfInt64
XdmfDataDesc::GetMemberSize( XdmfInt64 Index ) {
hsize_t    Length;
XdmfInt64  RetVal = 1;
//hsize_t     Dims[XDMF_MAX_DIMENSION];

if( Index >  ( H5Tget_nmembers( this->DataType ) - 1 ) ){
  XdmfErrorMessage("Index is Greater than Number of Members");
  return( 0 );
  }
Length =  H5Tget_size( H5Tget_member_type( this->DataType, Index ) );
if( Length <= 0 ){
  XdmfErrorMessage("Error Getting Length");
  RetVal = XDMF_FAIL;
} else {
  RetVal = Length;
}
return( RetVal );
}

XdmfString
XdmfDataDesc::GetMemberName( XdmfInt64 Index ) {

char     *Name;
static char  MemberName[256];

if( Index >  ( H5Tget_nmembers( this->DataType ) - 1 ) ){
  XdmfErrorMessage("Index is Greater than Number of Members");
  return( NULL );
  }
Name = H5Tget_member_name( this->DataType, Index );
strcpy( MemberName, Name );
free( Name );
return( MemberName );
}



XdmfInt32
XdmfDataDesc::AddCompoundMemberFromString( XdmfString Name,
    XdmfString NumberTypeString,
    XdmfString Shape,
    XdmfInt64 Offset ){

XdmfInt32  i, Rank = 0, NumberType;
XdmfInt64  Dim, Dimensions[XDMF_MAX_DIMENSION];
istrstream  ShapeString( Shape, strlen(Shape) );

NumberType = StringToXdmfType( NumberTypeString );
i = 0;
while( ICE_READ_STREAM64(ShapeString, Dim) ){
  Rank++;
  Dimensions[i++] = Dim;
  }
return( this->AddCompoundMember( Name, NumberType, Rank, Dimensions, Offset) );
}

XdmfInt32
XdmfDataDesc::AddCompoundMember( XdmfString Name,
    XdmfInt32 NumberType,
    XdmfInt32 Rank,
    XdmfInt64 *Dimensions,
    XdmfInt64 Offset ){

herr_t    status;
XdmfInt32  i, HNumberType;
XdmfInt64  One = 1;
XdmfInt64  Size;

if( Offset == 0 ){
  Offset = this->NextOffset;
  }
if( Dimensions == NULL ){
  Dimensions = &One;
  }
XdmfDebug("Inserting " << Name << " at Offset " << ICE_64BIT_CAST Offset << " as type " << XdmfTypeToString( NumberType ) );
if( this->GetNumberType() != XDMF_COMPOUND_TYPE ){
  this->SetNumberType( XDMF_COMPOUND_TYPE );
  }
HNumberType = XdmfTypeToHDF5Type( NumberType);
Size = H5Tget_size( HNumberType );
if( ( Rank == 1 ) && ( *Dimensions == 1 ) ){
  status = H5Tinsert( this->DataType,
    Name,
    Offset,
    HNumberType );
} else {
  hsize_t    Dims[ XDMF_MAX_DIMENSION ];

  for( i = 0; i < Rank ; i++ ){
    Dims[i] = Dimensions[i];
    }
  status = H5Tinsert( this->DataType,
    Name,
    Offset,
    H5Tarray_create( HNumberType, Rank, Dims, NULL ));
}
if( status < 0 ){
  return( XDMF_FAIL );
  }
for( i = 0 ; i < Rank ; i++ ){
  Size *= Dimensions[i];
  }
this->NextOffset += Size;
return( XDMF_SUCCESS );
}

XdmfString
XdmfDataDesc::GetSelectionTypeAsString( void ) {

switch( this->SelectionType ) {
  case XDMF_HYPERSLAB :
    return("XDMF_HYPERSLAB");
    break;
  case XDMF_COORDINATES :
    return("XDMF_COORDINATES");
    break;
  default :
    return( "Unknown" );
    break;
  }
}

XdmfInt64 *
XdmfDataDesc::GetCoordinates( XdmfInt64 Start, XdmfInt64 Nelements ){

XdmfInt64 i, Total, *Coordinates = NULL;
XdmfInt32 Rank = H5Sget_simple_extent_ndims(this->DataSpace );

if( this->SelectionType == XDMF_COORDINATES ){
  hsize_t  *HCoordinates;
  if( Nelements <= 0 ){
    Nelements = H5Sget_select_elem_npoints( this->DataSpace );
  }
  if ( Nelements > 0 ) {
    Total = Nelements * Rank;
    HCoordinates = new hsize_t[ Total ];
    Coordinates = new XdmfInt64[ Total ];
    H5Sget_select_elem_pointlist( this->DataSpace, Start, Nelements, HCoordinates);
    for( i = 0 ; i < Total ; i++ ){
      Coordinates[i] = HCoordinates[i];
      }
    delete HCoordinates;
    }
}
return( Coordinates );
}

XdmfString
XdmfDataDesc::GetCoordinatesAsString( XdmfInt64 Start, XdmfInt64 Nelements ){

hsize_t    i;
XdmfInt32 Rank = H5Sget_simple_extent_ndims(this->DataSpace );

ostrstream StringOutput;
char  *Ptr;
static char *Result = NULL;


if( this->SelectionType == XDMF_COORDINATES ){
  if( Nelements <= 0 ){
    Nelements = H5Sget_select_elem_npoints( this->DataSpace );
  }
  if ( Nelements > 0 ) {
    hsize_t *Coords = new hsize_t[ Nelements * Rank ];
    hsize_t j, *Cp = Coords;
    H5Sget_select_elem_pointlist( this->DataSpace, Start, Nelements, Coords );
    for( i = 0 ; i < Nelements ; i++ ){
      for( j = 0 ; j < Rank ; j++ ){
        StringOutput << (int)*Cp++ << " ";
        }
      }  
    delete [] Coords;
    }
}
Ptr = StringOutput.str();
if( Result != NULL ) delete [] Result;
Result = new char[ strlen( Ptr ) + 2 ];
strcpy( Result, Ptr );
delete [] Ptr;
return( Result );
}

void
XdmfDataDesc::Print( void ){

hsize_t  i, Dimensions[ XDMF_MAX_DIMENSION ];
hsize_t  Nelements;
XdmfInt32 Rank = H5Sget_simple_extent_ndims(this->DataSpace );

cout << "Rank " << Rank << endl;
H5Sget_simple_extent_dims( this->DataSpace, Dimensions, NULL );
for( i = 0 ; i < Rank ; i++ ){
  cout << "Dimansion[" << (int)i << "] " << (int)Dimensions[i] << endl;
  }
cout << "Selection Type : " << this->GetSelectionTypeAsString() << endl;
if( this->SelectionType == XDMF_COORDINATES ){
Nelements = H5Sget_select_elem_npoints( this->DataSpace );
cout << "Selected Elements : " << (int)Nelements << endl;
if ( Nelements > 0 ) {
  hsize_t *Coords = new hsize_t[ Nelements * Rank ];
  hsize_t j, *Cp = Coords;
  H5Sget_select_elem_pointlist( this->DataSpace, 0, Nelements, Coords );
  for( i = 0 ; i < Nelements ; i++ ){
    cout << "Element[" << (int)i << "] ";
    for( j = 0 ; j < Rank ; j++ ){
      cout << " " << (int)*Cp++;
      }
    cout << endl;
    }  
  delete [] Coords;
  }
}
if( this->SelectionType == XDMF_HYPERSLAB ){
int  k;
for( k = 0 ; k < Rank ; k++ ){
  cout << k << " : Start " << (int)Start[k] << " Stride " <<
    (int)Stride[k] << " Count " << (int)Count[k] << endl;
  }
}
}
