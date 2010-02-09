// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfAttribute.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"

#include <iomanip>

XdmfVisitor::XdmfVisitor() :
	mTabIndex(0),
	xmlData()
{
	std::cout << "Created Visitor " << this << std::endl;
}

XdmfVisitor::~XdmfVisitor()
{
	std::cout << "Deleted Visitor " << this << std::endl;
}

void XdmfVisitor::visit(const XdmfAttribute * const attribute)
{
	xmlData << std::setw(mTabIndex) << "" << "<Attribute> Name=\"" << attribute->getName() << "\" AttributeType=\"" << attribute->getAttributeTypeAsString() << "\" Center=\"Node\">\n";
	xmlData << std::setw(mTabIndex) << "" << "</Attribute>\n";
}

void XdmfVisitor::visit(const XdmfGeometry * const geometry)
{
	xmlData << std::setw(mTabIndex) << "" << "<Geometry GeometryType=\"" << geometry->getGeometryTypeAsString() << "\">\n";
	xmlData << std::setw(mTabIndex) << "" << "</Geometry>\n";
}

void XdmfVisitor::visit(const XdmfGrid * const grid)
{
	xmlData << std::setw(mTabIndex) << "" << "<Grid Name=\"" << grid->getName() <<"\">\n";
	mTabIndex++;
	visit(grid->getGeometry().get());
	visit(grid->getTopology().get());
	mTabIndex--;
	xmlData << std::setw(mTabIndex) << "" << "</Grid>\n";
}

void XdmfVisitor::visit(const XdmfTopology * const topology)
{
	xmlData << std::setw(mTabIndex) << "" << "<Topology TopologyType=\"" << topology->getTopologyTypeAsString() << "\" NumberOfElements=" << topology->getNumberElements() << "\">\n";
	xmlData << std::setw(mTabIndex) << "" << "</Topology>\n";
}

const std::string XdmfVisitor::printSelf() const
{
	return "XdmfVisitor:\n" + xmlData.str();
}
