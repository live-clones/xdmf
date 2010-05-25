/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfAttribute.hpp"
#include "XdmfGrid.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfSet.hpp"
#include "XdmfTopology.hpp"

XdmfGrid::XdmfGrid() :
	mGeometry(XdmfGeometry::New()),
	mTopology(XdmfTopology::New()),
	mName("Grid")
{
	std::cout << "Created Grid " << this << std::endl;
}

XdmfGrid::~XdmfGrid()
{
  std::cout << "Deleted Grid " << this << std::endl;
}

std::string XdmfGrid::ItemTag = "Grid";

boost::shared_ptr<XdmfAttribute> XdmfGrid::getAttribute(const unsigned int index)
{
	if(index >= mAttributes.size())
	{
		assert(false);
	}
	return mAttributes[index];
}

boost::shared_ptr<const XdmfAttribute> XdmfGrid::getAttribute(const unsigned int index) const
{
	if(index >= mAttributes.size())
	{
		assert(false);
	}
	return mAttributes[index];
}

boost::shared_ptr<XdmfGeometry> XdmfGrid::getGeometry()
{
	return mGeometry;
}

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

std::string XdmfGrid::getName() const
{
	return mName;
}

unsigned int XdmfGrid::getNumberOfAttributes() const
{
	return mAttributes.size();
}

unsigned int XdmfGrid::getNumberOfSets() const
{
	return mSets.size();
}

boost::shared_ptr<XdmfSet> XdmfGrid::getSet(const unsigned int index)
{
	if(index >= mSets.size())
	{
		assert(false);
	}
	return mSets[index];
}

boost::shared_ptr<const XdmfSet> XdmfGrid::getSet(const unsigned int index) const
{
	if(index >= mSets.size())
	{
		assert(false);
	}
	return mSets[index];
}

boost::shared_ptr<XdmfTopology> XdmfGrid::getTopology()
{
	return mTopology;
}

boost::shared_ptr<const XdmfTopology> XdmfGrid::getTopology() const
{
	return mTopology;
}

void XdmfGrid::insert(boost::shared_ptr<XdmfAttribute> attribute)
{
	mAttributes.push_back(attribute);
}

void XdmfGrid::insert(boost::shared_ptr<XdmfSet> set)
{
	mSets.push_back(set);
}

void XdmfGrid::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	std::map<std::string, std::string>::const_iterator name = itemProperties.find("Name");
	if(name != itemProperties.end())
	{
		mName = name->second;
	}
	else
	{
		assert(false);
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
		else if(boost::shared_ptr<XdmfSet> set = boost::shared_dynamic_cast<XdmfSet>(*iter))
		{
			this->insert(set);
		}
		else if(boost::shared_ptr<XdmfTopology> topology = boost::shared_dynamic_cast<XdmfTopology>(*iter))
		{
			mTopology = topology;
		}
		else
		{
			assert(false);
		}
	}
}

void XdmfGrid::removeAttribute(const unsigned int index)
{
	if(index >= mAttributes.size())
	{
		assert(false);
	}
	mAttributes.erase(mAttributes.begin() + index);
}

void XdmfGrid::removeSet(const unsigned int index)
{
	if(index >= mSets.size())
	{
		assert(false);
	}
	mSets.erase(mSets.begin() + index);
}

void XdmfGrid::setGeometry(boost::shared_ptr<XdmfGeometry> geometry)
{
	mGeometry = geometry;
}

void XdmfGrid::setName(const std::string & name)
{
	mName= name;
}

void XdmfGrid::setTopology(boost::shared_ptr<XdmfTopology> topology)
{
	mTopology = topology;
}

void XdmfGrid::traverse(boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	mGeometry->accept(visitor);
	mTopology->accept(visitor);
	for(std::vector<boost::shared_ptr<XdmfAttribute> >::const_iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
	for(std::vector<boost::shared_ptr<XdmfSet> >::const_iterator iter = mSets.begin(); iter != mSets.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
