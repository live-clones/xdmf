/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfDomain.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfUnstructuredGrid.hpp"

XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain,
                             XdmfGridCollection,
                             GridCollection,
                             Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain,
                             XdmfCurvilinearGrid,
                             CurvilinearGrid,
                             Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain,
                             XdmfRectilinearGrid,
                             RectilinearGrid,
                             Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain,
                             XdmfRegularGrid,
                             RegularGrid,
                             Name)
XDMF_CHILDREN_IMPLEMENTATION(XdmfDomain,
                             XdmfUnstructuredGrid,
                             UnstructuredGrid,
                             Name)

boost::shared_ptr<XdmfDomain>
XdmfDomain::New()
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

std::map<std::string, std::string>
XdmfDomain::getItemProperties() const
{
  std::map<std::string, std::string> domainProperties;
  return domainProperties;
}

std::string
XdmfDomain::getItemTag() const
{
  return ItemTag;
}

void
XdmfDomain::populateItem(const std::map<std::string, std::string> & itemProperties,
                         std::vector<boost::shared_ptr<XdmfItem> > & childItems,
                         const XdmfCoreReader * const reader)
{
  XdmfItem::populateItem(itemProperties, childItems, reader);
  for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter =
        childItems.begin();
      iter != childItems.end();
      ++iter) {
    if(boost::shared_ptr<XdmfGridCollection> gridCollection =
       boost::shared_dynamic_cast<XdmfGridCollection>(*iter)) {
      this->insert(gridCollection);
    }
    else if(boost::shared_ptr<XdmfCurvilinearGrid> grid =
            boost::shared_dynamic_cast<XdmfCurvilinearGrid>(*iter)) {
      this->insert(grid);
    }
    else if(boost::shared_ptr<XdmfRectilinearGrid> grid =
            boost::shared_dynamic_cast<XdmfRectilinearGrid>(*iter)) {
      this->insert(grid);
    }
    else if(boost::shared_ptr<XdmfRegularGrid> grid =
            boost::shared_dynamic_cast<XdmfRegularGrid>(*iter)) {
      this->insert(grid);
    }
    else if(boost::shared_ptr<XdmfUnstructuredGrid> grid =
            boost::shared_dynamic_cast<XdmfUnstructuredGrid>(*iter)) {
      this->insert(grid);
    }
  }
}

void
XdmfDomain::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
  XdmfItem::traverse(visitor);
  for(std::vector<boost::shared_ptr<XdmfGridCollection> >::const_iterator
        iter = mGridCollections.begin();
      iter != mGridCollections.end();
      ++iter) {
    (*iter)->accept(visitor);
  }
  for(std::vector<boost::shared_ptr<XdmfCurvilinearGrid> >::const_iterator
        iter = mCurvilinearGrids.begin();
      iter != mCurvilinearGrids.end();
      ++iter) {
    (*iter)->accept(visitor);
  }
  for(std::vector<boost::shared_ptr<XdmfRectilinearGrid> >::const_iterator
        iter = mRectilinearGrids.begin();
      iter != mRectilinearGrids.end();
      ++iter) {
    (*iter)->accept(visitor);
  }
  for(std::vector<boost::shared_ptr<XdmfRegularGrid> >::const_iterator
        iter = mRegularGrids.begin();
      iter != mRegularGrids.end();
      ++iter) {
    (*iter)->accept(visitor);
  }
  for(std::vector<boost::shared_ptr<XdmfUnstructuredGrid> >::const_iterator
        iter = mUnstructuredGrids.begin();
      iter != mUnstructuredGrids.end();
      ++iter) {
    (*iter)->accept(visitor);
  }
}
