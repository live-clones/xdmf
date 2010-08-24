#include "XdmfInformation.hpp"
#include "XdmfItem.hpp"

XDMF_CHILDREN_IMPLEMENTATION(XdmfItem, XdmfInformation, Information, Key)

XdmfItem::XdmfItem()
{
}

XdmfItem::~XdmfItem()
{
}

void XdmfItem::populateItem(const std::map<std::string, std::string> &, std::vector<boost::shared_ptr<XdmfItem > > & childItems, const XdmfCoreReader * const)
{
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfInformation> information = boost::shared_dynamic_cast<XdmfInformation>(*iter))
		{
			this->insert(information);
		}
	}
}

bool XdmfItem::IsEqual(boost::shared_ptr<XdmfItem> item)
{
	if (item == NULL) return false;
	if (this == item.get()) return(true);
	return(false);
}

void XdmfItem::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	for(std::vector<boost::shared_ptr<XdmfInformation> >::const_iterator iter = mInformations.begin(); iter != mInformations.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
