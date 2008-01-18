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
#include "XdmfTime.h"

#include "XdmfDOM.h"
#include "XdmfArray.h"
#include "XdmfDataItem.h"

XdmfTime::XdmfTime() {
    this->SetElementName("Time");
    this->Value = 0.0;
    this->Array = NULL;
    this->DataItem = new XdmfDataItem;
    this->TimeType = XDMF_TIME_SINGLE;
    this->Function = NULL;
}

XdmfTime::~XdmfTime() {
    if(this->DataItem) delete this->DataItem;
}

XdmfInt32
XdmfTime::Insert( XdmfElement *Child){
    if(Child && XDMF_WORD_CMP(Child->GetElementName(), "Time")){
        return(XdmfElement::Insert(Child));
    }else{
        XdmfErrorMessage("Time can only Insert Time elements");
    }
    return(XDMF_FAIL);
}

XdmfInt32 XdmfTime::UpdateInformation(){
    XdmfConstString attribute;
    XdmfFloat64 dValue;

    if(XdmfElement::UpdateInformation() != XDMF_SUCCESS) return(XDMF_FAIL);
    attribute = this->Get("TimeType");
    if(!attribute) attribute = this->Get("Type");
    if( XDMF_WORD_CMP(attribute, "Single") ){
        this->TimeType = XDMF_TIME_SINGLE;
    }else if( XDMF_WORD_CMP(attribute, "Uniform") ){
        this->TimeType = XDMF_TIME_UNIFORM;
    }else if( XDMF_WORD_CMP(attribute, "HyperSlab") ){
        this->TimeType = XDMF_TIME_HYPERSLAB;
    }else if( XDMF_WORD_CMP(attribute, "Function") ){
        this->TimeType = XDMF_TIME_FUNCTION;
    }else{
        if(attribute){
            XdmfErrorMessage("Unknown Time Type : " << attribute);
            return(XDMF_FAIL);
        }
        // Default
        this->TimeType = XDMF_TIME_SINGLE;
    }
    // Type == Function ?
    attribute = this->Get("Function");
    if(attribute){
        this->TimeType = XDMF_TIME_FUNCTION;
        this->SetFunction(attribute);
        return(XDMF_SUCCESS);
    }
    attribute = this->Get("Value");
    if(attribute){
        istrstream Value_ist(const_cast<char*>(attribute), strlen(attribute) );
        Value_ist >> dValue;
        this->SetValue(dValue);
    }else{
        XdmfXmlNode     node;

        if(this->TimeType == XDMF_TIME_SINGLE){
            XdmfErrorMessage("TimeType is Single but there is no Value Attribute");
            return(XDMF_FAIL);
        }
        node = this->DOM->FindDataElement(0, this->GetElement());
        if(!node){
            XdmfErrorMessage("No Time Value is set and there is no DataItem");
            return(XDMF_FAIL);
        }
        if(this->DataItem->SetDOM(this->DOM) == XDMF_FAIL) return(XDMF_FAIL);
        if(this->DataItem->SetElement(node) == XDMF_FAIL) return(XDMF_FAIL);
        if(this->DataItem->UpdateInformation() == XDMF_FAIL) return(XDMF_FAIL);
        if(this->DataItem->Update() == XDMF_FAIL) return(XDMF_FAIL);
        this->Array = this->DataItem->GetArray();
    }
    return(XDMF_SUCCESS);
}

XdmfInt32 XdmfTime::Build(){
    if(XdmfElement::Build() != XDMF_SUCCESS) return(XDMF_FAIL);
    this->Set("TimeType", this->GetTimeTypeAsString());
    if(this->TimeType == XDMF_TIME_FUNCTION){
        this->Set("Function", this->Function);
        return(XDMF_SUCCESS);
    }
    if(this->Array){
        XdmfDataItem    *di = NULL;
        XdmfXmlNode     node;

        XdmfDebug("Build for XdmfTime = " << this);
        //! Is there already a DataItem
        node = this->DOM->FindDataElement(0, this->GetElement());
        if(node) {
            di = (XdmfDataItem *)this->GetCurrentXdmfElement(node);
            XdmfDebug("DataItem  = " << di);
        }
        if(!di){
            di = new XdmfDataItem;
            node = this->DOM->InsertNew(this->GetElement(), "DataItem");
            di->SetDOM(this->DOM);
            di->SetElement(node);
            if(this->Array->GetNumberOfElements() > 100) di->SetFormat(XDMF_FORMAT_HDF);
        }
        if(this->Array != di->GetArray()){
            XdmfDebug("Setting Array since " << this->Array << " != " << di->GetArray());
            di->SetArray(this->Array);
        }
        XdmfDebug("Building DataItem");
        di->Build();
    }else{
        ostrstream   StringOutput;
        StringOutput << this->Value << ends;
        this->Set("Value", StringOutput.str());
    }
    return(XDMF_SUCCESS);
}

XdmfConstString
XdmfTime::GetTimeTypeAsString(void){
    switch(this->TimeType){
        case XDMF_TIME_UNIFORM :
            return("Uniform");
        case XDMF_TIME_HYPERSLAB:
            return("HyperSlab");
        case XDMF_TIME_FUNCTION:
            return("Function");
        default :
            return("Single");
    }
}
