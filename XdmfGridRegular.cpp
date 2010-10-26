/*
 * XdmfGridRegular.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include <cmath>
#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridRegular.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

/**
 * PIMPL
 */
class XdmfGridRegular::XdmfGridRegularImpl {

public:

	class XdmfGeometryRegular : public XdmfGeometry
	{

	public:

		static boost::shared_ptr<XdmfGeometryRegular> New(XdmfGridRegular * const regularGrid)
		{
			boost::shared_ptr<XdmfGeometryRegular> p(new XdmfGeometryRegular(regularGrid));
			return p;
		}

		unsigned int getNumberPoints() const
		{
			const boost::shared_ptr<const XdmfArray> dimensions = mRegularGrid->getDimensions();
			if(dimensions->getSize() == 0)
			{
				return 0;
			}
			unsigned int toReturn = 1;
			for(unsigned int i=0; i<dimensions->getSize(); ++i)
			{
				toReturn *= dimensions->getValue<unsigned int>(i);
			}
			return toReturn;
		}

		void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
		{
			boost::shared_ptr<XdmfArray> origin = mRegularGrid->getOrigin();
			boost::shared_ptr<XdmfArray> brickSize = mRegularGrid->getBrickSize();
			origin->accept(visitor);
			brickSize->accept(visitor);
		}

	private:

		XdmfGeometryRegular(XdmfGridRegular * const regularGrid) :
			mRegularGrid(regularGrid)
		{
			this->setType(XdmfGeometryTypeRegular::New(mRegularGrid));
		}

		XdmfGridRegular * const mRegularGrid;
	};

	class XdmfGeometryTypeRegular : public XdmfGeometryType
	{

	public:

		static boost::shared_ptr<const XdmfGeometryTypeRegular> New(const XdmfGridRegular * const regularGrid)
		{
			boost::shared_ptr<const XdmfGeometryTypeRegular> p(new XdmfGeometryTypeRegular(regularGrid));
			return p;
		}

		unsigned int getDimensions() const
		{
			return mRegularGrid->getDimensions()->getSize();
		}

		void getProperties(std::map<std::string, std::string> & collectedProperties) const
		{
			const unsigned int dimensions = this->getDimensions();
			if(dimensions == 3)
			{
				collectedProperties["Type"] = "ORIGIN_DXDYDZ";
			}
			else if(dimensions == 2)
			{
				collectedProperties["Type"] = "ORIGIN_DXDY";
			}
			else
			{
				assert(false);
			}
		}

	private:

		XdmfGeometryTypeRegular(const XdmfGridRegular * const regularGrid) :
			XdmfGeometryType("", 0),
			mRegularGrid(regularGrid)
		{
		}

		const XdmfGridRegular * const mRegularGrid;

	};

	class XdmfTopologyRegular : public XdmfTopology
	{

	public:

		static boost::shared_ptr<XdmfTopologyRegular> New(const XdmfGridRegular * const regularGrid)
		{
			boost::shared_ptr<XdmfTopologyRegular> p(new XdmfTopologyRegular(regularGrid));
			return p;
		}

		unsigned int getNumberElements() const
		{
			const boost::shared_ptr<const XdmfArray> dimensions = mRegularGrid->getDimensions();
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

		XdmfTopologyRegular(const XdmfGridRegular * const regularGrid) :
			mRegularGrid(regularGrid)
		{
			this->setType(XdmfTopologyTypeRegular::New(regularGrid));
		}

		const XdmfGridRegular * const mRegularGrid;
	};

	class XdmfTopologyTypeRegular : public XdmfTopologyType
	{

	public:

		static boost::shared_ptr<const XdmfTopologyTypeRegular> New(const XdmfGridRegular * const regularGrid)
		{
			boost::shared_ptr<const XdmfTopologyTypeRegular> p(new XdmfTopologyTypeRegular(regularGrid));
			return p;
		}

		unsigned int getNodesPerElement() const
		{
			// 2^Dimensions
			// e.g. 1D = 2 nodes per element and 2D = 4 nodes per element.
			return (unsigned int)std::pow(2, (double)mRegularGrid->getDimensions()->getSize());
		}

		void getProperties(std::map<std::string, std::string> & collectedProperties) const
		{
			boost::shared_ptr<const XdmfArray> dimensions = mRegularGrid->getDimensions();
			if(dimensions->getSize() == 3)
			{
				collectedProperties["Type"] = "3DCoRectMesh";
			}
			else if(dimensions->getSize() == 2)
			{
				collectedProperties["Type"] = "2DCoRectMesh";
			}
			else
			{
				assert(false);
			}
			collectedProperties["Dimensions"] = dimensions->getValuesString();
		}

	private:

		XdmfTopologyTypeRegular(const XdmfGridRegular * const regularGrid) :
			XdmfTopologyType(0, "foo", XdmfTopologyType::Structured, 0x1102),
			mRegularGrid(regularGrid)
		{
		}

		const XdmfGridRegular * const mRegularGrid;

	};

	XdmfGridRegularImpl(const boost::shared_ptr<XdmfArray> brickSize, const boost::shared_ptr<XdmfArray> numPoints, const boost::shared_ptr<XdmfArray> origin) :
		mBrickSize(brickSize),
		mDimensions(numPoints),
		mOrigin(origin)
	{
	}

	boost::shared_ptr<XdmfArray> mBrickSize;
	boost::shared_ptr<XdmfArray> mDimensions;
	boost::shared_ptr<XdmfArray> mOrigin;

};

boost::shared_ptr<XdmfGridRegular> XdmfGridRegular::New(const double xBrickSize, const double yBrickSize, const unsigned int xNumPoints,
	const unsigned int yNumPoints, const double xOrigin, const double yOrigin)
{
	boost::shared_ptr<XdmfArray> brickSize = XdmfArray::New();
	brickSize->resize<double>(2);
	brickSize->insert(0, xBrickSize);
	brickSize->insert(1, yBrickSize);
	boost::shared_ptr<XdmfArray> numPoints = XdmfArray::New();
	numPoints->resize<unsigned int>(2);
	numPoints->insert(0, xNumPoints);
	numPoints->insert(1, yNumPoints);
	boost::shared_ptr<XdmfArray> origin = XdmfArray::New();
	origin->resize<double>(2);
	origin->insert(0, xOrigin);
	origin->insert(1, yOrigin);
	boost::shared_ptr<XdmfGridRegular> p(new XdmfGridRegular(brickSize, numPoints, origin));
	return p;
}

boost::shared_ptr<XdmfGridRegular> XdmfGridRegular::New(const double xBrickSize, const double yBrickSize, const double zBrickSize,
	const unsigned int xNumPoints, const unsigned int yNumPoints, const unsigned int zNumPoints,
	const double xOrigin, const double yOrigin, const double zOrigin)
{
	boost::shared_ptr<XdmfArray> brickSize = XdmfArray::New();
	brickSize->resize<double>(3);
	brickSize->insert(0, xBrickSize);
	brickSize->insert(1, yBrickSize);
	brickSize->insert(2, zBrickSize);
	boost::shared_ptr<XdmfArray> numPoints = XdmfArray::New();
	numPoints->resize<unsigned int>(3);
	numPoints->insert(0, xNumPoints);
	numPoints->insert(1, yNumPoints);
	numPoints->insert(2, zNumPoints);
	boost::shared_ptr<XdmfArray> origin = XdmfArray::New();
	origin->resize<double>(3);
	origin->insert(0, xOrigin);
	origin->insert(1, yOrigin);
	origin->insert(2, zOrigin);
	boost::shared_ptr<XdmfGridRegular> p(new XdmfGridRegular(brickSize, numPoints, origin));
	return p;
}

boost::shared_ptr<XdmfGridRegular> XdmfGridRegular::New(const boost::shared_ptr<XdmfArray> brickSize, const boost::shared_ptr<XdmfArray> numPoints,
		const boost::shared_ptr<XdmfArray> origin)
{
	boost::shared_ptr<XdmfGridRegular> p(new XdmfGridRegular(brickSize, numPoints, origin));
	return p;
}

XdmfGridRegular::XdmfGridRegular(const boost::shared_ptr<XdmfArray> brickSize, const boost::shared_ptr<XdmfArray> numPoints,
	const boost::shared_ptr<XdmfArray> origin) :
	mImpl(new XdmfGridRegularImpl(brickSize, numPoints, origin))
{
	mGeometry = XdmfGridRegularImpl::XdmfGeometryRegular::New(this);
	mTopology = XdmfGridRegularImpl::XdmfTopologyRegular::New(this);
}

XdmfGridRegular::~XdmfGridRegular()
{
	delete mImpl;
}

const std::string XdmfGridRegular::ItemTag = "Grid";

boost::shared_ptr<XdmfArray> XdmfGridRegular::getBrickSize()
{
	return boost::const_pointer_cast<XdmfArray>(static_cast<const XdmfGridRegular &>(*this).getBrickSize());
}

boost::shared_ptr<const XdmfArray> XdmfGridRegular::getBrickSize() const
{
	return mImpl->mBrickSize;
}

boost::shared_ptr<XdmfArray> XdmfGridRegular::getDimensions()
{
	return boost::const_pointer_cast<XdmfArray>(static_cast<const XdmfGridRegular &>(*this).getDimensions());
}

boost::shared_ptr<const XdmfArray> XdmfGridRegular::getDimensions() const
{
	return mImpl->mDimensions;
}

boost::shared_ptr<XdmfArray> XdmfGridRegular::getOrigin()
{
	return boost::const_pointer_cast<XdmfArray>(static_cast<const XdmfGridRegular &>(*this).getOrigin());
}

boost::shared_ptr<const XdmfArray> XdmfGridRegular::getOrigin() const
{
	return mImpl->mOrigin;
}

void XdmfGridRegular::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	XdmfGrid::populateItem(itemProperties, childItems, reader);

	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfGridRegular> regularGrid = boost::shared_dynamic_cast<XdmfGridRegular>(*iter))
		{
			if(regularGrid->getBrickSize())
			{
				mImpl->mBrickSize = regularGrid->getBrickSize();
			}

			if(regularGrid->getDimensions())
			{
				mImpl->mDimensions = regularGrid->getDimensions();
			}

			if(regularGrid->getOrigin())
			{
				mImpl->mOrigin = regularGrid->getOrigin();
			}
		}
	}
}

void XdmfGridRegular::setBrickSize(const boost::shared_ptr<XdmfArray> brickSize)
{
	mImpl->mBrickSize = brickSize;
}

void XdmfGridRegular::setDimensions(const boost::shared_ptr<XdmfArray> dimensions)
{
	mImpl->mDimensions = dimensions;
}

void XdmfGridRegular::setOrigin(const boost::shared_ptr<XdmfArray> origin)
{
	mImpl->mOrigin = origin;
}
