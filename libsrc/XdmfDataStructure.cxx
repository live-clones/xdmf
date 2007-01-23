/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Values              */
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
#include "XdmfItem.h"
#include "XdmfDataItem.h"
#include "XdmfArray.h"

XdmfDataStructure::XdmfDataStructure() {
    this->ItemType = XDMF_UNIFORM
}

XdmfDataStructure::~XdmfDataStructure() {
}

XdmfInt32 
XdmfDataStructure::UpdateInformation(){
    XdmfConstString Val;
    if(XdmfElement::UpdateInformation() != XDMF_SUCCESS) return(XDMF_FAIL);
    Val = this->Get("ItemType");
    if(!Val){
        this->SetItemType(XDMF_UNIFORM);
    }else{
        if(XDMFCASECMP(Val, "Uniform") == 0){
            this->SetItemType(XDMF_UNIFORM);
        }else{
            if(XDMFCASECMP(Val, "Collection") == 0){
                this->SetItemType(XDMF_COLLECTION);
            }else{
                if(XDMFCASECMP(Val, "Tree") == 0){
                    this->SetItemType(XDMF_TREE);
                }else{
                    XdmfErrorMessage("Unknown DataItem Type = " << Val);
                    return(XDMF_FAIL);
                }
            }
        }
    }
    return(XDMF_SUCCESS);
}

