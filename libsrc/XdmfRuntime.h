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
#ifndef __XdmfRuntime_h
#define __XdmfRuntime_h

#include "XdmfNDGM.h"

#define XDMF_FREE_RUN    0
#define XDMF_SAFE    1
#define XDMF_DEFAULT_BARRIER  20

/*!
The Object for integrating a running code
with the XDMF Object Directory. The user
supplies a "Parse" routine that uses the XdmfDOM
object and can access the code's internal data values.
This object is used to listen for requests and set up
the DOM so the parse routine can just walk the tree
*/

class XdmfCharArray;
class XdmfDOM;

class XdmfRuntime : public XdmfNDGM {

public:
  XdmfRuntime();
  ~XdmfRuntime();

  const char * GetClassName() { return ( "XdmfRuntime" ) ; };

//! Set/Get Barrier Mode
  XdmfSetValueMacro( BarrierMode, XdmfInt32 );
  XdmfGetValueMacro( BarrierMode, XdmfInt32 );

//! Retreive the Associated DOM Object
  XdmfGetValueMacro(DOM, XdmfDOM *);

//! Get the Buffer for the raw XML Data
  XdmfGetValueMacro(ParseBufferArray, XdmfCharArray *);

/*!
Get the Iteration Frequency
The object automaically will require an
update every n'th iteration : 
if ( Iteration mod Frequency ) == 0
*/
  XdmfGetValueMacro(CycleFrequency, XdmfInt64);
//! Set the Iteration Frequency 
  XdmfSetValueMacro(CycleFrequency, XdmfInt64);
/*!
Get the TimeCycle Frequency
the object automatically will requre an 
update every time the current "time" 
as determined by the code is over the
next Frequency multiple
*/
  XdmfGetValueMacro(TimeStepFrequency, XdmfFloat64);
//! Set the TimeCycle Frequency
  XdmfSetValueMacro(TimeStepFrequency, XdmfFloat64);

//! Get the Last Iteration an update was requested
  XdmfGetValueMacro(LastCycle, XdmfInt64);
//! Set the Last Iteration an update was requested
  XdmfSetValueMacro(LastCycle, XdmfInt64);
//! Get the Last Time an update was requested
  XdmfGetValueMacro(LastTimeStep, XdmfFloat64);
//! Set the Last Time an update was requested
  XdmfSetValueMacro(LastTimeStep, XdmfFloat64);

//! Initialize the listening connection
  XdmfInt32  Init( XdmfInt32  IdOffset = 1 );
//! Check for update requests
  XdmfInt32  Check( XdmfInt32 Cycle = -1, XdmfFloat64 TimeStep = -1 );
//! Gracefully Clean up
  XdmfInt32  Term( void );
//! Update from XML in a Disk File
  XdmfInt32  ParseFile( XdmfString FileName = "XdmfRuntime.xml" );
//! Update from XML in a char *
  XdmfInt32  ParseBuffer( XdmfString Buffer = NULL  );

//! Signal Completed Update in Free Run or Safe Mode
  XdmfInt32  SignalComplete( XdmfInt32 Barrier = XDMF_DEFAULT_BARRIER, XdmfInt32 Mode = -1 ) {
#ifdef HAVE_NDGM
    XdmfInt32 Status;
    XdmfDebug("Clearing Barrier " << Barrier << endl);
    Status = ndgm_barrier_init(Barrier, -1);
    if( Mode < 0 ) Mode = this->BarrierMode;
    if ( Mode == XDMF_SAFE ) {
      XdmfDebug("Waiting in Barrier " << Barrier << endl);
      Status = ndgm_barrier_wait(Barrier);
    }
    XdmfDebug("Status = " << Status << endl );
    return( Status );
#else
    // This allows for ICE Aware Applications without NDGM
    return(XDMF_SUCCESS);
#endif
    }

//! Update the Time/Iteration Scalar Values
  XdmfInt32  UpdateTime( XdmfString CycleName = NULL , XdmfString TimeName = NULL );

protected:
  XdmfCharArray  *ParseBufferArray;
  XdmfDOM    *DOM;
  XdmfInt32  BarrierMode;
  XdmfInt64  CycleFrequency;
  XdmfInt64  LastCycle;
  XdmfFloat64  TimeStepFrequency;
  XdmfFloat64  LastTimeStep;
};

#endif // __XdmfRuntime_h
