#include "XdmfAttribute.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfInformation.hpp"
#include "XdmfItemFactory.hpp"
#include "XdmfSet.hpp"
#include "XdmfTime.hpp"
#include "XdmfTopology.hpp"

boost::shared_ptr<XdmfItemFactory> XdmfItemFactory::New()
{
	boost::shared_ptr<XdmfItemFactory> p(new XdmfItemFactory());
	return p;
}

XdmfItemFactory::XdmfItemFactory()
{
}

XdmfItemFactory::~XdmfItemFactory()
{
}

boost::shared_ptr<XdmfItem> XdmfItemFactory::createItem(const std::string & itemTag, const std::map<std::string, std::string> & itemProperties) const
{
	boost::shared_ptr<XdmfItem> newItem = XdmfCoreItemFactory::createItem(itemTag, itemProperties);

	if(newItem != NULL)
	{
		return newItem;
	}

	if(itemTag.compare(XdmfAttribute::ItemTag) == 0)
	{
		newItem = XdmfAttribute::New();
	}
	else if(itemTag.compare(XdmfDomain::ItemTag) == 0)
	{
		newItem = XdmfDomain::New();
	}
	else if(itemTag.compare(XdmfGeometry::ItemTag) == 0)
	{
		newItem = XdmfGeometry::New();
	}
	else if(itemTag.compare(XdmfGrid::ItemTag) == 0)
	{
		std::map<std::string, std::string>::const_iterator gridType = itemProperties.find("GridType");
		if(gridType != itemProperties.end() && gridType->second.compare("Collection") == 0)
		{
			newItem = XdmfGridCollection::New();
		}
		else
		{
			newItem = XdmfGrid::New();
		}
	}
	else if(itemTag.compare(XdmfInformation::ItemTag) == 0)
	{
		newItem = XdmfInformation::New();
	}
	else if(itemTag.compare(XdmfSet::ItemTag) == 0)
	{
		newItem = XdmfSet::New();
	}
	else if(itemTag.compare(XdmfTime::ItemTag) == 0)
	{
		newItem = XdmfTime::New();
	}
	else if(itemTag.compare(XdmfTopology::ItemTag) == 0)
	{
		newItem = XdmfTopology::New();
	}
	return newItem;
}
