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
#ifndef __XdmfMpi_h
#define __XdmfMpi_h

#include "XdmfArray.h"
#include "XdmfObject.h"


#define XDMF_MPI_MAX_MSG_SIZE 1024
#define XDMF_MPI_TAG    10

/*!
A Convenience object for dealing with MPI
This mainly exists so that we can easily
write MPI programs in Scripting Languages
*/
class XdmfMpi : public XdmfObject {

public:
//! Get a New onject and call MPI_INIT() by default
  XdmfMpi( XdmfInt32 InitalizeMpi = 1 );
//! This will call MPI_FINALIZE() is InitalizeMpi = 1
  ~XdmfMpi();

  const char * GetClassName() { return ( "XdmfMpi" ) ; };

//! Get the Node Id of the Last Message Received
   XdmfGetValueMacro( LastSourceId, int );
//! Get the Tag of the Last Message Received
   XdmfGetValueMacro( LastSourceTag, int );

//! Get this Node Id
   XdmfGetValueMacro( Id, int );
//! Get this number of nodes in the MPI World
   XdmfGetValueMacro( Size, int );

//! Get Flag to Manually stop MPI on delete
   XdmfGetValueMacro( TerminateMpi, int );
//! Set Flag to Manually stop MPI on delete
   XdmfSetValueMacro( TerminateMpi, int );

//! Get the Tag of the Current Message for Send
   XdmfGetValueMacro( SendTag, int );
//! Set the Tag of the Current Message for Send
   XdmfSetValueMacro( SendTag, int );
//! Get the Tag of the Current Message for Rec
   XdmfGetValueMacro( RecvTag, int );
//! Set the Tag of the Current Message for Rec
   XdmfSetValueMacro( RecvTag, int );
  
//! Check into a Barrier
  void    Barrier( void );
//! Send an Array
  XdmfInt32  Send( XdmfArray *Array, int Target );
//! Broadcast an Array
  XdmfInt32  Broadcast( XdmfArray *Array, int Source = 0 );
//! Recv an Array
  XdmfInt32  Recv( XdmfArray *Array, int Source = -1  );

protected:
  int  TerminateMpi;
  int  LastSourceId;
  int  LastSourceTag;
  int  SendTag;
  int  RecvTag;
  int  Id;
  int  Size;
};

#endif // __XdmfMpi_h
