#include <boost/tokenizer.hpp>
#include "XdmfAttribute.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfInformation.hpp"
#include "XdmfItemFactory.hpp"
#include "XdmfMap.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfSet.hpp"
#include "XdmfTime.hpp"
#include "XdmfTopology.hpp"
#include "XdmfUnstructuredGrid.hpp"

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
					return XdmfRegularGrid::New(brickSize, boost::shared_ptr<XdmfArray>(), origin);
				}
				return boost::shared_ptr<XdmfItem>();
			}
			else if(typeVal.compare("VXVY") == 0 || typeVal.compare("VXVYVZ") == 0)
			{
				std::vector<boost::shared_ptr<XdmfArray> > coordinateValues;
				for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
				{
					if(boost::shared_ptr<XdmfArray> array = boost::shared_dynamic_cast<XdmfArray>(*iter))
					{
						coordinateValues.push_back(array);
					}
				}
				return XdmfRectilinearGrid::New(coordinateValues);
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
				if(boost::shared_ptr<XdmfCurvilinearGrid> curvilinear = boost::shared_dynamic_cast<XdmfCurvilinearGrid>(*iter))
				{
					return XdmfCurvilinearGrid::New(0, 0);
				}
				else if(boost::shared_ptr<XdmfRegularGrid> regularGrid = boost::shared_dynamic_cast<XdmfRegularGrid>(*iter))
				{
					return XdmfRegularGrid::New(0, 0, 0, 0, 0, 0);
				}
				else if(boost::shared_ptr<XdmfRectilinearGrid> rectilinearGrid = boost::shared_dynamic_cast<XdmfRectilinearGrid>(*iter))
				{
					std::vector<boost::shared_ptr<XdmfArray> > coordinateValues;
					return XdmfRectilinearGrid::New(coordinateValues);
				}
			}
			return XdmfUnstructuredGrid::New();
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
			if(typeVal.compare("2DCORECTMESH") == 0 || typeVal.compare("3DCORECTMESH") == 0
				|| typeVal.compare("2DSMESH") == 0 || typeVal.compare("3DSMESH") == 0)
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
				if(typeVal.compare("2DCORECTMESH") == 0 || typeVal.compare("3DCORECTMESH") == 0)
				{
					return XdmfRegularGrid::New(boost::shared_ptr<XdmfArray>(), dimensionsArray, boost::shared_ptr<XdmfArray>());
				}
				else
				{
					return XdmfCurvilinearGrid::New(dimensionsArray);
				}
			}
			else if(typeVal.compare("2DRECTMESH") == 0 || typeVal.compare("3DRECTMESH") == 0)
			{
				std::vector<boost::shared_ptr<XdmfArray> > coordinateValues;
				return XdmfRectilinearGrid::New(coordinateValues);
			}

		}
		return XdmfTopology::New();
	}
	return boost::shared_ptr<XdmfItem>();
}
