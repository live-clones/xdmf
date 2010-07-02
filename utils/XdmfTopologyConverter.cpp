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

XdmfTopologyConverter::XdmfTopologyConverter()
{
}

XdmfTopologyConverter::~XdmfTopologyConverter()
{
}

struct XdmfTopologyConverter::PointComparison {
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

class XdmfTopologyConverter::HexahedronToHexahedron64 {
public:

	HexahedronToHexahedron64()
	{
	}

	inline void computeInteriorPoints(std::vector<double> & leftPoint, std::vector<double> & rightPoint, std::vector<double> & point1, std::vector<double> & point2) const
	{
	  leftPoint[0] = (1.0/3.0)*(point2[0] + 2*point1[0]);
	  leftPoint[1] = (1.0/3.0)*(point2[1] + 2*point1[1]);
	  leftPoint[2] = (1.0/3.0)*(point2[2] + 2*point1[2]);

	  rightPoint[0] = (1.0/3.0)*(2*point2[0] + point1[0]);
	  rightPoint[1] = (1.0/3.0)*(2*point2[1] + point1[1]);
	  rightPoint[2] = (1.0/3.0)*(2*point2[2] + point1[2]);
	}

	inline void computeLeftPoint(std::vector<double> & leftPoint, std::vector<double> & point1, std::vector<double> & point2) const
	{
	  leftPoint[0] = (1.0/3.0)*(point2[0] + 2*point1[0]);
	  leftPoint[1] = (1.0/3.0)*(point2[1] + 2*point1[1]);
	  leftPoint[2] = (1.0/3.0)*(point2[2] + 2*point1[2]);
	}

	inline void computeRightPoint(std::vector<double> & rightPoint, std::vector<double> & point1, std::vector<double> & point2) const
	{
	  rightPoint[0] = (1.0/3.0)*(2*point2[0] + point1[0]);
	  rightPoint[1] = (1.0/3.0)*(2*point2[1] + point1[1]);
	  rightPoint[2] = (1.0/3.0)*(2*point2[2] + point1[2]);
	}

	inline void insertPointWithoutCheck(std::vector<double> & newPoint, const boost::shared_ptr<XdmfArray> & newConnectivity, const boost::shared_ptr<XdmfArray> & newPoints) const
	{
	  newConnectivity->pushBack(newPoints->getSize() / 3);
	  newPoints->pushBack(newPoint[0]);
	  newPoints->pushBack(newPoint[1]);
	  newPoints->pushBack(newPoint[2]);
	}

	inline void insertPointWithCheck(std::vector<double> & newPoint, std::map<std::vector<double>, unsigned int, PointComparison> & coordToIdMap, const boost::shared_ptr<XdmfArray> & newConnectivity, const boost::shared_ptr<XdmfArray> & newPoints) const
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

		for(unsigned int i=0; i<gridToConvert->getTopology()->getNumberElements(); ++i)
		{
			// Fill localNodes with original coordinate information.
			std::vector<std::vector<double> > localNodes(8, std::vector<double>(3));
			localNodes.reserve(44);
			for(int j=0; j<8; ++j)
			{
				gridToConvert->getGeometry()->getArray()->getValuesCopy(gridToConvert->getTopology()->getArray()->getValueCopy<unsigned int>(8*i + j) * 3, &localNodes[j][0], 3);
			}

			// Add old connectivity information to newConnectivity.
			newConnectivity->resize(newConnectivity->getSize() + 8, 0);
			newConnectivity->copyValues(64*i, gridToConvert->getTopology()->getArray(), 8*i, 8);

			// Case 0
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[0], localNodes[1]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 1
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[1], localNodes[2]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 2
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[2], localNodes[3]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 3
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[3], localNodes[0]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 4
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[4], localNodes[5]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 5
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[5], localNodes[6]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 6
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[6], localNodes[7]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 7
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[7], localNodes[4]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 8
			computeLeftPoint(leftPoint, localNodes[0], localNodes[4]);
			localNodes.push_back(leftPoint);
			insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 9
			this->computeLeftPoint(leftPoint, localNodes[1], localNodes[5]);
			localNodes.push_back(leftPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 10
			this->computeLeftPoint(leftPoint, localNodes[2], localNodes[6]);
			localNodes.push_back(leftPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 11
			this->computeLeftPoint(leftPoint, localNodes[3], localNodes[7]);
			localNodes.push_back(leftPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 12
			this->computeRightPoint(leftPoint, localNodes[0], localNodes[4]);
			localNodes.push_back(leftPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			 // Case 13
			this->computeRightPoint(leftPoint, localNodes[1], localNodes[5]);
			localNodes.push_back(leftPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 14
			this->computeRightPoint(leftPoint, localNodes[2], localNodes[6]);
			localNodes.push_back(leftPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 15
			this->computeRightPoint(leftPoint, localNodes[3], localNodes[7]);
			localNodes.push_back(leftPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 16
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[27], localNodes[24]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 17
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[25], localNodes[26]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 18
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[24], localNodes[25]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 19
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[26], localNodes[27]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 20
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[31], localNodes[28]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
			this->insertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
			this->insertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

			// Case 21
			this->computeInteriorPoints(leftPoint, rightPoint, localNodes[29], localNodes[30]);
			localNodes.push_back(leftPoint);
			localNodes.push_back(rightPoint);
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

class XdmfTopologyConverter::Hexahedron64ToHexahedron {
public:

	Hexahedron64ToHexahedron()
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

		for(unsigned int i=0; i<gridToConvert->getNumberOfAttributes(); ++i)
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
			HexahedronToHexahedron64 converter;
			return converter.convert(gridToConvert);
		}
	}
	else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_64())
	{
		if(topologyType == XdmfTopologyType::Hexahedron())
		{
			Hexahedron64ToHexahedron converter;
			return converter.convert(gridToConvert);
		}
	}
	assert(false);
}
