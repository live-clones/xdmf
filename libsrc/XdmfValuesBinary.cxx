/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id */ 
/*  Date : $Date$ */
/*  Version : $Revision$  */
/*                                                                 */
/*  Author:Kenji Takizawa (Team for Advanced Flow Simulation and Modeling) */
/*                                                                 */
/*     Copyright @ 2008 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#include "XdmfValuesBinary.h"
#include "XdmfDataStructure.h"
#include "XdmfArray.h"
#include "XdmfHDF.h"


//#include <sys/stat.h>
//#include <cassert>

XdmfValuesBinary::XdmfValuesBinary() {
    this->Endian = NULL;
    this->SetFormat(XDMF_FORMAT_BINARY);
}
XdmfValuesBinary::~XdmfValuesBinary() {
}

XdmfArray *
XdmfValuesBinary::Read(XdmfArray *anArray){
    if(!this->DataDesc){
        XdmfErrorMessage("DataDesc has not been set");
        return(NULL);
    }
    XdmfArray   *RetArray = anArray;
    // Allocate anArray if Necessary
    if(!RetArray){
        RetArray = new XdmfArray();
        RetArray->CopyType(this->DataDesc);
        RetArray->CopyShape(this->DataDesc);
    }
    XdmfDebug("Accessing Binary CDATA");
    XdmfConstString Value = this->Get("Endian");
    if(Value){
        this->SetEndian(Value);
    }else{
        this->Endian = NULL;
    }
    XdmfString  DataSetName = 0;
    XDMF_STRING_DUPLICATE(DataSetName, this->Get("CDATA"));
    XDMF_WORD_TRIM(DataSetName);
    XdmfDebug("Opening Binary Data for Reading : " << DataSetName);
    XdmfInt64 dims[XDMF_MAX_DIMENSION];
    XdmfInt32 rank = this->DataDesc->GetShape(dims);
    XdmfInt64 total = 1;
    for(XdmfInt32 i=0;i<rank;++i){
        total *= dims[i];
    }
    XdmfDebug("Data Size : " << total);
    XdmfDebug("Size[Byte]: " << RetArray->GetCoreLength());
//check
//    struct stat buf;
//    stat(DataSetName, &buf);
//    assert(buf.st_size == RetArray->GetCoreLength());

    ifstream fs(DataSetName,std::ios::binary);
    if(!fs.good()){
        XdmfErrorMessage("Can't Open File " << DataSetName);
        return(NULL);
    }
    if( Array->GetDataPointer() == NULL ){
        XdmfErrorMessage("Memory Object Array has no data storage");
        return( NULL );
    }
    fs.read(reinterpret_cast<char*>(RetArray->GetDataPointer()), RetArray->GetCoreLength());
    //When endian is different to this system....
    //
    fs.close();
    return RetArray;
}
XdmfInt32
XdmfValuesBinary::Write(XdmfArray *anArray, XdmfConstString /*HeavyDataSetName*/){
    return 0;
}
// vim: expandtab sw=4 :

