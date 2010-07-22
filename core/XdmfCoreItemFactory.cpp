#include "XdmfArray.hpp"
#include "XdmfCoreItemFactory.hpp"

XdmfCoreItemFactory::XdmfCoreItemFactory()
{
}

XdmfCoreItemFactory::~XdmfCoreItemFactory()
{
}

boost::shared_ptr<XdmfItem> XdmfCoreItemFactory::createItem(const std::string & itemTag, const std::map<std::string, std::string> & itemProperties) const
{
	boost::shared_ptr<XdmfItem> newItem = boost::shared_ptr<XdmfItem>();
	if(itemTag.compare(XdmfArray::ItemTag) == 0)
	{
		newItem = XdmfArray::New();
	}
	return newItem;
}
