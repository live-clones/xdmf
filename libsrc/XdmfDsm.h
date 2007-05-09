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
/*     Copyright @ 2007 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#ifndef __XdmfDsm_h
#define __XdmfDsm_h

#include "XdmfObject.h"


//! Base comm object for Distributed Shared Memory implementation
/*!
*/


class XdmfDsmComm;
class XdmfArray;

#define XDMF_DSM_TYPE_UNIFORM       0
#define XDMF_DSM_TYPE_UNIFORM_RANGE 1
#define XDMF_DSM_TYPE_MIXED         2

#define XDMF_DSM_OPCODE_PUT     0x01
#define XDMF_DSM_OPCODE_GET     0x02

#define XDMF_DSM_DEFAULT_LENGTH 10000

class XDMF_EXPORT XdmfDsm : public XdmfObject {

public:
  XdmfDsm();
  ~XdmfDsm();

  XdmfConstString GetClassName() { return ( "XdmfDsm" ) ; };


//! Type
    XdmfGetValueMacro(DsmType, XdmfInt32);
    XdmfSetValueMacro(DsmType, XdmfInt32);

//! End Address
    XdmfGetValueMacro(EndAddress, XdmfInt64);
    XdmfSetValueMacro(EndAddress, XdmfInt64);

//! Start Address
    XdmfGetValueMacro(StartAddress, XdmfInt64);
    XdmfSetValueMacro(StartAddress, XdmfInt64);

//! Start Id
    XdmfGetValueMacro(StartServerId, XdmfInt32);
    XdmfSetValueMacro(StartServerId, XdmfInt32);

//! End Id
    XdmfGetValueMacro(EndServerId, XdmfInt32);
    XdmfSetValueMacro(EndServerId, XdmfInt32);

//! Length
    XdmfGetValueMacro(Length, XdmfInt64);
    XdmfInt32 SetLength(XdmfInt64 Length);

//! Storage
    XdmfGetValueMacro(Storage, XdmfArray *);
    XdmfSetValueMacro(Storage, XdmfArray *);

//! Comm
    XdmfGetValueMacro(Comm, XdmfDsmComm *);
    XdmfSetValueMacro(Comm, XdmfDsmComm *);

    //! Address Range
    XdmfInt32 GetAddressRangeForId(XdmfInt32 Id, XdmfInt64 *Start, XdmfInt64 *End);

    //! Configure the system. Set the Comm and ServerIds
    XdmfInt32   ConfigureUniform(XdmfDsmComm *Comm, XdmfInt64 Length, XdmfInt32 StartId=-1, XdmfInt32 EndId=-1);
    
    XdmfInt32   AddressToId(XdmfInt64 Address);

    XdmfInt32   Put(XdmfInt64 Address, XdmfInt64 Length, void *Data);

protected:
    XdmfInt32   DsmType;
    XdmfInt32   StartServerId;
    XdmfInt32   EndServerId;
    XdmfInt64   StartAddress;
    XdmfInt64   EndAddress;
    XdmfInt64   Length;
    XdmfArray   *Storage;
    XdmfDsmComm *Comm;
};

#endif // __XdmfDsm_h
