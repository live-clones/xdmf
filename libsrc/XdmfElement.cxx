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
#include "XdmfElement.h"
#include <libxml/tree.h>

XdmfElement::XdmfElement() {
    this->DOM = NULL;
    this->Element = NULL;
}

XdmfElement::~XdmfElement() {
}

XdmfInt32 XdmfElement::UpdateInformation(){
    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(XDMF_FAIL);
    }
    if(!this->Element){
        XdmfErrorMessage("No XML Node has been set");
        return(XDMF_FAIL);
    }
    return(XDMF_SUCCESS);
}

XdmfInt32 XdmfElement::Update(){
    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(XDMF_FAIL);
    }
    if(!this->Element){
        XdmfErrorMessage("No XML Node has been set");
        return(XDMF_FAIL);
    }
    return(XDMF_SUCCESS);
}

XdmfConstString XdmfElement::GetElementType(){
    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(NULL);
    }
    if(!this->Element){
        XdmfErrorMessage("No XML Node has been set");
        return(NULL);
    }
    xmlNode *node;
    node = (xmlNode *)this->Element;
    if(!node) return(NULL);
    return((XdmfConstString)node->name);
}

XdmfInt32 XdmfElement::UpdateDOM(){
    this->Set("Name", this->GetName());
    return(this->Set("Name", this->GetName()));
}

XdmfInt32 XdmfElement::Set(XdmfConstString Name, XdmfConstString Value){
    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(XDMF_FAIL);
    }
    if(!this->Element){
        XdmfErrorMessage("No XML Node has been set");
        return(XDMF_FAIL);
    }
    this->DOM->Set(this->Element, Name, Value);
    return(XDMF_SUCCESS);
}


XdmfConstString XdmfElement::Get(XdmfConstString Name){
    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(NULL);
    }
    if(!this->Element){
        XdmfErrorMessage("No XML Node has been set");
        return(NULL);
    }
    return(this->DOM->Get(this->Element, Name));
}
