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
XdmfDsmBuffer::ServiceOnce(XdmfInt32 *ReturnOpcode){
    XdmfInt32   status = XDMF_FAIL;

    status = this->Comm->Check(this->Msg);
    if(status != XDMF_SUCCESS){
        // Nothing to do
        return(XDMF_SUCCESS);
    }
    // Service One Call
    return(this->Service(ReturnOpcode));
}

XdmfInt32
XdmfDsmBuffer::ServiceUntilIdle(XdmfInt32 *ReturnOpcode){
    XdmfInt32   status = XDMF_SUCCESS;

    while(status == XDMF_SUCCESS){
        status = this->Comm->Check(this->Msg);
        if(status != XDMF_SUCCESS){
            // Nothing to do
            return(XDMF_SUCCESS);
        }
        // Service One Call
        status = this->Service(ReturnOpcode);
        if(status != XDMF_SUCCESS){
            XdmfErrorMessage("ServiceUntilIdle detected error in Service() Method");
            return(XDMF_FAIL);
        }
    }
    return(XDMF_SUCCESS);
}

XdmfInt32
XdmfDsmBuffer::ServiceLoop(XdmfInt32 *ReturnOpcode){
    XdmfInt32   op, status = XDMF_SUCCESS;

    while(status == XDMF_SUCCESS){
        status = this->Service(&op);
        if(status != XDMF_SUCCESS) return(XDMF_FAIL);
        if(ReturnOpcode) *ReturnOpcode = op;
        if(op == XDMF_DSM_OPCODE_DONE) return(XDMF_SUCCESS);
    }
    return(XDMF_SUCCESS);
}

XdmfInt32
XdmfDsmBuffer::Service(XdmfInt32 *ReturnOpcode){
    XdmfInt32   Opcode, who, status = XDMF_FAIL;
    XdmfInt64   Length, Address;
    XdmfByte    *datap;

    status = this->ReceiveCommandHeader(&Opcode, &who, &Address, &Length);
    if(status == XDMF_FAIL){
        XdmfErrorMessage("Error Receiving Command Header");
        return(XDMF_FAIL);
    }
    switch(Opcode){
        case XDMF_DSM_OPCODE_PUT :
            cout << "PUT request from " << who << " for " << Length << " bytes @ " << Address << endl;
            if(Length > (this->EndAddress - Address)){
                XdmfErrorMessage("Length too long");
                return(XDMF_FAIL);
            }
            datap = (XdmfByte *)this->Storage->GetDataPointer();
            datap += Address - this->StartAddress;
            status = this->ReceiveData(who, datap, Length); 
            if(status == XDMF_FAIL){
                XdmfErrorMessage("ReceiveData() failed");
                return(XDMF_FAIL);
            }
            cout << "Serviced PUT request from " << who << " for " << Length << " bytes @ " << Address << endl;
            break;
        case XDMF_DSM_OPCODE_GET :
            break;
        case XDMF_DSM_OPCODE_DONE :
            break;
        default :
            XdmfErrorMessage("Unknown Opcode " << Opcode);
            return(XDMF_FAIL);
    }
    if(ReturnOpcode) *ReturnOpcode = Opcode;
    return(XDMF_SUCCESS);
}

XdmfInt32
XdmfDsmBuffer::Put(XdmfInt64 Address, XdmfInt64 Length, void *Data){
    XdmfInt32   who, MyId = this->Comm->GetId();
    XdmfInt64   astart, aend, len;
    XdmfByte    *datap = (XdmfByte *)Data;

    while(Length){
        who = this->AddressToId(Address);
        if(who == XDMF_FAIL){
            XdmfErrorMessage("Address Error");
            return(XDMF_FAIL);
        }
        this->GetAddressRangeForId(who, &astart, &aend);
        // cout << "astart = " << astart << " aend = " << aend << endl;
        len = MIN(Length, aend - Address + 1);
        cout << "Put " << len << " Bytes to Address " << Address << " Id = " << who << endl;
        if(who == MyId){
            XdmfByte *dp;

            cout << "That's me!!" << endl;
            dp = (XdmfByte *)this->Storage->GetDataPointer();
            dp += Address - this->StartAddress;
            memcpy(dp, datap, len);

        }else{
            XdmfInt32   status;

            status = this->SendCommandHeader(XDMF_DSM_OPCODE_PUT, who, Address, len);
            if(status == XDMF_FAIL){
                XdmfErrorMessage("Failed to send PUT Header to " << who);
                return(XDMF_FAIL);
            }
            status = this->SendData(who, datap, len);
            if(status == XDMF_FAIL){
                XdmfErrorMessage("Failed to send " << len << " bytes of data to " << who);
                return(XDMF_FAIL);
            }

        }
        Length -= len;
        Address += len;
        datap += len;
        // cout << "Length = " << Length << " Address = " << Address << endl;
    }
    return(XDMF_SUCCESS);
}
