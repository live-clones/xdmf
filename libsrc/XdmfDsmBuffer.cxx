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
#include "XdmfDsmBuffer.h"
#include "XdmfDsmComm.h"
#include "XdmfDsmMsg.h"
#include "XdmfArray.h"

#define XDMF_DSM_OPCODE_PUT     0x01
#define XDMF_DSM_OPCODE_GET     0x02




XdmfDsmBuffer::XdmfDsmBuffer() {
}

XdmfDsmBuffer::~XdmfDsmBuffer() {
}


XdmfInt32
XdmfDsmBuffer::Put(XdmfInt64 Address, XdmfInt64 Length, void *Data){
    XdmfInt32   who, MyId = this->Comm->GetId();
    XdmfInt64   astart, aend, len;

    while(Length){
        who = this->AddressToId(Address);
        cout << " who = " << who << endl;
        if(who == XDMF_FAIL){
            XdmfErrorMessage("Address Error");
            return(XDMF_FAIL);
        }
        this->GetAddressRangeForId(who, &astart, &aend);
        // cout << "astart = " << astart << " aend = " << aend << endl;
        len = MIN(Length, aend - Address + 1);
        cout << "Put " << len << " Bytes to Address " << Address << " Id = " << who << endl;
        if(who == MyId) cout << "That's me!!" << endl;
        Length -= len;
        Address += len;
        // cout << "Length = " << Length << " Address = " << Address << endl;
    }
    return(XDMF_SUCCESS);
}
