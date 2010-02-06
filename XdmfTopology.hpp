/*
 * XdmfTopology.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFTOPOLOGY_HPP_
#define XDMFTOPOLOGY_HPP_

#include "XdmfObject.hpp"

class XdmfTopologyType {

public:

	const int numberVertices;
	const std::string name;

	// Supported Xdmf Topology Type
	static const XdmfTopologyType NoTopology;
	static const XdmfTopologyType Polyvertex;
	static const XdmfTopologyType Polyline;
	static const XdmfTopologyType Polygon;
	static const XdmfTopologyType Triangle;
	static const XdmfTopologyType Quadrilateral;
	static const XdmfTopologyType Tetrahedron;
	static const XdmfTopologyType Pyramid;
	static const XdmfTopologyType Wedge;
	static const XdmfTopologyType Hexahedron;
	static const XdmfTopologyType Edge_3;
	static const XdmfTopologyType Triangle_6;
	static const XdmfTopologyType Quadrilateral_8;
	static const XdmfTopologyType Tetrahedron_10;
	static const XdmfTopologyType Pyramid_13;
	static const XdmfTopologyType Wedge_15;
	static const XdmfTopologyType Hexahedron_20;
	static const XdmfTopologyType Mixed;
	static const XdmfTopologyType TwoDSMesh;
	static const XdmfTopologyType TwoDRectMesh;
	static const XdmfTopologyType TwoDCoRectMesh;
	static const XdmfTopologyType ThreeDSMesh;
	static const XdmfTopologyType ThreeDRectMesh;
	static const XdmfTopologyType ThreeDCoRectMesh;

	bool operator==(const XdmfTopologyType& top) const
	{
		return (this == &top) ? true : false;
	}

	bool operator!=(const XdmfTopologyType& top) const
	{
		return (this != &top) ? true : false;
	}

protected:

	XdmfTopologyType(const int & numberVertices, const std::string & name) :
		numberVertices(numberVertices),
		name(name)
	{};

private:

	//XdmfTopologyType(const XdmfTopologyType&);  // Not implemented.
	//void operator=(const XdmfTopologyType&);  // Not implemented.

};

class XdmfTopology : public XdmfObject {

public:

	XdmfNewMacro(XdmfTopology);

	// Get the topology type for this topology
	const XdmfTopologyType & getXdmfTopologyType() const
	{
		return *mTopologyType;
	}
	// Set the topology type for this topology
	void setTopologyType(const XdmfTopologyType & topType)
	{
		mTopologyType = &topType;
	}

	const std::string getTopologyTypeAsString() const;

	virtual const std::string printSelf() const;

protected:

	XdmfTopology();
	virtual ~XdmfTopology();

private:

  XdmfTopology(const XdmfTopology&);  // Not implemented.
  void operator=(const XdmfTopology&);  // Not implemented.

  const XdmfTopologyType * mTopologyType;
};

#endif /* XDMFTOPOLOGY_HPP_ */
