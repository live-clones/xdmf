
#include <math.h>
#include "XdmfArray.hpp"
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

	inline void insertPointWithoutCheck(std::vector<double> & newPoint, std::map<std::vector<double>, unsigned int, PointComparison> & coordToIdMap, const boost::shared_ptr<XdmfArray> newConnectivity, const boost::shared_ptr<XdmfArray> newPoints) const
	{
	  unsigned int newId = newPoints->getSize() / 3;
	  newConnectivity->pushBack(newId);
	  newPoints->pushBack(newPoint[0]);
	  newPoints->pushBack(newPoint[1]);
	  newPoints->pushBack(newPoint[2]);
	}

	inline void insertPointWithCheck(std::vector<double> & newPoint, std::map<std::vector<double>, unsigned int, PointComparison> & coordToIdMap, const boost::shared_ptr<XdmfArray> newConnectivity, const boost::shared_ptr<XdmfArray> newPoints) const
	{
	  std::map<std::vector<double>, unsigned int>::const_iterator iter = coordToIdMap.find(newPoint);
	  if(iter == coordToIdMap.end())
	  {
	    // Not inserted before
	    unsigned int newId = newPoints->getSize() / 3;
	    coordToIdMap[newPoint] = newId;
	    insertPointWithoutCheck(newPoint, coordToIdMap, newConnectivity, newPoints);
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
		toReturn->getGeometry()->setGeometryType(gridToConvert->getGeometry()->getGeometryType());
		toReturn->getTopology()->setTopologyType(XdmfTopologyType::Hexahedron_64());

		boost::shared_ptr<XdmfArray> newPoints = toReturn->getGeometry()->getArray();
		newPoints->initialize(gridToConvert->getGeometry()->getArray()->getType());
		newPoints->resize(gridToConvert->getGeometry()->getArray()->getSize(), 0);
		// Copy all geometry values from old grid into new grid because we are keeping all old points.
		newPoints->copyValues(0, gridToConvert->getGeometry()->getArray(), 0, gridToConvert->getGeometry()->getArray()->getSize());

		boost::shared_ptr<XdmfArray> newConnectivity = toReturn->getTopology()->getArray();
		newConnectivity->initialize(gridToConvert->getTopology()->getArray()->getType());
		newConnectivity->reserve(64 * gridToConvert->getTopology()->getNumberElements());

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
				gridToConvert->getGeometry()->getArray()->getValuesCopy((8 * i + j) * 3, &localNodes[j][0], 3);
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
		    this->insertPointWithoutCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 29
		    this->computeInteriorPoints(leftPoint, rightPoint, localNodes[35], localNodes[32]);
		    this->insertPointWithoutCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 30
		    this->computeInteriorPoints(leftPoint, rightPoint, localNodes[41], localNodes[42]);
		    this->insertPointWithoutCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

		    // Case 31
		    this->computeInteriorPoints(leftPoint, rightPoint, localNodes[43], localNodes[40]);
		    this->insertPointWithoutCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
		    this->insertPointWithoutCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);
		}

		return toReturn;
	}
};

boost::shared_ptr<XdmfGrid> XdmfTopologyConverter::convert(const boost::shared_ptr<XdmfGrid> gridToConvert, const boost::shared_ptr<const XdmfTopologyType> topologyType) const
{
	boost::shared_ptr<const XdmfTopologyType> topologyTypeToConvert = gridToConvert->getTopology()->getTopologyType();
	if(topologyTypeToConvert == topologyType)
	{
		// No conversion necessary
		return gridToConvert;
	}

	if(gridToConvert->getGeometry()->getGeometryType() != XdmfGeometryType::XYZ())
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
	assert(false);
}
