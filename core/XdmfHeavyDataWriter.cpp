/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHeavyDataWriter.cpp                                             */
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

#include "XdmfHeavyDataWriter.hpp"
#include "XdmfSystemUtils.hpp"

XdmfHeavyDataWriter::XdmfHeavyDataWriter() :
	mDataSetId(0),
	mFilePath(""),
	mMode(Default)
{
}

XdmfHeavyDataWriter::XdmfHeavyDataWriter(const std::string & filePath) :
	mDataSetId(0),
	mFilePath(XdmfSystemUtils::getRealPath(filePath)),
	mMode(Default)
{
}

XdmfHeavyDataWriter::~XdmfHeavyDataWriter()
{
}

std::string XdmfHeavyDataWriter::getFilePath() const
{
	return mFilePath;
}

XdmfHeavyDataWriter::Mode XdmfHeavyDataWriter::getMode() const
{
	return mMode;
}

void XdmfHeavyDataWriter::setMode(const Mode mode)
{
	mMode = mode;
}
