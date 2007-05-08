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
#ifndef __XdmfDsmComm_h
#define __XdmfDsmComm_h

#include "XdmfObject.h"


//! Base comm object for Distributed Shared Memory implementation
/*!
*/

//! Macros to Shift to XDR if necessary (implement later)
#define XDMF_SHIFT64(a)     (a)
#define XDMF_SHIFT32(a)     (a)

#define XDMF_DSM_DEFAULT_TAG    0x80

typedef struct {
    XdmfInt32   Source;
    XdmfInt32   Dest;
    XdmfInt32   Tag;
    XdmfInt64   Size;
    void        *Data;
} XdmfDsmMsg;

class XDMF_EXPORT XdmfDsmComm : public XdmfObject {

public:
  XdmfDsmComm();
  ~XdmfDsmComm();

  XdmfConstString GetClassName() { return ( "XdmfDsmComm" ) ; };


//! Id 
    XdmfGetValueMacro(Id, XdmfInt32);
    XdmfSetValueMacro(Id, XdmfInt32);

//! Total 
    XdmfGetValueMacro(TotalSize, XdmfInt32);
    XdmfSetValueMacro(TotalSize, XdmfInt32);

    virtual XdmfInt32   Init();
    virtual XdmfInt32   Send(XdmfDsmMsg *Msg);
    virtual XdmfInt32   Receive(XdmfDsmMsg *Msg);
    virtual XdmfInt32   Check(XdmfDsmMsg *Msg);


protected:
    XdmfInt32       Id;
    XdmfInt32       TotalSize;
};

#endif // __XdmfDsmComm_h
