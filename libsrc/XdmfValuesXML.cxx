/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and ValuesXML              */
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
#include "XdmfValuesXML.h"
#include "XdmfDataStructure.h"
#include "XdmfArray.h"

XdmfValuesXML::XdmfValuesXML() {
}

XdmfValuesXML::~XdmfValuesXML() {
}

XdmfArray *
XdmfValuesXML::Read(XdmfArray *Array){
    XdmfArray   *RetArray = Array;

    if(!this->DataStructure){
        XdmfErrorMessage("DataStructure has not been set");
        return(NULL);
    }
    // Allocate Array if Necessary
    if(!RetArray){
        RetArray = new XdmfArray();
        RetArray->CopyType(this->DataStructure->GetDataDesc());
        RetArray->CopyShape(this->DataStructure->GetDataDesc());
        RetArray->CopySelection(this->DataStructure->GetDataDesc());
    }
    if(RetArray->SetValues(0, this->DataStructure->Get("CDATA")) != XDMF_SUCCESS){
        XdmfErrorMessage("Error Accessing Actual Data Values");
        if(!Array) delete RetArray;
        RetArray = NULL;
    }
    return(RetArray);
}

XdmfInt32
XdmfValuesXML::Write(XdmfArray *Array){

    if(!this->DataStructure){
        XdmfErrorMessage("DataStructure has not been set");
        return(NULL);
    }
    if(!Array){
        XdmfErrorMessage("Array to Write is NULL");
        return(XDMF_FAIL);
    }
    return(this->DataStructure->Set("CDATA", Array->GetValues(0)));
}
