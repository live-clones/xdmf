#include "XdmfAttribute.hpp"
#include "XdmfGridUnstructured.hpp"
#include "XdmfInformation.hpp"
#include "XdmfSet.hpp"
#include "XdmfTime.hpp"

int main(int, char *)
{
	boost::shared_ptr<XdmfGridUnstructured> grid = XdmfGridUnstructured::New();
	std::string gridName = "Grid1";

	grid->setName(gridName);
	assert(grid->getName().compare(gridName) == 0);

	// Insert Attributes
	boost::shared_ptr<XdmfAttribute> attr = XdmfAttribute::New();
	attr->setName("foo");
	assert(grid->getNumberAttributes() == 0);
	grid->insert(attr);
	assert(grid->getNumberAttributes() == 1);
	assert(grid->getAttribute(0) == attr);
	assert(grid->getAttribute("foo") == attr);
	assert(grid->getAttribute(1) == NULL);
	assert(grid->getAttribute("foo1") == NULL);

	// Insert Sets
	boost::shared_ptr<XdmfSet> set = XdmfSet::New();
	set->setName("foo");
	assert(grid->getNumberSets() == 0);
	grid->insert(set);
	assert(grid->getNumberSets() == 1);
	assert(grid->getSet(0) == set);
	assert(grid->getSet("foo") == set);
	assert(grid->getSet("foo1") == NULL);
	assert(grid->getSet(1) == NULL);

	// Insert Time
	assert(grid->getTime() == NULL);
	boost::shared_ptr<XdmfTime> time = XdmfTime::New(0);
	grid->setTime(time);
	assert(grid->getTime() == time);

	// Remove Attributes and Sets
	grid->insert(attr);
	grid->insert(set);
	assert(grid->getNumberAttributes() == 2);
	assert(grid->getNumberSets() == 2);
	grid->removeAttribute(2);
	grid->removeSet(2);
	assert(grid->getNumberAttributes() == 2);
	assert(grid->getNumberSets() == 2);
	grid->removeAttribute(0);
	grid->removeSet(0);
	assert(grid->getNumberAttributes() == 1);
	assert(grid->getNumberSets() == 1);
	grid->removeAttribute("foo1");
	grid->removeSet("foo1");
	assert(grid->getNumberAttributes() == 1);
	assert(grid->getNumberSets() == 1);
	grid->removeAttribute("foo");
	grid->removeSet("foo");
	assert(grid->getNumberAttributes() == 0);
	assert(grid->getNumberSets() == 0);

	// Insert Information
	boost::shared_ptr<XdmfInformation> information = XdmfInformation::New("key", "value");
	grid->insert(information);

	return 0;
}
