#include "XdmfAttribute.hpp"
#include "XdmfGrid.hpp"
#include "XdmfSet.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfGrid> grid = XdmfGrid::New();
	std::string gridName = "Grid1";

	grid->setName(gridName);
	assert(grid->getName().compare(gridName) == 0);
	std::cout << grid->getName() << std::endl;

	boost::shared_ptr<XdmfAttribute> attr = XdmfAttribute::New();
	attr->setName("foo");
	assert(grid->getNumberOfAttributes() == 0);
	grid->insert(attr);
	assert(grid->getNumberOfAttributes() == 1);
	assert(grid->getAttribute(0) == attr);
	assert(grid->getAttribute("foo1") == NULL);
	assert(grid->getAttribute("foo") == attr);

	boost::shared_ptr<XdmfSet> set = XdmfSet::New();
	set->setName("foo");
	assert(grid->getNumberOfSets() == 0);
	grid->insert(set);
	assert(grid->getNumberOfSets() == 1);
	assert(grid->getSet(0) == set);
	assert(grid->getSet("foo1") == NULL);
	assert(grid->getSet("foo") == set);

	grid->insert(attr);
	grid->insert(set);
	assert(grid->getNumberOfAttributes() == 2);
	assert(grid->getNumberOfSets() == 2);
	grid->removeAttribute(0);
	grid->removeSet(0);
	assert(grid->getNumberOfAttributes() == 1);
	assert(grid->getNumberOfSets() == 1);
	grid->removeAttribute("foo");
	grid->removeSet("foo");
	assert(grid->getNumberOfAttributes() == 0);
	assert(grid->getNumberOfSets() == 0);


	return 0;
}
