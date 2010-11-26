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
#include "XdmfInformation.h"

XdmfInformation::XdmfInformation() {
    this->SetElementName("Information");
    this->Value = NULL;
    this->NumberOfInformations = 0;
    this->Informations = (XdmfInformation **)calloc(1, sizeof( XdmfInformation * ));
}

XdmfInformation::~XdmfInformation() {
  for (int Index = 0; Index < this->NumberOfInformations; Index ++ ){
    if (this->Informations[Index]->GetDeleteOnGridDelete()){
        delete this->Informations[Index];
    }
  }
  free(this->Informations);
  delete[] this->Value;
}

XdmfInt32
XdmfInformation::Insert( XdmfElement *Child){
    if(Child && XDMF_WORD_CMP(Child->GetElementName(), "Information")){
      XdmfInt32 status = XdmfElement::Insert(Child);
      if((status == XDMF_SUCCESS) && XDMF_WORD_CMP(Child->GetElementName(), "Information")){
            XdmfInformation *ChildInfo = (XdmfInformation *)Child;
            this->NumberOfInformations++;
            this->Informations = ( XdmfInformation **)realloc( this->Informations,
                this->NumberOfInformations * sizeof( XdmfInformation * ));
            if(!this->Informations) {
                XdmfErrorMessage("Realloc of Information List Failed");
                return(XDMF_FAIL);
            }
            this->Informations[this->NumberOfInformations - 1] = ChildInfo;
      }
    }else{
        XdmfErrorMessage("Information can only Insert Information elements");
    }
    return(XDMF_FAIL);
}

XdmfInt32 XdmfInformation::UpdateInformation(){
    XdmfConstString aValue;

    if(XdmfElement::UpdateInformation() != XDMF_SUCCESS) return(XDMF_FAIL);
    aValue = this->Get("Name");
    if(aValue) this->SetName(aValue);
    aValue = this->Get("Value");
    if(!aValue) aValue = this->Get("CDATA");
    if(aValue) this->SetValue(aValue);
    return(XDMF_SUCCESS);
}

XdmfInt32 XdmfInformation::Build(){
    if(XdmfElement::Build() != XDMF_SUCCESS) return(XDMF_FAIL);
    // If Value isn't already an XML Attribute and
    // the value is > 10 chars, put it in the CDATA
    if((this->Get("Value") == NULL)  && (strlen(this->Value) > 10)){
        this->Set("CDATA", this->Value);
    }else{
        this->Set("Value", this->Value);
    }
    return(XDMF_SUCCESS);
}
