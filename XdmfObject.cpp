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

std::string XdmfObject::printSelf() const
{
	return "XdmfObject";
}

std::ostream& operator<<(std::ostream& out, const XdmfObject& ob)
{
	out << ob.printSelf();
	return out;
}
