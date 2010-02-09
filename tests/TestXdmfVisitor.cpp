#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfVisitor> visitor = XdmfVisitor::New();
	boost::shared_ptr<XdmfTopology> top = XdmfTopology::New();
	top->write(visitor);
	std::cout << visitor->printSelf() << std::endl;

	boost::shared_ptr<XdmfVisitor> visitor2 = XdmfVisitor::New();
	boost::shared_ptr<XdmfGeometry> geom = XdmfGeometry::New();
	geom->write(visitor2);
	std::cout << visitor2->printSelf() << std::endl;

	boost::shared_ptr<XdmfVisitor> visitor3 = XdmfVisitor::New();
	boost::shared_ptr<XdmfGrid> grid = XdmfGrid::New();
	grid->setName("test");
	grid->getTopology()->setTopologyType(XdmfTopologyType::Hexahedron());
	grid->write(visitor3);
	std::cout << visitor3->printSelf() << std::endl;

	return 0;
}
