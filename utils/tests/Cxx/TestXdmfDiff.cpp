#include <math.h>
#include <iostream>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfDiff.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyConverter.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfWriter.hpp"

int main(int, char **)
{
  shared_ptr<XdmfUnstructuredGrid> grid1 = XdmfUnstructuredGrid::New();

  shared_ptr<XdmfTopology> topology1 = XdmfTopology::New();

  int connectivity[] = {0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10};
  topology1->setType(XdmfTopologyType::Hexahedron());
  topology1->insert(0, &connectivity[0], 16);

  grid1->setTopology(topology1);

  shared_ptr<XdmfGeometry> geometry1 = XdmfGeometry::New();

  double points[] = {0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1,
                     1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1,
                     0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1,
                     -1.1, 3.1, 2.1, -2.1};
  geometry1->setType(XdmfGeometryType::XYZ());
  geometry1->insert(0, &points[0], 36);

  grid1->setGeometry(geometry1);

  shared_ptr<XdmfAttribute> attr1 = XdmfAttribute::New();

  int nodeValues[] = {100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600,
                      700};
  attr1->setName("Nodal Attribute");
  attr1->setType(XdmfAttributeType::Scalar());
  attr1->setCenter(XdmfAttributeCenter::Node());
  attr1->insert(0, &nodeValues[0], 12);

  grid1->insert(attr1);

  // Grid with the same structure

  shared_ptr<XdmfUnstructuredGrid> grid2 = XdmfUnstructuredGrid::New();

  shared_ptr<XdmfTopology> topology2 = XdmfTopology::New();

  topology2->setType(XdmfTopologyType::Hexahedron());
  topology2->insert(0, &connectivity[0], 16);

  grid2->setTopology(topology2);

  shared_ptr<XdmfGeometry> geometry2 = XdmfGeometry::New();

  geometry2->setType(XdmfGeometryType::XYZ());
  geometry2->insert(0, &points[0], 36);

  grid2->setGeometry(geometry2);

  shared_ptr<XdmfAttribute> attr2 = XdmfAttribute::New();

  attr2->setName("Nodal Attribute");
  attr2->setType(XdmfAttributeType::Scalar());
  attr2->setCenter(XdmfAttributeCenter::Node());
  attr2->insert(0, &nodeValues[0], 12);

  grid2->insert(attr2);

  // Grid with structure partially off, difference of 5.

  shared_ptr<XdmfUnstructuredGrid> grid3 = XdmfUnstructuredGrid::New();

  shared_ptr<XdmfTopology> topology3 = XdmfTopology::New();

  topology3->setType(XdmfTopologyType::Hexahedron());
  topology3->insert(0, &connectivity[0], 16);

  grid3->setTopology(topology3);

  shared_ptr<XdmfGeometry> geometry3 = XdmfGeometry::New();

  double pointsdiff[] = {4.1, 0.1, 1.1, 1.1, 0.1, 3.1, 3.1, 0.1, 2.1, 0.1, 1.1,
                         1.1, 1.1, 1.1, 1.1, 6.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1,
                         0.1, -1.1, 3.1, 3.1, -4.1, 0.1, 1.1, -1.1, 1.1, 1.1,
                         -1.1, 3.1, 2.1, -2.1};
  geometry3->setType(XdmfGeometryType::XYZ());
  geometry3->insert(0, &pointsdiff[0], 36);

  grid3->setGeometry(geometry3);

  shared_ptr<XdmfAttribute> attr3 = XdmfAttribute::New();

  attr3->setName("Nodal Attribute");
  attr3->setType(XdmfAttributeType::Scalar());
  attr3->setCenter(XdmfAttributeCenter::Node());
  attr3->insert(0, &nodeValues[0], 12);

  grid3->insert(attr3);

  // Make diff checks

  shared_ptr<XdmfDiff> diff = XdmfDiff::New();

  if (!diff->compare(grid1, grid2))
  {
    std::cout << "equivalent grids are not compared correctly" << std::endl;
  }
  else
  {
    std::cout << "equivalent grids are compared correctly" << std::endl;
  }
  assert(diff->compare(grid1, grid2));

  if (diff->compare(grid1, grid3))
  {
    std::cout << "dissimilar grids are not compared correctly" << std::endl;
  }
  else
  {
    std::cout << "dissimilar grids are compared correctly" << std::endl;
  }
  assert(!diff->compare(grid1, grid3));

  std::cout << "default tolerance = " << diff->getAbsoluteTolerance() << std::endl;

  diff->setAbsoluteTolerance(6);

  if (!diff->compare(grid1, grid3))
  {
    std::cout << "tolerance is not applied correctly" << std::endl;
  }
  else
  {
    std::cout << "tolerance is applied correctly" << std::endl;
  }
  assert(diff->compare(grid1, grid3));

  return 0;
}
