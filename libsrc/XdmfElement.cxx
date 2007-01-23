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
    this->CopyReferenceData = 1;
}

XdmfElement::~XdmfElement() {
    if(this->ReferenceElement){
        if(this->GetReferenceObject(this->ReferenceElement) == this){
            this->SetReferenceObject(this->ReferenceElement, XDMF_ELEMENT_STATE_UNINITIALIZED);
        }
    }
    if(this->Element){
        if(this->GetReferenceObject(this->Element) == this){
            this->SetReferenceObject(this->Element, XDMF_ELEMENT_STATE_UNINITIALIZED);
        }
    }
}

void XdmfElement::SetReferenceObject(XdmfXmlNode Element, void *p){
    XdmfDebug("Old Ref = " << Element->_private);
    XdmfDebug("New Ref = " << p);
    if(Element) Element->_private = p;
}

void *
XdmfElement::GetReferenceObject(XdmfXmlNode Element){
    if(!Element){
        XdmfErrorMessage("NULL Reference Element");
        return(NULL);
    }
    if(Element->_private == XDMF_EMPTY_REFERENCE){
        XdmfDebug("XML Node contains no initialized object");
        return(NULL);
    }
    return(Element->_private);
}

XdmfInt32 XdmfElement::SetElement(XdmfXmlNode Element){
    if(!Element) {
        XdmfErrorMessage("Element is NULL");
        return(XDMF_FAIL);
    }
    // Clear the ReferenceObject underlying node
    XdmfDebug("Clearing ReferenceObject of XML node");
    this->SetReferenceObject(Element, XDMF_EMPTY_REFERENCE);
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
    XdmfXmlNode ref, Original;

    XdmfDebug("XdmfElement::UpdateInformation()");
    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(XDMF_FAIL);
    }
    if(!this->Element){
        XdmfErrorMessage("No XML Node has been set");
        return(XDMF_FAIL);
    }
    Value = this->Get("Name");
    if(Value) this->SetName(Value);
    ref = this->CheckForReference(this->Element);
    if(ref){
        XdmfXmlNode node;

        // "this" is now the ReferenceObject for this->ReferenceElement
        XdmfDebug("Setting ReferenceObject and Following Chain");
        this->SetReferenceObject(this->ReferenceElement, this);
        while(ref){
            e = (XdmfElement *)this->GetReferenceObject(ref);
            if(e && (e != this)){
                XdmfDebug("Updating Information from another Object");
                // XdmfDebug(e->Serialize());
                // There if an Object associated with this node. UpdateInformation()?
                if(e->GetState() < XDMF_ELEMENT_STATE_LIGHT_PARSED){
                    // Could cause a chain of UpdateInformation() 
                    XdmfDebug("Call UpdateInformation on ReferenceObject");
                    e->UpdateInformation();
                }
                // Copy out appropriate information and return
                XdmfDebug("Copying Information from Reference Object");
                this->Element = e->Element;
                return(this->Copy(e));
            }
            // No ReferenceObject Set. Is this a Reference as well?
            node = this->FollowReference(ref);
            if(node){
                ref = node;
            }else{
                // Not a Reference. Is it the right Type ?
                if(STRCMP((const char *)ref->name, (const char *)this->ReferenceElement->name) != 0){
                    XdmfErrorMessage("Reference node " << Value << " is a " << ref->name << " not " << ReferenceElement->name);
                    return(XDMF_FAIL);
                }
                // If this is a derived Class, UpdateInformation will act on this target.
                this->SetElement(ref);
                // This is the end of the chain and there is no ReferenceObject for the XML node.
                XdmfDebug("Setting Reference Object");
                this->SetReferenceObject(ref, this);
                ref = NULL;
            }
        }
    }else{
        XdmfDebug("Setting Reference Object");
        this->SetReferenceObject(this->Element, this);
    }
    this->State = XDMF_ELEMENT_STATE_LIGHT_PARSED;
    return(XDMF_SUCCESS);
}

XdmfInt32 XdmfElement::Update(){
    XdmfXmlNode node, ref;
    XdmfElement *e;

    if(!this->DOM) {
        XdmfErrorMessage("No DOM has been set");
        return(XDMF_FAIL);
    }
    if(!this->Element){
        XdmfErrorMessage("No XML Node has been set");
        return(XDMF_FAIL);
    }
    if(this->GetIsReference()){
        // Don't assume that state has not changed since last UpdateInformation()
        ref = this->FollowReference(this->ReferenceElement);
        while(ref){
            e = (XdmfElement *)this->GetReferenceObject(ref);
            if(e && (e != this)){
                // Does it need Updating ?
                if(e->GetState() < XDMF_ELEMENT_STATE_LIGHT_PARSED) e->UpdateInformation();
                if(e->GetState() < XDMF_ELEMENT_STATE_HEAVY_READ) e->Update();
                this->Element = e->Element;
                return(this->Copy(e));
            }
            // No External Reference Object
            node = this->FollowReference(ref);
            if(node){
                ref = node;
            }else{
                // No Reference Object and this is the end of the chain
                // If this is a derived Class, Update will act on this target.
                this->SetElement(ref);
                // This is the end of the chain and there is no ReferenceObject for the XML node.
                XdmfDebug("Setting Reference Object");
                this->SetReferenceObject(ref, this);
                ref = NULL;
            }
        }
    }
    this->State = XDMF_ELEMENT_STATE_HEAVY_READ;
    return(XDMF_SUCCESS);
}

XdmfXmlNode
XdmfElement::FollowReference(XdmfXmlNode Element){
    XdmfConstString Value;
    XdmfXmlNode     ref = NULL;

    if(!Element){
        XdmfErrorMessage("Element is NULL");
        return(NULL);
    }
    Value = this->DOM->Get(Element, "Reference");
    if(Value){
        if(STRCASECMP(Value, "XML") == 0){
            Value = this->DOM->GetCData(Element);
            if(!Value){
                XdmfErrorMessage("Reference to CDATA is NULL");
                return(NULL);
            }
        }
        XdmfDebug("Following Reference to " << Value);
        ref = this->DOM->FindElementByPath(Value);
        if(!ref){
            XdmfErrorMessage("Can't Find Node of Path " << Value);
            return(NULL);
        }
    }
    return(ref);
}

XdmfXmlNode
XdmfElement::CheckForReference(XdmfXmlNode Element){
    XdmfElement *e;
    XdmfXmlNode node;

    XdmfDebug("XdmfElement::CheckForReference(XdmfXmlNode Element)");
    if(!Element) return(NULL);
    // Does the Referenced Node Exist and is it of the Same Type
    node = this->FollowReference(Element);
    if(node){
        XdmfDebug("Element is a Reference");
        // Check Type (low level XML "name") against this->Element
        if(STRCMP((const char *)node->name, (const char *)Element->name) != 0){
            XdmfErrorMessage("Reference node is a " << node->name << " not " << Element->name);
            return(NULL);
        }
    }else{
        // This is not a Reference Node
        return(NULL);
    }
    XdmfDebug("Setting ReferenceElement");
    this->ReferenceElement = Element;
    this->SetIsReference(1);
    return(node);
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
    if(this->GetIsReference()){
        return(this->DOM->Serialize(this->ReferenceElement));
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
