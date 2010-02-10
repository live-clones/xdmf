#include "XdmfAttribute.hpp"
#include "XdmfGrid.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfGrid> grid = XdmfGrid::New();
	std::string gridName = "Grid1";

	grid->setName(gridName);
	assert(grid->getName().compare(gridName) == 0);
	std::cout << grid->getName() << std::endl;

	boost::shared_ptr<XdmfAttribute> attr = XdmfAttribute::New();
	attr->setName("Attr1");
	assert(grid->getNumberOfAttributes() == 0);
	grid->insert(attr);
	assert(grid->getNumberOfAttributes() == 1);
	assert(grid->getAttribute(0) == attr);

	return 0;
}
