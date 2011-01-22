/*
 * XdmfUnstructuredGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfUnstructuredGrid.hpp"

boost::shared_ptr<XdmfUnstructuredGrid>
XdmfUnstructuredGrid::New()
{
  boost::shared_ptr<XdmfUnstructuredGrid> p(new XdmfUnstructuredGrid());
  return p;
}

XdmfUnstructuredGrid::XdmfUnstructuredGrid() :
  XdmfGrid(XdmfGeometry::New(), XdmfTopology::New())
{
}

XdmfUnstructuredGrid::~XdmfUnstructuredGrid()
{
}

const std::string XdmfUnstructuredGrid::ItemTag = "Grid";

boost::shared_ptr<XdmfGeometry>
XdmfUnstructuredGrid::getGeometry()
{
  return boost::const_pointer_cast<XdmfGeometry>
    (static_cast<const XdmfGrid &>(*this).getGeometry());
}

std::string
XdmfUnstructuredGrid::getItemTag() const
{
  return ItemTag;
}

boost::shared_ptr<XdmfTopology>
XdmfUnstructuredGrid::getTopology()
{
  return boost::const_pointer_cast<XdmfTopology>
    (static_cast<const XdmfGrid &>(*this).getTopology());
}

void
XdmfUnstructuredGrid::setGeometry(const boost::shared_ptr<XdmfGeometry> geometry)
{
  mGeometry = geometry;
}

void
XdmfUnstructuredGrid::setTopology(const boost::shared_ptr<XdmfTopology> topology)
{
  mTopology = topology;
}
