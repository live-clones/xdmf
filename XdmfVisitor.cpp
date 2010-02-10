// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfAttribute.hpp"
#include "XdmfDataItem.hpp"
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
	xmlData << std::setw(mTabIndex) << "" << "<Attribute> Name=\"" << attribute->getName() << "\" AttributeType=\"" << attribute->getAttributeTypeAsString() << "\" Center=\"" << attribute->getAttributeCenterAsString() << "\">\n";
	xmlData << std::setw(mTabIndex) << "" << "</Attribute>\n";
}

void XdmfVisitor::visit(const XdmfDataItem * const dataItem)
{
	xmlData << std::setw(mTabIndex) << "" << "<DataItem Format=\"XML\" DataType=\"Int\" Precision=\"4\" Dimensions=\"" << dataItem->getNumberValues() << "\">";
	const int* const intPointer = dataItem->getData<int>();
	mTabIndex++;
	for (unsigned int i=0; i<dataItem->getNumberValues(); ++i)
	{
		if (i % 10 == 0)
		{
			xmlData << "\n" << std::setw(mTabIndex) << "" << intPointer[i] << " ";
		}
		else
		{
			xmlData << intPointer[i] << " ";
		}
	}
	mTabIndex--;
	xmlData << "\n" << std::setw(mTabIndex) << "" << "</DataItem>\n";
}

void XdmfVisitor::visit(const XdmfGeometry * const geometry)
{
	xmlData << std::setw(mTabIndex) << "" << "<Geometry GeometryType=\"" << geometry->getGeometryTypeAsString() << "\">\n";
	mTabIndex++;
	visit(geometry->getData().get());
	mTabIndex--;
	xmlData << std::setw(mTabIndex) << "" << "</Geometry>\n";
}

void XdmfVisitor::visit(const XdmfGrid * const grid)
{
	xmlData << std::setw(mTabIndex) << "" << "<Grid Name=\"" << grid->getName() <<"\">\n";
	mTabIndex++;
	visit(grid->getGeometry().get());
	visit(grid->getTopology().get());
	for(unsigned int i=0; i<grid->getNumberOfAttribute(); i++)
	{
		visit(grid->getAttribute(i).get());
	}
	mTabIndex--;
	xmlData << std::setw(mTabIndex) << "" << "</Grid>\n";
}

void XdmfVisitor::visit(const XdmfTopology * const topology)
{
	xmlData << std::setw(mTabIndex) << "" << "<Topology TopologyType=\"" << topology->getTopologyTypeAsString() << "\" NumberOfElements=\"" << topology->getNumberElements() << "\">\n";
	mTabIndex++;
	visit(topology->getData().get());
	mTabIndex--;
	xmlData << std::setw(mTabIndex) << "" << "</Topology>\n";
}

std::string XdmfVisitor::printSelf() const
{
	return "XdmfVisitor:\n" + xmlData.str();
}
