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
#include "XdmfHeavyDataController.hpp"
#include "XdmfSystemUtils.hpp"

XdmfHeavyDataController::XdmfHeavyDataController(const std::string & filePath,
                                                 const std::string & dataSetPath,
                                                 const boost::shared_ptr<const XdmfArrayType> type,
                                                 const std::vector<unsigned int> & dimensions) :
  mDataSetPath(dataSetPath),
  mDimensions(dimensions),
  mFilePath(XdmfSystemUtils::getRealPath(filePath)),
  mType(type)
{
}

XdmfHeavyDataController::~XdmfHeavyDataController()
{
}

std::string
XdmfHeavyDataController::getDataSetPath() const
{
  return mDataSetPath;
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
                         std::multiplies<int>());
}

boost::shared_ptr<const XdmfArrayType>
XdmfHeavyDataController::getType() const
{
  return mType;
}
