/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfDomain.hpp"
#include "XdmfGrid.hpp"
#include "XdmfGridCollection.hpp"

XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain, XdmfGrid, Grid, Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain, XdmfGridCollection, GridCollection, Name)

boost::shared_ptr<XdmfDomain> XdmfDomain::New()
{
	boost::shared_ptr<XdmfDomain> p(new XdmfDomain());
	return p;
}

XdmfDomain::XdmfDomain()
{
}

XdmfDomain::~XdmfDomain()
{
}

const std::string XdmfDomain::ItemTag = "Domain";

std::map<std::string, std::string> XdmfDomain::getItemProperties() const
{
	std::map<std::string, std::string> domainProperties;
	return domainProperties;
}

std::string XdmfDomain::getItemTag() const
{
	return ItemTag;
}

void XdmfDomain::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	XdmfItem::populateItem(itemProperties, childItems, reader);
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfGridCollection> gridCollection = boost::shared_dynamic_cast<XdmfGridCollection>(*iter))
		{
			this->insert(gridCollection);
		}
		else if(boost::shared_ptr<XdmfGrid> grid = boost::shared_dynamic_cast<XdmfGrid>(*iter))
		{
			this->insert(grid);
		}
	}
}

void XdmfDomain::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	XdmfItem::traverse(visitor);
	for(std::vector<boost::shared_ptr<XdmfGrid> >::const_iterator iter = mGrids.begin(); iter != mGrids.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
	for(std::vector<boost::shared_ptr<XdmfGridCollection> >::const_iterator iter = mGridCollections.begin(); iter != mGridCollections.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
