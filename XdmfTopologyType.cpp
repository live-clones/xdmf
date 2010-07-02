/*
 * XdmfTopologyType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include <map>
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

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Hexahedron_125()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(125, "Hexahedron_125", Quartic));
	return p;
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::Mixed()
{
	static boost::shared_ptr<const XdmfTopologyType> p(new XdmfTopologyType(0, "Mixed", Arbitrary));
	return p;
}

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
}

XdmfTopologyType::XdmfTopologyType(const int& nodesPerElement, const std::string& name, const CellType& cellType) :
	mNodesPerElement(nodesPerElement),
	mName(name),
	mCellType(cellType)
{
}

XdmfTopologyType::~XdmfTopologyType()
{
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopologyType::New(const std::map<std::string, std::string> & itemProperties)
{
	std::map<std::string, std::string>::const_iterator type = itemProperties.find("TopologyType");
	std::map<std::string, std::string>::const_iterator nodesPerElement = itemProperties.find("NodesPerElement");
	if(type != itemProperties.end())
	{
		const std::string typeVal = type->second;
		if(typeVal.compare("NoTopology") == 0)
		{
			return NoTopologyType();
		}
		else if(typeVal.compare("Polyvertex") == 0)
		{
			return Polyvertex();
		}
		else if(typeVal.compare("Polyline") == 0)
		{
			return Polyline();
		}
		else if(typeVal.compare("Polygon") == 0)
		{
			if(nodesPerElement != itemProperties.end())
			{
				return Polygon(atoi(nodesPerElement->second.c_str()));
			}
			assert(false);
		}
		else if(typeVal.compare("Triangle") == 0)
		{
			return Triangle();
		}
		else if(typeVal.compare("Quadrilateral") == 0)
		{
			return Quadrilateral();
		}
		else if(typeVal.compare("Tetrahedron") == 0)
		{
			return Tetrahedron();
		}
		else if(typeVal.compare("Pyramid") == 0)
		{
			return Pyramid();
		}
		else if(typeVal.compare("Wedge") == 0)
		{
			return Wedge();
		}
		else if(typeVal.compare("Hexahedron") == 0)
		{
			return Hexahedron();
		}
		else if(typeVal.compare("Edge_3") == 0)
		{
			return Edge_3();
		}
		else if(typeVal.compare("Triangle_6") == 0)
		{
			return Triangle_6();
		}
		else if(typeVal.compare("Quadrilateral_8") == 0)
		{
			return Quadrilateral_8();
		}
		else if(typeVal.compare("Tetrahedron_10") == 0)
		{
			return Tetrahedron_10();
		}
		else if(typeVal.compare("Pyramid_13") == 0)
		{
			return Pyramid_13();
		}
		else if(typeVal.compare("Wedge_15") == 0)
		{
			return Wedge_15();
		}
		else if(typeVal.compare("Hexahedron_20") == 0)
		{
			return Hexahedron_20();
		}
		else if(typeVal.compare("Hexahedron_24") == 0)
		{
			return Hexahedron_24();
		}
		else if(typeVal.compare("Hexahedron_27") == 0)
		{
			return Hexahedron_27();
		}
		else if(typeVal.compare("Hexahedron_64") == 0)
		{
			return Hexahedron_64();
		}
		else if(typeVal.compare("Mixed") == 0)
		{
			return Mixed();
		}
		else if(typeVal.compare("2DSMesh") == 0)
		{
			return TwoDSMesh();
		}
		else if(typeVal.compare("2DRectMesh") == 0)
		{
			return TwoDRectMesh();
		}
		else if(typeVal.compare("2DCoRectMesh") == 0)
		{
			return TwoDCoRectMesh();
		}
		else if(typeVal.compare("3DSMesh") == 0)
		{
			return ThreeDSMesh();
		}
		else if(typeVal.compare("3DRectMesh") == 0)
		{
			return ThreeDRectMesh();
		}
		else if(typeVal.compare("3DCoRectMesh") == 0)
		{
			return ThreeDCoRectMesh();
		}
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

XdmfTopologyType::CellType XdmfTopologyType::getCellType() const
{
	return mCellType;
}

unsigned int XdmfTopologyType::getNodesPerElement() const
{
	return mNodesPerElement;
}

void XdmfTopologyType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["TopologyType"] = mName;
	if(mName.compare("Polygon") == 0)
	{
		std::stringstream nodesPerElement;
		nodesPerElement << mNodesPerElement;
		collectedProperties["NodesPerElement"] = nodesPerElement.str();
	}
}
