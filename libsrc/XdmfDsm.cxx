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
#include "XdmfDsm.h"
#include "XdmfDsmComm.h"
#include "XdmfDsmMsg.h"
#include "XdmfArray.h"


XdmfDsm::XdmfDsm() {
    this->DsmType = XDMF_DSM_TYPE_UNIFORM;
    this->Storage = new XdmfArray;
    // For Alignment
    this->Storage->SetNumberType(XDMF_INT64_TYPE);
    this->SetLength(XDMF_DSM_DEFAULT_LENGTH);
    this->StartAddress = 0;
    this->EndAddress = this->StartAddress + this->Length - 1;
    this->Comm = 0;
    this->StartServerId = this->EndServerId = -1;
}

XdmfDsm::~XdmfDsm() {
}

XdmfInt32
XdmfDsm::ConfigureUniform(XdmfDsmComm *Comm, XdmfInt64 Length, XdmfInt32 StartId, XdmfInt32 EndId){
    if(StartId < 0) StartId = 0;
    if(EndId < 0) EndId = Comm->GetTotalSize() - 1;
    this->SetDsmType(XDMF_DSM_TYPE_UNIFORM_RANGE);
    if((StartId == 0) && (EndId == Comm->GetTotalSize() - 1)){
        this->SetDsmType(XDMF_DSM_TYPE_UNIFORM);
    }
    this->SetStartServerId(StartId);
    this->SetEndServerId(EndId);
    this->SetComm(Comm);
    if((Comm->GetId() >= StartId) && (Comm->GetId() <= EndId)){
        this->SetLength(Length);
        this->StartAddress = (StartId - Comm->GetId()) * Length;
        this->EndAddress = this->StartAddress + Length - 1;
    }else{
        this->Length = Length;
    }
    return(XDMF_SUCCESS);
}

XdmfInt32
XdmfDsm::GetAddressRangeForId(XdmfInt32 Id, XdmfInt64 *Start, XdmfInt64 *End){
    switch(this->DsmType) {
        case XDMF_DSM_TYPE_UNIFORM :
        case XDMF_DSM_TYPE_UNIFORM_RANGE :
            // All Servers have same length
            *Start = (Id - this->StartServerId) * this->Length;
            *End = *Start + Length - 1;
            break;
        default :
            // Not Implemented
            XdmfErrorMessage("DsmType " << this->DsmType << " not yet implemented");
            return(XDMF_FAIL);
            break;
    }
    return(XDMF_SUCCESS);
}

XdmfInt32
XdmfDsm::AddressToId(XdmfInt64 Address){
    XdmfInt32   ServerId = XDMF_FAIL;

    switch(this->DsmType) {
        case XDMF_DSM_TYPE_UNIFORM :
        case XDMF_DSM_TYPE_UNIFORM_RANGE :
            // All Servers have same length
            ServerId = this->StartServerId + (Address / this->Length);
            if(ServerId > this->EndServerId ){
                XdmfErrorMessage("ServerId " << ServerId << " for Address " << Address << " is larger than EndServerId");
            }
            break;
        default :
            // Not Implemented
            XdmfErrorMessage("DsmType " << this->DsmType << " not yet implemented");
            break;
    }
    return(ServerId);
}

XdmfInt32
XdmfDsm::SetLength(XdmfInt64 Length){
    // Make it longer than actually needed for round off.
    if(this->Storage->SetNumberOfElements((Length / sizeof(XdmfInt64)) + 1) != XDMF_SUCCESS){
        XdmfErrorMessage("Cannot set Dsm Length to " << Length);
        return(XDMF_FAIL);
    }
    this->Length = Length;
    return(XDMF_SUCCESS);
}

XdmfInt32
XdmfDsm::Put(XdmfInt64 Address, XdmfInt64 Length, void *Data){
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
        // cout << "Put " << len << " Bytes to Address " << Address << " Id = " << who << endl;
        // if(who == MyId) cout << "That's me!!" << endl;
        Length -= len;
        Address += len;
        // cout << "Length = " << Length << " Address = " << Address << endl;
    }
    return(XDMF_SUCCESS);
}
