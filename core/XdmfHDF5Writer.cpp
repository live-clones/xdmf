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
#include <cmath>
#include <set>
#include <list>
#include "XdmfItem.hpp"
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfError.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"


namespace {

  const static unsigned int DEFAULT_CHUNK_SIZE = 0;

}

/**
 * PIMPL
 */
class XdmfHDF5Writer::XdmfHDF5WriterImpl {

public:

  XdmfHDF5WriterImpl():
    mHDF5Handle(-1),
    mChunkSize(DEFAULT_CHUNK_SIZE),
    mOpenFile(""),
    mHDF5FileSizeLimit(0),
    mAllowSplitDataSets(false),
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
      /*herr_t status =*/H5Fclose(mHDF5Handle);
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
      /*herr_t status = */H5Gget_num_objs(mHDF5Handle,
                                          &numObjects);
      toReturn = numObjects;
    }
    else {
	//this is where it currently fails
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
  unsigned int mChunkSize;
  std::string mOpenFile;
  unsigned int mHDF5FileSizeLimit;
  bool mAllowSplitDataSets;
  int mDepth;
  int mFileIndex;
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
  try {
    return XdmfHDF5Controller::New(hdf5FilePath,
                                   dataSetPath,
                                   type,
                                   start,
                                   stride,
                                   dimensions,
                                   dataspaceDimensions);
  }
  catch (XdmfError e) {
    throw e;
  }
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
XdmfHDF5Writer::setAllowSetSplitting(bool newAllow)
{
  mImpl->mAllowSplitDataSets = newAllow;
}

int
XdmfHDF5Writer::getAllowSetSplitting()
{
  return mImpl->mAllowSplitDataSets;
}

void
XdmfHDF5Writer::setFileIndex(int newSize)
{
  mImpl->mFileIndex = newSize;
}

int
XdmfHDF5Writer::getFileIndex()
{
  return mImpl->mFileIndex;
}

unsigned int
XdmfHDF5Writer::getChunkSize() const
{
  return mImpl->mChunkSize;
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
XdmfHDF5Writer::setChunkSize(const unsigned int chunkSize)
{
  mImpl->mChunkSize = chunkSize;
}

void
XdmfHDF5Writer::visit(XdmfArray & array,
                      const shared_ptr<XdmfBaseVisitor> visitor)
{
  mImpl->mDepth++;
  std::set<const XdmfItem *>::iterator checkWritten = mImpl->mWrittenItems.find(&array);
  if (checkWritten == mImpl->mWrittenItems.end() || array.getItemTag() == "DataItem") {
    //if it has children send the writer to them too.
    try {
      array.traverse(visitor);
    }
    catch (XdmfError e) {
      throw e;
    }
    if (array.isInitialized()) {
      //only do this if the object has not already been written
      try {
        this->write(array, H5P_DEFAULT);
      }
      catch (XdmfError e) {
        throw e;
      }
      mImpl->mWrittenItems.insert(&array);
    }
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
  //if it was written once then all instances of the object should have the controller
  std::set<const XdmfItem *>::iterator checkWritten = mImpl->mWrittenItems.find(&item);
  if (checkWritten == mImpl->mWrittenItems.end()) {
    mImpl->mWrittenItems.insert(&item);
    try {
      item.traverse(visitor);
    }
    catch (XdmfError e) {
      throw e;
    }
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

  const unsigned int hdf5Overhead = 800;//base size of an hdf5 file is 800

  //determining data type
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
    else if(array.getArrayType() == XdmfArrayType::String()) {//strings are a special case as they have mutable size
      datatype = H5Tcopy(H5T_C_S1);
      H5Tset_size(datatype, H5T_VARIABLE);
      closeDatatype = true;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Array of unsupported type in "
                           "XdmfHDF5Writer::write");
      }
      catch (XdmfError e) {
        throw e;
      }
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

    //hold the controllers in order to base the new controllers on them
    for(unsigned int i = 0; i < array.getNumberHeavyDataControllers(); ++i) {
      previousControllers.push_back(array.getHeavyDataController(i));
    }

    //remove controllers from the array, they will be replaced by the controllers created by this function.
    while(array.getNumberHeavyDataControllers() != 0) {
      array.removeHeavyDataController(array.getNumberHeavyDataControllers() -1);
    }



    if (previousControllers.size() == 0) {
      //create a temporary controller if the array doesn't have one
      try {
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
      catch (XdmfError e) {
        throw e;
      }
    }

    int controllerIndexOffset = 0;

    for(unsigned int i = 0; i < previousControllers.size(); ++i)// it is assumed that the array will have at least one controller, if it didn't have one a temporary one was generated
    {
      if (mMode == Append) {//append only cares about the last controller, so add the rest back in
	for (; i < previousControllers.size() - 1; ++i) {
          array.insert(previousControllers[i]);
	}
      }

      std::list<std::string> filesWritten;
      std::list<shared_ptr<XdmfArray> > arraysWritten;
      std::list<std::vector<unsigned int> > startsWritten;
      std::list<std::vector<unsigned int> > stridesWritten;
      std::list<std::vector<unsigned int> > dimensionsWritten;
      std::list<std::vector<unsigned int> > dataSizesWritten;
      std::list<unsigned int> arrayOffsetsWritten;

      // Open a hdf5 dataset and write to it on disk.
      hsize_t size = array.getSize();

      // Save old error handler and turn off error handling for now
      H5E_auto_t old_func;
      void * old_client_data;
      H5Eget_auto(0, &old_func, &old_client_data);
      H5Eset_auto2(0, NULL, NULL);

      bool startedloop = false;
      unsigned int origFileIndex = mImpl->mFileIndex;
      while ((mMode == Hyperslab && i < previousControllers.size()) || !startedloop) {
        //Hyperslab mode wants to assign all data using the current location without writing until all data sets are determined

        startedloop = true;

        shared_ptr<XdmfHeavyDataController> heavyDataController =
          previousControllers[i];
        //stats for the data currently stored in the array
    
        std::vector<unsigned int> dimensions;
        if (mMode != Hyperslab) {
          dimensions = array.getDimensions();
        }
        else {
	  dimensions = heavyDataController->getDimensions();
        }
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

        //check here for if the file would become larger than the limit after the addition.
        //then check subsequent files for the same limitation

        //this is the file splitting algorithm
        if (mImpl->mHDF5FileSizeLimit > 0) {//only if the file limit is positive, disabled if 0 or negative
          unsigned int previousDataSize = 0;

          std::vector<unsigned int> previousDimensions;
          std::vector<unsigned int> previousDataSizes;
          unsigned int amountAlreadyWritten = 0;
          //Even though theoretically this could be an infinite loop
          //if all possible files with the specified name are produced
          //the chances of that happening are small.
          //It can handle up to 65535 different files.
          //This value may vary depending on the compiler and platform.
          //The variable UINT_MAX holds the value in question.
          //If all files are take up it will loop until a file opens up since adding past the max causes overflow.
	

          unsigned int containedInController = 1;
          for (unsigned int j = 0; j < dataspaceDimensions.size(); ++j) {
            containedInController *= dataspaceDimensions[j];
          }
          hssize_t hyperslabSize = 0;
          while (amountAlreadyWritten < containedInController) {


            std::vector<unsigned int> partialStarts;
            std::vector<unsigned int> partialStrides;
            std::vector<unsigned int> partialDimensions;
            std::vector<unsigned int> partialDataSizes;

            std::stringstream testFile;
            if (mImpl->mFileIndex == 0) {//if sequentially named files need to be created or referenced
              testFile << checkFileName << "." << checkFileExt;
            }
            else {
              testFile << checkFileName << mImpl->mFileIndex << "." << checkFileExt;
            }
            FILE *checkFile = NULL;
            unsigned int fileSize = 0;//if the file doesn't exist the size is 0 because there's no data
            // get the file stream
            checkFile = fopen(testFile.str().c_str(), "a");
            if (checkFile != NULL) {
              // set the file pointer to end of file
              fseek(checkFile, 0, SEEK_END);
              // get the file size, in bytes
              fileSize = ftell(checkFile);

              //if overwrite subtract previous data size.
              if (mMode == Overwrite || mMode == Hyperslab) {
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
                if (mMode == Overwrite) {
                  if (checksize > fileSize) {
                    fileSize = 0;
                  }
                  else {
                    fileSize = fileSize - checksize;//remove previous set's size, since it's overwritten
                  }
                  if (fileSize == 0) {
                    fileSize += hdf5Overhead;
                  }
                }
                else if (mMode == Hyperslab) {
                  hyperslabSize = checksize;
                }
              }
              if (fileSize == 0) {
                fileSize += hdf5Overhead;
              }
              fclose(checkFile);
            }
            else if (previousDataSize == 0) {
              fileSize += hdf5Overhead;
            }
            // close stream and release buffer
            //check size to see if it's within range

            if (closeDatatype == true) { //closetype is only true if strings are being used, it's set at the beginning when types are checked.
              //size needed is equal to the dataspaceDimensions if in hyperslab mode
              //otherwise is equal to the size of the written array
              unsigned int remainingValues = 0;
              unsigned int sizeArrayIndex = 0;
              if (mMode == Hyperslab) {
                remainingValues += 1;
                sizeArrayIndex += 1;
                for (unsigned int j = 0; j < dataspaceDimensions[j]; ++j) {
                  remainingValues *= dataspaceDimensions[j];
                  sizeArrayIndex *= dimensions[j];
                }
              }
              else {
                remainingValues += array.getSize();
                sizeArrayIndex = amountAlreadyWritten;
              }
              remainingValues -= amountAlreadyWritten;//reduce by number of values already written
              if (remainingValues == 0) {//end if no remaining values
                break;
              }
              //if remaining size is less than available space, just write all of what's left
              //calculate remaining size
              unsigned int remainingSize = 0;
              for (unsigned int j = sizeArrayIndex; j < array.getSize(); ++j) {
                remainingSize += array.getValue<std::string>(j).size() * 8;
              }
              if (mMode == Hyperslab) {
                //size is estimated based on averages
                remainingSize = (remainingSize / (array.getSize() - sizeArrayIndex)) * remainingValues;
              }
              if (remainingSize + previousDataSize + fileSize < mImpl->mHDF5FileSizeLimit*(1024*1024)) {
                //if the array hasn't been split
                if (amountAlreadyWritten == 0) {
                  //just pass all data to the partial vectors
                  for (unsigned int j = 0; j < dimensions.size(); ++j) {//done using a loop so that data is copied, not referenced
                    partialStarts.push_back(start[j]);
                    partialStrides.push_back(stride[j]);
                    partialDimensions.push_back(dimensions[j]);
                    partialDataSizes.push_back(dataspaceDimensions[j]);
                  }
                }
                else {//if the array has been split
                  int dimensionIndex = previousDimensions.size() - 1;
                  //loop previous dimensions in
                  int j = 0;
                  for (j = 0; j < dimensionIndex; ++j) {
                    partialStarts.push_back(start[j]);
                    partialStrides.push_back(stride[j]);
                    partialDimensions.push_back(dimensions[j]);
                    partialDataSizes.push_back(dataspaceDimensions[j]);
                  }
                  if (mMode == Hyperslab) {
                    int newStart = (start[j] + stride[j] * previousDimensions[j]) - previousDataSizes[j];
                    while (newStart < 0) {
                      newStart += stride[j];
                    }
                    partialStarts.push_back(newStart);
                    //stride should not change in this algorithm
                    partialStrides.push_back(stride[j]);
                    //total up number of blocks for the higher dimesions and subtract the amount already written
                    unsigned int dimensiontotal = dimensions[j];
                    unsigned int dataspacetotal = dataspaceDimensions[j];
                    for (unsigned int k = j + 1; k < dimensions.size(); ++k) {
                      dimensiontotal *= dimensions[k];
                      dataspacetotal *= dataspaceDimensions[k];
                    }
                    if (previousDimensions.size() > 0) {
                      partialDimensions.push_back(dimensiontotal-previousDimensions[j]);
                    }
                    else {
                      partialDimensions.push_back(dimensiontotal);
                    }
                    if (previousDataSizes.size() > 0) {
                      partialDataSizes.push_back(dataspacetotal-previousDataSizes[j]);
                    }
                    else {
                      partialDataSizes.push_back(dataspacetotal);
                    }
                  }
                  else {
                    //start and stride are not used outside of hyperslab
                    partialStarts.push_back(start[j]);
                    partialStrides.push_back(stride[j]);
                    //total up number of blocks for the higher dimesions and subtract the amount already written
                    //since it isn't hyperslab dimensions and dataspacedimensions should be the same
                    unsigned int dimensiontotal = dimensions[j];
                    for (unsigned int k = j + 1; k < dimensions.size(); ++k) {
                      dimensiontotal *= dimensions[k];
                    }
                    if (previousDimensions.size() > 0) {
                      partialDimensions.push_back(dimensiontotal-previousDimensions[j]);
                    }
                    else {
                      partialDimensions.push_back(dimensiontotal);
                    }
                    if (previousDataSizes.size() > 0) {
                      partialDataSizes.push_back(dimensiontotal-previousDataSizes[j]);
                    }
                    else {
                      partialDataSizes.push_back(dimensiontotal);
                    }
                  }
                }
              }
              else {//otherwise, take remaining size and start removing dimensions until the dimension block is less, then take a fraction of the dimension
                //calculate the number of values of the data type you're using will fit
                unsigned int usableSpace = (mImpl->mHDF5FileSizeLimit*(1024*1024) - fileSize);
                if (previousDataSize + fileSize > mImpl->mHDF5FileSizeLimit*(1024*1024)) {
                  usableSpace = 0;
                }
                usableSpace += hyperslabSize-previousDataSize;
                //if the array hasn't been split
                if (amountAlreadyWritten == 0) {
                  //see if it will fit in the next file
                  //if it will just go to the next file
                  //otherwise split it.
                  if (remainingSize + hdf5Overhead > mImpl->mHDF5FileSizeLimit*(1024*1024) && usableSpace > 0) {
                    if (mImpl->mAllowSplitDataSets) {
                      //figure out the size of the largest block that will fit.
                      unsigned int blockSizeSubtotal = 0;
                      unsigned int dimensionSizeTotal = 1;
                      unsigned int dimensionIndex = 0;
                      unsigned int previousBlockSize = 0;
                      //find the dimension that was split
                      while (dimensionIndex < dataspaceDimensions.size() && blockSizeSubtotal <= usableSpace) {
                        //this is totally different for strings
                        dimensionSizeTotal *= dimensions[dimensionIndex];
                        previousBlockSize = blockSizeSubtotal;
                        blockSizeSubtotal = 0;
                        for (unsigned int k = 0; k < dimensionSizeTotal; ++k) {
                          if (amountAlreadyWritten + k > array.getSize()) {
                            try {
                              XdmfError::message(XdmfError::FATAL,
                                                 "Error: Invalid Dimension in HDF5 Write.\n");
                            }
                            catch (XdmfError e) {
                              throw e;
                            }
                          }
                          blockSizeSubtotal += array.getValue<std::string>(amountAlreadyWritten + k).size();
                        }
                        dimensionIndex++;
                      }//It should end on the "blockSizeSubtotal <= usableSpace" statement, the other half is for backup
                      //move back one dimension so we're working on the dimension that was split, not the one after it
                      dimensionIndex--;
                      blockSizeSubtotal = previousBlockSize;
                      //determine how many of those blocks will fit
                      unsigned int numBlocks = usableSpace / blockSizeSubtotal;//this should be less than the current value for the dimension
                      //add dimensions as required.
                      unsigned int j = 0;
                      for (; j < dimensionIndex; ++j) {
                        partialStarts.push_back(start[j]);
                        partialStrides.push_back(stride[j]);
                        partialDimensions.push_back(dimensions[j]);
                        partialDataSizes.push_back(dataspaceDimensions[j]);
                      }
                      if (start[j] > numBlocks) {
                        partialStarts.push_back(numBlocks-1);
                      }
                      else {
                        partialStarts.push_back(start[j]);
                      }
                      partialStrides.push_back(stride[j]);
                      partialDataSizes.push_back(numBlocks);
                      if (dimensions[j] == dataspaceDimensions[j]) {//this is for non-hyperslab and specific cases of hyperslab
                        partialDimensions.push_back(numBlocks);
                      }
                      else {//for hyperslab in general
                        //determine how many values from the array will fit into the blocks being used with the dimensions specified
                        unsigned int displacement = numBlocks / stride[j];
                        if (((int)displacement * (int)stride[j]) + (start[j] % stride[j]) < numBlocks) {
                          displacement++;
                        }
                        displacement -= start[j]/stride[j];
                        if (start[j] > numBlocks) {
                          displacement = 0;
                        }
                        if (dimensions[j] <= displacement) {//if there are less values than there are space for, just write all of them.
                          partialDimensions.push_back(dimensions[j]);
                        }
                        else {//otherwise write what space allows for
                          partialDimensions.push_back(displacement);
                        }
                      }
                    }
                    else {
                      //just pass all data to the partial vectors
                      for (unsigned int j = 0; j < dimensions.size(); ++j) {//done using a loop so that data is copied, not referenced
                        partialStarts.push_back(start[j]);
                        partialStrides.push_back(stride[j]);
                        partialDimensions.push_back(dimensions[j]);
                        partialDataSizes.push_back(dataspaceDimensions[j]);
                      }
                    }
                  }
                }
                else {//if the array has been split
                  //This case should not come up often as it requires truly gigantic data sets
                  //see if the remaining data will fit in the next file
                  //if yes, skip to it
                  //if no, split
                  if (remainingSize + hdf5Overhead > mImpl->mHDF5FileSizeLimit*(1024*1024) && usableSpace > 0) {
                    //figure out the size of the largest block that will fit.
                    unsigned int blockSizeSubtotal = 0;
                    unsigned int tempTotal = 0;
                    unsigned int dimensionSizeTotal = 1;
                    unsigned int dimensionIndex = 0;
                    //find the dimension that was split
                    while (dimensionIndex < dataspaceDimensions.size() && blockSizeSubtotal <= usableSpace) {
                      //this is totally different for strings
                      dimensionSizeTotal *= dimensions[dimensionIndex];
                      tempTotal = blockSizeSubtotal;
                      blockSizeSubtotal = 0;
                      for (unsigned int k = 0; k < dimensionSizeTotal; ++k) {
                        if (amountAlreadyWritten + k > array.getSize()) {
                          try {
                            XdmfError::message(XdmfError::FATAL,
                                               "Error: Invalid Dimension in HDF5 Write.\n");
                          }
                          catch (XdmfError e) {
                            throw e;
                          }
                        }
                        blockSizeSubtotal += array.getValue<std::string>(amountAlreadyWritten + k).size();
                      }
                      dimensionIndex++;
                    }//It should end on the "blockSizeSubtotal <= usableSpace" statement, the other half is for backup
                    //move back one dimension so we're working on the dimension that was split, not the one after it
                    dimensionIndex--;
                    blockSizeSubtotal = tempTotal;
                    unsigned int j = 0;
                    for (; j < dimensionIndex; ++j) {
                      partialStarts.push_back(start[j]);
                      partialStrides.push_back(stride[j]);
                      partialDimensions.push_back(dimensions[j]);
                      partialDataSizes.push_back(dataspaceDimensions[j]);
                    }
                    //continue if the block is smaller than the available size
                    if (blockSizeSubtotal <=usableSpace) {
                      //find number of blocks that will fit
                      //this should be less than the current value for the dimension
                      unsigned int numBlocks = usableSpace / blockSizeSubtotal;
                      //add dimensions to the partial vectors
                      if (mMode == Hyperslab) {
                        int newStart = (start[j] + stride[j] * previousDimensions[j]) - previousDataSizes[j];
                        while (newStart < 0) {
                          newStart += stride[j];
                        }
                        partialStarts.push_back(newStart);
                        //stride should not change in this algorithm
                        partialStrides.push_back(stride[j]);
                        partialDataSizes.push_back(numBlocks);
                        //determine how many values from the array will fit into the blocks being used
                        //with the dimensions specified
                        unsigned int displacement = (numBlocks - newStart) / stride[j];
                        if (((int)displacement * (int)stride[j]) + (newStart % stride[j]) < numBlocks) {
                          displacement++;
                        }
                        displacement -= newStart/stride[j];
                        if (newStart > (int)numBlocks) {
                          displacement = 0;
                        }
                        if ((dimensions[j] - previousDimensions[j]) <= displacement) {//if there are less values than there are space for, just write all of them.
                          partialDimensions.push_back(dimensions[j] - previousDimensions[j]);
                        }
                        else {//otherwise write what space allows for
                          partialDimensions.push_back(displacement);
                        }
                      }
                      else {
                        //start and stride are only specified in hyperslab
                        partialStarts.push_back(start[j]);
                        partialStrides.push_back(stride[j]);
                        partialDataSizes.push_back(numBlocks);
                        partialDimensions.push_back(numBlocks);
                      }
                      //place dimensions into previous dimensions for later iterations
                    }
                    else {//if this is larger than usable space, try the next file
                      //if moving to next file, just do nothing and pass out of the if statement
                      //but also check if specified file size is too small
                      if (mImpl->mHDF5FileSizeLimit*(1024*1024) < blockSizeSubtotal) {//this shouldn't ever trigger, but it's good to cover ourselves
                        //and throw an error if the block size won't work
                        try {
                          XdmfError::message(XdmfError::FATAL,
                                             "Error: Dimension Block size / Maximum File size mismatch.\n");
                        }
                        catch (XdmfError e) {
                          throw e;
                        }
                      }
                    }
                  }
                }
                //move to next file
                mImpl->mFileIndex++;
              }
            }
            else {
              //if needed split the written array into smaller arrays based on dimension blocks
              //working with strings has a more resource intensive version of this algorithm
              //size needed is equal to the dataspaceDimensions if in hyperslab mode
              //otherwise is equal to the size of the written array
              unsigned int remainingValues = 0;
              if (mMode == Hyperslab) {
                remainingValues += 1;
                for (unsigned int j = 0; j < dataspaceDimensions.size(); ++j) {
                  remainingValues *= dataspaceDimensions[j];
                }
              }
              else {
                remainingValues += 1;
                for (unsigned int j = 0; j < dimensions.size(); ++j) {
                  remainingValues *= dimensions[j];
                }
              }
              remainingValues -= amountAlreadyWritten;//reduce by number of values already written
              if (remainingValues == 0) {//end if no remaining values
                break;
              }
              unsigned int dataItemSize = array.getArrayType()->getElementSize();
              //if remaining size is less than available space, just write all of what's left
              if ((remainingValues * dataItemSize) + previousDataSize + fileSize < mImpl->mHDF5FileSizeLimit*(1024*1024)) {
                //if the array hasn't been split
                if (amountAlreadyWritten == 0) {
                  //just pass all data to the partial vectors
                  for (unsigned int j = 0; j < dimensions.size(); ++j) {//done using a loop so that data is copied, not referenced
                    partialStarts.push_back(start[j]);
                    partialStrides.push_back(stride[j]);
                    partialDimensions.push_back(dimensions[j]);
                    partialDataSizes.push_back(dataspaceDimensions[j]);
                  }
                }
                else {//if the array has been split
                  int dimensionIndex = previousDimensions.size() - 1;
                  //loop previous dimensions in
                  int j = 0;
                  for (j = 0; j < dimensionIndex; ++j) {
                    partialStarts.push_back(start[j]);
                    partialStrides.push_back(stride[j]);
                    partialDimensions.push_back(dimensions[j]);
                    partialDataSizes.push_back(dataspaceDimensions[j]);
                  }
                  if (mMode == Hyperslab) {
                    int newStart = (start[j] + stride[j] * previousDimensions[j]) - previousDataSizes[j];
                    while (newStart < 0) {
                      newStart += stride[j];
                    }
                    partialStarts.push_back(newStart);
                    //stride should not change in this algorithm
                    partialStrides.push_back(stride[j]);
                    //total up number of blocks for the higher dimesions and subtract the amount already written
                    unsigned int dimensiontotal = dimensions[j];
                    unsigned int dataspacetotal = dataspaceDimensions[j];
                    for (unsigned int k = j + 1; k < dimensions.size(); ++k) {
                      dimensiontotal *= dimensions[k];
                      dataspacetotal *= dataspaceDimensions[k];
                    }
                    if (previousDimensions.size() > 0) {
                      partialDimensions.push_back(dimensiontotal-previousDimensions[j]);
                    }
                    else {
                      partialDimensions.push_back(dimensiontotal);
                    }
                    if (previousDataSizes.size() > 0) {
                      partialDataSizes.push_back(dataspacetotal-previousDataSizes[j]);
                    }
                    else {
                      partialDataSizes.push_back(dataspacetotal);
                    }
                  }
                  else {
                    //start and stride are not used outside of hyperslab
                    partialStarts.push_back(start[j]);
                    partialStrides.push_back(stride[j]);
                    //total up number of blocks for the higher dimesions and subtract the amount already written
                    //since it isn't hyperslab dimensions and dataspacedimensions should be the same
                    unsigned int dimensiontotal = dimensions[j];
                    for (unsigned int k = j + 1; k < dimensions.size(); ++k) {
                      dimensiontotal *= dimensions[k];
                    }
                    if (previousDimensions.size() > 0) {
                      partialDimensions.push_back(dimensiontotal-previousDimensions[j]);
                    }
                    else {
                      partialDimensions.push_back(dimensiontotal);
                    }
                    if (previousDataSizes.size() > 0) {
                      partialDataSizes.push_back(dimensiontotal-previousDataSizes[j]);
                    }
                    else {
                      partialDataSizes.push_back(dimensiontotal);
                    }
                  }
                }
              }
              else {//otherwise, take remaining size and start removing dimensions until the dimension block is less, then take a fraction of the dimension
                //calculate the number of values of the data type you're using will fit
                unsigned int usableSpace = (mImpl->mHDF5FileSizeLimit*(1024*1024) -  fileSize) / dataItemSize;
                if (mImpl->mHDF5FileSizeLimit*(1024*1024) < fileSize) {
                  usableSpace = 0;
                }
                usableSpace += hyperslabSize-previousDataSize;
                //if the array hasn't been split
                if (amountAlreadyWritten == 0) {
                  //see if it will fit in the next file
                  //if it will just go to the next file
                  //otherwise split it.
                  if ((remainingValues * dataItemSize) + hdf5Overhead > mImpl->mHDF5FileSizeLimit*(1024*1024) && usableSpace > 0) {
                    if (mImpl->mAllowSplitDataSets) {
                      //figure out the size of the largest block that will fit.
                      unsigned int blockSizeSubtotal = 1;
                      unsigned int dimensionIndex = 0;
                      //find the dimension that was split
                      while (dimensionIndex < dataspaceDimensions.size() && blockSizeSubtotal <= usableSpace) {
                        blockSizeSubtotal *= dataspaceDimensions[dimensionIndex];
                        dimensionIndex++;
                      }//It should end on the "blockSizeSubtotal <= arrayStartIndex" statement, the other half is for backup
                      //move back one dimension so we're working on the dimension that was split, not the one after it
                      dimensionIndex--;
                      blockSizeSubtotal /= dataspaceDimensions[dimensionIndex];
                      //determine how many of those blocks will fit
                      unsigned int numBlocks = usableSpace / blockSizeSubtotal;//this should be less than the current value for the dimension
                      //add dimensions as required.
                      unsigned int j = 0;
                      for (j = 0; j < dimensionIndex; ++j) {
                        partialStarts.push_back(start[j]);
                        partialStrides.push_back(stride[j]);
                        partialDimensions.push_back(dimensions[j]);
                        partialDataSizes.push_back(dataspaceDimensions[j]);
                      }
                      if (start[j] > numBlocks) {
                        partialStarts.push_back(numBlocks-1);
                      }
                      else {
                        partialStarts.push_back(start[j]);
                      }
                      partialStrides.push_back(stride[j]);
                      partialDataSizes.push_back(numBlocks);
                      if (dimensions[j] == dataspaceDimensions[j]) {//this is for non-hyperslab and specific cases of hyperslab
                        partialDimensions.push_back(numBlocks);
                      }
                      else {//for hyperslab in general
                        //determine how many values from the array will fit into the blocks being used with the dimensions specified
                        unsigned int displacement = numBlocks / stride[j];
                        if (((int)displacement * (int)stride[j]) + (start[j] % stride[j]) < numBlocks) {
                          displacement++;
                        }
                        displacement -= start[j]/stride[j];
                        if (start[j] > numBlocks) {
                          displacement = 0;
                        }
                        if (dimensions[j] <= displacement) {//if there are less values than there are space for, just write all of them.
                          partialDimensions.push_back(dimensions[j]);
                        }
                        else {//otherwise write what space allows for
                          partialDimensions.push_back(displacement);
                        }
                      }
                    }
                    else {
                      //just pass all data to the partial vectors
                      for (unsigned int j = 0; j < dimensions.size(); ++j) {//done using a loop so that data is copied, not referenced
                        partialStarts.push_back(start[j]);
                        partialStrides.push_back(stride[j]);
                        partialDimensions.push_back(dimensions[j]);
                        partialDataSizes.push_back(dataspaceDimensions[j]);
                      }
                    }
                  }
                }
                else {//if the array has been split
                  //This case should not come up often as it requires truly gigantic data sets
                  //see if it will fit in the next file
                  //if it will just go to the next file
                  //otherwise split it.
                  if ((remainingValues * dataItemSize) + hdf5Overhead > mImpl->mHDF5FileSizeLimit*(1024*1024) && usableSpace > 0) {
                    unsigned int blockSizeSubtotal = 1;
                    unsigned int dimensionIndex = 0;
                    //find the dimension that was split
                    while (dimensionIndex < dataspaceDimensions.size() && blockSizeSubtotal <= amountAlreadyWritten) {
                      blockSizeSubtotal *= dataspaceDimensions[dimensionIndex];
                      dimensionIndex++;
                    }//It should end on the "blockSizeSubtotal <= arrayStartIndex" statement, the other half is for backup
                    //move back one dimension so we're working on the dimension that was split, not the one after it
                    dimensionIndex--;
                    blockSizeSubtotal /= dataspaceDimensions[dimensionIndex];
                    unsigned int j = 0;
                    for (j = 0; j < dimensionIndex; ++j) {
                      partialStarts.push_back(start[j]);
                      partialStrides.push_back(stride[j]);
                      partialDimensions.push_back(dimensions[j]);
                      partialDataSizes.push_back(dataspaceDimensions[j]);
                    }
                    //continue if the block is smaller than the available size
                    if (blockSizeSubtotal <=usableSpace) {
                      //find number of blocks that will fit
                      //this should be less than the current value for the dimension
                      unsigned int numBlocks = usableSpace / blockSizeSubtotal;
                      //add dimensions to the partial vectors
                      if (mMode == Hyperslab) {
                        int newStart = (start[j] + stride[j] * previousDimensions[j]) - previousDataSizes[j];
                        while (newStart < 0) {
                          newStart += stride[j];
                        }
                        partialStarts.push_back(newStart);
                        //stride should not change in this algorithm
                        partialStrides.push_back(stride[j]);
                        partialDataSizes.push_back(numBlocks);
                        //determine how many values from the array will fit into the blocks being used
                        //with the dimensions specified
                        unsigned int displacement = numBlocks / stride[j];
                        if (((int)displacement * (int)stride[j]) + (newStart % stride[j]) < numBlocks) {
                          displacement++;
                        }
                        displacement -= newStart/stride[j];
                        if (newStart > (int)numBlocks) {
                          displacement = 0;
                        }
                        if ((dimensions[j] - previousDimensions[j]) <= displacement) {//if there are less values than there are space for, just write all of them.
                          partialDimensions.push_back(dimensions[j] - previousDimensions[j]);
                        }
                        else {//otherwise write what space allows for
                          partialDimensions.push_back(displacement);
                        }
                      }
                      else {
                        //start and stride are only specified in hyperslab
                        partialStarts.push_back(start[j]);
                        partialStrides.push_back(stride[j]);
                        partialDataSizes.push_back(numBlocks);
                        partialDimensions.push_back(numBlocks);
                      }
                      //place dimensions into previous dimensions for later iterations
                    }
                    else {//if this is larger than usable space, try the next file
                      //if moving to next file, just do nothing and pass out of the if statement
                      //but also check if specified file size is too small
                      if (mImpl->mHDF5FileSizeLimit*(1024*1024) < blockSizeSubtotal) {//this shouldn't ever trigger, but it's good to cover ourselves
                        //and throw an error if the block size won't work
                        try {
                          XdmfError::message(XdmfError::FATAL,
                                             "Error: Dimension Block size / Maximum File size mismatch.\n");
                        }
                        catch (XdmfError e) {
                          throw e;
                        }
                      }
                    }
                  }
                }
                //move to next file
                mImpl->mFileIndex++;
              }
            }


            if (partialDimensions.size() > 0) {//building the array to be written
              int containedInDimensions = 1;//count moved
              for (unsigned int j = 0 ; j < partialDimensions.size(); ++j) {
                containedInDimensions *= partialDimensions[j];
              }
              int containedInPriorDimensions = controllerIndexOffset;//starting index
              int startOffset = 1;
              for (unsigned int j = 0; j < previousDimensions.size(); ++j) {
                startOffset *= previousDimensions[j];
              }
              if (previousDimensions.size() == 0) {
                startOffset = 0;
              }
              containedInPriorDimensions += startOffset;
              int dimensionTotal = 1;
              for (unsigned int j = 0; j < dimensions.size(); ++j) {
                dimensionTotal *= dimensions[j];
              }
              if (containedInDimensions > 0) {
                shared_ptr<XdmfArray> partialArray = XdmfArray::New();
                if (datatype == H5T_NATIVE_CHAR) {
                  partialArray->initialize(XdmfArrayType::Int8(), 0);
                  char movedData [containedInDimensions];
                  array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
                  partialArray->insert(0, movedData, containedInDimensions);
                }
                else if (datatype == H5T_NATIVE_SHORT) {
                  partialArray->initialize(XdmfArrayType::Int16(), 0);
                  short movedData [containedInDimensions];
                  array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
                  partialArray->insert(0, movedData, containedInDimensions);
                }
                else if (datatype == H5T_NATIVE_INT) {
                  partialArray->initialize(XdmfArrayType::Int32(), 0);
                  int movedData [containedInDimensions];
                  array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
                  partialArray->insert(0, movedData, containedInDimensions);
                }
                else if (datatype == H5T_NATIVE_LONG) {
                  partialArray->initialize(XdmfArrayType::Int64(), 0);
                  long movedData [containedInDimensions];
                  array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
                  partialArray->insert(0, movedData, containedInDimensions);
                }
                else if (datatype == H5T_NATIVE_FLOAT) {
                  partialArray->initialize(XdmfArrayType::Float32(), 0);
                  float movedData [containedInDimensions];
                  array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
                  partialArray->insert(0, movedData, containedInDimensions);
                }
                else if (datatype == H5T_NATIVE_DOUBLE) {
                  partialArray->initialize(XdmfArrayType::Float64(), 0);
                  double movedData [containedInDimensions];
                  array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
                  partialArray->insert(0, movedData, containedInDimensions);
                }
                else if (datatype == H5T_NATIVE_UCHAR) {
                  partialArray->initialize(XdmfArrayType::UInt8(), 0);
                  unsigned char movedData [containedInDimensions];
                  array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
                  partialArray->insert(0, movedData, containedInDimensions);
                }
                else if (datatype == H5T_NATIVE_USHORT) {
                  partialArray->initialize(XdmfArrayType::UInt16(), 0);
                  unsigned short movedData [containedInDimensions];
                  array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
                  partialArray->insert(0, movedData, containedInDimensions);
                }
                else if (datatype == H5T_NATIVE_UINT) {
                  partialArray->initialize(XdmfArrayType::UInt32(), 0);
                  unsigned int movedData [containedInDimensions];
                  array.getValues(containedInPriorDimensions, movedData, containedInDimensions);
                  partialArray->insert(0, movedData, containedInDimensions);
                }
                else if (closeDatatype) {//closeDatatype is only true if strings are being used
                  partialArray->initialize(XdmfArrayType::String(), 0);
                  for (int j = containedInPriorDimensions; j < containedInPriorDimensions + containedInDimensions; ++j) {
                    partialArray->pushBack(array.getValue<std::string>(j));
                  }
                }
                arraysWritten.push_back(partialArray);
                filesWritten.push_back(testFile.str());
                startsWritten.push_back(partialStarts);
                stridesWritten.push_back(partialStrides);
                dimensionsWritten.push_back(partialDimensions);
                dataSizesWritten.push_back(partialDataSizes);
                arrayOffsetsWritten.push_back(containedInPriorDimensions);
              }
              if (mMode == Hyperslab) {
                containedInPriorDimensions -= controllerIndexOffset;
              }
              if (containedInDimensions + containedInPriorDimensions == dimensionTotal) {
                controllerIndexOffset += dimensionTotal;
              }
              //for hyperslab the space is controlled by the dataspace dimensions
              //so use that since the dimensions should be equal to the dataspace dimensions in all other variations
              //total up written data space
              unsigned int writtenDataSpace = 1;
              for (unsigned int j = 0; j < partialDataSizes.size(); ++j) {
                writtenDataSpace *= partialDataSizes[j];
              }
              amountAlreadyWritten += writtenDataSpace;
              //generate previous dimensions
              if (previousDataSizes.size() == 0) {
                previousDataSizes = partialDataSizes;
                previousDimensions = partialDimensions;
              }
              else {
                //determine if the sizes match
                //if they do, add the top values together
                //otherwise, compress the higher dimensions and then add them
                if (previousDimensions.size() == partialDimensions.size()) {
                  previousDimensions[previousDimensions.size()-1] += partialDimensions[previousDimensions.size()-1];
                }
                else if (previousDimensions.size() < partialDimensions.size()) {
                  unsigned int overflowDimensions = 1;
                  for (unsigned int j = previousDimensions.size() - 1; j < partialDimensions.size(); ++j) {
                    overflowDimensions *= partialDimensions[j];
                  }
                  previousDimensions[previousDimensions.size()-1] += overflowDimensions;
                }
                else if (previousDimensions.size() > partialDimensions.size()) {
                  unsigned int overflowDimensions = 1;
                  for (unsigned int j = partialDimensions.size() - 1; j < previousDimensions.size(); ++j) {
                    overflowDimensions *= previousDimensions[j];
                  }
                  previousDimensions.resize(partialDimensions.size());
                  previousDimensions[partialDimensions.size()-1] = overflowDimensions;
                  previousDimensions[previousDimensions.size()-1] += partialDimensions[previousDimensions.size()-1];
                }
                if (previousDataSizes.size() == partialDataSizes.size()) {
                  previousDataSizes[previousDataSizes.size()-1] += partialDataSizes[previousDataSizes.size()-1];
                }
                else if (previousDataSizes.size() < partialDataSizes.size()) {
                  unsigned int overflowDataSizes = 1;
                  for (unsigned int j = previousDataSizes.size() - 1; j < partialDataSizes.size(); ++j) {
                    overflowDataSizes *= partialDataSizes[j];
                  }
                  previousDataSizes[previousDataSizes.size()-1] += overflowDataSizes;
                }
                else if (previousDataSizes.size() > partialDataSizes.size()) {
                  unsigned int overflowDataSizes = 1;
                  for (unsigned int j = partialDataSizes.size() - 1; j < previousDataSizes.size(); ++j) {
                    overflowDataSizes *= previousDataSizes[j];
                  }
                  previousDataSizes.resize(partialDataSizes.size());
                  previousDataSizes[partialDataSizes.size()-1] = overflowDataSizes;
                  previousDataSizes[previousDataSizes.size()-1] += partialDataSizes[previousDataSizes.size()-1];
                }
              }
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
          unsigned int j = controllerIndexOffset;

          try {
            heavyDataController =
                this->createHDF5Controller(hdf5FilePath,
                                           dataSetPath.str(),
                                           array.getArrayType(),
                                           start,
                                           stride,
                                           dimensions,
                                           dataspaceDimensions);
          }
          catch (XdmfError e) {
            throw e;
          }

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
          }
          else if (closeDatatype) {//closeDatatype is only true if strings are being used
            partialArray->initialize(XdmfArrayType::String(), 0);
            //transfering via loop because the getValues function is not fully tested with strings
            for (j = controllerIndexOffset; j < controllerIndexOffset + heavyDataController->getSize() && j < array.getSize(); ++j){
              partialArray->pushBack(array.getValue<std::string>(j));
            }
          }
          if (partialArray->getSize()==0) {
            break;
          }
          arrayOffsetsWritten.push_back(controllerIndexOffset);
          controllerIndexOffset = j;//set the offset to the point after the end of the current subset

          arraysWritten.push_back(partialArray);
          filesWritten.push_back(hdf5FilePath);
          //also need to push the starts and strides loaded from the HeavyDataController
          startsWritten.push_back(start);
          stridesWritten.push_back(stride);
          dimensionsWritten.push_back(dimensions);
          dataSizesWritten.push_back(dataspaceDimensions);
        }

        if (mMode == Hyperslab)
        {
          i++;
          mImpl->mFileIndex = origFileIndex;
        }

      }


      std::list<std::string>::iterator fileNameWalker = filesWritten.begin();
      std::list<shared_ptr<XdmfArray> >::iterator arrayWalker = arraysWritten.begin();
      std::list<std::vector<unsigned int> >::iterator startWalker = startsWritten.begin();
      std::list<std::vector<unsigned int> >::iterator strideWalker = stridesWritten.begin();
      std::list<std::vector<unsigned int> >::iterator dimensionWalker = dimensionsWritten.begin();
      std::list<std::vector<unsigned int> >::iterator dataSizeWalker = dataSizesWritten.begin();
      std::list<unsigned int>::iterator arrayOffsetWalker = arrayOffsetsWritten.begin();


      //loop based on the amount of blocks split from the array.
      for (unsigned int writeIndex = 0; writeIndex < arraysWritten.size(); ++writeIndex) {

	//this is the section where the data is written to hdf5
	//if you want to change the writer to write to a different data format, do it here

        std::string curFileName = *fileNameWalker;
        shared_ptr<XdmfArray> curArray = *arrayWalker;
        std::vector<unsigned int> curStart = *startWalker;
        std::vector<unsigned int> curStride = *strideWalker;
        std::vector<unsigned int> curDimensions = *dimensionWalker;
        std::vector<unsigned int> curDataSize = *dataSizeWalker;
        unsigned int curArrayOffset = *arrayOffsetWalker;


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

	htri_t testingSet = H5Lexists(mImpl->mHDF5Handle,
                                      dataSetPath.str().c_str(),
                                      H5P_DEFAULT);

        hid_t dataset = 0;

        if (testingSet == 0) {
          dataset = -1;
        }
        else {
          dataset = H5Dopen(mImpl->mHDF5Handle,
                            dataSetPath.str().c_str(),
                            H5P_DEFAULT);
        }

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

          const hsize_t totalDimensionsSize =
            std::accumulate(current_dims.begin(),
                            current_dims.end(),
                            1,
                            std::multiplies<hsize_t>());
          //the Nth root of the chunk size divided by the dimensions added together
          const double factor =
            std::pow(((double)mImpl->mChunkSize / totalDimensionsSize),
                     1.0 / current_dims.size());//The end result is the amount of slots alloted per unit of dimension
          std::vector<hsize_t> chunk_size(current_dims.begin(),
                                          current_dims.end());
	  if (mImpl->mChunkSize > 0) {//The chunk size won't do anything unless it's positive
            for(std::vector<hsize_t>::iterator iter = chunk_size.begin();
                iter != chunk_size.end(); ++iter) {
              *iter = (hsize_t)(*iter * factor);
              if(*iter == 0) {
                *iter = 1;
              }
            }
          }

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

          //reset the datasize if the file or set is different
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
          if(ndims != current_dims.size()) {
            try {
            XdmfError::message(XdmfError::FATAL,                            \
                               "Data set rank different -- ndims != "
                               "current_dims.size() -- in "
                               "XdmfHDF5Writer::write");
            }
            catch (XdmfError e) {
              throw e;
            }
          }

          status = H5Dset_extent(dataset, &current_dims[0]);
          dataspace = H5Dget_space(dataset);
        }
        else if(mMode == Hyperslab) {
          // Hyperslab - dataset rank must remain the same (hdf5 constraint)
          dataspace = H5Dget_space(dataset);

          const unsigned int ndims = H5Sget_simple_extent_ndims(dataspace);
          if(ndims != current_dims.size()) {
            try {
              XdmfError::message(XdmfError::FATAL,                            \
                                 "Data set rank different -- ndims != "
                                 "current_dims.size() -- in "
                                 "XdmfHDF5Writer::write");
            }
            catch (XdmfError e) {
              throw e;
            }
          }
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
            try {
              XdmfError::message(XdmfError::FATAL,
                                 "H5Dset_extent returned failure in "
                                 "XdmfHDF5Writer::write -- status: " + status);
            }
            catch (XdmfError e) {
              throw e;
            }
          }
        }

        status = H5Dwrite(dataset,
                          datatype,
                          memspace,
                          dataspace,
                          H5P_DEFAULT,
                          curArray->getValuesInternal());

        if(status < 0) {
          try {
            XdmfError::message(XdmfError::FATAL,
                               "H5Dwrite returned failure in XdmfHDF5Writer::write "
                               "-- status: " + status);
          }
          catch (XdmfError e) {
            throw e;
          }
        }

        if(dataspace != H5S_ALL) {
          status = H5Sclose(dataspace);
        }

        if(memspace != H5S_ALL) {
          status = H5Sclose(memspace);
        }

        status = H5Dclose(dataset);

	//this is causing a lot of overhead
        if(closeFile) {
          mImpl->closeFile();
        }

        if(mMode == Default) {
          ++mDataSetId;
        }




        // Attach a new controller to the array
        shared_ptr<XdmfHDF5Controller> newDataController =
          shared_ptr<XdmfHDF5Controller>();//This generates an empty pointer

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

          try {
            newDataController =
              this->createHDF5Controller(curFileName,
                                         dataSetPath.str(),
                                         curArray->getArrayType(),
                                         insertStarts,
                                         insertStrides,
                                         insertDimensions,
                                         insertDataSpaceDimensions);
          }
          catch (XdmfError e) {
            throw e;
          }
        }

        if(!newDataController) {//if the controller wasn't generated by append
          try {
            newDataController =
              this->createHDF5Controller(curFileName,
                                         dataSetPath.str(),
                                         curArray->getArrayType(),
                                         curStart,
                                         curStride,
                                         curDimensions,
                                         curDataSize);
          }
          catch (XdmfError e) {
            throw e;
          }
        }

        newDataController->setArrayOffset(curArrayOffset);

        array.insert(newDataController);

        fileNameWalker++;
        arrayWalker++;
        startWalker++;
        strideWalker++;
        dimensionWalker++;
        dataSizeWalker++;
        arrayOffsetWalker++;


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
