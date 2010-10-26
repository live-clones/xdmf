/*
 * XdmfGridCurvilinear.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include <cmath>
#include "XdmfArray.hpp"
#include "XdmfGridCurvilinear.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

/**
 * PIMPL
 */
class XdmfGridCurvilinear::XdmfGridCurvilinearImpl {

public:

	class XdmfTopologyCurvilinear : public XdmfTopology
	{

	public:

		static boost::shared_ptr<XdmfTopologyCurvilinear> New(const XdmfGridCurvilinear * const curvilinearGrid)
		{
			boost::shared_ptr<XdmfTopologyCurvilinear> p(new XdmfTopologyCurvilinear(curvilinearGrid));
			return p;
		}

		unsigned int getNumberElements() const
		{
			const boost::shared_ptr<const XdmfArray> dimensions = mCurvilinearGrid->getDimensions();
			if(dimensions->getSize() == 0)
			{
				return 0;
			}
			unsigned int toReturn = 1;
			for(unsigned int i=0; i<dimensions->getSize(); ++i)
			{
				toReturn *= (dimensions->getValue<unsigned int>(i) - 1);
			}
			return toReturn;
		}

	private:

		XdmfTopologyCurvilinear(const XdmfGridCurvilinear * const curvilinearGrid) :
			mCurvilinearGrid(curvilinearGrid)
		{
			this->setType(XdmfTopologyTypeCurvilinear::New(curvilinearGrid));
		}

		const XdmfGridCurvilinear * const mCurvilinearGrid;
	};

	class XdmfTopologyTypeCurvilinear : public XdmfTopologyType
	{

	public:

		static boost::shared_ptr<const XdmfTopologyTypeCurvilinear> New(const XdmfGridCurvilinear * const curvilinearGrid)
		{
			boost::shared_ptr<const XdmfTopologyTypeCurvilinear> p(new XdmfTopologyTypeCurvilinear(curvilinearGrid));
			return p;
		}

		unsigned int getNodesPerElement() const
		{
			// 2^Dimensions
			// e.g. 1D = 2 nodes per element and 2D = 4 nodes per element.
			return (unsigned int)std::pow(2, (double)mCurvilinearGrid->getDimensions()->getSize());
		}

		void getProperties(std::map<std::string, std::string> & collectedProperties) const
		{
			boost::shared_ptr<const XdmfArray> dimensions = mCurvilinearGrid->getDimensions();
			if(dimensions->getSize() == 3)
			{
				collectedProperties["Type"] = "3DSMesh";
			}
			else if(dimensions->getSize() == 2)
			{
				collectedProperties["Type"] = "2DSMesh";
			}
			else
			{
				assert(false);
			}
			collectedProperties["Dimensions"] = dimensions->getValuesString();
		}

	private:

		XdmfTopologyTypeCurvilinear(const XdmfGridCurvilinear * const curvilinearGrid) :
			XdmfTopologyType(0, "foo", XdmfTopologyType::Structured, 0x1110),
			mCurvilinearGrid(curvilinearGrid)
		{
		}

		const XdmfGridCurvilinear * const mCurvilinearGrid;

	};

	XdmfGridCurvilinearImpl(const boost::shared_ptr<XdmfArray> numPoints) :
		mDimensions(numPoints)
	{
	}

	boost::shared_ptr<XdmfArray> mDimensions;

};

boost::shared_ptr<XdmfGridCurvilinear> XdmfGridCurvilinear::New(const unsigned int xNumPoints, const unsigned int yNumPoints)
{
	boost::shared_ptr<XdmfArray> numPoints = XdmfArray::New();
	numPoints->resize<unsigned int>(2);
	numPoints->insert(0, xNumPoints);
	numPoints->insert(1, yNumPoints);
	boost::shared_ptr<XdmfGridCurvilinear> p(new XdmfGridCurvilinear(numPoints));
	return p;
}

boost::shared_ptr<XdmfGridCurvilinear> XdmfGridCurvilinear::New(const unsigned int xNumPoints, const unsigned int yNumPoints,
	const unsigned int zNumPoints)
{
	boost::shared_ptr<XdmfArray> numPoints = XdmfArray::New();
	numPoints->resize<unsigned int>(3);
	numPoints->insert(0, xNumPoints);
	numPoints->insert(1, yNumPoints);
	numPoints->insert(2, zNumPoints);
	boost::shared_ptr<XdmfGridCurvilinear> p(new XdmfGridCurvilinear(numPoints));
	return p;
}

boost::shared_ptr<XdmfGridCurvilinear> XdmfGridCurvilinear::New(const boost::shared_ptr<XdmfArray> numPoints)
{
	boost::shared_ptr<XdmfGridCurvilinear> p(new XdmfGridCurvilinear(numPoints));
	return p;
}

XdmfGridCurvilinear::XdmfGridCurvilinear(const boost::shared_ptr<XdmfArray> numPoints) :
	mImpl(new XdmfGridCurvilinearImpl(numPoints))
{
	mTopology = XdmfGridCurvilinearImpl::XdmfTopologyCurvilinear::New(this);
}

XdmfGridCurvilinear::~XdmfGridCurvilinear()
{
	delete mImpl;
}

const std::string XdmfGridCurvilinear::ItemTag = "Grid";

boost::shared_ptr<XdmfArray> XdmfGridCurvilinear::getDimensions()
{
	return boost::const_pointer_cast<XdmfArray>(static_cast<const XdmfGridCurvilinear &>(*this).getDimensions());
}

boost::shared_ptr<const XdmfArray> XdmfGridCurvilinear::getDimensions() const
{
	return mImpl->mDimensions;
}

boost::shared_ptr<XdmfGeometry> XdmfGridCurvilinear::getGeometry()
{
	return boost::const_pointer_cast<XdmfGeometry>(static_cast<const XdmfGrid &>(*this).getGeometry());
}

void XdmfGridCurvilinear::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	XdmfGrid::populateItem(itemProperties, childItems, reader);

	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfGridCurvilinear> curvilinearGrid = boost::shared_dynamic_cast<XdmfGridCurvilinear>(*iter))
		{
			mImpl->mDimensions = curvilinearGrid->getDimensions();
		}
	}
}

void XdmfGridCurvilinear::setDimensions(const boost::shared_ptr<XdmfArray> dimensions)
{
	mImpl->mDimensions = dimensions;
}

void XdmfGridCurvilinear::setGeometry(const boost::shared_ptr<XdmfGeometry> geometry)
{
	mGeometry = geometry;
}

