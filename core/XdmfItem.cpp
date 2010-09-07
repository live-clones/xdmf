#include "XdmfInformation.hpp"
#include "XdmfItem.hpp"

XdmfItem::XdmfItem()
{
}

XdmfItem::~XdmfItem()
{
}

boost::shared_ptr<XdmfInformation> XdmfItem::getInformation(const unsigned int index)
{
	return boost::const_pointer_cast<XdmfInformation>(static_cast<const XdmfItem &>(*this).getInformation(index));
}

boost::shared_ptr<const XdmfInformation> XdmfItem::getInformation(const unsigned int index) const
{
	if(index < mInformations.size())
	{
		return mInformations[index];
	}
	return boost::shared_ptr<XdmfInformation>();
}

boost::shared_ptr<XdmfInformation> XdmfItem::getInformation(const std::string & key)
{
	return boost::const_pointer_cast<XdmfInformation>(static_cast<const XdmfItem &>(*this).getInformation(key));
}

boost::shared_ptr<const XdmfInformation> XdmfItem::getInformation(const std::string & key) const
{
	for(std::vector<boost::shared_ptr<XdmfInformation> >::const_iterator iter = mInformations.begin(); iter != mInformations.end(); ++iter)
	{
		if((*iter)->getKey().compare(key) == 0)
		{
			return *iter;
		}
	}
	return boost::shared_ptr<XdmfInformation>();
}

unsigned int XdmfItem::getNumberInformations() const
{
	return mInformations.size();
}

void XdmfItem::insert(const boost::shared_ptr<XdmfInformation> information)
{
	mInformations.push_back(information);
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

void XdmfItem::removeInformation(const unsigned int index)
{
	if(index < mInformations.size())
	{
		mInformations.erase(mInformations.begin() + index);
	}
}

void XdmfItem::removeInformation(const std::string & key)
{
	for(std::vector<boost::shared_ptr<XdmfInformation> >::iterator iter = mInformations.begin(); iter != mInformations.end(); ++iter)
	{
		if((*iter)->getKey().compare(key) == 0)
		{
			mInformations.erase(iter);
			return;
		}
	}
}

void XdmfItem::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	for(std::vector<boost::shared_ptr<XdmfInformation> >::const_iterator iter = mInformations.begin(); iter != mInformations.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
