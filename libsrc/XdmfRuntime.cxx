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
#include "XdmfRuntime.h"

#include "XdmfHDF.h"
#include "XdmfCharArray.h"
#include "XdmfDOM.h"

XdmfRuntime::XdmfRuntime() {
  this->ParseBufferArray = new XdmfCharArray;
  this->DOM = new XdmfDOM;
  this->CycleFrequency = 0;
  this->LastCycle = 0;
  this->TimeStepFrequency = 0;
  this->LastTimeStep = 0;
  this->BarrierMode = XDMF_FREE_RUN;

  this->SetNdgmMode( XDMF_NDGM_MSG_SERVER );
}

XdmfRuntime::~XdmfRuntime() {
  if( this->ParseBufferArray ) delete ParseBufferArray;
  if( this->DOM ) delete DOM;
}

XdmfInt32
XdmfRuntime::Init( XdmfInt32 IdOffset ) {
XdmfInt32 Status;

Status = this->Open( IdOffset );
return( Status );
}

XdmfInt32
XdmfRuntime::Term( void ) {
  this->Close();
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfRuntime::Check( XdmfInt32 Cycle, XdmfFloat64 TimeStep ) {

XdmfInt32  Status;

// First Check NDGM
Status = this->XdmfNDGM::Check( );
XdmfDebug("NDGM Check = " << Status );
if( Status > 0 ){
  XdmfDebug("NDGM Ready");
  this->Recv( this->ParseBufferArray );
  return( XDMF_SUCCESS );
}
//
if( ( Cycle > 0 ) && ( this->CycleFrequency > 0 ) && ( ( Cycle % this->CycleFrequency ) == 0 )){
  XdmfDebug("Cycle Ready");
  this->LastCycle = Cycle;
  this->LastTimeStep = TimeStep;
  return( XDMF_SUCCESS );
  }
if( ( TimeStep > 0 ) && ( this->TimeStepFrequency > 0 ) ) {
  if( ( TimeStep - this->LastTimeStep ) >= this->TimeStepFrequency ){
    XdmfDebug("TimeStep Ready");
    this->LastCycle = Cycle;
    this->LastTimeStep = TimeStep;
    return( XDMF_SUCCESS );
    }
  }
return( XDMF_FAIL );
}

XdmfInt32
XdmfRuntime::UpdateTime( XdmfString CycleName, XdmfString TimeName ) {

if( TimeName != NULL ) {
  XdmfHDF      Hdf;
  XdmfArray    Time( XDMF_FLOAT32_TYPE );

  XdmfDebug("Update Time in " << TimeName );
  Hdf.SetAccess("rw");
  if( Hdf.Open( TimeName ) == XDMF_FAIL ){
    XdmfErrorMessage("Can't Open DataSet Named <" << 
      TimeName << ">");
    return( XDMF_FAIL );
    }
  Time.SetNumberOfElements( 1 );
  Time.SetValue( 0, this->GetLastTimeStep() );
  Hdf.Write( &Time );
  Hdf.Close();
} 
if( CycleName != NULL ) {
  XdmfHDF      Hdf;
  XdmfArray    Cycle( XDMF_INT32_TYPE );

  XdmfDebug("Update Cycle in " << CycleName );
  Hdf.SetAccess("rw");
  if( Hdf.Open( CycleName ) == XDMF_FAIL ){
    XdmfErrorMessage("Can't Open DataSet Named <" << 
      CycleName << ">");
    return( XDMF_FAIL );
    }
  Cycle.SetNumberOfElements( 1 );
  Cycle.SetValue( 0, this->GetLastCycle() );
  // Hdf is already set to All
  // so no SetSlabToAll is Necessary
  Hdf.Write( &Cycle );
  Hdf.Close();
} 
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfRuntime::ParseBuffer( XdmfString Buffer )
{
XdmfDebug("Parsing Buffer");
if( Buffer == NULL ) Buffer = (XdmfString)this->ParseBufferArray->GetDataPointer();

if( this->DOM ) delete this->DOM;
this->DOM = new XdmfDOM;
this->DOM->Parse( Buffer );
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfRuntime::ParseFile( XdmfConstString FileName ) {

XdmfInt32 Status;

Status = this->ParseBufferArray->SetFromFile( FileName );
if( Status <= 0 ){
  XdmfErrorMessage("Can't open file " << FileName );
  return( XDMF_FAIL );
  }
return( this->ParseBuffer() );
}

