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
			this->setType(XdmfGeometryTypeRegular::New(regularGrid));
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
			const boost::shared_ptr<const XdmfArray> dimensions = mRegularGrid->getDimensions();
			if(dimensions->getSize() == 3)
			{
				collectedProperties["Type"] = "ORIGIN_DXDYDZ";
			}
			else if(dimensions->getSize() == 2)
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
			// 2 ^ Dimensions
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
			XdmfTopologyType(0, "foo", XdmfTopologyType::Structured),
			mRegularGrid(regularGrid)
		{
		}

		const XdmfGridRegular * const mRegularGrid;

	};

	XdmfGridRegularImpl(const unsigned int dimension) :
		mBrickSize(XdmfArray::New()),
		mDimensions(XdmfArray::New()),
		mOrigin(XdmfArray::New())
	{
		mBrickSize->initialize<double>(dimension);
		mDimensions->initialize<unsigned int>(dimension);
		mOrigin->initialize<double>(dimension);
	}

	boost::shared_ptr<XdmfArray> mBrickSize;
	boost::shared_ptr<XdmfArray> mDimensions;
	boost::shared_ptr<XdmfArray> mOrigin;

};

boost::shared_ptr<XdmfGridRegular> XdmfGridRegular::New(const double brickSizeX, const double brickSizeY, const unsigned int numPointsX,
	const unsigned int numPointsY, const double originX, const double originY)
{
	boost::shared_ptr<XdmfGridRegular> p(new XdmfGridRegular(brickSizeX, brickSizeY, numPointsX, numPointsY, originX, originY));
	return p;
}

boost::shared_ptr<XdmfGridRegular> XdmfGridRegular::New(const double brickSizeX, const double brickSizeY, const double brickSizeZ,
	const unsigned int numPointsX, const unsigned int numPointsY, const unsigned int numPointsZ,
	const double originX, const double originY, const double originZ)
{
	boost::shared_ptr<XdmfGridRegular> p(new XdmfGridRegular(brickSizeX, brickSizeY, brickSizeZ, numPointsX, numPointsY, numPointsZ, originX, originY, originZ));
	return p;
}

XdmfGridRegular::XdmfGridRegular(const double brickSizeX, const double brickSizeY, const unsigned int numPointsX,
	const unsigned int numPointsY, const double originX, const double originY) :
	mImpl(new XdmfGridRegularImpl(2))
{
	this->setGeometry(XdmfGridRegularImpl::XdmfGeometryRegular::New(this));
	this->setTopology(XdmfGridRegularImpl::XdmfTopologyRegular::New(this));
	mImpl->mBrickSize->insert(0, brickSizeX);
	mImpl->mBrickSize->insert(1, brickSizeY);
	mImpl->mDimensions->insert(0, numPointsX);
	mImpl->mDimensions->insert(1, numPointsY);
	mImpl->mOrigin->insert(0, originX);
	mImpl->mOrigin->insert(1, originY);
}

XdmfGridRegular::XdmfGridRegular(const double brickSizeX, const double brickSizeY, const double brickSizeZ,
	const unsigned int numPointsX, const unsigned int numPointsY, const unsigned int numPointsZ,
	const double originX, const double originY, const double originZ) :
	mImpl(new XdmfGridRegularImpl(3))
{
	this->setGeometry(XdmfGridRegularImpl::XdmfGeometryRegular::New(this));
	this->setTopology(XdmfGridRegularImpl::XdmfTopologyRegular::New(this));
	mImpl->mBrickSize->insert(0, brickSizeX);
	mImpl->mBrickSize->insert(1, brickSizeY);
	mImpl->mBrickSize->insert(2, brickSizeZ);
	mImpl->mDimensions->insert(0, numPointsX);
	mImpl->mDimensions->insert(1, numPointsY);
	mImpl->mDimensions->insert(2, numPointsZ);
	mImpl->mOrigin->insert(0, originX);
	mImpl->mOrigin->insert(1, originY);
	mImpl->mOrigin->insert(2, originZ);
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
