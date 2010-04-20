#include "XdmfAttribute.hpp"
#include "XdmfArray.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfTopology.hpp"

class XdmfTestDataGenerator {
public:

	/**
	 * Number of Cells = 2
	 * Number of Points = 12
	 * Number of Attributes = 2
	 * 	Cell Attributes = 1
	 * 	Nodal Attributes = 1
	 * Total Number of Values = 66
	 */
	static boost::shared_ptr<XdmfGrid> createHexahedron()
	{
		boost::shared_ptr<XdmfGrid> grid = XdmfGrid::New();
		grid->setName("Hexahedron");

		// Set Geometry
		double points[] = {0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1, 1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1,
				0.1, 0.1, -1.1, 1.1, 0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1, -1.1, 3.1, 2.1, -2.1};
		grid->getGeometry()->setGeometryType(XdmfGeometryType::XYZ());
		grid->getGeometry()->getArray()->copyValues(0, &points[0], 36);

		// Set Topology
		int connectivity[] = {0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10};
		grid->getTopology()->setTopologyType(XdmfTopologyType::Hexahedron());
		grid->getTopology()->getArray()->copyValues(0, &connectivity[0], 16);

		// Add Node Attribute
		boost::shared_ptr<XdmfAttribute> nodalAttribute = XdmfAttribute::New();
		int nodeValues[] = {100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600, 700};
		nodalAttribute->setName("Nodal Attribute");
		nodalAttribute->setAttributeType(XdmfAttributeType::Scalar());
		nodalAttribute->setAttributeCenter(XdmfAttributeCenter::Node());
		nodalAttribute->getArray()->copyValues(0, &nodeValues[0], 12);

		// Add Cell Attribute
		boost::shared_ptr<XdmfAttribute> cellAttribute = XdmfAttribute::New();
		int cellValues[] = {100, 200};
		cellAttribute->setName("Cell Attribute");
		cellAttribute->setAttributeType(XdmfAttributeType::Scalar());
		cellAttribute->setAttributeCenter(XdmfAttributeCenter::Cell());
		cellAttribute->getArray()->copyValues(0, &cellValues[0], 2);

		grid->insert(nodalAttribute);
		grid->insert(cellAttribute);
		return grid;
	}

};

