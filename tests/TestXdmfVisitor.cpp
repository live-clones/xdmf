#include "XdmfAttribute.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfVisitor> visitor3 = XdmfVisitor::New();
	boost::shared_ptr<XdmfGrid> grid = XdmfGrid::New();
	grid->setName("test");
	grid->getTopology()->setTopologyType(XdmfTopologyType::Hexahedron());
	boost::shared_ptr<XdmfAttribute> attr = XdmfAttribute::New();
	attr->setName("Attr1");
	attr->setAttributeType(XdmfAttributeType::Scalar());
	grid->insert(attr);
	grid->write(visitor3);
	std::cout << visitor3->printSelf() << std::endl;

	return 0;
}
