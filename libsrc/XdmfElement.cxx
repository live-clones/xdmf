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
#include "XdmfDOM.h"
#include <libxml/tree.h>

#define XDMF_EMPTY_REFERENCE   0x00

XdmfElement::XdmfElement() {
    this->DOM = NULL;
    this->Element = NULL;
    this->IsReference = 0;
    this->ReferenceElement = NULL;
    this->State = XDMF_ELEMENT_STATE_UNINITIALIZED;
}

XdmfElement::~XdmfElement() {
}

void XdmfElement::SetReferenceObject(void *p){
    if(this->Element) this->Element->_private = p;
}

void *
XdmfElement::GetReferenceObject(){
    if(!this->ReferenceElement){
        XdmfErrorMessage("NULL Reference Element");
        return(NULL);
    }
    if(this->ReferenceElement->_private == XDMF_EMPTY_REFERENCE){
        XdmfDebug("XML Node contains no initialized object");
        return(NULL);
    }
    return(this->ReferenceElement->_private);
}

XdmfInt32 XdmfElement::SetElement(XdmfXmlNode Element){
    if(!Element) {
        XdmfErrorMessage("Element is NULL");
        return(XDMF_FAIL);
    }
    // Clear the application data of the underlying node
    XdmfDebug("Clearing _private member of XML node");
    Element->_private = XDMF_EMPTY_REFERENCE;
    this->Element = Element;
    return(XDMF_SUCCESS);
}

XdmfInt32 XdmfElement::InsertChildElement(XdmfXmlNode Child){
    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(XDMF_FAIL);
    }
    if(!this->Element) {
        XdmfErrorMessage("Current Element is empty");
        return(XDMF_FAIL);
    }
    if(this->DOM->Insert(this->Element, Child)){
        return(XDMF_SUCCESS);
    }
    return(XDMF_FAIL);
}

XdmfInt32
XdmfElement::Copy(XdmfElement *Source){
    return(XDMF_SUCCESS);
}

XdmfInt32 XdmfElement::UpdateInformation(){
    XdmfConstString Value;
    XdmfInt32   Status;
    XdmfElement *e;
    XdmfXmlNode Original;

    XdmfDebug("XdmfElement::UpdateInformation()");
    XdmfDebug("Class Name = " << this->GetClassName());
    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(XDMF_FAIL);
    }
    if(!this->Element){
        XdmfErrorMessage("No XML Node has been set");
        return(XDMF_FAIL);
    }
    Value = this->Get("Reference");
    if(Value){
        XdmfXmlNode node;

        XdmfDebug("Element is a Reference");
        node = this->DOM->FindElementByPath(Value);
        if(!node){
            XdmfErrorMessage("Can't find node of Path " << Value);
            return(XDMF_FAIL);
        }
        XdmfDebug("Found Referenced Node");
        if(this->SetReference(node) == XDMF_FAIL){
            XdmfErrorMessage("Unable to Set Reference");
            return(XDMF_FAIL);
        }
        e = (XdmfElement *)this->GetReferenceObject();
        if(e){
            if(e->GetState() < XDMF_ELEMENT_STATE_LIGHT_PARSED){
                // Could cause a chain of events
                e->UpdateInformation();
            }
            // Copy out Appropriate Info
            this->Copy(e);
        }else{
            // Reference to Empty Reference
            // Follow Reference and UpdateInformation
            // from the target
            while(Value){
                Value = this->GetDOM()->Get(node, "Reference");
                if(Value){
                    XdmfDebug("Following Reference " << Value);
                    node = this->DOM->FindElementByPath(Value);
                    if(!node){
                        XdmfErrorMessage("Can't find node of Path " << Value);
                        return(XDMF_FAIL);
                    }
                }
            }
            // Temporarily SetElement() so the Child Class Parses the Correct XML Node
            Original = this->Element;
            this->Element = node;
            this->IsReference = 0;
            XdmfDebug("Update Information from Taget Element");
            // cout << this->DOM->Serialize(node) << endl;;
            // cout << this->Serialize() << endl;;
            Status = this->UpdateInformation();
            // Restore Original Element Reguardless of Success
            // XXXXX This is flaky
            // this->Element = Original;
            this->IsReference = 1;
            if(Status == XDMF_FAIL){
                XdmfErrorMessage("Error Updating Information of Reference Node");
                return(XDMF_FAIL);
            }
        }
    }
    this->State = XDMF_ELEMENT_STATE_LIGHT_PARSED;
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
    // Don't do this until AFTER a successful Update or UpdateInformation
    // XdmfDebug("Setting _private member of XML node");
    // this->Element->_private = this;
    this->State = XDMF_ELEMENT_STATE_HEAVY_READ;
    return(XDMF_SUCCESS);
}

XdmfXmlNode XdmfElement::GetReference(){
    return(this->ReferenceElement);
    }

XdmfInt32 XdmfElement::SetReference(XdmfXmlNode Element){
    XdmfElement *e;

    XdmfDebug("XdmfElement::SetReference(XdmfXmlNode Element)");
    if(!Element) return(XDMF_FAIL);
    // if(Element->_private == XDMF_EMPTY_REFERENCE){
    //     XdmfDebug("XML Node contains no initialized object");
    //     this->SetIsReference(0);
    //     return(XDMF_FAIL);
    // }
    // Is it the proper class ?
    e = (XdmfElement *)Element->_private;
    if(!e){
        // Not Yet Parsed
        XdmfDebug("No ReferenceObject in Element");
        this->ReferenceElement = Element;
        this->SetIsReference(1);
        return(XDMF_SUCCESS);
    }
    if(STRCASECMP(this->GetClassName(), e->GetClassName()) == 0){
        this->ReferenceElement = Element;
        this->SetIsReference(1);
        return(XDMF_SUCCESS);
    }else{
        XdmfErrorMessage("Class Name Mismatch. Referenced node " << e->GetClassName() << " != " << this->GetClassName());
    }
    return(XDMF_FAIL);
}

XdmfConstString XdmfElement::Serialize(){
    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(NULL);
    }
    if(!this->Element){
        XdmfErrorMessage("No XML Node has been set");
        return(NULL);
    }
    return(this->DOM->Serialize(this->Element));
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
    return((XdmfConstString)this->Element->name);
}

XdmfInt32 XdmfElement::Build(){
    XdmfConstString  name;

    name = this->GetName();
    if(name && (strlen(name) > 1)){
        return(this->Set("Name", name));
    }
    return(XDMF_SUCCESS);
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
