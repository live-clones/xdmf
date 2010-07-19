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

std::string XdmfObject::getCWD()
{
	char buffer[256];
	getcwd(buffer, 256);
	return buffer;
}
