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
#ifndef __XdmfNDGM_h
#define __XdmfNDGM_h

#ifndef SWIG
extern "C" {
#ifdef HAVE_NDGM
#include "Ndgm/ndgm.h"
#else
#define NDGM_CMD_NOP  0
	typedef struct {
		char *dummy;
		} NDGM_NODE;
#endif
}
#endif /* SWIG */

#include "XdmfHeavyData.h"
#include "XdmfArray.h"

#define XDMF_NDGM_MSG_SERVER  0
#define XDMF_NDGM_MSG_CLIENT  1
#define XDMF_NDGM_CLIENT  2


/*!
Object to Handle Low level NDGM Details
like sending and recciving NDGM Messages
*/
class XDMF_EXPORT XdmfNDGM : public XdmfDataDesc {

public:
  XdmfNDGM();
  ~XdmfNDGM();

  const char * GetClassName() { return ( "XdmfNDGM" ) ; };

//! Get the Default hostname of the NDGM Server
        XdmfGetValueMacro(NdgmHost, XdmfString);
//! Set the Default hostname of the NDGM Server
        void SetNdgmHost( char *String ) { strcpy( this->NdgmHost, String ); }

//! Get the low level transport mechanism
  XdmfGetValueMacro(NdgmDriver, XdmfInt32);
//! Set the low level transport mechanism
  XdmfSetValueMacro(NdgmDriver, XdmfInt32);

//! Get the NDGM_UID : used for port number calculations
  XdmfGetValueMacro(NdgmUid, XdmfInt32);
//! Set the NDGM_UID : used for port number calculations
  XdmfSetValueMacro(NdgmUid, XdmfInt32);

/*!
Get the mode of this object. Mode are :
    XDMF_NDGM_MSG_SERVER  A Message Server 
    XDMF_NDGM_MSG_CLIENT  A Message Client
    XDMF_NDGM_CLIENT  A DSM Client
*/
  XdmfGetValueMacro(NdgmMode, XdmfInt32);
//! Set the mode of the object
  XdmfSetValueMacro(NdgmMode, XdmfInt32);
//! Convenience
  void SetModeToMsgServer( void ) {
    this->NdgmMode = XDMF_NDGM_MSG_SERVER;
    };
  void SetModeToMsgClient( void ) {
    this->NdgmMode = XDMF_NDGM_MSG_CLIENT;
    };
  void SetModeToClient( void ) {
    this->NdgmMode = XDMF_NDGM_CLIENT;
    };

//! Get the low level NDGM Node structure
  XdmfGetValueMacro(NdgmNode, NDGM_NODE *);
//! Set the low level NDGM Node structure
  XdmfSetValueMacro(NdgmNode, NDGM_NODE *);

//! Get the other end of the communication mechanism
  XdmfGetValueMacro(NdgmClient, NDGM_NODE *);
//! Set the other end of the communication mechanism
  XdmfSetValueMacro(NdgmClient, NDGM_NODE *);
//! Get Length of NDGM in Bytes
  XdmfInt64  GetTotalLength( void ) {
    return( this->GetLastAddress() + 1 );
    };

//! Get Last Valid NDGM Address
  XdmfInt64  GetLastAddress( void );

//! Zero Out all of NDGM
  XdmfInt32  Clear( void );

/*!
Open a connection
  For XDMF_NDGM_MSG_SERVER "bind()" and "listen()"
  For XDMF_NDGM_MSG_CLIENT connect to a message server
  For XDMF_NDGM_CLIENT connect to a ndgm_server for DSM
*/
  XdmfInt32  Open( XdmfInt32  IdOffset = 1 );
//! Check for new messages
  XdmfInt32  Check( void );
//! Close down connection and Optionally disconnect from NDGM server
  XdmfInt32  Close( XdmfInt32 Disconnect = 0 );
//! Put to DSM
  XdmfInt32  Put(XdmfInt64 Address, XdmfArray *Array );
//! Get from DSM
  XdmfInt32  Get(XdmfInt64 Address, XdmfArray *Array );
//! Send an Array
  XdmfInt32  Send( XdmfArray *Array, XdmfInt32 Opcode = NDGM_CMD_NOP );
//! Receive an Array
  XdmfInt32  Recv( XdmfArray *Array );
//! Initialize a Barrier
  XdmfInt32  BarrierInit( XdmfInt32  Barrier = 20 , XdmfInt32 Value = -1 ){
#ifdef HAVE_NDGM
      return( ndgm_barrier_init( Barrier, Value ));
#else
      return(-1);
#endif
      }
//! Wait in a Barrier
  XdmfInt32  BarrierWait( XdmfInt32  Barrier ) {
#ifdef HAVE_NDGM
        return( ndgm_barrier_wait( Barrier ) );
#else
      return(-1);
#endif
        }
//! Wait in a Barrier without effecting count
  XdmfInt32  BarrierAudit( XdmfInt32  Barrier ) {
#ifdef HAVE_NDGM
        return( ndgm_barrier_audit( Barrier ) );
#else
      return(-1);
#endif
        }

  XdmfInt32  BarrierPoll( XdmfInt32  Barrier ) {
#ifdef HAVE_NDGM
        return( ndgm_barrier_poll( Barrier ) );
#else
      return(-1);
#endif
        }


protected:
  char    NdgmHost[XDMF_MAX_STRING_LENGTH];
  XdmfInt32  NdgmDriver;
  XdmfInt32  NdgmUid;
  XdmfInt32  NdgmMode;
  NDGM_NODE  *NdgmNode;
  NDGM_NODE  *NdgmClient;
};


extern "C" {
extern XDMF_EXPORT char *XdmfGetNdgmEntries( void );
extern XDMF_EXPORT void XdmfDeleteAllNdgmEntries( void );
extern XDMF_EXPORT XdmfInt64 XdmfAddNdgmEntry( char *Name, XdmfInt64 Length );
  }

#endif // __XdmfNDGM_h
