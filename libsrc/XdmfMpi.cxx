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
#include "XdmfMpi.h"

#ifndef NOMPI
#include "mpi.h"
#endif

#ifndef ABS
#define ABS(a) ((a) > 0 ? (a) : -(a) )
#endif

#ifndef NOMPI
static MPI_Datatype
MpiDataTypeFromXdmfArray( XdmfArray *Array ){

MPI_Datatype DataType;

switch ( Array->GetNumberType() ) {
  case XDMF_INT8_TYPE :
    DataType = MPI_CHAR;
    break;
  case XDMF_INT32_TYPE :
    DataType = MPI_INT;
    break;
  case XDMF_INT64_TYPE :
    DataType = MPI_UNSIGNED_LONG;
    break;
  case XDMF_FLOAT32_TYPE :
    DataType = MPI_FLOAT;
    break;
  case XDMF_FLOAT64_TYPE :
    DataType = MPI_DOUBLE;
    break;
  default :
    DataType = MPI_DATATYPE_NULL;
    break;
  }
return( DataType );
}
#endif

XdmfMpi::XdmfMpi( XdmfInt32 InitalizeMpi ) {
int  Zero = 0;

#ifndef NOMPI
  if( InitalizeMpi ) {
    MPI_Init( &Zero, NULL );
    }
  MPI_Comm_size(MPI_COMM_WORLD, &this->Size);
  MPI_Comm_rank(MPI_COMM_WORLD, &this->Id);
  this->TerminateMpi = InitalizeMpi;
  this->LastSourceId = -1;
  this->LastSourceTag = -1;
  this->SendTag = XDMF_MPI_TAG;
  this->RecvTag = MPI_ANY_TAG;
#endif
}

XdmfMpi::~XdmfMpi() {
  if( this->TerminateMpi ) {
#ifndef NOMPI
    MPI_Finalize();
#endif
    }
}


XdmfInt32
XdmfMpi::Send( XdmfArray *Array, int Target )
{
int    status;
XdmfInt32  ArrayLength;

#ifndef NOMPI
MPI_Datatype  DataType;

if ( Target >= this->GetSize() ){
  XdmfErrorMessage("Target " << Target << " is >= Size " << this->Size );
  }
DataType = MpiDataTypeFromXdmfArray( Array );
if( DataType == MPI_DATATYPE_NULL ){
  XdmfErrorMessage("UnSupported DataType" << Array->GetNumberType() );
  return( XDMF_FAIL );
  }

ArrayLength = Array->GetNumberOfElements();

  status = MPI_Send( &ArrayLength,
    1,
    MPI_INT,
    Target,
    this->SendTag,
    MPI_COMM_WORLD);

  status = MPI_Send( Array->GetDataPointer(),
    ArrayLength,
    DataType,
    Target,
    this->SendTag,
    MPI_COMM_WORLD);
#endif

return( status );
}

XdmfInt32
XdmfMpi::Broadcast( XdmfArray *Array, int Source )
{
int    status;
XdmfInt32  ArrayLength;

#ifndef NOMPI
MPI_Datatype  DataType;

if ( Source >= this->GetSize() ){
  XdmfErrorMessage("Source " << Source << " is >= Size " << this->Size );
  }
DataType = MpiDataTypeFromXdmfArray( Array );
if( DataType == MPI_DATATYPE_NULL ){
  XdmfErrorMessage("UnSupported DataType" << Array->GetNumberType() );
  return( XDMF_FAIL );
  }

  ArrayLength = Array->GetNumberOfElements();

  status = MPI_Bcast( &ArrayLength,
    1,
    MPI_INT,
    Source,
    MPI_COMM_WORLD );

  if( Source != this->Id ) {
    Array->SetNumberOfElements( ArrayLength );
    }

  status = MPI_Bcast( Array->GetDataPointer(),
    Array->GetNumberOfElements(),
    DataType,
    Source,
    MPI_COMM_WORLD );
#endif

return( status );
}

XdmfInt32
XdmfMpi::Recv( XdmfArray *Array, int Source )
{
int    status;

#ifndef NOMPI
MPI_Status  mpi_message_status;
MPI_Datatype  DataType;
XdmfInt32  ArrayLength;

if ( Source >= this->GetSize() ){
  XdmfErrorMessage("Source " << Source << " is >= Size " << this->Size );
  }
DataType = MpiDataTypeFromXdmfArray( Array );
if( DataType == MPI_DATATYPE_NULL ){
  XdmfErrorMessage("UnSupported DataType" << Array->GetNumberType() );
  return( XDMF_FAIL );
  }


if( Source < 0 ) Source = MPI_ANY_SOURCE;
status = MPI_Recv( &ArrayLength,
    1,
    MPI_INT,
    Source,
    this->RecvTag,
    MPI_COMM_WORLD,
    &mpi_message_status );
Array->SetNumberOfElements( ArrayLength );
status = MPI_Recv( Array->GetDataPointer(),
    ArrayLength,
    DataType,
    Source,
    this->RecvTag,
    MPI_COMM_WORLD,
    &mpi_message_status );
this->LastSourceId = mpi_message_status.MPI_SOURCE;
this->LastSourceTag = mpi_message_status.MPI_TAG;
#endif
return( status );
}

void
XdmfMpi::Barrier( void )
{
#ifndef NOMPI
MPI_Barrier(MPI_COMM_WORLD);
#endif
}

