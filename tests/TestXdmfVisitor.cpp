#include "XdmfAttribute.hpp"
#include "XdmfDataItem.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfVisitor> visitor3 = XdmfVisitor::New();
	boost::shared_ptr<XdmfGrid> grid = XdmfGrid::New();

	grid->setName("test");

	// Set Geometry
	int points[] = { 0, 0, 1, 1, 0, 1, 3, 0, 2, 0, 1, 1, 1, 1, 1, 3, 2, 2,
			0, 0, -1, 1, 0, -1, 3, 0, -2, 0, 1, -1, 1, 1, -1, 3, 2, -2};
	grid->getGeometry()->setGeometryType(XdmfGeometryType::XYZ());
	grid->getGeometry()->getData()->setData(points);
	grid->getGeometry()->getData()->setNumberValues(36);

	// Set Topology
	grid->getTopology()->setTopologyType(XdmfTopologyType::Hexahedron());
	grid->getTopology()->setNumberElements(2);
	int connectivity[] = { 0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10};
	grid->getTopology()->getData()->setNumberValues(16);
	grid->getTopology()->getData()->setData(connectivity);

	boost::shared_ptr<XdmfAttribute> attr = XdmfAttribute::New();
	attr->setName("Attr1");
	attr->setAttributeType(XdmfAttributeType::Scalar());
	grid->insert(attr);
	grid->write(visitor3);
	std::cout << visitor3->printSelf() << std::endl;

	return 0;
}
