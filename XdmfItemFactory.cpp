#include <boost/tokenizer.hpp>
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
		std::map<std::string, std::string>::const_iterator type = itemProperties.find("Type");
		if(type == itemProperties.end())
		{
			type = itemProperties.find("GeometryType");
		}

		if(type != itemProperties.end())
		{
			const std::string typeVal = type->second;
			if(typeVal.compare("ORIGIN_DXDY") == 0 || typeVal.compare("ORIGIN_DXDYDZ") == 0)
			{
				boost::shared_ptr<XdmfArray> origin = boost::shared_ptr<XdmfArray>();
				boost::shared_ptr<XdmfArray> brickSize = boost::shared_ptr<XdmfArray>();
				for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
				{
					if(boost::shared_ptr<XdmfArray> array = boost::shared_dynamic_cast<XdmfArray>(*iter))
					{
						if(!origin)
						{
							origin = array;
						}
						else if(!brickSize)
						{
							brickSize = array;
							break;
						}
					}
				}
				if(origin && brickSize)
				{
					return XdmfGridRegular::New(brickSize, boost::shared_ptr<XdmfArray>(), origin);
				}
				return boost::shared_ptr<XdmfItem>();
			}
		}
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
				if(boost::shared_ptr<XdmfGridRegular> regularGrid = boost::shared_dynamic_cast<XdmfGridRegular>(*iter))
				{
					return XdmfGridRegular::New(0, 0, 0, 0, 0, 0);
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
		std::map<std::string, std::string>::const_iterator type = itemProperties.find("Type");
		if(type == itemProperties.end())
		{
			type = itemProperties.find("TopologyType");
		}

		if(type != itemProperties.end())
		{
			std::string typeVal = type->second;
			std::transform(typeVal.begin(), typeVal.end(), typeVal.begin(), (int(*)(int))std::toupper);
			if(typeVal.compare("2DCORECTMESH") == 0 || typeVal.compare("3DCORECTMESH") == 0)
			{
				boost::shared_ptr<XdmfArray> dimensionsArray = XdmfArray::New();
				std::string dimensionsString = "";
				std::map<std::string, std::string>::const_iterator dimensions = itemProperties.find("Dimensions");
				if(dimensions != itemProperties.end())
				{
					dimensionsString = dimensions->second;
				}
				boost::tokenizer<> tokens(dimensionsString);
				for(boost::tokenizer<>::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
				{
					dimensionsArray->pushBack<unsigned int>(atoi((*iter).c_str()));
				}
				return XdmfGridRegular::New(boost::shared_ptr<XdmfArray>(), dimensionsArray, boost::shared_ptr<XdmfArray>());
			}
		}
		return XdmfTopology::New();
	}
	return boost::shared_ptr<XdmfItem>();
}
