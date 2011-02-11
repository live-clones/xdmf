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

#include "XdmfHeavyDataController.hpp"
#include "XdmfSystemUtils.hpp"

XdmfHeavyDataController::XdmfHeavyDataController(const std::string & filePath, const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type) :
	mDataSetPath(dataSetPath),
	mFilePath(XdmfSystemUtils::getRealPath(filePath)),
	mSize(size),
	mType(type)
{
}

XdmfHeavyDataController::~XdmfHeavyDataController()
{
}

std::string XdmfHeavyDataController::getDataSetPath() const
{
	return mDataSetPath;
}

std::string XdmfHeavyDataController::getFilePath() const
{
	return mFilePath;
}

unsigned int XdmfHeavyDataController::getSize() const
{
	return mSize;
}

boost::shared_ptr<const XdmfArrayType> XdmfHeavyDataController::getType() const
{
	return mType;
}
