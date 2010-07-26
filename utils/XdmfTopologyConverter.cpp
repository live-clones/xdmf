#include <math.h>
#include "XdmfArray.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGrid.hpp"
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
		newConnectivity->pushBack(newPoints->getSize() / 3);
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

	boost::shared_ptr<XdmfGrid> convert(const boost::shared_ptr<XdmfGrid> gridToConvert) const
	{
		boost::shared_ptr<XdmfGrid> toReturn = XdmfGrid::New();
		toReturn->setName(gridToConvert->getName());
		toReturn->getGeometry()->setType(gridToConvert->getGeometry()->getType());
		toReturn->getTopology()->setType(XdmfTopologyType::Hexahedron_64());

		boost::shared_ptr<XdmfArray> newPoints = toReturn->getGeometry()->getArray();
		newPoints->initialize(gridToConvert->getGeometry()->getArray()->getType());
		newPoints->resize(gridToConvert->getGeometry()->getArray()->getSize(), 0);

		if(!gridToConvert->getGeometry()->getArray()->isInitialized())
		{
			gridToConvert->getGeometry()->getArray()->read();
		}

		// Copy all geometry values from old grid into new grid because we are keeping all old points.
		newPoints->copyValues(0, gridToConvert->getGeometry()->getArray(), 0, gridToConvert->getGeometry()->getArray()->getSize());

		boost::shared_ptr<XdmfArray> newConnectivity = toReturn->getTopology()->getArray();
		newConnectivity->initialize(gridToConvert->getTopology()->getArray()->getType());
		newConnectivity->reserve(64 * gridToConvert->getTopology()->getNumberElements());

		if(!gridToConvert->getTopology()->getArray()->isInitialized())
		{
			gridToConvert->getTopology()->getArray()->read();
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
				gridToConvert->getGeometry()->getArray()->getValuesCopy(gridToConvert->getTopology()->getArray()->getValueCopy<unsigned int>(8*i + j) * 3, &localNodes[j][0], 3);
			}

			// Add old connectivity information to newConnectivity.
			newConnectivity->resize(newConnectivity->getSize() + 8, 0);
			newConnectivity->copyValues(64*i, gridToConvert->getTopology()->getArray(), 8*i, 8);

			// Case 0
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[0], localNodes[1]);
			localNodes[8] = leftPoint;
			localNodes[9] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 1
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[1], localNodes[2]);
			localNodes[10] = leftPoint;
			localNodes[11] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 2
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[2], localNodes[3]);
			localNodes[12] = leftPoint;
			localNodes[13] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 3
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[3], localNodes[0]);
			localNodes[14] = leftPoint;
			localNodes[15] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 4
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[4], localNodes[5]);
			localNodes[16] = leftPoint;
			localNodes[17] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 5
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[5], localNodes[6]);
			localNodes[18] = leftPoint;
			localNodes[19] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 6
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[6], localNodes[7]);
			localNodes[20] = leftPoint;
			localNodes[21] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 7
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[7], localNodes[4]);
			localNodes[22] = leftPoint;
			localNodes[23] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 8
			computeLeftPoint(leftPoint, localNodes[0], localNodes[4]);
			localNodes[24] = leftPoint;
			insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 9
			this->computeLeftPoint(leftPoint, localNodes[1], localNodes[5]);
			localNodes[25] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 10
			this->computeLeftPoint(leftPoint, localNodes[2], localNodes[6]);
			localNodes[26] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 11
			this->computeLeftPoint(leftPoint, localNodes[3], localNodes[7]);
			localNodes[27] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 12
			this->computeRightPoint(leftPoint, localNodes[0], localNodes[4]);
			localNodes[28] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			 // Case 13
			this->computeRightPoint(leftPoint, localNodes[1], localNodes[5]);
			localNodes[29] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 14
			this->computeRightPoint(leftPoint, localNodes[2], localNodes[6]);
			localNodes[30] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 15
			this->computeRightPoint(leftPoint, localNodes[3], localNodes[7]);
			localNodes[31] = leftPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 16
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[27], localNodes[24]);
			localNodes[32] = leftPoint;
			localNodes[33] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 17
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[25], localNodes[26]);
			localNodes[34] = leftPoint;
			localNodes[35] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 18
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[24], localNodes[25]);
			localNodes[36] = leftPoint;
			localNodes[37] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 19
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[26], localNodes[27]);
			localNodes[38] = leftPoint;
			localNodes[39] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 20
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[31], localNodes[28]);
			localNodes[40] = leftPoint;
			localNodes[41] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 21
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[29], localNodes[30]);
			localNodes[42] = leftPoint;
			localNodes[43] = rightPoint;
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 22
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[28], localNodes[29]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 23
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[30], localNodes[31]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 24
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[15], localNodes[10]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 25
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[11], localNodes[14]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 26
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[23], localNodes[18]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 27
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[19], localNodes[22]);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 28
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[33], localNodes[34]);
			this->insertPointWithoutCheck(leftPoint, newConnectivity, newPoints);
			this->insertPointWithoutCheck(rightPoint, newConnectivity, newPoints);

			// Case 29
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[35], localNodes[32]);
			this->insertPointWithoutCheck(leftPoint, newConnectivity, newPoints);
			this->insertPointWithoutCheck(rightPoint, newConnectivity, newPoints);

			// Case 30
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[41], localNodes[42]);
			this->insertPointWithoutCheck(leftPoint, newConnectivity, newPoints);
			this->insertPointWithoutCheck(rightPoint, newConnectivity, newPoints);

			// Case 31
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[43], localNodes[40]);
			this->insertPointWithoutCheck(leftPoint, newConnectivity, newPoints);
			this->insertPointWithoutCheck(rightPoint, newConnectivity, newPoints);
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

	boost::shared_ptr<XdmfGrid> convert(const boost::shared_ptr<XdmfGrid> gridToConvert) const
	{
		boost::shared_ptr<XdmfGrid> toReturn = XdmfGrid::New();
		toReturn->setName(gridToConvert->getName());
		toReturn->getGeometry()->setType(gridToConvert->getGeometry()->getType());
		toReturn->getTopology()->setType(XdmfTopologyType::Hexahedron_125());

		boost::shared_ptr<XdmfArray> newPoints = toReturn->getGeometry()->getArray();
		newPoints->initialize(gridToConvert->getGeometry()->getArray()->getType());
		newPoints->resize(gridToConvert->getGeometry()->getArray()->getSize(), 0);

		if(!gridToConvert->getGeometry()->getArray()->isInitialized())
		{
			gridToConvert->getGeometry()->getArray()->read();
		}

		// Copy all geometry values from old grid into new grid because we are keeping all old points.
		newPoints->copyValues(0, gridToConvert->getGeometry()->getArray(), 0, gridToConvert->getGeometry()->getArray()->getSize());

		boost::shared_ptr<XdmfArray> newConnectivity = toReturn->getTopology()->getArray();
		newConnectivity->initialize(gridToConvert->getTopology()->getArray()->getType());
		newConnectivity->reserve(125 * gridToConvert->getTopology()->getNumberElements());

		if(!gridToConvert->getTopology()->getArray()->isInitialized())
		{
			gridToConvert->getTopology()->getArray()->read();
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
				gridToConvert->getGeometry()->getArray()->getValuesCopy(gridToConvert->getTopology()->getArray()->getValueCopy<unsigned int>(8*i + j) * 3, &localNodes[j][0], 3);
			}

			// Add old connectivity information to newConnectivity.
			newConnectivity->resize(newConnectivity->getSize() + 8, 0);
			newConnectivity->copyValues(125*i, gridToConvert->getTopology()->getArray(), 8*i, 8);

			// Case 0
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[0], localNodes[1]);
			localNodes[8] = quarterPoint;
			localNodes[9] = midPoint;
			localNodes[10] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 1
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[1], localNodes[2]);
			localNodes[11] = quarterPoint;
			localNodes[12] = midPoint;
			localNodes[13] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 2
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[2], localNodes[3]);
			localNodes[14] = quarterPoint;
			localNodes[15] = midPoint;
			localNodes[16] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 3
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[3], localNodes[0]);
			localNodes[17] = quarterPoint;
			localNodes[18] = midPoint;
			localNodes[19] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 4
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[4], localNodes[5]);
			localNodes[20] = quarterPoint;
			localNodes[21] = midPoint;
			localNodes[22] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 5
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[5], localNodes[6]);
			localNodes[23] = quarterPoint;
			localNodes[24] = midPoint;
			localNodes[25] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 6
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[6], localNodes[7]);
			localNodes[26] = quarterPoint;
			localNodes[27] = midPoint;
			localNodes[28] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 7
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[7], localNodes[4]);
			localNodes[29] = quarterPoint;
			localNodes[30] = midPoint;
			localNodes[31] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 8
			this->computeQuarterPoint(quarterPoint, localNodes[0], localNodes[4]);
			localNodes[32] = quarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 9
			this->computeQuarterPoint(quarterPoint, localNodes[1], localNodes[5]);
			localNodes[33] = quarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 10
			this->computeQuarterPoint(quarterPoint, localNodes[2], localNodes[6]);
			localNodes[34] = quarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 11
			this->computeQuarterPoint(quarterPoint, localNodes[3], localNodes[7]);
			localNodes[35] = quarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 12
			this->computeThreeQuarterPoint(threeQuarterPoint, localNodes[0], localNodes[4]);
			localNodes[36] = threeQuarterPoint;
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 13
			this->computeThreeQuarterPoint(threeQuarterPoint, localNodes[1], localNodes[5]);
			localNodes[37] = threeQuarterPoint;
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 14
			this->computeThreeQuarterPoint(threeQuarterPoint, localNodes[2], localNodes[6]);
			localNodes[38] = threeQuarterPoint;
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 15
			this->computeThreeQuarterPoint(threeQuarterPoint, localNodes[3], localNodes[7]);
			localNodes[39] = threeQuarterPoint;
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 16
			this->computeMidPoint(midPoint, localNodes[0], localNodes[4]);
			localNodes[40] = midPoint;
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 17
			this->computeMidPoint(midPoint, localNodes[1], localNodes[5]);
			localNodes[41] = midPoint;
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 18
			this->computeMidPoint(midPoint, localNodes[2], localNodes[6]);
			localNodes[42] = midPoint;
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 19
			this->computeMidPoint(midPoint, localNodes[3], localNodes[7]);
			localNodes[43] = midPoint;
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 20
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[32], localNodes[33]);
			localNodes[44] = quarterPoint;
			localNodes[45] = midPoint;
			localNodes[46] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 21
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[33], localNodes[34]);
			localNodes[47] = quarterPoint;
			localNodes[48] = midPoint;
			localNodes[49] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 22
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[34], localNodes[35]);
			localNodes[50] = quarterPoint;
			localNodes[51] = midPoint;
			localNodes[52] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			 // Case 23
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[35], localNodes[32]);
			localNodes[53] = quarterPoint;
			localNodes[54] = midPoint;
			localNodes[55] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 24
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[36], localNodes[37]);
			localNodes[56] = quarterPoint;
			localNodes[57] = midPoint;
			localNodes[58] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 25
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[37], localNodes[38]);
			localNodes[59] = quarterPoint;
			localNodes[60] = midPoint;
			localNodes[61] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 26
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[38], localNodes[39]);
			localNodes[62] = quarterPoint;
			localNodes[63] = midPoint;
			localNodes[64] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 27
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[39], localNodes[36]);
			localNodes[65] = quarterPoint;
			localNodes[66] = midPoint;
			localNodes[67] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 28
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[40], localNodes[41]);
			localNodes[68] = quarterPoint;
			localNodes[69] = midPoint;
			localNodes[70] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 29
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[41], localNodes[42]);
			localNodes[71] = quarterPoint;
			localNodes[72] = midPoint;
			localNodes[73] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 30
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[42], localNodes[43]);
			localNodes[74] = quarterPoint;
			localNodes[75] = midPoint;
			localNodes[76] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 31
			this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[43], localNodes[40]);
			localNodes[77] = quarterPoint;
			localNodes[78] = midPoint;
			localNodes[79] = threeQuarterPoint;
			this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 32
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[19], localNodes[11]);
		    this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 33
		    this->computeMidPoint(midPoint, localNodes[10], localNodes[14]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 34
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[13], localNodes[17]);
		    this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 35
		    this->computeMidPoint(midPoint, localNodes[16], localNodes[8]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 36
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[31], localNodes[23]);
		    this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 37
		    this->computeMidPoint(midPoint, localNodes[22], localNodes[26]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 38
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[25], localNodes[29]);
		    this->insertPointWithCheck(quarterPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithCheck(threeQuarterPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 39
		    this->computeMidPoint(midPoint, localNodes[28], localNodes[20]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 40
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[55], localNodes[47]);
		    this->insertPointWithoutCheck(quarterPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(threeQuarterPoint, newConnectivity, newPoints);

		    // Case 41
		    this->computeMidPoint(midPoint, localNodes[46], localNodes[50]);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);

		    // Case 42
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[49], localNodes[53]);
		    this->insertPointWithoutCheck(quarterPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(threeQuarterPoint, newConnectivity, newPoints);

		    // Case 43
		    this->computeMidPoint(midPoint, localNodes[52], localNodes[44]);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);

		    // Case 44
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[67], localNodes[59]);
		    this->insertPointWithoutCheck(quarterPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(threeQuarterPoint, newConnectivity, newPoints);

		    // Case 45
		    this->computeMidPoint(midPoint, localNodes[62], localNodes[58]);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);

		    // Case 46
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[61], localNodes[65]);
		    this->insertPointWithoutCheck(quarterPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(threeQuarterPoint, newConnectivity, newPoints);

		    // Case 47
		    this->computeMidPoint(midPoint, localNodes[56], localNodes[64]);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);

		    // Case 48
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[79], localNodes[71]);
		    this->insertPointWithoutCheck(quarterPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(threeQuarterPoint, newConnectivity, newPoints);

		    // Case 49
		    this->computeMidPoint(midPoint, localNodes[70], localNodes[74]);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);

		    // Case 50
		    this->computeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[73], localNodes[77]);
		    this->insertPointWithoutCheck(quarterPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(threeQuarterPoint, newConnectivity, newPoints);

		    // Case 51
		    this->computeMidPoint(midPoint, localNodes[76], localNodes[68]);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);

		    // Case 52
		    this->computeMidPoint(midPoint, localNodes[12], localNodes[18]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 53
		    this->computeMidPoint(midPoint, localNodes[24], localNodes[30]);
		    this->insertPointWithCheck(midPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 54
		    this->computeMidPoint(midPoint, localNodes[48], localNodes[54]);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);

		    // Case 55
		    this->computeMidPoint(midPoint, localNodes[60], localNodes[66]);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);

		    // Case 56
		    this->computeMidPoint(midPoint, localNodes[72], localNodes[78]);
		    this->insertPointWithoutCheck(midPoint, newConnectivity, newPoints);
		}
		return toReturn;
	}
};

class XdmfTopologyConverter::XdmfTopologyConverterImpl::Hexahedron_64ToHexahedron : public XdmfTopologyConverter::XdmfTopologyConverterImpl {
public:

	Hexahedron_64ToHexahedron()
	{
	}

	boost::shared_ptr<XdmfGrid> convert(const boost::shared_ptr<XdmfGrid> gridToConvert) const
	{
		boost::shared_ptr<XdmfGrid> toReturn = XdmfGrid::New();
		toReturn->setName(gridToConvert->getName());

		toReturn->setGeometry(gridToConvert->getGeometry());
		toReturn->getTopology()->setType(XdmfTopologyType::Hexahedron());

		boost::shared_ptr<XdmfArray> oldConnectivity = gridToConvert->getTopology()->getArray();
		boost::shared_ptr<XdmfArray> newConnectivity = toReturn->getTopology()->getArray();
		newConnectivity->initialize(oldConnectivity->getType());
		newConnectivity->resize(216 * gridToConvert->getTopology()->getNumberElements(), 0);

		if(!oldConnectivity->isInitialized())
		{
			oldConnectivity->read();
		}

		for(unsigned int i=0; i<gridToConvert->getTopology()->getNumberElements(); ++i)
		{
			const unsigned int startIndex = 216 * i;
			const unsigned int valuesStartIndex = 64 * i;
			newConnectivity->copyValues(startIndex + 0, oldConnectivity, 0 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 1, oldConnectivity, 8 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 2, oldConnectivity, 48 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 3, oldConnectivity, 15 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 4, oldConnectivity, 24 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 5, oldConnectivity, 36 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 6, oldConnectivity, 56 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 7, oldConnectivity, 33 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 8, oldConnectivity, 8 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 9, oldConnectivity, 9 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 10, oldConnectivity, 49 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 11, oldConnectivity, 48 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 12, oldConnectivity, 36 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 13, oldConnectivity, 37 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 14, oldConnectivity, 57 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 15, oldConnectivity, 56 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 16, oldConnectivity, 9 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 17, oldConnectivity, 1 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 18, oldConnectivity, 10 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 19, oldConnectivity, 49 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 20, oldConnectivity, 37 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 21, oldConnectivity, 25 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 22, oldConnectivity, 34 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 23, oldConnectivity, 57 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 24, oldConnectivity, 15 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 25, oldConnectivity, 48 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 26, oldConnectivity, 51 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 27, oldConnectivity, 14 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 28, oldConnectivity, 33 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 29, oldConnectivity, 56 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 30, oldConnectivity, 59 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 31, oldConnectivity, 32 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 32, oldConnectivity, 48 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 33, oldConnectivity, 49 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 34, oldConnectivity, 50 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 35, oldConnectivity, 51 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 36, oldConnectivity, 56 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 37, oldConnectivity, 57 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 38, oldConnectivity, 58 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 39, oldConnectivity, 59 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 40, oldConnectivity, 49 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 41, oldConnectivity, 10 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 42, oldConnectivity, 11 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 43, oldConnectivity, 50 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 44, oldConnectivity, 57 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 45, oldConnectivity, 34 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 46, oldConnectivity, 35 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 47, oldConnectivity, 58 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 48, oldConnectivity, 14 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 49, oldConnectivity, 51 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 50, oldConnectivity, 13 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 51, oldConnectivity, 3 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 52, oldConnectivity, 32 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 53, oldConnectivity, 59 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 54, oldConnectivity, 39 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 55, oldConnectivity, 27 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 56, oldConnectivity, 51 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 57, oldConnectivity, 50 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 58, oldConnectivity, 12 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 59, oldConnectivity, 13 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 60, oldConnectivity, 59 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 61, oldConnectivity, 58 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 62, oldConnectivity, 38 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 63, oldConnectivity, 39 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 64, oldConnectivity, 50 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 65, oldConnectivity, 11 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 66, oldConnectivity, 2 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 67, oldConnectivity, 12 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 68, oldConnectivity, 58 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 69, oldConnectivity, 35 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 70, oldConnectivity, 26 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 71, oldConnectivity, 38 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 72, oldConnectivity, 24 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 73, oldConnectivity, 36 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 74, oldConnectivity, 56 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 75, oldConnectivity, 33 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 76, oldConnectivity, 28 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 77, oldConnectivity, 44 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 78, oldConnectivity, 60 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 79, oldConnectivity, 41 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 80, oldConnectivity, 36 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 81, oldConnectivity, 37 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 82, oldConnectivity, 57 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 83, oldConnectivity, 56 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 84, oldConnectivity, 44 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 85, oldConnectivity, 45 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 86, oldConnectivity, 61 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 87, oldConnectivity, 60 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 88, oldConnectivity, 37 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 89, oldConnectivity, 25 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 90, oldConnectivity, 34 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 91, oldConnectivity, 57 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 92, oldConnectivity, 45 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 93, oldConnectivity, 29 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 94, oldConnectivity, 42 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 95, oldConnectivity, 61 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 96, oldConnectivity, 33 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 97, oldConnectivity, 56 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 98, oldConnectivity, 59 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 99, oldConnectivity, 32 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 100, oldConnectivity, 41 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 101, oldConnectivity, 60 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 102, oldConnectivity, 63 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 103, oldConnectivity, 40 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 104, oldConnectivity, 56 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 105, oldConnectivity, 57 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 106, oldConnectivity, 58 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 107, oldConnectivity, 59 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 108, oldConnectivity, 60 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 109, oldConnectivity, 61 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 110, oldConnectivity, 62 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 111, oldConnectivity, 63 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 112, oldConnectivity, 57 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 113, oldConnectivity, 34 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 114, oldConnectivity, 35 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 115, oldConnectivity, 58 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 116, oldConnectivity, 61 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 117, oldConnectivity, 42 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 118, oldConnectivity, 43 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 119, oldConnectivity, 62 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 120, oldConnectivity, 32 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 121, oldConnectivity, 59 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 122, oldConnectivity, 39 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 123, oldConnectivity, 27 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 124, oldConnectivity, 40 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 125, oldConnectivity, 63 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 126, oldConnectivity, 47 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 127, oldConnectivity, 31 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 128, oldConnectivity, 59 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 129, oldConnectivity, 58 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 130, oldConnectivity, 38 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 131, oldConnectivity, 39 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 132, oldConnectivity, 63 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 133, oldConnectivity, 62 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 134, oldConnectivity, 46 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 135, oldConnectivity, 47 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 136, oldConnectivity, 58 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 137, oldConnectivity, 35 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 138, oldConnectivity, 26 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 139, oldConnectivity, 38 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 140, oldConnectivity, 62 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 141, oldConnectivity, 43 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 142, oldConnectivity, 30 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 143, oldConnectivity, 46 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 144, oldConnectivity, 28 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 145, oldConnectivity, 44 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 146, oldConnectivity, 60 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 147, oldConnectivity, 41 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 148, oldConnectivity, 4 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 149, oldConnectivity, 16 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 150, oldConnectivity, 52 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 151, oldConnectivity, 23 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 152, oldConnectivity, 44 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 153, oldConnectivity, 45 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 154, oldConnectivity, 61 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 155, oldConnectivity, 60 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 156, oldConnectivity, 16 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 157, oldConnectivity, 17 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 158, oldConnectivity, 53 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 159, oldConnectivity, 52 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 160, oldConnectivity, 45 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 161, oldConnectivity, 29 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 162, oldConnectivity, 42 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 163, oldConnectivity, 61 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 164, oldConnectivity, 17 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 165, oldConnectivity, 5 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 166, oldConnectivity, 18 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 167, oldConnectivity, 53 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 168, oldConnectivity, 41 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 169, oldConnectivity, 60 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 170, oldConnectivity, 63 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 171, oldConnectivity, 40 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 172, oldConnectivity, 23 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 173, oldConnectivity, 52 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 174, oldConnectivity, 55 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 175, oldConnectivity, 22 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 176, oldConnectivity, 60 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 177, oldConnectivity, 61 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 178, oldConnectivity, 62 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 179, oldConnectivity, 63 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 180, oldConnectivity, 52 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 181, oldConnectivity, 53 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 182, oldConnectivity, 54 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 183, oldConnectivity, 55 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 184, oldConnectivity, 61 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 185, oldConnectivity, 42 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 186, oldConnectivity, 43 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 187, oldConnectivity, 62 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 188, oldConnectivity, 53 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 189, oldConnectivity, 18 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 190, oldConnectivity, 19 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 191, oldConnectivity, 54 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 192, oldConnectivity, 40 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 193, oldConnectivity, 63 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 194, oldConnectivity, 47 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 195, oldConnectivity, 31 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 196, oldConnectivity, 22 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 197, oldConnectivity, 55 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 198, oldConnectivity, 21 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 199, oldConnectivity, 7 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 200, oldConnectivity, 63 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 201, oldConnectivity, 62 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 202, oldConnectivity, 46 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 203, oldConnectivity, 47 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 204, oldConnectivity, 55 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 205, oldConnectivity, 54 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 206, oldConnectivity, 20 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 207, oldConnectivity, 21 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 208, oldConnectivity, 62 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 209, oldConnectivity, 43 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 210, oldConnectivity, 30 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 211, oldConnectivity, 46 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 212, oldConnectivity, 54 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 213, oldConnectivity, 19 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 214, oldConnectivity, 6 + valuesStartIndex, 1);
			newConnectivity->copyValues(startIndex + 215, oldConnectivity, 20 + valuesStartIndex, 1);
		}

		for(unsigned int i=0; i<gridToConvert->getNumberAttributes(); ++i)
		{
			boost::shared_ptr<XdmfAttribute> currAttribute = gridToConvert->getAttribute(i);
			if(currAttribute->getCenter() == XdmfAttributeCenter::Node())
			{
				toReturn->insert(currAttribute);
			}
			else if(currAttribute->getCenter() == XdmfAttributeCenter::Cell())
			{
				if(!currAttribute->getArray()->isInitialized())
				{
					currAttribute->getArray()->read();
				}

				boost::shared_ptr<XdmfAttribute> newAttribute = XdmfAttribute::New();
				newAttribute->setName(currAttribute->getName());
				newAttribute->setType(currAttribute->getType());
				newAttribute->setCenter(currAttribute->getCenter());

				boost::shared_ptr<XdmfArray> vals = newAttribute->getArray();
				vals->initialize(currAttribute->getArray()->getType());
				vals->resize(currAttribute->getArray()->getSize() * 27, 0);
				for(unsigned int j=0; j<currAttribute->getArray()->getSize(); ++j)
				{
					vals->copyValues(j * 27, currAttribute->getArray(), j, 27, 1, 0);
				}
				toReturn->insert(newAttribute);
			}
		}
		return toReturn;
	}
};

XdmfTopologyConverter::XdmfTopologyConverter()
{
}

XdmfTopologyConverter::~XdmfTopologyConverter()
{
}

boost::shared_ptr<XdmfGrid> XdmfTopologyConverter::convert(const boost::shared_ptr<XdmfGrid> gridToConvert, const boost::shared_ptr<const XdmfTopologyType> topologyType) const
{
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
			return converter.convert(gridToConvert);
		}
		else if(topologyType == XdmfTopologyType::Hexahedron_125())
		{
			XdmfTopologyConverterImpl::HexahedronToHexahedron_125 converter;
			return converter.convert(gridToConvert);
		}
	}
	else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_64())
	{
		if(topologyType == XdmfTopologyType::Hexahedron())
		{
			XdmfTopologyConverterImpl::Hexahedron_64ToHexahedron converter;
			return converter.convert(gridToConvert);
		}
	}
	assert(false);
}
