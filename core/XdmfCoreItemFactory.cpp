#include "XdmfArray.hpp"
#include "XdmfCoreItemFactory.hpp"

XdmfCoreItemFactory::XdmfCoreItemFactory()
{
}

XdmfCoreItemFactory::~XdmfCoreItemFactory()
{
}

boost::shared_ptr<XdmfItem> XdmfCoreItemFactory::createItem(const std::string & itemTag, const std::map<std::string, std::string> &, const std::vector<boost::shared_ptr<XdmfItem> > &) const
{
	if(itemTag.compare(XdmfArray::ItemTag) == 0)
	{
		return XdmfArray::New();
	}
	return boost::shared_ptr<XdmfItem>();
}
