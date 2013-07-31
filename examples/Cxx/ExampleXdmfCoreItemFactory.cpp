#include "XdmfAttribute.hpp"
#include "XdmfItemFactory.hpp"

int main(int, char **)
{
	//#createItem begin

	//using XdmfItemFactory because XdmfCoreItemFactory is abstract
	shared_ptr<XdmfItemFactory> exampleFactory = XdmfItemFactory::New();
	std::map<std::string, std::string> newProperties;
	std::vector<shared_ptr<XdmfItem> > newChildren;
	shared_ptr<XdmfAttribute> exampleAttribute =
		shared_dynamic_cast<XdmfAttribute>(exampleFactory->createItem(XdmfAttribute::ItemTag, newProperties, newChildren));
	//Same usage as the individual constructors
	//But the item has to be cast afterwards to the correct type.
	//childItems and itemProperties are not added to the item when created this way
	//the collections are used to determine type

	//#createItem end

	return 0;
}
