/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfUnstructuredGrid.cpp                                            */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfUnstructuredGrid.hpp"

shared_ptr<XdmfUnstructuredGrid>
XdmfUnstructuredGrid::New()
{
  shared_ptr<XdmfUnstructuredGrid> p(new XdmfUnstructuredGrid());
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

shared_ptr<XdmfGeometry>
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

shared_ptr<XdmfTopology>
XdmfUnstructuredGrid::getTopology()
{
  return boost::const_pointer_cast<XdmfTopology>
    (static_cast<const XdmfGrid &>(*this).getTopology());
}

void
XdmfUnstructuredGrid::setGeometry(const shared_ptr<XdmfGeometry> geometry)
{
  mGeometry = geometry;
}

void
XdmfUnstructuredGrid::setTopology(const shared_ptr<XdmfTopology> topology)
{
  mTopology = topology;
}
