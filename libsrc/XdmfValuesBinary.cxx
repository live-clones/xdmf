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
template<size_t T>
struct ByteSwaper {
    static inline void swap(void*p){}
    static inline void swap(void*p,XdmfInt64 length){
        char* data = static_cast<char*>(p);
        for(XdmfInt64 i=0;i<length;++i, data+=T){
            ByteSwaper<T>::swap(data);
        }
    }
};

template<>
void ByteSwaper<2>::swap(void*p){
    char one_byte;
    char* data = static_cast<char*>(p);
    one_byte = data[0]; data[0] = data[1]; data[1] = one_byte;
};
template<>
void ByteSwaper<4>::swap(void*p){
    char one_byte;
    char* data = static_cast<char*>(p);
    one_byte = data[0]; data[0] = data[3]; data[3] = one_byte;
    one_byte = data[1]; data[1] = data[2]; data[2] = one_byte;
};
template<>
void ByteSwaper<8>::swap(void*p){
    char one_byte;
    char* data = static_cast<char*>(p);
    one_byte = data[0]; data[0] = data[7]; data[7] = one_byte;
    one_byte = data[1]; data[1] = data[6]; data[6] = one_byte;
    one_byte = data[2]; data[2] = data[5]; data[5] = one_byte;
    one_byte = data[3]; data[3] = data[4]; data[4] = one_byte;
};
void XdmfValuesBinary::byteSwap(XdmfArray * RetArray){
    if(needByteSwap()){
        switch(RetArray->GetElementSize()){
        case 1:
            break;
        case 2:
            ByteSwaper<2>::swap(RetArray->GetDataPointer(),RetArray->GetNumberOfElements());
            break;
        case 4:
            ByteSwaper<4>::swap(RetArray->GetDataPointer(),RetArray->GetNumberOfElements());
            break;
        case 8:
            ByteSwaper<8>::swap(RetArray->GetDataPointer(),RetArray->GetNumberOfElements());
            break;
        default:
            break;
        }
    }
}

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
    if( RetArray->GetDataPointer() == NULL ){
        XdmfErrorMessage("Memory Object Array has no data storage");
        return( NULL );
    }
    fs.read(reinterpret_cast<char*>(RetArray->GetDataPointer()), RetArray->GetCoreLength());
    fs.close();
    //When endian is different to this system....
    byteSwap(RetArray);
    return RetArray;
}


XdmfInt32
XdmfValuesBinary::Write(XdmfArray *anArray, XdmfConstString aHeavyDataSetName){
    if(!aHeavyDataSetName) aHeavyDataSetName = this->GetHeavyDataSetName();
    if(anArray->GetHeavyDataSetName()){
        aHeavyDataSetName = (XdmfConstString)anArray->GetHeavyDataSetName();
    }else{
        return(XDMF_FAIL);
    }
    XdmfDebug("Writing Values to " << aHeavyDataSetName);
    if(!this->DataDesc ){
        XdmfErrorMessage("DataDesc has not been set");
        return(XDMF_FAIL);
    }
    if(!anArray){
        XdmfErrorMessage("Array to Write is NULL");
        return(XDMF_FAIL);
    }
    char* hds;
    XDMF_STRING_DUPLICATE(hds, aHeavyDataSetName);
    XDMF_WORD_TRIM( hds );
    this->Set("CDATA", hds);
    byteSwap(anArray);
    ofstream fs(aHeavyDataSetName,std::ios::binary);
    if(!fs.good()){
        XdmfErrorMessage("Can't Open File " << aHeavyDataSetName);
        return(NULL);
    }
    if( anArray->GetDataPointer() == NULL ){
        XdmfErrorMessage("Memory Object Array has no data storage");
        return( NULL );
    }
    fs.write(reinterpret_cast<char*>(anArray->GetDataPointer()), anArray->GetCoreLength());
    fs.close();
    byteSwap(anArray);
    delete [] hds;
    return(XDMF_SUCCESS);
}
#ifdef CMAKE_WORDS_BIGENDIAN
bool XdmfValuesBinary::needByteSwap(){
    return XDMF_WORD_CMP(Endian, "Little");
}
#else
bool XdmfValuesBinary::needByteSwap(){
    return XDMF_WORD_CMP(Endian, "Big");
}
#endif
// vim: expandtab sw=4 :
