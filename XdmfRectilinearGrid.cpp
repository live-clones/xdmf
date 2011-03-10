/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfRectilinearGrid.cpp                                             */
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

#include <cmath>
#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfError.hpp"

/**
 * PIMPL
 */
class XdmfRectilinearGrid::XdmfRectilinearGridImpl {

public:

  class XdmfGeometryRectilinear : public XdmfGeometry
  {

  public:

    static boost::shared_ptr<XdmfGeometryRectilinear>
    New(XdmfRectilinearGrid * const rectilinearGrid)
    {
      boost::shared_ptr<XdmfGeometryRectilinear>
        p(new XdmfGeometryRectilinear(rectilinearGrid));
      return p;
    }

    unsigned int
    getNumberPoints() const
    {
      const boost::shared_ptr<const XdmfArray> dimensions =
        mRectilinearGrid->getDimensions();
      if(dimensions->getSize() == 0) {
        return 0;
      }
      unsigned int toReturn = 1;
      for(unsigned int i=0; i<dimensions->getSize(); ++i) {
        toReturn *= dimensions->getValue<unsigned int>(i);
      }
      return toReturn;
    }

    void
    traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
    {
      const std::vector<boost::shared_ptr<XdmfArray> > & coordinates =
        mRectilinearGrid->getCoordinates();
      for(std::vector<boost::shared_ptr<XdmfArray> >::const_iterator iter =
            coordinates.begin();
          iter != coordinates.end();
          ++iter) {
        (*iter)->accept(visitor);
      }
    }

  private:

    XdmfGeometryRectilinear(XdmfRectilinearGrid * const rectilinearGrid) :
      mRectilinearGrid(rectilinearGrid)
    {
      this->setType(XdmfGeometryTypeRectilinear::New(mRectilinearGrid));
    }

    const XdmfRectilinearGrid * const mRectilinearGrid;
  };

  class XdmfGeometryTypeRectilinear : public XdmfGeometryType
  {

  public:

    static boost::shared_ptr<const XdmfGeometryTypeRectilinear>
    New(const XdmfRectilinearGrid * const rectilinearGrid)
    {
      boost::shared_ptr<const XdmfGeometryTypeRectilinear>
        p(new XdmfGeometryTypeRectilinear(rectilinearGrid));
      return p;
    }

    unsigned int
    getDimensions() const
    {
      return mRectilinearGrid->getDimensions()->getSize();
    }

    void
    getProperties(std::map<std::string, std::string> & collectedProperties) const
    {
      const unsigned int dimensions = this->getDimensions();
      if(dimensions == 3) {
        collectedProperties["Type"] = "VXVYVZ";
      }
      else if(dimensions == 2) {
        collectedProperties["Type"] = "VXVY";
      }
      else {
        XdmfError::message(XdmfError::FATAL, "Number of dimensions not 2 or 3 in XdmfGeometryTypeRectilinear::getProperties");
      }
    }

  private:

    XdmfGeometryTypeRectilinear(const XdmfRectilinearGrid * const rectilinearGrid) :
      XdmfGeometryType("", 0),
      mRectilinearGrid(rectilinearGrid)
    {
    }

    const XdmfRectilinearGrid * const mRectilinearGrid;

  };

  class XdmfTopologyRectilinear : public XdmfTopology
  {

  public:

    static boost::shared_ptr<XdmfTopologyRectilinear>
    New(const XdmfRectilinearGrid * const rectilinearGrid)
    {
      boost::shared_ptr<XdmfTopologyRectilinear>
        p(new XdmfTopologyRectilinear(rectilinearGrid));
      return p;
    }

    unsigned int
    getNumberElements() const
    {
      const boost::shared_ptr<const XdmfArray> dimensions =
        mRectilinearGrid->getDimensions();
      if(dimensions->getSize() == 0) {
        return 0;
      }
      unsigned int toReturn = 1;
      for(unsigned int i=0; i<dimensions->getSize(); ++i) {
        toReturn *= (dimensions->getValue<unsigned int>(i) - 1);
      }
      return toReturn;
    }

  private:

    XdmfTopologyRectilinear(const XdmfRectilinearGrid * const rectilinearGrid) :
      mRectilinearGrid(rectilinearGrid)
    {
      this->setType(XdmfTopologyTypeRectilinear::New(rectilinearGrid));
    }

    const XdmfRectilinearGrid * const mRectilinearGrid;
  };

  class XdmfTopologyTypeRectilinear : public XdmfTopologyType
  {

  public:

    static boost::shared_ptr<const XdmfTopologyTypeRectilinear>
    New(const XdmfRectilinearGrid * const rectilinearGrid)
    {
      boost::shared_ptr<const XdmfTopologyTypeRectilinear>
        p(new XdmfTopologyTypeRectilinear(rectilinearGrid));
      return p;
    }

    unsigned int
    getNodesPerElement() const
    {
      // 2^Dimensions
      // e.g. 1D = 2 nodes per element and 2D = 4 nodes per element.
      return (unsigned int)
        std::pow(2, (double)mRectilinearGrid->getDimensions()->getSize());
    }

    void
    getProperties(std::map<std::string, std::string> & collectedProperties) const
    {
      boost::shared_ptr<const XdmfArray> dimensions =
        mRectilinearGrid->getDimensions();
      if(dimensions->getSize() == 3) {
        collectedProperties["Type"] = "3DRectMesh";
      }
      else if(dimensions->getSize() == 2) {
        collectedProperties["Type"] = "2DRectMesh";
      }
      else {
        XdmfError::message(XdmfError::FATAL, "Number of dimensions not 2 or 3 in XdmfTopologyTypeRectilinear::getProperties");
      }
      collectedProperties["Dimensions"] = dimensions->getValuesString();
    }

  private:

    XdmfTopologyTypeRectilinear(const XdmfRectilinearGrid * const rectilinearGrid) :
      XdmfTopologyType(0, "foo", XdmfTopologyType::Structured, 0x1101),
      mRectilinearGrid(rectilinearGrid)
    {
    }

    const XdmfRectilinearGrid * const mRectilinearGrid;

  };

  XdmfRectilinearGridImpl(const std::vector<boost::shared_ptr<XdmfArray> > & coordinates) :
    mCoordinates(coordinates.begin(), coordinates.end())
  {
  }

  std::vector<boost::shared_ptr<XdmfArray> > mCoordinates;

};

boost::shared_ptr<XdmfRectilinearGrid>
XdmfRectilinearGrid::New(const boost::shared_ptr<XdmfArray> xCoordinates,
                         const boost::shared_ptr<XdmfArray> yCoordinates)
{
  std::vector<boost::shared_ptr<XdmfArray> > axesCoordinates;
  axesCoordinates.resize(2);
  axesCoordinates[0] = xCoordinates;
  axesCoordinates[1] = yCoordinates;
  boost::shared_ptr<XdmfRectilinearGrid>
    p(new XdmfRectilinearGrid(axesCoordinates));
  return p;
}

boost::shared_ptr<XdmfRectilinearGrid>
XdmfRectilinearGrid::New(const boost::shared_ptr<XdmfArray> xCoordinates,
                         const boost::shared_ptr<XdmfArray> yCoordinates,
                         const boost::shared_ptr<XdmfArray> zCoordinates)
{
  std::vector<boost::shared_ptr<XdmfArray> > axesCoordinates;
  axesCoordinates.resize(3);
  axesCoordinates[0] = xCoordinates;
  axesCoordinates[1] = yCoordinates;
  axesCoordinates[2] = zCoordinates;
  boost::shared_ptr<XdmfRectilinearGrid>
    p(new XdmfRectilinearGrid(axesCoordinates));
  return p;
}

boost::shared_ptr<XdmfRectilinearGrid>
XdmfRectilinearGrid::New(const std::vector<boost::shared_ptr<XdmfArray> > & axesCoordinates)
{
  boost::shared_ptr<XdmfRectilinearGrid>
    p(new XdmfRectilinearGrid(axesCoordinates));
  return p;
}

XdmfRectilinearGrid::XdmfRectilinearGrid(const std::vector<boost::shared_ptr<XdmfArray> > & axesCoordinates) :
  XdmfGrid(XdmfRectilinearGridImpl::XdmfGeometryRectilinear::New(this),
           XdmfRectilinearGridImpl::XdmfTopologyRectilinear::New(this)),
  mImpl(new XdmfRectilinearGridImpl(axesCoordinates))
{
}

XdmfRectilinearGrid::~XdmfRectilinearGrid()
{
  delete mImpl;
}

const std::string XdmfRectilinearGrid::ItemTag = "Grid";

boost::shared_ptr<XdmfArray>
XdmfRectilinearGrid::getCoordinates(const unsigned int axisIndex)
{
  return boost::const_pointer_cast<XdmfArray>
    (static_cast<const XdmfRectilinearGrid &>
     (*this).getCoordinates(axisIndex));
}

boost::shared_ptr<const XdmfArray>
XdmfRectilinearGrid::getCoordinates(const unsigned int axisIndex) const
{
  if(axisIndex < mImpl->mCoordinates.size()) {
    return mImpl->mCoordinates[axisIndex];
  }
  return boost::shared_ptr<XdmfArray>();
}

std::vector<boost::shared_ptr<XdmfArray> >
XdmfRectilinearGrid::getCoordinates()
{
  return static_cast<const XdmfRectilinearGrid &>(*this).getCoordinates();
}

const std::vector<boost::shared_ptr<XdmfArray> >
XdmfRectilinearGrid::getCoordinates() const
{
  return mImpl->mCoordinates;
}

boost::shared_ptr<XdmfArray>
XdmfRectilinearGrid::getDimensions()
{
  return boost::const_pointer_cast<XdmfArray>
    (static_cast<const XdmfRectilinearGrid &>(*this).getDimensions());
}

boost::shared_ptr<const XdmfArray>
XdmfRectilinearGrid::getDimensions() const
{
  boost::shared_ptr<XdmfArray> dimensions = XdmfArray::New();
  dimensions->reserve(mImpl->mCoordinates.size());
  for(std::vector<boost::shared_ptr<XdmfArray> >::const_iterator iter =
        mImpl->mCoordinates.begin();
      iter != mImpl->mCoordinates.end();
      ++iter) {
    dimensions->pushBack((*iter)->getSize());
  }
  return dimensions;
}

void
XdmfRectilinearGrid::populateItem(const std::map<std::string, std::string> & itemProperties,
                                  std::vector<boost::shared_ptr<XdmfItem> > & childItems,
                                  const XdmfCoreReader * const reader)
{
  XdmfGrid::populateItem(itemProperties, childItems, reader);

  for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter =
        childItems.begin();
      iter != childItems.end();
      ++iter) {
    if(boost::shared_ptr<XdmfRectilinearGrid> rectilinearGrid =
       boost::shared_dynamic_cast<XdmfRectilinearGrid>(*iter)) {
      if(rectilinearGrid->getGeometry()->getType()->getDimensions() > 0) {
        this->setCoordinates(rectilinearGrid->getCoordinates());
        break;
      }
    }
  }
}

void
XdmfRectilinearGrid::setCoordinates(const unsigned int axisIndex,
                                    const boost::shared_ptr<XdmfArray> axisCoordinates)
{
  if(mImpl->mCoordinates.size() <= axisIndex) {
    mImpl->mCoordinates.reserve(axisIndex + 1);
    unsigned int numArraysToInsert =
      axisIndex - mImpl->mCoordinates.size() + 1;
    for(unsigned int i=0; i<numArraysToInsert; ++i) {
      mImpl->mCoordinates.push_back(XdmfArray::New());
    }
  }
  mImpl->mCoordinates[axisIndex] = axisCoordinates;
}

void
XdmfRectilinearGrid::setCoordinates(const std::vector<boost::shared_ptr<XdmfArray> > axesCoordinates)
{
  mImpl->mCoordinates = axesCoordinates;
}
