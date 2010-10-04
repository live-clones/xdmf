/*
 * XdmfTopologyType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include <sstream>
#include "XdmfTopologyType.hpp"

// Supported XdmfTopologyTypes
boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::NoTopologyType()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(0, "NoTopology", NoCellType));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Polyvertex()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(1, "Polyvertex", Linear));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Polyline()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(1, "Polyline", Linear));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Polygon(const unsigned int nodesPerElement)
{
	static std::map<unsigned int, boost::shared_ptr<const XdmfTopologyType> > previousTypes;
	std::map<unsigned int, boost::shared_ptr<const XdmfTopologyType> >::const_iterator type = previousTypes.find(nodesPerElement);
	if(type != previousTypes.end())
	{
		return type->second;
	}
	boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(nodesPerElement, "Polygon", Linear));
	previousTypes[nodesPerElement] = p;
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Triangle()
{
	boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(3, "Triangle", Linear));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Quadrilateral()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(4, "Quadrilateral", Linear));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Tetrahedron()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(4, "Tetrahedron", Linear));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Pyramid()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(5, "Pyramid", Linear));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Wedge()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(6, "Wedge", Linear));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Hexahedron()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(8, "Hexahedron", Linear));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Edge_3()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(3, "Edge_3", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Triangle_6()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(6, "Triangle_6", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Quadrilateral_8()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(8, "Quadrilateral_8", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Quadrilateral_9()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(9, "Quadrilateral_9", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Tetrahedron_10()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(10, "Tetrahedron_10", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Pyramid_13()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(13, "Pyramid_13", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Wedge_15()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(15, "Wedge_15", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Wedge_18()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(18, "Wedge_18", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Hexahedron_20()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(20, "Hexahedron_20", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Hexahedron_24()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(24, "Hexahedron_24", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Hexahedron_27()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(27, "Hexahedron_27", Quadratic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Hexahedron_64()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(64, "Hexahedron_64", Cubic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Hexahedron_64_GLL()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(64, "Hexahedron_64_GLL", Cubic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Hexahedron_125()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(125, "Hexahedron_125", Quartic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Hexahedron_125_GLL()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(125, "Hexahedron_125_GLL", Quartic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Mixed()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(0, "Mixed", Arbitrary));
	return p;
}

/*
boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::TwoDSMesh()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(1, "2DSMesh", Structured));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::TwoDRectMesh()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(1, "2DRectMesh", Structured));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::TwoDCoRectMesh()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(1, "2DCoRectMesh", Structured));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::ThreeDSMesh()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(1, "3DSMesh", Structured));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::ThreeDRectMesh()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(1, "3DRectMesh", Structured));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::ThreeDCoRectMesh()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(1, "3DCoRectMesh", Structured));
	return p;
}*/

XdmfTopologyType::XdmfTopologyType(const unsigned int nodesPerElement, const std::string & name, const CellType cellType) :
	mCellType(cellType),
	mName(name),
	mNodesPerElement(nodesPerElement)
{
}

XdmfTopologyType::~XdmfTopologyType()
{
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::New(const std::map<std::string, std::string> & itemProperties)
{
	std::map<std::string, std::string>::const_iterator type = itemProperties.find("Type");
	if(type == itemProperties.end())
	{
		type = itemProperties.find("TopologyType");
	}

	std::map<std::string, std::string>::const_iterator nodesPerElement = itemProperties.find("NodesPerElement");
	if(type != itemProperties.end())
	{
		std::string typeVal = type->second;
		std::transform(typeVal.begin(), typeVal.end(), typeVal.begin(), (int(*)(int))std::toupper);
		if(typeVal.compare("NOTOPOLOGY") == 0)
		{
			return NoTopologyType();
		}
		else if(typeVal.compare("POLYVERTEX") == 0)
		{
			return Polyvertex();
		}
		else if(typeVal.compare("POLYLINE") == 0)
		{
			return Polyline();
		}
		else if(typeVal.compare("POLYGON") == 0)
		{
			if(nodesPerElement != itemProperties.end())
			{
				return Polygon(atoi(nodesPerElement->second.c_str()));
			}
			assert(false);
		}
		else if(typeVal.compare("TRIANGLE") == 0)
		{
			return Triangle();
		}
		else if(typeVal.compare("QUADRILATERAL") == 0)
		{
			return Quadrilateral();
		}
		else if(typeVal.compare("TETRAHEDRON") == 0)
		{
			return Tetrahedron();
		}
		else if(typeVal.compare("PYRAMID") == 0)
		{
			return Pyramid();
		}
		else if(typeVal.compare("WEDGE") == 0)
		{
			return Wedge();
		}
		else if(typeVal.compare("HEXAHEDRON") == 0)
		{
			return Hexahedron();
		}
		else if(typeVal.compare("EDGE_3") == 0)
		{
			return Edge_3();
		}
		else if(typeVal.compare("TRIANGLE_6") == 0)
		{
			return Triangle_6();
		}
		else if(typeVal.compare("QUADRILATERAL_8") == 0)
		{
			return Quadrilateral_8();
		}
		else if(typeVal.compare("TETRAHEDRON_10") == 0)
		{
			return Tetrahedron_10();
		}
		else if(typeVal.compare("PYRAMID_13") == 0)
		{
			return Pyramid_13();
		}
		else if(typeVal.compare("WEDGE_15") == 0)
		{
			return Wedge_15();
		}
		else if(typeVal.compare("HEXAHEDRON_20") == 0)
		{
			return Hexahedron_20();
		}
		else if(typeVal.compare("HEXAHEDRON_24") == 0)
		{
			return Hexahedron_24();
		}
		else if(typeVal.compare("HEXAHEDRON_27") == 0)
		{
			return Hexahedron_27();
		}
		else if(typeVal.compare("HEXAHEDRON_64") == 0)
		{
			return Hexahedron_64();
		}
		else if(typeVal.compare("MIXED") == 0)
		{
			return Mixed();
		}
		/*else if(typeVal.compare("2DSMESH") == 0)
		{
			return TwoDSMesh();
		}
		else if(typeVal.compare("2DRECTMESH") == 0)
		{
			return TwoDRectMesh();
		}
		else if(typeVal.compare("2DCORECTMESH") == 0 || typeVal.compare("3DCORECTMESH") == 0)
		{
			// Special case --- Regular Grid
			boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(0, "REGULAR", Structured));
			return p;
		}
		else if(typeVal.compare("3DSMESH") == 0)
		{
			return ThreeDSMesh();
		}
		else if(typeVal.compare("3DRECTMESH") == 0)
		{
			return ThreeDRectMesh();
		}*/
		else
		{
			assert(false);
		}
	}
	assert(false);
}

bool XdmfTopologyType::operator==(const XdmfTopologyType& topologyType) const
{
	return mNodesPerElement == topologyType.mNodesPerElement && mName.compare(topologyType.mName) == 0 && mCellType == topologyType.mCellType;
}

bool XdmfTopologyType::operator!=(const XdmfTopologyType& topologyType) const
{
	return !this->operator==(topologyType);
}

bool XdmfTopologyType::IsEqual(boost::shared_ptr<XdmfTopologyType> topologyType)
{
        if(topologyType == NULL) return false;
        if(this == topologyType.get()
	   &&   mNodesPerElement == topologyType->mNodesPerElement
	   &&	mCellType == topologyType->mCellType
           &&   mName == topologyType->mName
        ) return true;
        return false;
}

XdmfTopologyType::CellType XdmfTopologyType::getCellType() const
{
	return mCellType;
}

std::string XdmfTopologyType::getName() const
{
	return mName;
}

unsigned int XdmfTopologyType::getNodesPerElement() const
{
	return mNodesPerElement;
}

void XdmfTopologyType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["Type"] = this->getName();
	if(mName.compare("Polygon") == 0)
	{
		std::stringstream nodesPerElement;
		nodesPerElement << mNodesPerElement;
		collectedProperties["NodesPerElement"] = nodesPerElement.str();
	}
}
