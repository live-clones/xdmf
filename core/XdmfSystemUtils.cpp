/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfSystemUtils.cpp                                                 */
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

#include <limits.h>
#include <stdlib.h>
#include "XdmfSystemUtils.hpp"

XdmfSystemUtils::XdmfSystemUtils()
{
}

XdmfSystemUtils::~XdmfSystemUtils()
{
}

std::string
XdmfSystemUtils::getRealPath(const std::string & path)
{
  char realPath[PATH_MAX];
  realpath(path.c_str(), realPath);
  return realPath;
}
