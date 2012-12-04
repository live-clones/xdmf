#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
#include "XdmfTime.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"

class XdmfTestDataGenerator {
public:

  /**
   * Number of Cells = 2
   * Number of Points = 12
   * Number of Attributes = 2
   *   Cell Attributes = 1
   *   Nodal Attributes = 1
   *   Grid Attributes = 1
   * Number of Sets = 1
   *   Nodal Set = 1
   * Time = 100
   * Total Number of Values = 71
   */
  static shared_ptr<XdmfUnstructuredGrid> createHexahedron()
  {
    shared_ptr<XdmfUnstructuredGrid> grid = XdmfUnstructuredGrid::New();
    grid->setName("Hexahedron");

    // Set Geometry
    double points[] = {0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1,
                       1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1,
                       0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1,
                       -1.1, 3.1, 2.1, -2.1};
    grid->getGeometry()->setType(XdmfGeometryType::XYZ());
    grid->getGeometry()->insert(0, &points[0], 36);
    //grid->getGeometry()->setName("Geom 1");

    // Set Topology
    int connectivity[] = {0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10};
    grid->getTopology()->setType(XdmfTopologyType::Hexahedron());
    grid->getTopology()->insert(0, &connectivity[0], 16);

    // Add Node Attribute
    shared_ptr<XdmfAttribute> nodalAttribute = XdmfAttribute::New();
    int nodeValues[] = {100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600,
                        700};
    nodalAttribute->setName("Nodal Attribute");
    nodalAttribute->setType(XdmfAttributeType::Scalar());
    nodalAttribute->setCenter(XdmfAttributeCenter::Node());
    nodalAttribute->insert(0, &nodeValues[0], 12);

    // Add Cell Attribute
    shared_ptr<XdmfAttribute> cellAttribute = XdmfAttribute::New();
    int cellValues[] = {100, 200};
    cellAttribute->setName("Cell Attribute");
    cellAttribute->setType(XdmfAttributeType::Scalar());
    cellAttribute->setCenter(XdmfAttributeCenter::Cell());
    cellAttribute->insert(0, &cellValues[0], 2);

    // Add Grid Attribute
    shared_ptr<XdmfAttribute> gridAttribute = XdmfAttribute::New();
    std::string gridValues[] = {"foo", "bar"};
    //int gridValues[] = {1, 2};
    gridAttribute->setName("Grid Attribute");
    gridAttribute->setType(XdmfAttributeType::Vector());
    gridAttribute->setCenter(XdmfAttributeCenter::Grid());
    gridAttribute->insert(0, &gridValues[0], 2);


    // Add Node Set
    shared_ptr<XdmfSet> nodeSet = XdmfSet::New();
    int nodeIds[] = {0, 1, 2};
    nodeSet->setName("Node Set");
    nodeSet->setType(XdmfSetType::Node());
    nodeSet->insert(0, &nodeIds[0], 3);

    // Add Time
    shared_ptr<XdmfTime> time = XdmfTime::New(100);
    grid->setTime(time);

    grid->insert(nodalAttribute);
    grid->insert(cellAttribute);
    grid->insert(gridAttribute);
    grid->insert(nodeSet);
    return grid;
  }

};

