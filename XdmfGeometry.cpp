/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfGeometry.hpp"

XdmfGeometry::XdmfGeometry()
{
	std::cout << "Created Geometry " << this << std::endl;
}

XdmfGeometry::~XdmfGeometry()
{
	std::cout << "Deleted Geometry " << this << std::endl;
}

std::string XdmfGeometry::printSelf() const
{
	return "XdmfGeometry";
}
