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
#include "XdmfNDGM.h"
#include "XdmfArray.h"

#ifndef WIN32
#include <unistd.h>
#endif

extern "C" {
#ifdef HAVE_NDGM
  NDGM_32_INT
  XdmfNDGMErrorCallback( NDGM_NODE *node,
          NDGM_32_INT     reason,
          NDGM_PTR        user_data) {
    return(NDGM_SUCCESS); /* Device Close and Free Lower Data Structs */
    }
#else
   XdmfString XdmfGetNdgmEntries( void ){return(NULL);}
   void XdmfDeleteAllNdgmEntries( void ){return;}
   XdmfInt64 XdmfAddNdgmEntry( XdmfString Name, XdmfInt64 Length )
     {
     (void)Name;
     (void)Length;
     return(0);
     }
#endif
  }


XdmfNDGM::XdmfNDGM() {
  // Default NDGM UID
#ifdef HAVE_NDGM
  this->NdgmUid = ndgm_unique( NULL, 1, 0 );
  gethostname( this->NdgmHost, XDMF_MAX_STRING_LENGTH );
  this->NdgmMode = XDMF_NDGM_MSG_SERVER;
  this->NdgmNode = NULL;
  this->NdgmDriver = NDGM_SOC;
#endif

}

XdmfNDGM::~XdmfNDGM() {
#ifdef HAVE_NDGM
  if( this->NdgmNode ) {
    ndgm_msg_close( this->NdgmNode );
    }
#endif
}

XdmfInt64
XdmfNDGM::GetLastAddress( void ) {
#ifdef HAVE_NDGM
  return( NDGM_LAST_ADDRESS() );
#else
  return(0);
#endif
  }

XdmfInt32
XdmfNDGM::Clear( void ) {
#ifdef HAVE_NDGM
XdmfInt64  Address = 0, LastAddress = NDGM_LAST_ADDRESS();
XdmfInt8  buffer[ 1000010 ];

memset( buffer, 0, 1000000 );
while( Address < ( LastAddress - 1000000)  ){
  ndgm_put( Address, buffer, 1000000 );
  Address += 1000000;
  }
if( Address < LastAddress ){
  ndgm_put( Address, buffer, LastAddress - Address + 1 );
  }
#endif
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfNDGM::Open( XdmfInt32 IdOffset ) {
#ifdef HAVE_NDGM
char  connection[ XDMF_MAX_STRING_LENGTH ];


switch ( this->NdgmMode ) {
  case XDMF_NDGM_CLIENT :
    // Nomrmal NDGM Client
    if( ndgm_inited > 0 ) {
      return ( XDMF_SUCCESS );
      }
    {
      int   i, argc = 0;
      char  args[10][80];
      XdmfString argv[10];

      strcpy(args[argc++], "XDMF_NDGM_CLIENT");
      strcpy(args[argc++], "-host");
      strcpy(args[argc++], this->NdgmHost );
      strcpy(args[argc++], "-uid");
      sprintf( args[argc++], "%d" , this->NdgmUid);
      for( i = 0 ; i < argc ; i++ ){
        argv[i] = args[i];
        }
      XdmfDebug("Attempt to Connect to NDGM Server on " << this->NdgmHost );
      if( ndgm_init( &argc, argv ) == NDGM_FAIL ){
        XdmfErrorMessage("Can't Become NDGM Client");
        return( XDMF_FAIL );
        }
    }
    break;
  case XDMF_NDGM_MSG_SERVER :
    // Message Server
    ndgm_msg_init( IdOffset + 1 );
    ndgm_mpi_connection( this->NdgmDriver,
      this->NdgmHost,
      IdOffset + 1,
      connection );  
    XdmfDebug("Attempt Server with Connection : " << connection );
    if( ( this->NdgmNode =  ndgm_msg_server(connection,
          NDGM_SELF_ID,
          this->NdgmDriver,
          sizeof(NDGM_CMD) + NDGM_MAX_PACKET_SIZE + 100,
          NULL) ) == NULL ){
      XdmfErrorMessage("Can't Become Server");
      return( XDMF_FAIL );
    }
    ndgm_msg_set( this->NdgmNode,
      NDGM_NODE_USER_PROC, XdmfNDGMErrorCallback,
      NDGM_NODE_USER_DATA, NULL,
      NULL );
    break;
  default :
    // Message Client
    ndgm_msg_init( NDGM_DUMMY_WORKER_ID  );
    ndgm_mpi_connection( this->NdgmDriver,
      this->NdgmHost,
      IdOffset + 1,
      connection );  
    XdmfDebug("Attempt Connect to : " << connection );
    if( ( this->NdgmNode =  ndgm_msg_open(connection,
          IdOffset + 1,
          this->NdgmDriver,
          sizeof(NDGM_CMD) + NDGM_MAX_PACKET_SIZE + 100,
          NULL) ) == NULL ){
      XdmfErrorMessage("Can't Become Server");
      return( XDMF_FAIL );
      }
    this->NdgmClient = this->NdgmNode;  
    break;
  }
return( XDMF_SUCCESS );
#else
(void)IdOffset;
return(XDMF_FAIL);
#endif
}

XdmfInt32
XdmfNDGM::Close( XdmfInt32 Disconnect ) {
#ifdef HAVE_NDGM
   switch ( this->NdgmMode ) {
  case XDMF_NDGM_CLIENT :
    XdmfDebug("Closing Client Connection");
    if( Disconnect ) {
      XdmfDebug("Disconnecting from NDGM Server");
      ndgm_shutdown();
    }
    break;
  default :
    XdmfDebug("Closing NDGM Message Connection");
    if( this->NdgmNode ) {
      ndgm_msg_close( this->NdgmNode );
      }
    this->NdgmClient = this->NdgmNode = NULL;
    break;
  }
return( XDMF_SUCCESS );
#else
(void)Disconnect;
return(XDMF_FAIL);
#endif
}

XdmfInt32
XdmfNDGM::Check( void ) {
#ifdef HAVE_NDGM
NDGM_CHAR  dummy[NDGM_MAX_PACKET_SIZE];
NDGM_CMD  *Cmd;

XdmfDebug("Perform Select");
ndgm_set_default_select_time( 0 );
if( this->NdgmNode == NULL ){
  ndgm_msg_select_any((NDGM_PTR)dummy,
      sizeof(NDGM_CHAR),
      NDGM_MAX_PACKET_SIZE);
  return( XDMF_FAIL );
} else {
  Cmd = (NDGM_CMD *)NDGM_MSG_DATA( this->NdgmNode );
  Cmd->opcode = NDGM_CMD_NOP;
  if ( ( this->NdgmClient = ndgm_msg_select((NDGM_PTR)Cmd,
      sizeof(NDGM_CMD),
      1,
      this->NdgmNode ) ) == NULL ){
    return( XDMF_FAIL );
  }
  XdmfDebug("Select Found Message");
  // Fix Opcode, etc. for non-XDR Machines
  Cmd = (NDGM_CMD *)NDGM_MSG_DATA( this->NdgmClient );
  Cmd->opcode = XDR_32_TO_HOST( Cmd->opcode );
  Cmd->start = XDR_32_TO_HOST( Cmd->start );
  Cmd->length = XDR_32_TO_HOST( Cmd->length );
  this->NdgmClient->id = this->NdgmClient->msg->owner;
  }
return( XDMF_SUCCESS );
#else
return(XDMF_FAIL);
#endif
}

XdmfInt32
XdmfNDGM::Get(XdmfInt64 Address, XdmfArray *Array )
{
#ifdef HAVE_NDGM
NDGM_LENGTH  Length;

Length = Array->GetCoreLength();
XdmfDebug("Getting " << Length << " Bytes from Address " << Address );
if( ndgm_get( Address, Array->GetDataPointer(), Length ) == XDMF_FAIL ){
  XdmfErrorMessage("NDGM_GET Failed for " << Length << " Bytes from Address " << Address );
  return( XDMF_FAIL );
  }
return( XDMF_SUCCESS );
#else
(void)Address;
(void)Array;
return(XDMF_FAIL);
#endif
}

XdmfInt32
XdmfNDGM::Put(XdmfInt64 Address, XdmfArray *Array )
{
#ifdef HAVE_NDGM
NDGM_LENGTH  Length;

Length = Array->GetCoreLength();
XdmfDebug("Putting " << Length << " Bytes to Address " << Address );
if( ndgm_put( Address, Array->GetDataPointer(), Length ) == XDMF_FAIL ){
  XdmfErrorMessage("NDGM_PUT Failed for " << Length << " Bytes to Address " << Address );
  return( XDMF_FAIL );
  }
return( XDMF_SUCCESS );
#else
(void)Address;
(void)Array;
return(XDMF_FAIL);
#endif
}

XdmfInt32
XdmfNDGM::Send(XdmfArray *Array, XdmfInt32 Opcode )
{
#ifdef HAVE_NDGM
NDGM_CMD  *Cmd;
XdmfInt32  Status;

if( this->NdgmClient == NULL ) return( XDMF_FAIL  );
Cmd = (NDGM_CMD *)NDGM_MSG_DATA( this->NdgmClient );
Cmd->opcode = Opcode;
Cmd->start = 0;
Cmd->length = Array->GetCoreLength();
memcpy( Cmd->data, Array->GetDataPointer(), Cmd->length );

XdmfDebug("Sending " << Cmd->length << " bytes");
Status = ndgm_xdr_cmd_write( Cmd, 
        sizeof(NDGM_CMD) + Cmd->length,
        this->NdgmClient);

if( Status > 0 ) {
  Status -= sizeof(NDGM_CMD);
  }
return( Status );
#else
(void)Array;
(void)Opcode;
return(XDMF_FAIL);
#endif
}

XdmfInt32
XdmfNDGM::Recv( XdmfArray *Array )
{
#ifdef HAVE_NDGM
NDGM_CMD  *Cmd;

if( this->NdgmClient == NULL ) return( XDMF_FAIL  );

Cmd = (NDGM_CMD *)NDGM_MSG_DATA( this->NdgmClient );

XdmfDebug("Receiving " << Array->GetCoreLength() << " bytes MAX");
if( this->NdgmMode == XDMF_NDGM_MSG_CLIENT ) {
  this->NdgmClient = ndgm_xdr_cmd_read( Cmd,
        Array->GetCoreLength(),
        this->NdgmNode );
}
if( this->NdgmNode ) {
  Array->SetNumberOfElements( Cmd->length / Array->GetElementSize()  );
  memcpy( Array->GetDataPointer(),
      Cmd->data,
      Cmd->length );
  return( Cmd->length );
}
#else
(void)Array;
#endif
return( XDMF_FAIL );
}

