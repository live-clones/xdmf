/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHavyDataController.cpp                                          */
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

#include <functional>
#include <numeric>
#include "XdmfError.hpp"
#include "XdmfHeavyDataController.hpp"
#include "XdmfSystemUtils.hpp"

XdmfHeavyDataController::XdmfHeavyDataController(const std::string & filePath,
                                                 const std::string & dataSetPath,
                                                 const shared_ptr<const XdmfArrayType> type,
                                                 const std::vector<unsigned int> & start,
                                                 const std::vector<unsigned int> & stride,
                                                 const std::vector<unsigned int> & dimensions,
                                                 const std::vector<unsigned int> & dataspaceDimensions) :
  mDataSetPath(dataSetPath),
  mDataspaceDimensions(dataspaceDimensions),
  mDimensions(dimensions),
  mFilePath(filePath),
  mStart(start),
  mStride(stride),
  mType(type),
  mArrayStartOffset(0)
{
  if(!(mStart.size() == mStride.size() && 
       mStride.size() == mDimensions.size()))
    XdmfError::message(XdmfError::FATAL,
                       "mStart, mStride, mDimensions must all be of equal "
                       "length in XdmfHeavyDataController constructor");
}

XdmfHeavyDataController::~XdmfHeavyDataController()
{
}

std::string
XdmfHeavyDataController::getDataSetPath() const
{
  return mDataSetPath;
}

std::vector<unsigned int> 
XdmfHeavyDataController::getDataspaceDimensions() const
{
  return mDataspaceDimensions;
}

std::vector<unsigned int> XdmfHeavyDataController::getDimensions() const
{
  return mDimensions;
}

std::string
XdmfHeavyDataController::getFilePath() const
{
  return mFilePath;
}

unsigned int
XdmfHeavyDataController::getSize() const
{
  return std::accumulate(mDimensions.begin(),
                         mDimensions.end(),
                         1,
                         std::multiplies<unsigned int>());
}

void
XdmfHeavyDataController::setArrayOffset(unsigned int newOffset)
{
  mArrayStartOffset = newOffset;
}

unsigned int
XdmfHeavyDataController::getArrayOffset() const
{
  return mArrayStartOffset;
}

std::vector<unsigned int> 
XdmfHeavyDataController::getStart() const
{
  return mStart;
}

std::vector<unsigned int> 
XdmfHeavyDataController::getStride() const
{
  return mStride;
}

shared_ptr<const XdmfArrayType>
XdmfHeavyDataController::getType() const
{
  return mType;
}
