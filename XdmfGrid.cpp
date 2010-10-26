/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfAttribute.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfMap.hpp"
#include "XdmfSet.hpp"
#include "XdmfTime.hpp"
#include "XdmfTopology.hpp"

XDMF_CHILDREN_IMPLEMENTATION(XdmfGrid, XdmfAttribute, Attribute, Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfGrid, XdmfSet, Set, Name)

XdmfGrid::XdmfGrid(const std::string & name) :
	mGeometry(XdmfGeometry::New()),
	mTopology(XdmfTopology::New()),
	mMap(boost::shared_ptr<XdmfMap>()),
	mName(name),
	mTime(boost::shared_ptr<XdmfTime>())
{
}

XdmfGrid::~XdmfGrid()
{
}

const std::string XdmfGrid::ItemTag = "Grid";

boost::shared_ptr<const XdmfGeometry> XdmfGrid::getGeometry() const
{
	return mGeometry;
}

std::map<std::string, std::string> XdmfGrid::getItemProperties() const
{
	std::map<std::string, std::string> gridProperties;
	gridProperties["Name"] = mName;
	return gridProperties;
}

std::string XdmfGrid::getItemTag() const
{
	return ItemTag;
}

boost::shared_ptr<XdmfMap> XdmfGrid::getMap()
{
	return boost::const_pointer_cast<XdmfMap>(static_cast<const XdmfGrid &>(*this).getMap());
}

boost::shared_ptr<const XdmfMap> XdmfGrid::getMap() const
{
	return mMap;
}

std::string XdmfGrid::getName() const
{
	return mName;
}

boost::shared_ptr<XdmfTime> XdmfGrid::getTime()
{
	return boost::const_pointer_cast<XdmfTime>(static_cast<const XdmfGrid &>(*this).getTime());
}

boost::shared_ptr<const XdmfTime> XdmfGrid::getTime() const
{
	return mTime;
}

boost::shared_ptr<const XdmfTopology> XdmfGrid::getTopology() const
{
	return mTopology;
}

void XdmfGrid::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	XdmfItem::populateItem(itemProperties, childItems, reader);
	std::map<std::string, std::string>::const_iterator name = itemProperties.find("Name");
	if(name != itemProperties.end())
	{
		mName = name->second;
	}
	else
	{
		mName = "";
	}
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfAttribute> attribute = boost::shared_dynamic_cast<XdmfAttribute>(*iter))
		{
			this->insert(attribute);
		}
		else if(boost::shared_ptr<XdmfGeometry> geometry = boost::shared_dynamic_cast<XdmfGeometry>(*iter))
		{
			mGeometry = geometry;
		}
		else if(boost::shared_ptr<XdmfMap> map = boost::shared_dynamic_cast<XdmfMap>(*iter))
		{
			mMap = map;
		}
		else if(boost::shared_ptr<XdmfSet> set = boost::shared_dynamic_cast<XdmfSet>(*iter))
		{
			this->insert(set);
		}
		else if(boost::shared_ptr<XdmfTime> time = boost::shared_dynamic_cast<XdmfTime>(*iter))
		{
			mTime = time;
		}
		else if(boost::shared_ptr<XdmfTopology> topology = boost::shared_dynamic_cast<XdmfTopology>(*iter))
		{
			mTopology = topology;
		}
	}
}

void XdmfGrid::setMap(boost::shared_ptr<XdmfMap> map)
{
	mMap = map;
}

void XdmfGrid::setName(const std::string & name)
{
	mName = name;
}

void XdmfGrid::setTime(const boost::shared_ptr<XdmfTime> time)
{
	mTime = time;
}

void XdmfGrid::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	XdmfItem::traverse(visitor);
	if(mTime)
	{
		mTime->accept(visitor);
	}
	if(mGeometry)
	{
		mGeometry->accept(visitor);
	}
	if(mTopology)
	{
		mTopology->accept(visitor);
	}
	if(mMap)
	{
		mMap->accept(visitor);
	}
	for(std::vector<boost::shared_ptr<XdmfAttribute> >::const_iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
	for(std::vector<boost::shared_ptr<XdmfSet> >::const_iterator iter = mSets.begin(); iter != mSets.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
