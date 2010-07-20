/*
 * XdmfObject.cpp
 *
 *  Created on: Feb 1, 2010
 *      Author: kleiter
 */

#include "XdmfObject.hpp"

XdmfObject::XdmfObject()
{
}

XdmfObject::~XdmfObject()
{
}

std::string XdmfObject::getRealPath(const std::string & path)
{
	char realPath[PATH_MAX];
	realpath(path.c_str(), realPath);
	return realPath;
}
