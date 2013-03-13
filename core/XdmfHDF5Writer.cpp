/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5Writer.cpp                                                  */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#include <hdf5.h>
#include <sstream>
#include <cstdio>
#include <set>
#include <list>
#include "XdmfItem.hpp"
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfError.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"

/**
 * PIMPL
 */
class XdmfHDF5Writer::XdmfHDF5WriterImpl {

public:

  XdmfHDF5WriterImpl():
    mHDF5Handle(-1),
    mOpenFile(""),
    mHDF5FileSizeLimit(-1),
    mDepth(0),
    mFileIndex(0)
  {
  };

  ~XdmfHDF5WriterImpl()
  {
    closeFile();
  };

  void
  closeFile()
  {
    if(mHDF5Handle >= 0) {
      herr_t status = H5Fclose(mHDF5Handle);
      mHDF5Handle = -1;
    }
    mOpenFile = "";
  };  

  int
  openFile(const std::string & filePath,
           const int fapl)
  {
    if(mHDF5Handle >= 0) {
      // Perhaps we should throw a warning.
      closeFile();
    }

    // Save old error handler and turn off error handling for now
    H5E_auto_t old_func;
    void * old_client_data;
    H5Eget_auto(0, &old_func, &old_client_data);
    H5Eset_auto2(0, NULL, NULL);
  
    int toReturn = 0;

    mOpenFile.assign(filePath);

    if(H5Fis_hdf5(filePath.c_str()) > 0) {
      mHDF5Handle = H5Fopen(filePath.c_str(), 
                            H5F_ACC_RDWR, 
                            fapl);
      hsize_t numObjects;
      herr_t status = H5Gget_num_objs(mHDF5Handle,
                                      &numObjects);
      toReturn = numObjects;
    }
    else {
      mHDF5Handle = H5Fcreate(filePath.c_str(),
                              H5F_ACC_TRUNC,
                              H5P_DEFAULT,
                              fapl);
    }

    // Restore previous error handler
    H5Eset_auto2(0, old_func, old_client_data);

    return toReturn;

  }

  hid_t mHDF5Handle;
  std::string mOpenFile;
  int mHDF5FileSizeLimit;
  int mFileIndex;
  int mDepth;
  std::set<const XdmfItem *> mWrittenItems;

};

shared_ptr<XdmfHDF5Writer>
XdmfHDF5Writer::New(const std::string & filePath,
                    const bool clobberFile)
{
  if(clobberFile) {
    std::remove(filePath.c_str());
  }
  shared_ptr<XdmfHDF5Writer> p(new XdmfHDF5Writer(filePath));
  return p;
}

XdmfHDF5Writer::XdmfHDF5Writer(const std::string & filePath) :
  XdmfHeavyDataWriter(filePath),
  mImpl(new XdmfHDF5WriterImpl())
{
}

XdmfHDF5Writer::~XdmfHDF5Writer()
{
  delete mImpl;
}

shared_ptr<XdmfHDF5Controller>
XdmfHDF5Writer::createHDF5Controller(const std::string & hdf5FilePath,
                                     const std::string & dataSetPath,
                                     const shared_ptr<const XdmfArrayType> type,
                                     const std::vector<unsigned int> & start,
                                     const std::vector<unsigned int> & stride,
                                     const std::vector<unsigned int> & dimensions,
                                     const std::vector<unsigned int> & dataspaceDimensions)
{
  return XdmfHDF5Controller::New(hdf5FilePath,
                                 dataSetPath,
                                 type,
                                 start,
                                 stride,
                                 dimensions,
                                 dataspaceDimensions);
}


void
XdmfHDF5Writer::setFileSizeLimit(int newSize)
{
  mImpl->mHDF5FileSizeLimit = newSize;
}

int
XdmfHDF5Writer::getFileSizeLimit()
{
  return mImpl->mHDF5FileSizeLimit;
}

void 
XdmfHDF5Writer::closeFile()
{
  mImpl->closeFile();
}

void 
XdmfHDF5Writer::openFile()
{
  this->openFile(H5P_DEFAULT);
}

void
XdmfHDF5Writer::openFile(const int fapl)
{
  mDataSetId = mImpl->openFile(mFilePath,
                               fapl);
}

void
XdmfHDF5Writer::visit(XdmfArray & array,
                      const shared_ptr<XdmfBaseVisitor> visitor)
{
  mImpl->mDepth++;
  std::set<const XdmfItem *>::iterator checkWritten = mImpl->mWrittenItems.find(&array);
  if (checkWritten == mImpl->mWrittenItems.end() || array.getItemTag() == "DataItem") {
    //if it has children send the writer to them too.
    array.traverse(visitor);
    //only do this if the object has not already been written
    this->write(array, H5P_DEFAULT);
    mImpl->mWrittenItems.insert(&array);

  }
  //if the object has already been written, just end, it already has the data
  mImpl->mDepth--;
  if(mImpl->mDepth <= 0) {
    mImpl->mWrittenItems.clear();
  }
}

void
XdmfHDF5Writer::visit(XdmfItem & item,
                      const shared_ptr<XdmfBaseVisitor> visitor)
{
  mImpl->mDepth++;
  //This is similar to the algorithm for writing XPaths
  //shouldn't be a problem if XPaths are turned off because all this does is avoid writing an object twice
  //if it was written once then all instances of the object should have the ocntroller
  std::set<const XdmfItem *>::iterator checkWritten = mImpl->mWrittenItems.find(&item);
  if (checkWritten == mImpl->mWrittenItems.end()) {
    mImpl->mWrittenItems.insert(&item);
    item.traverse(visitor);
  }
  mImpl->mDepth--;
  if(mImpl->mDepth <= 0) {
    mImpl->mWrittenItems.clear();
  }
}


void
XdmfHDF5Writer::write(XdmfArray & array,
                      const int fapl)
{
  hid_t datatype = -1;
  bool closeDatatype = false;

  if(array.isInitialized()) {
    if(array.getArrayType() == XdmfArrayType::Int8()) {
      datatype = H5T_NATIVE_CHAR;
    }
    else if(array.getArrayType() == XdmfArrayType::Int16()) {
      datatype = H5T_NATIVE_SHORT;
    }
    else if(array.getArrayType() == XdmfArrayType::Int32()) {
      datatype = H5T_NATIVE_INT;
    }
    else if(array.getArrayType() == XdmfArrayType::Int64()) {
      datatype = H5T_NATIVE_LONG;
    }
    else if(array.getArrayType() == XdmfArrayType::Float32()) {
      datatype = H5T_NATIVE_FLOAT;
    }
    else if(array.getArrayType() == XdmfArrayType::Float64()) {
      datatype = H5T_NATIVE_DOUBLE;
    }
    else if(array.getArrayType() == XdmfArrayType::UInt8()) {
      datatype = H5T_NATIVE_UCHAR;
    }
    else if(array.getArrayType() == XdmfArrayType::UInt16()) {
      datatype = H5T_NATIVE_USHORT;
    }
    else if(array.getArrayType() == XdmfArrayType::UInt32()) {
      datatype = H5T_NATIVE_UINT;
    }
    else if(array.getArrayType() == XdmfArrayType::String()) {
      datatype = H5Tcopy(H5T_C_S1);
      H5Tset_size(datatype, H5T_VARIABLE);
      closeDatatype = true;
    }
    else {
      XdmfError::message(XdmfError::FATAL,
                         "Array of unsupported type in "
                         "XdmfHDF5Writer::write");
    }
  }

  herr_t status;

  if(datatype != -1) {
    std::string hdf5FilePath = mFilePath;

    size_t extIndex;
    std::string checkFileName;
    std::string checkFileExt;
    extIndex = hdf5FilePath.find_last_of(".");
    if (extIndex == std::string::npos) {
      checkFileName = hdf5FilePath;
      checkFileExt = "";
    }
    else {
      checkFileName = hdf5FilePath.substr(0, extIndex);
      checkFileExt = hdf5FilePath.substr(extIndex+1);
    }

    std::stringstream dataSetPath;

    std::vector<shared_ptr<XdmfHeavyDataController> > previousControllers;

    for(int i = 0; i < array.getNumberHeavyDataControllers(); i++) {
      previousControllers.push_back(array.getHeavyDataController(i));
    }

    //remove controllers from the array, they will be replaced by the controllers created by this function.
    while(array.getNumberHeavyDataControllers() != 0) {
      array.removeHeavyDataController(array.getNumberHeavyDataControllers() -1);
    }



    if (previousControllers.size() == 0) {
      //create a temporary controller if the array doesn't have one
      shared_ptr<XdmfHDF5Controller> tempDataController =
        this->createHDF5Controller(hdf5FilePath,
                                   "Data",
                                   array.getArrayType(),
                                   std::vector<unsigned int>(1, 0),
                                   std::vector<unsigned int>(1, 1),
                                   std::vector<unsigned int>(1, array.getSize()),
                                   std::vector<unsigned int>(1, array.getSize()));
      previousControllers.push_back(tempDataController);
    }

    int controllerIndexOffset = 0;

    for(int i = 0; i < previousControllers.size(); i++)// it is assumed that the array will have at least one controller
    {
      if (mMode == Append) {//append only cares about the last controller, so add the rest back in
	for (; i < previousControllers.size() - 1; i++) {
          array.insert(previousControllers[i]);
	}
      }

      shared_ptr<XdmfHeavyDataController> heavyDataController =
        previousControllers[i];
      //stats for the data currently stored in the array
      const std::vector<unsigned int> & dimensions = array.getDimensions();
      std::vector<unsigned int> dataspaceDimensions = dimensions;
      std::vector<unsigned int> start(dimensions.size(), 0);
      std::vector<unsigned int> stride(dimensions.size(), 1);

      if((mMode == Overwrite || mMode == Append || mMode == Hyperslab)
        && heavyDataController) {

        // Write to the previous dataset
        dataSetPath.str(std::string());
        dataSetPath << heavyDataController->getDataSetPath();
        hdf5FilePath = heavyDataController->getFilePath();
        if(mMode == Hyperslab) {//start, stride, and dataspace dimensions only matter for hyperslab mode
          dataspaceDimensions = heavyDataController->getDataspaceDimensions();
          start = heavyDataController->getStart();
          stride = heavyDataController->getStride();
        }
      }
      else {
        dataSetPath.str(std::string());
        dataSetPath << "Data" << mDataSetId;
      }

      // Open a hdf5 dataset and write to it on disk.
      hsize_t size = array.getSize();

      // Save old error handler and turn off error handling for now
      H5E_auto_t old_func;
      void * old_client_data;
      H5Eget_auto(0, &old_func, &old_client_data);
      H5Eset_auto2(0, NULL, NULL);

      //check here for if the file would become larger than the limit after the addition.
      //then check subsequent files for the same limitation
      //currently assuming that at least one dimension will be written per file.
      //probably for append and default	
      //no need to check for overwrite the data is already alloted
      //  might need to check after all as overwrite might expand data size.
      //might need to check for hyperslab, if the data wasn't alloted yet.

      bool splittingPossible = false;
      //check if splitting is necessary
      if (mMode == Overwrite) {//might need a split if data written is larger than data alotted
        splittingPossible = true;
      }
      if (mMode == Default) {//new data set is created with each call
        splittingPossible = true;
      }
      if (mMode == Append) {//data is added onto end of sets
        splittingPossible = true;
      }
      if (mMode == Hyperslab) {//splitting is only required if the slab is not set up yet
        //check if slab already exists
        int numData = mImpl->openFile(hdf5FilePath,
                                      fapl);
        if (numData > 0) {//if it already exists the file does not need to be split.
          splittingPossible = false;
        }
        else {
          splittingPossible = true;
        }
      }

      std::list<std::string> filesWritten;
      std::list<shared_ptr<XdmfArray> > arraysWritten;
      std::list<std::vector<unsigned int> > startsWritten;
      std::list<std::vector<unsigned int> > stridesWritten;
      std::list<std::vector<unsigned int> > dimensionsWritten;
      std::list<std::vector<unsigned int> > dataSizesWritten;

      //this is the file splitting algorithm
      if (mImpl->mHDF5FileSizeLimit > 0 && splittingPossible) {//only if the file limit is positive, disabled if 0 or negative
        //repeat until a suitable file is found
        bool suitableFound = false;
        unsigned int currentDimension = 0;
        unsigned int previousDimension = currentDimension;
        unsigned int previousDataSize = 0;
        //Even though theoretically this could be an infinite loop
        //if all possible files with the specified name are produced
        //the chances of that happening are small.
        //It can handle up to 65535 different files.
        //This value may vary depending on the compiler and platform.
        //The variable UINT_MAX holds the value in question.
        //If all files are take up it will loop until a file opens up since adding past the max causes overflow.
        while (currentDimension < dimensions.size()) {

          std::stringstream testFile;
          if (mImpl->mFileIndex == 0) {//if sequentially named files need to be created or referenced
            testFile << checkFileName << "." << checkFileExt;
          }
          else {
            testFile << checkFileName << mImpl->mFileIndex << "." << checkFileExt;
          }
          int sizeOffset = 0;
          FILE *checkFile = NULL;
          int fileSize = 0;//if the file doesn't exist the size is 0 because there's no data
          // get the file stream
          checkFile = fopen(testFile.str().c_str(), "a");
          if (checkFile != NULL) {
            // set the file pointer to end of file
            fseek(checkFile, 0, SEEK_END);
            // get the file size, in bytes
            fileSize = ftell(checkFile);

            //if overwrite subtract previous data size.
            if (mMode == Overwrite) {
              //find previous data size
              mImpl->openFile(testFile.str(),
                              fapl);
              hid_t checkset = H5Dopen(mImpl->mHDF5Handle,
                                       dataSetPath.str().c_str(),
                                       H5P_DEFAULT);
              hid_t checkspace = H5S_ALL;
              checkspace = H5Dget_space(checkset);
              hssize_t checksize = H5Sget_simple_extent_npoints(checkspace);
              status = H5Dclose(checkset);
              if(checkspace != H5S_ALL) {
                status = H5Sclose(checkspace);
              }
              fileSize = fileSize - checksize;//remove previous set's size, since it's overwritten
              if (fileSize < 0) {
                fileSize = 0;
              }
            }
            fclose(checkFile);
          }
          else if (previousDataSize == 0) {
            sizeOffset += 800;//base size of an hdf5 file is 800
          }
          // close stream and release buffer
          //check size to see if it's within range
          if (closeDatatype == true) {//closetype is only true if strings are being used, it's set at the beginning when types are checked.
            //compile the length of all stored strings
            //this would be time consuming but it's required to accurately guess the size of the data stored
            //Advise against using split HDF5 files and strings in conjunction.
            //Probably would lengthen execution times significantly
            int arrayStartIndex = 0;//starting index
            for (int j = 0; j < previousDimension; j++) {
              arrayStartIndex += dimensions[j];
            }
            //if needed split the written array into smaller arrays based on dimension blocks
            while (currentDimension < dataspaceDimensions.size()) {
              int sizeSubTotal = 0;
              for (int j = arrayStartIndex; j < arrayStartIndex+dimensions[currentDimension]; j++) {
                sizeSubTotal += array.getValue<std::string>(j).length() * 8;
              }
              if (sizeSubTotal+previousDataSize+fileSize > mImpl->mHDF5FileSizeLimit*(1024*1024)) {
                if (sizeSubTotal + 800 > mImpl->mHDF5FileSizeLimit*(1024*1024)) {
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Specified file size is too small for dimensions of XdmfArray being written.\n");
                }
                mImpl->mFileIndex++;
                break;
              }
              else {
                sizeOffset += sizeSubTotal;
              }
              currentDimension++;
            }
          }
          else {
            int dataItemSize = 0;
            //data item size depends on the variable type
            dataItemSize = array.getArrayType()->getElementSize();

            //if needed split the written array into smaller arrays based on dimension blocks
            int sizeSubTotal = 0;
            while (currentDimension < dataspaceDimensions.size()) {
              sizeSubTotal = dataspaceDimensions[currentDimension] * dataItemSize;
              if (sizeSubTotal+previousDataSize+fileSize > mImpl->mHDF5FileSizeLimit*(1024*1024)) {
                if (sizeSubTotal + 800 > mImpl->mHDF5FileSizeLimit*(1024*1024)) {
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Specified file size is too small for dimensions of XdmfArray being written.\n");
                }
                mImpl->mFileIndex++;
                break;
              }
              else {
                sizeOffset += sizeSubTotal;
              }
              currentDimension++;
              //maybe move to next controller?
            }
          }
          if (sizeOffset > 0) {//If data is written place the partition in the appropriate vectors
            previousDataSize += sizeOffset;
            //place up to dimension j-1 into the array and add it to the arrays to be written
            int containedInPriorDimensions = controllerIndexOffset;//starting index
            int containedInDimensions = 0;//count moved
            std::vector<unsigned int> partialStarts;
            std::vector<unsigned int> partialStrides;
            std::vector<unsigned int> partialDimensions;
            std::vector<unsigned int> partialDataSizes;
            int j = 0;
            for (j = 0; j < previousDimension; j++) {
              containedInPriorDimensions += dimensions[j];
            }
            for (j = previousDimension; j < currentDimension; j++) {
              containedInDimensions += dimensions[j];
              //if the array fits inside the dimensions without garbage data
              if (containedInDimensions + containedInPriorDimensions < array.getSize()) {
                partialStarts.push_back(start[j]);
                partialStrides.push_back(stride[j]);
                partialDimensions.push_back(dimensions[j]);
                partialDataSizes.push_back(dataspaceDimensions[j]);
              }
              else {//otherwise resize the current dimension to fit what is left in the array
                if (array.getSize() - containedInPriorDimensions > 0) {
                  partialDimensions.push_back(array.getSize() - (containedInDimensions - dimensions[j]));
                  partialStarts.push_back(start[j]);
                  partialStrides.push_back(stride[j]);				
                  partialDataSizes.push_back(dataspaceDimensions[j]);
                  containedInDimensions = (containedInDimensions - dimensions[j]) + (array.getSize() - (containedInDimensions - dimensions[j]));
                }
                else { //if the array fit perfectly in the prior dimensions then just end
                  break;
                }
              }
            }
            if (j == dimensions.size()) {
              controllerIndexOffset = containedInPriorDimensions + containedInDimensions;
            }
            shared_ptr<XdmfArray> partialArray = XdmfArray::New();
            j = containedInPriorDimensions;
            if (datatype == H5T_NATIVE_CHAR){
              partialArray->initialize(XdmfArrayType::Int8(), 0);

              char movedData [containedInDimensions];
              array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
              partialArray->insert(0, movedData, containedInDimensions);

/*
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<char>(j));
              }
*/
            }
            else if (datatype == H5T_NATIVE_SHORT){
              partialArray->initialize(XdmfArrayType::Int16(), 0);

              short movedData [containedInDimensions];
              array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
              partialArray->insert(0, movedData, containedInDimensions);

/*
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<short>(j));
              }
*/
            }
            else if (datatype == H5T_NATIVE_INT){
              partialArray->initialize(XdmfArrayType::Int32(), 0);

              int movedData [containedInDimensions];
              array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
              partialArray->insert(0, movedData, containedInDimensions);

/*
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<int>(j));
              }
*/
            }
            else if (datatype == H5T_NATIVE_LONG){
              partialArray->initialize(XdmfArrayType::Int64(), 0);

              long movedData [containedInDimensions];
              array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
              partialArray->insert(0, movedData, containedInDimensions);

/*
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<long>(j));
              }
*/
            }
            else if (datatype == H5T_NATIVE_FLOAT){
              partialArray->initialize(XdmfArrayType::Float32(), 0);

              float movedData [containedInDimensions];
              array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
              partialArray->insert(0, movedData, containedInDimensions);

/*
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<float>(j));
              }
*/
            }
            else if (datatype == H5T_NATIVE_DOUBLE){
              partialArray->initialize(XdmfArrayType::Float64(), 0);

              double movedData [containedInDimensions];
              array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
              partialArray->insert(0, movedData, containedInDimensions);

/*
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<double>(j));
              }
*/
            }
            else if (datatype == H5T_NATIVE_UCHAR){
              partialArray->initialize(XdmfArrayType::UInt8(), 0);

              unsigned char movedData [containedInDimensions];
              array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
              partialArray->insert(0, movedData, containedInDimensions);

/*
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<unsigned char>(j));
              }
*/
            }
            else if (datatype == H5T_NATIVE_USHORT){
              partialArray->initialize(XdmfArrayType::UInt16(), 0);

              unsigned short movedData [containedInDimensions];
              array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
              partialArray->insert(0, movedData, containedInDimensions);

/*
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<unsigned short>(j));
              }
*/
            }
            else if (datatype == H5T_NATIVE_UINT) {
              partialArray->initialize(XdmfArrayType::UInt32(), 0);

              unsigned int movedData [containedInDimensions];
              array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
              partialArray->insert(0, movedData, containedInDimensions);

/*
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<unsigned int>(j));
              }
*/
            }
            else if (closeDatatype) {//closeDatatype is only true if strings are being used
              partialArray->initialize(XdmfArrayType::String(), 0);
              for (j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; j++){
                partialArray->pushBack(array.getValue<std::string>(j));
              }
            }

            if (partialDimensions.size() == 0) {
              break;
            }


            arraysWritten.push_back(partialArray);
            filesWritten.push_back(testFile.str());
            startsWritten.push_back(partialStarts);
            stridesWritten.push_back(partialStrides);
            dimensionsWritten.push_back(partialDimensions);
            dataSizesWritten.push_back(partialDataSizes);

            previousDimension = currentDimension;
          }
          else {//otherwise reset the dimension location
            currentDimension = previousDimension;
          }
        }
        if (mMode == Append) {
          //if the written filename is different write add the previous controller
          if (*(filesWritten.rbegin()) != heavyDataController->getFilePath()) {
            //should also be different from previous controller
            if (filesWritten.size() > 1) {
              if (*(filesWritten.rbegin()) != *((filesWritten.rbegin())++)) {
                array.insert(heavyDataController);
              }
            }
            else {
              array.insert(heavyDataController);
            }
          }
        }
      }
      else {
        //otherwise work with the full array
        shared_ptr<XdmfArray> partialArray = XdmfArray::New();
	//need to copy by duplicating the contents of the array
	int j = controllerIndexOffset;
        int movedSize = 0;
        if (datatype == H5T_NATIVE_CHAR){
          partialArray->initialize(XdmfArrayType::Int8(), 0);

          if ((array.getSize() - controllerIndexOffset) <= heavyDataController->getSize()) {
            movedSize = array.getSize() - controllerIndexOffset;
          }
          else if (heavyDataController->getSize() < (array.getSize() - controllerIndexOffset)) {
            movedSize = heavyDataController->getSize();
          }
          char movedData [movedSize];
          array.getValues(controllerIndexOffset, movedData, movedSize);
          partialArray->insert(0, movedData, movedSize);
          j+=movedSize;

/*
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize() ; j++){
            partialArray->pushBack(array.getValue<char>(j));
          }
*/
        }
        else if (datatype == H5T_NATIVE_SHORT){
          partialArray->initialize(XdmfArrayType::Int16(), 0);

          if ((array.getSize() - controllerIndexOffset) <= heavyDataController->getSize()) {
            movedSize = array.getSize() - controllerIndexOffset;
          }
          else if (heavyDataController->getSize() < (array.getSize() - controllerIndexOffset)) {
            movedSize = heavyDataController->getSize();
          }
          short movedData [movedSize];
          array.getValues(controllerIndexOffset, movedData, movedSize);
          partialArray->insert(0, movedData, movedSize);
          j+=movedSize;

/*
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); j++){
            partialArray->pushBack(array.getValue<short>(j));
          }
*/
        }
        else if (datatype == H5T_NATIVE_INT){
          partialArray->initialize(XdmfArrayType::Int32(), 0);

          if ((array.getSize() - controllerIndexOffset) <= heavyDataController->getSize()) {
            movedSize = array.getSize() - controllerIndexOffset;
          }
          else if (heavyDataController->getSize() < (array.getSize() - controllerIndexOffset)) {
            movedSize = heavyDataController->getSize();
          }
          int movedData [movedSize];
          array.getValues(controllerIndexOffset, movedData, movedSize);
          partialArray->insert(0, movedData, movedSize);
          j+=movedSize;

/*
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); j++){
            partialArray->pushBack(array.getValue<int>(j));
          }
*/
        }
        else if (datatype == H5T_NATIVE_LONG){
          partialArray->initialize(XdmfArrayType::Int64(), 0);

          if ((array.getSize() - controllerIndexOffset) <= heavyDataController->getSize()) {
            movedSize = array.getSize() - controllerIndexOffset;
          }
          else if (heavyDataController->getSize() < (array.getSize() - controllerIndexOffset)) {
            movedSize = heavyDataController->getSize();
          }
          long movedData [movedSize];
          array.getValues(controllerIndexOffset, movedData, movedSize);
          partialArray->insert(0, movedData, movedSize);
          j+=movedSize;

/*
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); j++){
            partialArray->pushBack(array.getValue<long>(j));
          }
*/
        }
        else if (datatype == H5T_NATIVE_FLOAT){
          partialArray->initialize(XdmfArrayType::Float32(), 0);

          if ((array.getSize() - controllerIndexOffset) <= heavyDataController->getSize()) {
            movedSize = array.getSize() - controllerIndexOffset;
          }
          else if (heavyDataController->getSize() < (array.getSize() - controllerIndexOffset)) {
            movedSize = heavyDataController->getSize();
          }
          float movedData [movedSize];
          array.getValues(controllerIndexOffset, movedData, movedSize);
          partialArray->insert(0, movedData, movedSize);
          j+=movedSize;

/*
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); j++){
            partialArray->pushBack(array.getValue<float>(j));
          }
*/
        }
        else if (datatype == H5T_NATIVE_DOUBLE){
          partialArray->initialize(XdmfArrayType::Float64(), 0);

          if ((array.getSize() - controllerIndexOffset) <= heavyDataController->getSize()) {
            movedSize = array.getSize() - controllerIndexOffset;
          }
          else if (heavyDataController->getSize() < (array.getSize() - controllerIndexOffset)) {
            movedSize = heavyDataController->getSize();
          }
          double movedData [movedSize];
          array.getValues(controllerIndexOffset, movedData, movedSize);
          partialArray->insert(0, movedData, movedSize);
          j+=movedSize;

/*
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); j++){
            partialArray->pushBack(array.getValue<double>(j));
          }
*/
        }
        else if (datatype == H5T_NATIVE_UCHAR){
          partialArray->initialize(XdmfArrayType::UInt8(), 0);

          if ((array.getSize() - controllerIndexOffset) <= heavyDataController->getSize()) {
            movedSize = array.getSize() - controllerIndexOffset;
          }
          else if (heavyDataController->getSize() < (array.getSize() - controllerIndexOffset)) {
            movedSize = heavyDataController->getSize();
          }
          unsigned char movedData [movedSize];
          array.getValues(controllerIndexOffset, movedData, movedSize);
          partialArray->insert(0, movedData, movedSize);
          j+=movedSize;

/*
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); j++){
            partialArray->pushBack(array.getValue<unsigned char>(j));
          }
*/
        }
        else if (datatype == H5T_NATIVE_USHORT){
          partialArray->initialize(XdmfArrayType::UInt16(), 0);

          if ((array.getSize() - controllerIndexOffset) <= heavyDataController->getSize()) {
            movedSize = array.getSize() - controllerIndexOffset;
          }
          else if (heavyDataController->getSize() < (array.getSize() - controllerIndexOffset)) {
            movedSize = heavyDataController->getSize();
          }
          unsigned short movedData [movedSize];
          array.getValues(controllerIndexOffset, movedData, movedSize);
          partialArray->insert(0, movedData, movedSize);
          j+=movedSize;

/*
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); j++){
            partialArray->pushBack(array.getValue<unsigned short>(j));
          }
*/
        }
        else if (datatype == H5T_NATIVE_UINT) {
          partialArray->initialize(XdmfArrayType::UInt32(), 0);

          if ((array.getSize() - controllerIndexOffset) <= heavyDataController->getSize()) {
            movedSize = array.getSize() - controllerIndexOffset;
          }
          else if (heavyDataController->getSize() < (array.getSize() - controllerIndexOffset)) {
            movedSize = heavyDataController->getSize();
          }
          unsigned int movedData [movedSize];
          array.getValues(controllerIndexOffset, movedData, movedSize);
          partialArray->insert(0, movedData, movedSize);
          j+=movedSize;

/*
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); j++){
            partialArray->pushBack(array.getValue<unsigned int>(j));
          }
*/
        }
        else if (closeDatatype) {//closeDatatype is only true if strings are being used
          partialArray->initialize(XdmfArrayType::String(), 0);
          //transfering via loop because the getValues function is not fully tested with strings
          for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); j++){
            partialArray->pushBack(array.getValue<std::string>(j));
          }

        }

	controllerIndexOffset = j;//set the offset to the point after the end of the current subset
        if (partialArray->getSize()==0) {
          break;
        }

        arraysWritten.push_back(partialArray);
        filesWritten.push_back(hdf5FilePath);
        //also need to push the starts and strides loaded from the HeavyDataController
        startsWritten.push_back(start);
        stridesWritten.push_back(stride);
        dimensionsWritten.push_back(dimensions);
        dataSizesWritten.push_back(dataspaceDimensions);
      }

      std::list<std::string>::iterator fileNameWalker = filesWritten.begin();
      std::list<shared_ptr<XdmfArray> >::iterator arrayWalker = arraysWritten.begin();
      std::list<std::vector<unsigned int> >::iterator startWalker = startsWritten.begin();
      std::list<std::vector<unsigned int> >::iterator strideWalker = stridesWritten.begin();
      std::list<std::vector<unsigned int> >::iterator dimensionWalker = dimensionsWritten.begin();
      std::list<std::vector<unsigned int> >::iterator dataSizeWalker = dataSizesWritten.begin();

      //loop based on the amount of blocks split from the array.
      for (int writeIndex = 0; writeIndex < arraysWritten.size(); writeIndex++) {

        std::string curFileName = *fileNameWalker;
        shared_ptr<XdmfArray> curArray = *arrayWalker;
        std::vector<unsigned int> curStart = *startWalker;
        std::vector<unsigned int> curStride = *strideWalker;
        std::vector<unsigned int> curDimensions = *dimensionWalker;
        std::vector<unsigned int> curDataSize = *dataSizeWalker;

	bool closeFile = false;
        //This is meant to open files if it isn't already opened by the write prior
        //If it wasn't open prior to writing it will be closed after writing
        if (mImpl->mOpenFile.compare(curFileName) != 0) {
          if(mImpl->mHDF5Handle < 0) {
            closeFile = true;
          }
          mImpl->openFile(curFileName,
                          fapl);
        }

        hid_t dataset = H5Dopen(mImpl->mHDF5Handle,
                                dataSetPath.str().c_str(),
                                H5P_DEFAULT);

        //hid_t checkspace = H5S_ALL;
        //checkspace = H5Dget_space(dataset);
        //hssize_t checksize = H5Sget_simple_extent_npoints(checkspace);
        //if(checkspace != H5S_ALL) {
        //  status = H5Sclose(checkspace);
        //}

        // if default mode find a new data set to write to (keep
        // incrementing dataSetId)
        if(dataset >=0 && mMode == Default) {
          while(true) {
            dataSetPath.str(std::string());
            dataSetPath << "Data" << ++mDataSetId;
            if(!H5Lexists(mImpl->mHDF5Handle,
                          dataSetPath.str().c_str(),
                          H5P_DEFAULT)) {
              dataset = H5Dopen(mImpl->mHDF5Handle,
                                dataSetPath.str().c_str(),
                                H5P_DEFAULT);
              break;
            }
          }
        }

        // Restore previous error handler
        H5Eset_auto2(0, old_func, old_client_data);

        hid_t dataspace = H5S_ALL;
        hid_t memspace = H5S_ALL;

        std::vector<hsize_t> current_dims(curDataSize.begin(),
                                          curDataSize.end());

        if(dataset < 0) {//if the dataset doesn't contain anything

          std::vector<hsize_t> maximum_dims(curDimensions.size(), H5S_UNLIMITED);
          //create a new dataspace
          dataspace = H5Screate_simple(current_dims.size(),
                                       &current_dims[0],
                                       &maximum_dims[0]);
          hid_t property = H5Pcreate(H5P_DATASET_CREATE);
          std::vector<hsize_t> chunk_size(current_dims.size(), 1024);

          status = H5Pset_chunk(property, current_dims.size(), &chunk_size[0]);
          //use that dataspace to create a new dataset
          dataset = H5Dcreate(mImpl->mHDF5Handle,
                              dataSetPath.str().c_str(),
                              datatype,
                              dataspace,
                              H5P_DEFAULT,
                              property,
                              H5P_DEFAULT);
          status = H5Pclose(property);
        }

        if(mMode == Append) {
          // Need to resize dataset to fit new data

          // Get size of old dataset
          dataspace = H5Dget_space(dataset);
          hssize_t datasize = H5Sget_simple_extent_npoints(dataspace);
          status = H5Sclose(dataspace);


		//rethink this algorithm for finding the size
          if (curFileName != previousControllers[i]->getFilePath()) {
            datasize = 0;
          }
          if (dataSetPath.str() != previousControllers[i]->getDataSetPath()) {
            datasize = 0;
          }

          // Resize to fit size of old and new data.
          hsize_t newSize = curArray->getSize() + datasize;
          status = H5Dset_extent(dataset, &newSize);

          // Select hyperslab to write to.
          memspace = H5Screate_simple(1, &size, NULL);
          dataspace = H5Dget_space(dataset);
          hsize_t dataStart = datasize;
          status = H5Sselect_hyperslab(dataspace,
                                       H5S_SELECT_SET,
                                       &dataStart,
                                       NULL,
                                       &size,
                                       NULL);
        }
        else if(mMode == Overwrite) {
          // Overwriting - dataset rank must remain the same (hdf5 constraint)
          dataspace = H5Dget_space(dataset);

          const unsigned int ndims = H5Sget_simple_extent_ndims(dataspace);
          if(ndims != current_dims.size())
            XdmfError::message(XdmfError::FATAL,                            \
                               "Data set rank different -- ndims != "
                               "current_dims.size() -- in "
                               "XdmfHDF5Writer::write");

          status = H5Dset_extent(dataset, &current_dims[0]);
          dataspace = H5Dget_space(dataset);
        }
        else if(mMode == Hyperslab) {
          // Hyperslab - dataset rank must remain the same (hdf5 constraint)
          dataspace = H5Dget_space(dataset);

          const unsigned int ndims = H5Sget_simple_extent_ndims(dataspace);
          if(ndims != current_dims.size())
            XdmfError::message(XdmfError::FATAL,                            \
                               "Data set rank different -- ndims != "
                               "current_dims.size() -- in "
                               "XdmfHDF5Writer::write");

          status = H5Dset_extent(dataset, &current_dims[0]);
          dataspace = H5Dget_space(dataset);




          std::vector<hsize_t> count(curDimensions.begin(),
                                     curDimensions.end());
          std::vector<hsize_t> currStride(curStride.begin(),
                                          curStride.end());
          std::vector<hsize_t> currStart(curStart.begin(),
                                         curStart.end());

          memspace = H5Screate_simple(count.size(),
                                      &(count[0]),
                                      NULL);
          status = H5Sselect_hyperslab(dataspace,
                                       H5S_SELECT_SET,
                                       &currStart[0],
                                       &currStride[0],
                                       &count[0],
                                       NULL) ;

          if(status < 0) {
            XdmfError::message(XdmfError::FATAL,
                               "H5Dset_extent returned failure in "
                               "XdmfHDF5Writer::write -- status: " + status);
          }
        }


        status = H5Dwrite(dataset,
                          datatype,
                          memspace,
                          dataspace,
                          H5P_DEFAULT,
                          curArray->getValuesInternal());


        if(status < 0) {
          XdmfError::message(XdmfError::FATAL,
                             "H5Dwrite returned failure in XdmfHDF5Writer::write "
                             "-- status: " + status);
        }

        if(dataspace != H5S_ALL) {
          status = H5Sclose(dataspace);
        }
        if(memspace != H5S_ALL) {
          status = H5Sclose(memspace);
        }
        status = H5Dclose(dataset);
        if(closeFile) {
          mImpl->closeFile();
        }

        if(mMode == Default) {
          ++mDataSetId;
        }




        // Attach a new controller to the array
        shared_ptr<XdmfHDF5Controller> newDataController =
          shared_ptr<XdmfHDF5Controller>();

        unsigned int newSize;
        if(mMode == Append) {
          //find data size
          mImpl->openFile(curFileName,
                          fapl);
          hid_t checkset = H5Dopen(mImpl->mHDF5Handle,
                                   dataSetPath.str().c_str(),
                                   H5P_DEFAULT);
          hid_t checkspace = H5S_ALL;
          checkspace = H5Dget_space(checkset);
          newSize = H5Sget_simple_extent_npoints(checkspace);
          status = H5Dclose(checkset);
	  if(checkspace != H5S_ALL) {
	    status = H5Sclose(checkspace);
          }
 
          std::vector<unsigned int> insertStarts;
          insertStarts.push_back(0);
          std::vector<unsigned int> insertStrides;
          insertStrides.push_back(1);
          std::vector<unsigned int> insertDimensions;
          insertDimensions.push_back(newSize);
          std::vector<unsigned int> insertDataSpaceDimensions;
          insertDataSpaceDimensions.push_back(newSize);

          newDataController =
            this->createHDF5Controller(curFileName,
                                       dataSetPath.str(),
                                       curArray->getArrayType(),
                                       insertStarts,
                                       insertStrides,
                                       insertDimensions,
                                       insertDataSpaceDimensions);
        }

        if(!newDataController) {
          newDataController =
            this->createHDF5Controller(curFileName,
                                       dataSetPath.str(),
                                       curArray->getArrayType(),
                                       curStart,
                                       curStride,
                                       curDimensions,
                                       curDataSize);
        }

        array.insert(newDataController);

        fileNameWalker++;
        arrayWalker++;
        startWalker++;
        strideWalker++;
        dimensionWalker++;
        dataSizeWalker++;


      }

    }

    if(closeDatatype) {
      status = H5Tclose(datatype);
    }

    if(mReleaseData) {
      array.release();
    }
  }
}
