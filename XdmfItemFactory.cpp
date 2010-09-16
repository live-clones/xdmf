#include "XdmfAttribute.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGrid.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridRegular.hpp"
#include "XdmfInformation.hpp"
#include "XdmfItemFactory.hpp"
#include "XdmfMap.hpp"
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

boost::shared_ptr<XdmfItem> XdmfItemFactory::createItem(const std::string & itemTag, const std::map<std::string, std::string> & itemProperties, const std::vector<boost::shared_ptr<XdmfItem> > & childItems) const
{
	boost::shared_ptr<XdmfItem> newItem = XdmfCoreItemFactory::createItem(itemTag, itemProperties, childItems);

	if(newItem)
	{
		return newItem;
	}

	if(itemTag.compare(XdmfAttribute::ItemTag) == 0)
	{
		return XdmfAttribute::New();
	}
	else if(itemTag.compare(XdmfDomain::ItemTag) == 0)
	{
		return XdmfDomain::New();
	}
	else if(itemTag.compare(XdmfGeometry::ItemTag) == 0)
	{
		return XdmfGeometry::New();
	}
	else if(itemTag.compare(XdmfGrid::ItemTag) == 0)
	{
		// For backwards compatibility with the old format, this tag can correspond to multiple XdmfItems.
		std::map<std::string, std::string>::const_iterator gridType = itemProperties.find("GridType");
		if(gridType != itemProperties.end() && gridType->second.compare("Collection") == 0)
		{
			return XdmfGridCollection::New();
		}
		else
		{
			// Find out what kind of grid we have
			for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
			{
				if(boost::shared_ptr<XdmfGeometry> geometry = boost::shared_dynamic_cast<XdmfGeometry>(*iter))
				{
					if(geometry->getType()->getName().compare("REGULAR") == 0)
					{
						return XdmfGridRegular::New(0, 0, 0, 0, 0, 0);
					}
					else
					{
						break;
					}
				}
			}
			return XdmfGrid::New();
		}
	}
	else if(itemTag.compare(XdmfInformation::ItemTag) == 0)
	{
		return XdmfInformation::New();
	}
	else if(itemTag.compare(XdmfMap::ItemTag) == 0)
	{
		return XdmfMap::New();
	}
	else if(itemTag.compare(XdmfSet::ItemTag) == 0)
	{
		return XdmfSet::New();
	}
	else if(itemTag.compare(XdmfTime::ItemTag) == 0)
	{
		return XdmfTime::New();
	}
	else if(itemTag.compare(XdmfTopology::ItemTag) == 0)
	{
		return XdmfTopology::New();
	}
	return boost::shared_ptr<XdmfItem>();
}
