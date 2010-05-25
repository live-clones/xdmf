/*
 * XdmfTopologyType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfTopologyType.hpp"

// Supported XdmfTopologyTypes
XdmfTopologyType XdmfTopologyType::NoTopologyType()
{
	return XdmfTopologyType(0, "NoTopology", NoCellType);
}

XdmfTopologyType XdmfTopologyType::Polyvertex()
{
	return XdmfTopologyType(1, "Polyvertex", Linear);
}

XdmfTopologyType XdmfTopologyType::Polyline()
{
	return XdmfTopologyType(1, "Polyline", Linear);
}

XdmfTopologyType XdmfTopologyType::Polygon(const int nodesPerElement)
{
	return XdmfTopologyType(nodesPerElement, "Polygon", Linear);
}

XdmfTopologyType XdmfTopologyType::Triangle()
{
	return XdmfTopologyType(3, "Triangle", Linear);
}

XdmfTopologyType XdmfTopologyType::Quadrilateral()
{
	return XdmfTopologyType(4, "Quadrilateral", Linear);
}

XdmfTopologyType XdmfTopologyType::Tetrahedron()
{
	return XdmfTopologyType(4, "Tetrahedron", Linear);
}

XdmfTopologyType XdmfTopologyType::Pyramid()
{
	return XdmfTopologyType(5, "Pyramid", Linear);
}

XdmfTopologyType XdmfTopologyType::Wedge()
{
	return XdmfTopologyType(6, "Wedge", Linear);
}

XdmfTopologyType XdmfTopologyType::Hexahedron()
{
	return XdmfTopologyType(8, "Hexahedron", Linear);
}

XdmfTopologyType XdmfTopologyType::Edge_3()
{
	return XdmfTopologyType(3, "Edge_3", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Triangle_6()
{
	return XdmfTopologyType(6, "Triangle_6", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Quadrilateral_8()
{
	return XdmfTopologyType(8, "Quadrilateral_8", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Tetrahedron_10()
{
	return XdmfTopologyType(10, "Tetrahedron_10", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Pyramid_13()
{
	return XdmfTopologyType(13, "Pyramid_13", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Wedge_15()
{
	return XdmfTopologyType(15, "Wedge_15", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Hexahedron_20()
{
	return XdmfTopologyType(20, "Hexahedron_20", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Hexahedron_24()
{
	return XdmfTopologyType(20, "Hexahedron_24", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Hexahedron_27()
{
	return XdmfTopologyType(20, "Hexahedron_27", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Hexahedron_64()
{
	return XdmfTopologyType(20, "Hexahedron_64", Cubic);
}

XdmfTopologyType XdmfTopologyType::Mixed()
{
	return XdmfTopologyType(0, "Mixed", Arbitrary);
}

XdmfTopologyType XdmfTopologyType::TwoDSMesh()
{
	return XdmfTopologyType(1, "2DSMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::TwoDRectMesh()
{
	return XdmfTopologyType(1, "2DRectMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::TwoDCoRectMesh()
{
	return XdmfTopologyType(1, "2DCoRectMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::ThreeDSMesh()
{
	return XdmfTopologyType(1, "3DSMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::ThreeDRectMesh()
{
	return XdmfTopologyType(1, "3DRectMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::ThreeDCoRectMesh()
{
	return XdmfTopologyType(1, "3DCoRectMesh", Structured);
}

XdmfTopologyType::XdmfTopologyType(const int& nodesPerElement, const std::string& name, const CellType& cellType) :
	mNodesPerElement(nodesPerElement),
	mName(name),
	mCellType(cellType)
{
}

XdmfTopologyType::XdmfTopologyType(const XdmfTopologyType& topologyType):
	mNodesPerElement(topologyType.mNodesPerElement),
	mName(topologyType.mName),
	mCellType(topologyType.mCellType)
{
}

XdmfTopologyType XdmfTopologyType::New(const std::map<std::string, std::string> & itemProperties)
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

XdmfTopologyType & XdmfTopologyType::operator=(const XdmfTopologyType& topologyType)
{
	if(this != &topologyType)
	{
		mNodesPerElement = topologyType.mNodesPerElement;
		mName = topologyType.mName;
		mCellType = topologyType.mCellType;
	}
	return *this;
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
	collectedProperties["TopologyType"] = this->mName;
	if(mName.compare("Polygon") == 0)
	{
		collectedProperties["NodesPerElement"] = this->mNodesPerElement;
	}
}
