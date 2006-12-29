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
    this->SetFormat(XDMF_FORMAT_XML);
}

XdmfValuesXML::~XdmfValuesXML() {
}

XdmfArray *
XdmfValuesXML::Read(XdmfArray *Array){
    XdmfArray   *RetArray = Array;

    if(!this->DataDesc){
        XdmfErrorMessage("DataDesc has not been set");
        return(NULL);
    }
    // Allocate Array if Necessary
    if(!RetArray){
        RetArray = new XdmfArray();
        RetArray->CopyType(this->DataDesc);
        RetArray->CopyShape(this->DataDesc);
        RetArray->CopySelection(this->DataDesc);
    }
    if(RetArray->SetValues(0, this->Get("CDATA")) != XDMF_SUCCESS){
        XdmfErrorMessage("Error Accessing Actual Data Values");
        if(!Array) delete RetArray;
        RetArray = NULL;
    }
    return(RetArray);
}

XdmfInt32
XdmfValuesXML::Write(XdmfArray *Array){

    XdmfConstString DataValues;
    ostrstream   StringOutput;
    XdmfInt32   rank, r;
    XdmfInt64   i, index, nelements, len, idims[XDMF_MAX_DIMENSION], dims[XDMF_MAX_DIMENSION];

    if(!this->DataDesc ){
        XdmfErrorMessage("DataDesc has not been set");
        return(XDMF_FAIL);
    }
    if(!Array){
        XdmfErrorMessage("Array to Write is NULL");
        return(XDMF_FAIL);
    }
    /*
    DataValues = Array->GetValues(0);
    if(!DataValues){
        XdmfErrorMessage("Error Getting Values from Array");
        return(XDMF_FAIL);
    }
    */
    rank = this->DataDesc->GetShape(dims);
    for(i = 0 ; i < rank ; i++){
        idims[i] = dims[i];
    }
    // At most 10 values per line
    len = MIN(dims[rank - 1], 10);
    nelements = this->DataDesc->GetNumberOfElements();
    index = 0;
    r = rank - 2;
    StringOutput << endl;
    while(nelements){
        len = MIN(len, nelements);
        DataValues = Array->GetValues(index, len);
        index += len;
        StringOutput << DataValues << endl;
        nelements -= len;
        if(nelements && (r >= 0)){
            dims[r] -= 1;
            if(dims[r] == 0){
                StringOutput << endl;
                // reset
                dims[r] = idims[r];
                while(r > 0){
                    r--;
                    if(dims[r]) dims[r] -= 1;
                    if(dims[r] == 0){
                        dims[r+1] = idims[r+1];
                        StringOutput << "---" << endl;
                    }
                }
                r = rank - 2;
            }
        }
    }
    return(this->Set("CDATA", StringOutput.str()));
}
