#include <math.h>
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGrid.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyConverter.hpp"
#include "XdmfTopologyType.hpp"

/**
 * PIMPL
 */
class XdmfTopologyConverter::XdmfTopologyConverterImpl {

public:

	XdmfTopologyConverterImpl()
	{
	}

	~XdmfTopologyConverterImpl()
	{
	}

	struct PointComparison {
		bool operator()(const std::vector<double> & point1, const std::vector<double> & point2) const
		{
			double epsilon = 1e-6;
			for(unsigned int i=0; i<3; ++i)
			{
				if(fabs(point1[i] - point2[i]) > epsilon)
				{
					return point1[i] < point2[i];
				}
			}
			return false;
		}
	};

	inline void insertPointWithoutCheck(const std::vector<double> & newPoint, const boost::shared_ptr<XdmfArray> & newConnectivity, const boost::shared_ptr<XdmfArray> & newPoints) const
	{
		newConnectivity->pushBack<unsigned int>(newPoints->getSize() / 3);
		newPoints->pushBack(newPoint[0]);
		newPoints->pushBack(newPoint[1]);
		newPoints->pushBack(newPoint[2]);
	}

	inline void insertPointWithCheck(const std::vector<double> & newPoint, std::map<std::vector<double>, unsigned int, PointComparison> & coordToIdMap, const boost::shared_ptr<XdmfArray> & newConnectivity, const boost::shared_ptr<XdmfArray> & newPoints) const
	{
		std::map<std::vector<double>, unsigned int>::const_iterator iter = coordToIdMap.find(newPoint);
		if(iter == coordToIdMap.end())
		{
			// Not inserted before
			coordToIdMap[newPoint] = newPoints->getSize() / 3;;
			insertPointWithoutCheck(newPoint, newConnectivity, newPoints);
		}
		else
		{
			newConnectivity->pushBack(iter->second);
		}
	}

	class HexahedronToHexahedron_64;
	class HexahedronToHexahedron_125;
	class Hexahedron_64ToHexahedron;
};

class XdmfTopologyConverter::XdmfTopologyConverterImpl::HexahedronToHexahedron_64 : public XdmfTopologyConverter::XdmfTopologyConverterImpl {
public:

	HexahedronToHexahedron_64()
	{
	}

	inline void computeInteriorPoints(std::vector<double> & leftPoint, std::vector<double> & rightPoint, const std::vector<double> & point1, const std::vector<double> & point2) const
	{
		leftPoint[0] = (1.0/3.0)*(point2[0] + 2*point1[0]);
		leftPoint[1] = (1.0/3.0)*(point2[1] + 2*point1[1]);
		leftPoint[2] = (1.0/3.0)*(point2[2] + 2*point1[2]);

		rightPoint[0] = (1.0/3.0)*(2*point2[0] + point1[0]);
		rightPoint[1] = (1.0/3.0)*(2*point2[1] + point1[1]);
		rightPoint[2] = (1.0/3.0)*(2*point2[2] + point1[2]);
	}

	inline void computeLeftPoint(std::vector<double> & leftPoint, const std::vector<double> & point1, const std::vector<double> & point2) const
	{
		leftPoint[0] = (1.0/3.0)*(point2[0] + 2*point1[0]);
		leftPoint[1] = (1.0/3.0)*(point2[1] + 2*point1[1]);
		leftPoint[2] = (1.0/3.0)*(point2[2] + 2*point1[2]);
	}

	inline void computeRightPoint(std::vector<double> & rightPoint, const std::vector<double> & point1, const std::vector<double> & point2) const
	{
		rightPoint[0] = (1.0/3.0)*(2*point2[0] + point1[0]);
		rightPoint[1] = (1.0/3.0)*(2*point2[1] + point1[1]);
		rightPoint[2] = (1.0/3.0)*(2*point2[2] + point1[2]);
	}

	boost::shared_ptr<XdmfGrid> convert(const boost::shared_ptr<XdmfGrid> gridToConvert, const boost::shared_ptr<XdmfHDF5Writer> heavyDataWriter) const
	{
		boost::shared_ptr<XdmfGrid> toReturn = XdmfGrid::New();
		toReturn->setName(gridToConvert->getName());

		boost::shared_ptr<XdmfGeometry> toReturnGeometry = toReturn->getGeometry();
		toReturnGeometry->setType(gridToConvert->getGeometry()->getType());
		toReturnGeometry->initialize(gridToConvert->getGeometry()->getArrayType(), gridToConvert->getGeometry()->getSize());

		bool releaseGeometry = false;
		if(!gridToConvert->getGeometry()->isInitialized())
		{
			gridToConvert->getGeometry()->read();
			releaseGeometry = true;
		}

		// Copy all geometry values from old grid into new grid because we are keeping all old points.
		toReturnGeometry->insert(0, gridToConvert->getGeometry(), 0, gridToConvert->getGeometry()->getSize());

		if(releaseGeometry)
		{
			gridToConvert->getGeometry()->release();
		}

		boost::shared_ptr<XdmfTopology> toReturnTopology = toReturn->getTopology();
		toReturnTopology->setType(XdmfTopologyType::Hexahedron_64());
		toReturnTopology->initialize(gridToConvert->getTopology()->getArrayType());
		toReturnTopology->reserve(64 * gridToConvert->getTopology()->getNumberElements());

		bool releaseTopology = false;
		if(!gridToConvert->getTopology()->isInitialized())
		{
			gridToConvert->getTopology()->read();
		}

		std::vector<double> leftPoint(3);
		std::vector<double> rightPoint(3);
		std::map<std::vector<double>, unsigned int, PointComparison> coordToIdMap;

		std::vector<std::vector<double> > localNodes(44, std::vector<double>(3));

		for(unsigned int i=0; i<gridToConvert->getTopology()->getNumberElements(); ++i)
		{
			// Fill localNodes with original coordinate information.
			for(int j=0; j<8; ++j)
			{
				toReturnGeometry->getValues(gridToConvert->getTopology()->getValue<unsigned int>(8*i + j) * 3, &localNodes[j][0], 3);
			}

			// Add old connectivity information to newConnectivity.
			toReturnTopology->resize(toReturnTopology->getSize() + 8, 0);
			toReturnTopology->insert(64*i, gridToConvert->getTopology(), 8*i, 8);

			// Case 0
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[0], localNodes[1]);
			localNodes[8] = leftPoint;
			localNodes[9] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 1
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[1], localNodes[2]);
			localNodes[10] = leftPoint;
			localNodes[11] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 2
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[2], localNodes[3]);
			localNodes[12] = leftPoint;
			localNodes[13] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 3
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[3], localNodes[0]);
			localNodes[14] = leftPoint;
			localNodes[15] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 4
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[4], localNodes[5]);
			localNodes[16] = leftPoint;
			localNodes[17] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 5
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[5], localNodes[6]);
			localNodes[18] = leftPoint;
			localNodes[19] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 6
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[6], localNodes[7]);
			localNodes[20] = leftPoint;
			localNodes[21] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 7
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[7], localNodes[4]);
			localNodes[22] = leftPoint;
			localNodes[23] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 8
			computeLeftPoint(leftPoint, localNodes[0], localNodes[4]);
			localNodes[24] = leftPoint;
			insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 9
			this->computeLeftPoint(leftPoint, localNodes[1], localNodes[5]);
			localNodes[25] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 10
			this->computeLeftPoint(leftPoint, localNodes[2], localNodes[6]);
			localNodes[26] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 11
			this->computeLeftPoint(leftPoint, localNodes[3], localNodes[7]);
			localNodes[27] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 12
			this->computeRightPoint(leftPoint, localNodes[0], localNodes[4]);
			localNodes[28] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			 // Case 13
			this->computeRightPoint(leftPoint, localNodes[1], localNodes[5]);
			localNodes[29] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 14
			this->computeRightPoint(leftPoint, localNodes[2], localNodes[6]);
			localNodes[30] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 15
			this->computeRightPoint(leftPoint, localNodes[3], localNodes[7]);
			localNodes[31] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 16
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[27], localNodes[24]);
			localNodes[32] = leftPoint;
			localNodes[33] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 17
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[25], localNodes[26]);
			localNodes[34] = leftPoint;
			localNodes[35] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 18
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[24], localNodes[25]);
			localNodes[36] = leftPoint;
			localNodes[37] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 19
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[26], localNodes[27]);
			localNodes[38] = leftPoint;
			localNodes[39] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 20
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[31], localNodes[28]);
			localNodes[40] = leftPoint;
			localNodes[41] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 21
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[29], localNodes[30]);
			localNodes[42] = leftPoint;
			localNodes[43] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 22
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[28], localNodes[29]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 23
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[30], localNodes[31]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 24
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[15], localNodes[10]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 25
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[11], localNodes[14]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 26
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[23], localNodes[18]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 27
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[19], localNodes[22]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(rightPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 28
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[33], localNodes[34]);
			this->insertPointWithoutCheck(leftPoint, toReturnTopology, toReturnGeometry);
			this->insertPointWithoutCheck(rightPoint, toReturnTopology, toReturnGeometry);

			// Case 29
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[35], localNodes[32]);
			this->insertPointWithoutCheck(leftPoint, toReturnTopology, toReturnGeometry);
			this->insertPointWithoutCheck(rightPoint, toReturnTopology, toReturnGeometry);

			// Case 30
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[41], localNodes[42]);
			this->insertPointWithoutCheck(leftPoint, toReturnTopology, toReturnGeometry);
			this->insertPointWithoutCheck(rightPoint, toReturnTopology, toReturnGeometry);

			// Case 31
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[43], localNodes[40]);
			this->insertPointWithoutCheck(leftPoint, toReturnTopology, toReturnGeometry);
			this->insertPointWithoutCheck(rightPoint, toReturnTopology, toReturnGeometry);
		}
		if(releaseTopology)
		{
			gridToConvert->getTopology()->release();
		}
		if(heavyDataWriter)
		{
			toReturnTopology->accept(heavyDataWriter);
			toReturnTopology->release();
			toReturnGeometry->accept(heavyDataWriter);
			toReturnGeometry->release();
		}
		return toReturn;
	}
};

class XdmfTopologyConverter::XdmfTopologyConverterImpl::HexahedronToHexahedron_125 : public XdmfTopologyConverter::XdmfTopologyConverterImpl {
public:

	HexahedronToHexahedron_125()
	{
	}

	inline void computeInteriorPoints(std::vector<double> & quarterPoint, std::vector<double> & midPoint, std::vector<double> & threeQuarterPoint, const std::vector<double> & point1, const std::vector<double> & point2) const
	{
		quarterPoint[0] = (1.0/4.0)*(point2[0] + 3*point1[0]);
		quarterPoint[1] = (1.0/4.0)*(point2[1] + 3*point1[1]);
		quarterPoint[2] = (1.0/4.0)*(point2[2] + 3*point1[2]);

		midPoint[0] = (1.0/2.0)*(point2[0] + point1[0]);
		midPoint[1] = (1.0/2.0)*(point2[1] + point1[1]);
		midPoint[2] = (1.0/2.0)*(point2[2] + point1[2]);

		threeQuarterPoint[0] = (1.0/4.0)*(3.0*point2[0] + point1[0]);
		threeQuarterPoint[1] = (1.0/4.0)*(3.0*point2[1] + point1[1]);
		threeQuarterPoint[2] = (1.0/4.0)*(3.0*point2[2] + point1[2]);
	}

	inline void computeQuarterPoint (std::vector<double> & quarterPoint, const std::vector<double> & point1, const std::vector<double> & point2) const
	{
		quarterPoint[0] = (1.0/4.0)*(point2[0] + 3*point1[0]);
		quarterPoint[1] = (1.0/4.0)*(point2[1] + 3*point1[1]);
		quarterPoint[2] = (1.0/4.0)*(point2[2] + 3*point1[2]);
	}

	inline void computeMidPoint(std::vector<double> & midPoint, const std::vector<double> & point1, const std::vector<double> & point2) const
	{
		midPoint[0] = (1.0/2.0)*(point2[0] + point1[0]);
		midPoint[1] = (1.0/2.0)*(point2[1] + point1[1]);
		midPoint[2] = (1.0/2.0)*(point2[2] + point1[2]);
	}

	inline void computeThreeQuarterPoint(std::vector<double> & threeQuarterPoint, const std::vector<double> & point1, const std::vector<double> & point2) const
	{
		threeQuarterPoint[0] = (1.0/4.0)*(3.0*point2[0] + point1[0]);
		threeQuarterPoint[1] = (1.0/4.0)*(3.0*point2[1] + point1[1]);
		threeQuarterPoint[2] = (1.0/4.0)*(3.0*point2[2] + point1[2]);
	}

	boost::shared_ptr<XdmfGrid> convert(const boost::shared_ptr<XdmfGrid> gridToConvert, const boost::shared_ptr<XdmfHDF5Writer> heavyDataWriter) const
	{
		boost::shared_ptr<XdmfGrid> toReturn = XdmfGrid::New();
		toReturn->setName(gridToConvert->getName());

		boost::shared_ptr<XdmfGeometry> toReturnGeometry = toReturn->getGeometry();
		toReturnGeometry->setType(gridToConvert->getGeometry()->getType());
		toReturnGeometry->initialize(gridToConvert->getGeometry()->getArrayType(), gridToConvert->getGeometry()->getSize());

		bool releaseGeometry = false;
		if(!gridToConvert->getGeometry()->isInitialized())
		{
			gridToConvert->getGeometry()->read();
			releaseGeometry = true;
		}

		// Copy all geometry values from old grid into new grid because we are keeping all old points.
		toReturnGeometry->insert(0, gridToConvert->getGeometry(), 0, gridToConvert->getGeometry()->getSize());

		if(releaseGeometry)
		{
			gridToConvert->getGeometry()->release();
		}

		boost::shared_ptr<XdmfTopology> toReturnTopology = toReturn->getTopology();
		toReturn->getTopology()->setType(XdmfTopologyType::Hexahedron_125());
		toReturnTopology->initialize(gridToConvert->getTopology()->getArrayType());
		toReturnTopology->reserve(125 * gridToConvert->getTopology()->getNumberElements());

		bool releaseTopology = false;
		if(!gridToConvert->getTopology()->isInitialized())
		{
			gridToConvert->getTopology()->read();
		}

		std::vector<double> quarterPoint(3);
		std::vector<double> midPoint(3);
		std::vector<double> threeQuarterPoint(3);
		std::map<std::vector<double>, unsigned int, PointComparison> coordToIdMap;

		std::vector<std::vector<double> > localNodes(80, std::vector<double>(3));

		for(unsigned int i=0; i<gridToConvert->getTopology()->getNumberElements(); ++i)
		{
			// Fill localNodes with original coordinate information.

			for(int j=0; j<8; ++j)
			{
				toReturnGeometry->getValues(gridToConvert->getTopology()->getValue<unsigned int>(8*i + j) * 3, &localNodes[j][0], 3);
			}

			// Add old connectivity information to toReturnTopology.
			toReturnTopology->resize(toReturnTopology->getSize() + 8, 0);
			toReturnTopology->insert(125*i, gridToConvert->getTopology(), 8*i, 8);

			// Case 0
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[0], localNodes[1]);
			localNodes[8] = quarterPoint;
			localNodes[9] = midPoint;
			localNodes[10] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 1
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[1], localNodes[2]);
			localNodes[11] = quarterPoint;
			localNodes[12] = midPoint;
			localNodes[13] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 2
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[2], localNodes[3]);
			localNodes[14] = quarterPoint;
			localNodes[15] = midPoint;
			localNodes[16] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 3
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[3], localNodes[0]);
			localNodes[17] = quarterPoint;
			localNodes[18] = midPoint;
			localNodes[19] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 4
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[4], localNodes[5]);
			localNodes[20] = quarterPoint;
			localNodes[21] = midPoint;
			localNodes[22] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 5
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[5], localNodes[6]);
			localNodes[23] = quarterPoint;
			localNodes[24] = midPoint;
			localNodes[25] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 6
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[6], localNodes[7]);
			localNodes[26] = quarterPoint;
			localNodes[27] = midPoint;
			localNodes[28] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 7
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[7], localNodes[4]);
			localNodes[29] = quarterPoint;
			localNodes[30] = midPoint;
			localNodes[31] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 8
			this->computeQuarterPoint(quarterPoint, localNodes[0], localNodes[4]);
			localNodes[32] = quarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 9
			this->computeQuarterPoint(quarterPoint, localNodes[1], localNodes[5]);
			localNodes[33] = quarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 10
			this->computeQuarterPoint(quarterPoint, localNodes[2], localNodes[6]);
			localNodes[34] = quarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 11
			this->computeQuarterPoint(quarterPoint, localNodes[3], localNodes[7]);
			localNodes[35] = quarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 12
			this->computeThreeQuarterPoint(threeQuarterPoint, localNodes[0], localNodes[4]);
			localNodes[36] = threeQuarterPoint;
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 13
			this->computeThreeQuarterPoint(threeQuarterPoint, localNodes[1], localNodes[5]);
			localNodes[37] = threeQuarterPoint;
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 14
			this->computeThreeQuarterPoint(threeQuarterPoint, localNodes[2], localNodes[6]);
			localNodes[38] = threeQuarterPoint;
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 15
			this->computeThreeQuarterPoint(threeQuarterPoint, localNodes[3], localNodes[7]);
			localNodes[39] = threeQuarterPoint;
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 16
			this->computeMidPoint(midPoint, localNodes[0], localNodes[4]);
			localNodes[40] = midPoint;
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 17
			this->computeMidPoint(midPoint, localNodes[1], localNodes[5]);
			localNodes[41] = midPoint;
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 18
			this->computeMidPoint(midPoint, localNodes[2], localNodes[6]);
			localNodes[42] = midPoint;
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 19
			this->computeMidPoint(midPoint, localNodes[3], localNodes[7]);
			localNodes[43] = midPoint;
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 20
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[32], localNodes[33]);
			localNodes[44] = quarterPoint;
			localNodes[45] = midPoint;
			localNodes[46] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 21
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[33], localNodes[34]);
			localNodes[47] = quarterPoint;
			localNodes[48] = midPoint;
			localNodes[49] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 22
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[34], localNodes[35]);
			localNodes[50] = quarterPoint;
			localNodes[51] = midPoint;
			localNodes[52] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			 // Case 23
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[35], localNodes[32]);
			localNodes[53] = quarterPoint;
			localNodes[54] = midPoint;
			localNodes[55] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 24
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[36], localNodes[37]);
			localNodes[56] = quarterPoint;
			localNodes[57] = midPoint;
			localNodes[58] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 25
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[37], localNodes[38]);
			localNodes[59] = quarterPoint;
			localNodes[60] = midPoint;
			localNodes[61] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 26
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[38], localNodes[39]);
			localNodes[62] = quarterPoint;
			localNodes[63] = midPoint;
			localNodes[64] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 27
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[39], localNodes[36]);
			localNodes[65] = quarterPoint;
			localNodes[66] = midPoint;
			localNodes[67] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 28
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[40], localNodes[41]);
			localNodes[68] = quarterPoint;
			localNodes[69] = midPoint;
			localNodes[70] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 29
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[41], localNodes[42]);
			localNodes[71] = quarterPoint;
			localNodes[72] = midPoint;
			localNodes[73] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 30
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[42], localNodes[43]);
			localNodes[74] = quarterPoint;
			localNodes[75] = midPoint;
			localNodes[76] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

			// Case 31
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[43], localNodes[40]);
			localNodes[77] = quarterPoint;
			localNodes[78] = midPoint;
			localNodes[79] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 32
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[19], localNodes[11]);
		    this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
		    this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 33
		    this->computeMidPoint(midPoint, localNodes[10], localNodes[14]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 34
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[13], localNodes[17]);
		    this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
		    this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 35
		    this->computeMidPoint(midPoint, localNodes[16], localNodes[8]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 36
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[31], localNodes[23]);
		    this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
		    this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 37
		    this->computeMidPoint(midPoint, localNodes[22], localNodes[26]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 38
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[25], localNodes[29]);
		    this->insertPointWithCheck(quarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);
		    this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 39
		    this->computeMidPoint(midPoint, localNodes[28], localNodes[20]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 40
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[55], localNodes[47]);
		    this->insertPointWithoutCheck(quarterPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(threeQuarterPoint, toReturnTopology, toReturnGeometry);

		    // Case 41
		    this->computeMidPoint(midPoint, localNodes[46], localNodes[50]);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);

		    // Case 42
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[49], localNodes[53]);
		    this->insertPointWithoutCheck(quarterPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(threeQuarterPoint, toReturnTopology, toReturnGeometry);

		    // Case 43
		    this->computeMidPoint(midPoint, localNodes[52], localNodes[44]);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);

		    // Case 44
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[67], localNodes[59]);
		    this->insertPointWithoutCheck(quarterPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(threeQuarterPoint, toReturnTopology, toReturnGeometry);

		    // Case 45
		    this->computeMidPoint(midPoint, localNodes[62], localNodes[58]);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);

		    // Case 46
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[61], localNodes[65]);
		    this->insertPointWithoutCheck(quarterPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(threeQuarterPoint, toReturnTopology, toReturnGeometry);

		    // Case 47
		    this->computeMidPoint(midPoint, localNodes[56], localNodes[64]);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);

		    // Case 48
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[79], localNodes[71]);
		    this->insertPointWithoutCheck(quarterPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(threeQuarterPoint, toReturnTopology, toReturnGeometry);

		    // Case 49
		    this->computeMidPoint(midPoint, localNodes[70], localNodes[74]);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);

		    // Case 50
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[73], localNodes[77]);
		    this->insertPointWithoutCheck(quarterPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);
		    this->insertPointWithoutCheck(threeQuarterPoint, toReturnTopology, toReturnGeometry);

		    // Case 51
		    this->computeMidPoint(midPoint, localNodes[76], localNodes[68]);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);

		    // Case 52
		    this->computeMidPoint(midPoint, localNodes[12], localNodes[18]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 53
		    this->computeMidPoint(midPoint, localNodes[24], localNodes[30]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, toReturnTopology, toReturnGeometry);

		    // Case 54
		    this->computeMidPoint(midPoint, localNodes[48], localNodes[54]);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);

		    // Case 55
		    this->computeMidPoint(midPoint, localNodes[60], localNodes[66]);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);

		    // Case 56
		    this->computeMidPoint(midPoint, localNodes[72], localNodes[78]);
		    this->insertPointWithoutCheck(midPoint, toReturnTopology, toReturnGeometry);
		}
		if(releaseTopology)
		{
			gridToConvert->getTopology()->release();
		}
		if(heavyDataWriter)
		{
			toReturnTopology->accept(heavyDataWriter);
			toReturnTopology->release();
			toReturnGeometry->accept(heavyDataWriter);
			toReturnGeometry->release();
		}
		return toReturn;
	}
};

class XdmfTopologyConverter::XdmfTopologyConverterImpl::Hexahedron_64ToHexahedron : public XdmfTopologyConverter::XdmfTopologyConverterImpl {
public:

	Hexahedron_64ToHexahedron()
	{
	}

	boost::shared_ptr<XdmfGrid> convert(const boost::shared_ptr<XdmfGrid> gridToConvert, const boost::shared_ptr<XdmfHDF5Writer> heavyDataWriter) const
	{
		boost::shared_ptr<XdmfGrid> toReturn = XdmfGrid::New();
		toReturn->setName(gridToConvert->getName());
		toReturn->setGeometry(gridToConvert->getGeometry());

		if(heavyDataWriter)
		{
			if(!toReturn->getGeometry()->isInitialized())
			{
				toReturn->getGeometry()->read();
			}
			toReturn->getGeometry()->accept(heavyDataWriter);
			toReturn->getGeometry()->release();
		}

		boost::shared_ptr<XdmfArray> gridToConvertTopology = gridToConvert->getTopology();
		boost::shared_ptr<XdmfTopology> toReturnTopology = toReturn->getTopology();
		toReturnTopology->setType(XdmfTopologyType::Hexahedron());
		toReturnTopology->initialize(gridToConvertTopology->getArrayType(), 216 * gridToConvert->getTopology()->getNumberElements());

		bool releaseTopology;
		if(!gridToConvertTopology->isInitialized())
		{
			gridToConvertTopology->read();
			releaseTopology = true;
		}

		for(unsigned int i=0; i<gridToConvert->getTopology()->getNumberElements(); ++i)
		{
			const unsigned int startIndex = 216 * i;
			const unsigned int valuesStartIndex = 64 * i;
			toReturnTopology->insert(startIndex + 0, gridToConvertTopology, 0 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 1, gridToConvertTopology, 8 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 2, gridToConvertTopology, 48 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 3, gridToConvertTopology, 15 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 4, gridToConvertTopology, 24 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 5, gridToConvertTopology, 36 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 6, gridToConvertTopology, 56 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 7, gridToConvertTopology, 33 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 8, gridToConvertTopology, 8 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 9, gridToConvertTopology, 9 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 10, gridToConvertTopology, 49 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 11, gridToConvertTopology, 48 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 12, gridToConvertTopology, 36 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 13, gridToConvertTopology, 37 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 14, gridToConvertTopology, 57 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 15, gridToConvertTopology, 56 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 16, gridToConvertTopology, 9 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 17, gridToConvertTopology, 1 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 18, gridToConvertTopology, 10 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 19, gridToConvertTopology, 49 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 20, gridToConvertTopology, 37 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 21, gridToConvertTopology, 25 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 22, gridToConvertTopology, 34 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 23, gridToConvertTopology, 57 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 24, gridToConvertTopology, 15 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 25, gridToConvertTopology, 48 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 26, gridToConvertTopology, 51 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 27, gridToConvertTopology, 14 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 28, gridToConvertTopology, 33 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 29, gridToConvertTopology, 56 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 30, gridToConvertTopology, 59 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 31, gridToConvertTopology, 32 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 32, gridToConvertTopology, 48 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 33, gridToConvertTopology, 49 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 34, gridToConvertTopology, 50 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 35, gridToConvertTopology, 51 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 36, gridToConvertTopology, 56 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 37, gridToConvertTopology, 57 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 38, gridToConvertTopology, 58 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 39, gridToConvertTopology, 59 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 40, gridToConvertTopology, 49 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 41, gridToConvertTopology, 10 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 42, gridToConvertTopology, 11 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 43, gridToConvertTopology, 50 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 44, gridToConvertTopology, 57 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 45, gridToConvertTopology, 34 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 46, gridToConvertTopology, 35 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 47, gridToConvertTopology, 58 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 48, gridToConvertTopology, 14 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 49, gridToConvertTopology, 51 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 50, gridToConvertTopology, 13 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 51, gridToConvertTopology, 3 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 52, gridToConvertTopology, 32 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 53, gridToConvertTopology, 59 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 54, gridToConvertTopology, 39 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 55, gridToConvertTopology, 27 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 56, gridToConvertTopology, 51 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 57, gridToConvertTopology, 50 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 58, gridToConvertTopology, 12 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 59, gridToConvertTopology, 13 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 60, gridToConvertTopology, 59 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 61, gridToConvertTopology, 58 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 62, gridToConvertTopology, 38 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 63, gridToConvertTopology, 39 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 64, gridToConvertTopology, 50 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 65, gridToConvertTopology, 11 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 66, gridToConvertTopology, 2 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 67, gridToConvertTopology, 12 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 68, gridToConvertTopology, 58 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 69, gridToConvertTopology, 35 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 70, gridToConvertTopology, 26 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 71, gridToConvertTopology, 38 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 72, gridToConvertTopology, 24 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 73, gridToConvertTopology, 36 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 74, gridToConvertTopology, 56 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 75, gridToConvertTopology, 33 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 76, gridToConvertTopology, 28 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 77, gridToConvertTopology, 44 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 78, gridToConvertTopology, 60 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 79, gridToConvertTopology, 41 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 80, gridToConvertTopology, 36 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 81, gridToConvertTopology, 37 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 82, gridToConvertTopology, 57 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 83, gridToConvertTopology, 56 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 84, gridToConvertTopology, 44 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 85, gridToConvertTopology, 45 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 86, gridToConvertTopology, 61 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 87, gridToConvertTopology, 60 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 88, gridToConvertTopology, 37 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 89, gridToConvertTopology, 25 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 90, gridToConvertTopology, 34 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 91, gridToConvertTopology, 57 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 92, gridToConvertTopology, 45 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 93, gridToConvertTopology, 29 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 94, gridToConvertTopology, 42 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 95, gridToConvertTopology, 61 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 96, gridToConvertTopology, 33 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 97, gridToConvertTopology, 56 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 98, gridToConvertTopology, 59 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 99, gridToConvertTopology, 32 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 100, gridToConvertTopology, 41 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 101, gridToConvertTopology, 60 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 102, gridToConvertTopology, 63 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 103, gridToConvertTopology, 40 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 104, gridToConvertTopology, 56 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 105, gridToConvertTopology, 57 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 106, gridToConvertTopology, 58 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 107, gridToConvertTopology, 59 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 108, gridToConvertTopology, 60 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 109, gridToConvertTopology, 61 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 110, gridToConvertTopology, 62 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 111, gridToConvertTopology, 63 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 112, gridToConvertTopology, 57 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 113, gridToConvertTopology, 34 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 114, gridToConvertTopology, 35 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 115, gridToConvertTopology, 58 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 116, gridToConvertTopology, 61 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 117, gridToConvertTopology, 42 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 118, gridToConvertTopology, 43 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 119, gridToConvertTopology, 62 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 120, gridToConvertTopology, 32 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 121, gridToConvertTopology, 59 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 122, gridToConvertTopology, 39 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 123, gridToConvertTopology, 27 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 124, gridToConvertTopology, 40 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 125, gridToConvertTopology, 63 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 126, gridToConvertTopology, 47 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 127, gridToConvertTopology, 31 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 128, gridToConvertTopology, 59 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 129, gridToConvertTopology, 58 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 130, gridToConvertTopology, 38 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 131, gridToConvertTopology, 39 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 132, gridToConvertTopology, 63 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 133, gridToConvertTopology, 62 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 134, gridToConvertTopology, 46 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 135, gridToConvertTopology, 47 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 136, gridToConvertTopology, 58 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 137, gridToConvertTopology, 35 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 138, gridToConvertTopology, 26 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 139, gridToConvertTopology, 38 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 140, gridToConvertTopology, 62 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 141, gridToConvertTopology, 43 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 142, gridToConvertTopology, 30 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 143, gridToConvertTopology, 46 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 144, gridToConvertTopology, 28 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 145, gridToConvertTopology, 44 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 146, gridToConvertTopology, 60 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 147, gridToConvertTopology, 41 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 148, gridToConvertTopology, 4 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 149, gridToConvertTopology, 16 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 150, gridToConvertTopology, 52 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 151, gridToConvertTopology, 23 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 152, gridToConvertTopology, 44 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 153, gridToConvertTopology, 45 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 154, gridToConvertTopology, 61 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 155, gridToConvertTopology, 60 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 156, gridToConvertTopology, 16 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 157, gridToConvertTopology, 17 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 158, gridToConvertTopology, 53 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 159, gridToConvertTopology, 52 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 160, gridToConvertTopology, 45 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 161, gridToConvertTopology, 29 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 162, gridToConvertTopology, 42 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 163, gridToConvertTopology, 61 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 164, gridToConvertTopology, 17 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 165, gridToConvertTopology, 5 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 166, gridToConvertTopology, 18 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 167, gridToConvertTopology, 53 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 168, gridToConvertTopology, 41 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 169, gridToConvertTopology, 60 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 170, gridToConvertTopology, 63 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 171, gridToConvertTopology, 40 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 172, gridToConvertTopology, 23 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 173, gridToConvertTopology, 52 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 174, gridToConvertTopology, 55 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 175, gridToConvertTopology, 22 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 176, gridToConvertTopology, 60 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 177, gridToConvertTopology, 61 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 178, gridToConvertTopology, 62 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 179, gridToConvertTopology, 63 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 180, gridToConvertTopology, 52 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 181, gridToConvertTopology, 53 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 182, gridToConvertTopology, 54 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 183, gridToConvertTopology, 55 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 184, gridToConvertTopology, 61 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 185, gridToConvertTopology, 42 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 186, gridToConvertTopology, 43 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 187, gridToConvertTopology, 62 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 188, gridToConvertTopology, 53 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 189, gridToConvertTopology, 18 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 190, gridToConvertTopology, 19 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 191, gridToConvertTopology, 54 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 192, gridToConvertTopology, 40 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 193, gridToConvertTopology, 63 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 194, gridToConvertTopology, 47 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 195, gridToConvertTopology, 31 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 196, gridToConvertTopology, 22 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 197, gridToConvertTopology, 55 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 198, gridToConvertTopology, 21 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 199, gridToConvertTopology, 7 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 200, gridToConvertTopology, 63 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 201, gridToConvertTopology, 62 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 202, gridToConvertTopology, 46 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 203, gridToConvertTopology, 47 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 204, gridToConvertTopology, 55 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 205, gridToConvertTopology, 54 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 206, gridToConvertTopology, 20 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 207, gridToConvertTopology, 21 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 208, gridToConvertTopology, 62 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 209, gridToConvertTopology, 43 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 210, gridToConvertTopology, 30 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 211, gridToConvertTopology, 46 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 212, gridToConvertTopology, 54 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 213, gridToConvertTopology, 19 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 214, gridToConvertTopology, 6 + valuesStartIndex, 1);
			toReturnTopology->insert(startIndex + 215, gridToConvertTopology, 20 + valuesStartIndex, 1);
		}

		if(releaseTopology)
		{
			gridToConvertTopology->release();
		}

		if(heavyDataWriter)
		{
			toReturnTopology->accept(heavyDataWriter);
			toReturnTopology->release();
		}

		for(unsigned int i=0; i<gridToConvert->getNumberAttributes(); ++i)
		{
			boost::shared_ptr<XdmfAttribute> currAttribute = gridToConvert->getAttribute(i);
			boost::shared_ptr<XdmfAttribute> createdAttribute = boost::shared_ptr<XdmfAttribute>();
			if(currAttribute->getCenter() == XdmfAttributeCenter::Node())
			{
				createdAttribute = currAttribute;
			}
			else if(currAttribute->getCenter() == XdmfAttributeCenter::Cell())
			{
				bool releaseAttribute = false;
				if(!currAttribute->isInitialized())
				{
					currAttribute->read();
					releaseAttribute = true;
				}

				createdAttribute = XdmfAttribute::New();
				createdAttribute->setName(currAttribute->getName());
				createdAttribute->setType(currAttribute->getType());
				createdAttribute->setCenter(currAttribute->getCenter());
				createdAttribute->initialize(currAttribute->getArrayType(), currAttribute->getSize() * 27);
				for(unsigned int j=0; j<currAttribute->getSize(); ++j)
				{
					createdAttribute->insert(j * 27, currAttribute, j, 27, 1, 0);
				}

				if(releaseAttribute)
				{
					currAttribute->release();
				}
			}
			if(createdAttribute)
			{
				toReturn->insert(createdAttribute);
				if(heavyDataWriter)
				{
					if(!createdAttribute->isInitialized())
					{
						createdAttribute->read();
					}
					createdAttribute->accept(heavyDataWriter);
					createdAttribute->release();
				}
			}
		}
		return toReturn;
	}
};

boost::shared_ptr<XdmfTopologyConverter> XdmfTopologyConverter::New()
{
	boost::shared_ptr<XdmfTopologyConverter> p(new XdmfTopologyConverter());
	return p;
}

XdmfTopologyConverter::XdmfTopologyConverter()
{
}

XdmfTopologyConverter::~XdmfTopologyConverter()
{
}

boost::shared_ptr<XdmfGrid> XdmfTopologyConverter::convert(const boost::shared_ptr<XdmfGrid> gridToConvert, const boost::shared_ptr<const XdmfTopologyType> topologyType,
	const boost::shared_ptr<XdmfHDF5Writer> heavyDataWriter) const
{
	// Make sure geometry and topology are non null
	assert(gridToConvert->getGeometry() && gridToConvert->getTopology());

	boost::shared_ptr<const XdmfTopologyType> topologyTypeToConvert = gridToConvert->getTopology()->getType();
	if(topologyTypeToConvert == topologyType)
	{
		// No conversion necessary
		return gridToConvert;
	}

	if(gridToConvert->getGeometry()->getType() != XdmfGeometryType::XYZ())
	{
		assert(false);
	}

	if(topologyTypeToConvert == XdmfTopologyType::Hexahedron())
	{
		if(topologyType == XdmfTopologyType::Hexahedron_64())
		{
			XdmfTopologyConverterImpl::HexahedronToHexahedron_64 converter;
			return converter.convert(gridToConvert, heavyDataWriter);
		}
		else if(topologyType == XdmfTopologyType::Hexahedron_125())
		{
			XdmfTopologyConverterImpl::HexahedronToHexahedron_125 converter;
			return converter.convert(gridToConvert, heavyDataWriter);
		}
	}
	else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_64())
	{
		if(topologyType == XdmfTopologyType::Hexahedron())
		{
			XdmfTopologyConverterImpl::Hexahedron_64ToHexahedron converter;
			return converter.convert(gridToConvert, heavyDataWriter);
		}
	}
	assert(false);
}
