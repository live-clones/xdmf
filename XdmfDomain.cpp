/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfDomain.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCurvilinear.hpp"
#include "XdmfGridRectilinear.hpp"
#include "XdmfGridRegular.hpp"
#include "XdmfGridUnstructured.hpp"

XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain, XdmfGridCollection, GridCollection, Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain, XdmfGridCurvilinear, GridCurvilinear, Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain, XdmfGridRectilinear, GridRectilinear, Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain, XdmfGridRegular, GridRegular, Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain, XdmfGridUnstructured, GridUnstructured, Name)

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
		else if(boost::shared_ptr<XdmfGridCurvilinear> grid = boost::shared_dynamic_cast<XdmfGridCurvilinear>(*iter))
		{
			this->insert(grid);
		}
		else if(boost::shared_ptr<XdmfGridRectilinear> grid = boost::shared_dynamic_cast<XdmfGridRectilinear>(*iter))
		{
			this->insert(grid);
		}
		else if(boost::shared_ptr<XdmfGridRegular> grid = boost::shared_dynamic_cast<XdmfGridRegular>(*iter))
		{
			this->insert(grid);
		}
		else if(boost::shared_ptr<XdmfGridUnstructured> grid = boost::shared_dynamic_cast<XdmfGridUnstructured>(*iter))
		{
			this->insert(grid);
		}
	}
}

void XdmfDomain::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	XdmfItem::traverse(visitor);
	for(std::vector<boost::shared_ptr<XdmfGridCollection> >::const_iterator iter = mGridCollections.begin(); iter != mGridCollections.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
	for(std::vector<boost::shared_ptr<XdmfGridCurvilinear> >::const_iterator iter = mGridCurvilinears.begin(); iter != mGridCurvilinears.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
	for(std::vector<boost::shared_ptr<XdmfGridRectilinear> >::const_iterator iter = mGridRectilinears.begin(); iter != mGridRectilinears.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
	for(std::vector<boost::shared_ptr<XdmfGridRegular> >::const_iterator iter = mGridRegulars.begin(); iter != mGridRegulars.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
	for(std::vector<boost::shared_ptr<XdmfGridUnstructured> >::const_iterator iter = mGridUnstructureds.begin(); iter != mGridUnstructureds.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
