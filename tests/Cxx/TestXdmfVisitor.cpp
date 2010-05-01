#include "XdmfVisitor.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfVisitor> visitor = XdmfVisitor::New();
	visitor->setLightDataLimit(10);

	boost::shared_ptr<XdmfGrid> grid = XdmfTestDataGenerator::createHexahedron();

	boost::shared_ptr<XdmfDomain> domain = XdmfDomain::New();
	domain->insert(grid);
	domain->accept(visitor);

	return 0;
}
