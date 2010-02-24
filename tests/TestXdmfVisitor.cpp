#include "XdmfAttribute.hpp"
#include "XdmfDataItem.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfVisitor> visitor = XdmfVisitor::New();
	visitor->setLightDataLimit(10);
	boost::shared_ptr<XdmfGrid> grid = XdmfGrid::New();

	grid->setName("test");

	// Set Geometry
	int points[] = {0, 0, 1, 1, 0, 1, 3, 0, 2, 0, 1, 1, 1, 1, 1, 3, 2, 2,
			0, 0, -1, 1, 0, -1, 3, 0, -2, 0, 1, -1, 1, 1, -1, 3, 2, -2};
	grid->getGeometry()->setNumberPoints(12);
	grid->getGeometry()->setGeometryType(XdmfGeometryType::XYZ());
	grid->getGeometry()->setValues(points);

	// Set Topology
	int connectivity[] = {0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10};
	grid->getTopology()->setNumberElements(2);
	grid->getTopology()->setTopologyType(XdmfTopologyType::Hexahedron());
	grid->getTopology()->setValues(connectivity);

	// Add Node Attribute
	boost::shared_ptr<XdmfAttribute> nodalAttribute = XdmfAttribute::New();
	int nodeValues[] = {100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600, 700};
	nodalAttribute->setName("Nodal Attribute");
	nodalAttribute->setNumberValues(12);
	nodalAttribute->setAttributeType(XdmfAttributeType::Scalar());
	nodalAttribute->setAttributeCenter(XdmfAttributeCenter::Node());
	nodalAttribute->setValues(nodeValues);

	// Add Cell Attribute
	boost::shared_ptr<XdmfAttribute> cellAttribute = XdmfAttribute::New();
	int cellValues[] = {100, 200};
	cellAttribute->setName("Cell Attribute");
	cellAttribute->setNumberValues(2);
	cellAttribute->setAttributeType(XdmfAttributeType::Scalar());
	cellAttribute->setAttributeCenter(XdmfAttributeCenter::Cell());
	cellAttribute->setValues(cellValues);

	grid->insert(nodalAttribute);
	grid->insert(cellAttribute);

	boost::shared_ptr<XdmfDomain> domain = XdmfDomain::New();
	domain->insert(grid);
	domain->write(visitor);

	std::cout << visitor->printSelf() << std::endl;

	return 0;
}
