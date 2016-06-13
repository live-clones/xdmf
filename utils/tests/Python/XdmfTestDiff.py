from Xdmf import *
from XdmfUtils import *

if __name__ == "__main__":
  grid1 = XdmfUnstructuredGrid.New()

  topology1 = XdmfTopology.New()

  connectivity = [0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10]
  topology1.setType(XdmfTopologyType.Hexahedron())
  topology1.insertAsInt32(0, connectivity, 0, 16, 1, 1)

  grid1.setTopology(topology1)

  geometry1 = XdmfGeometry.New();

  points = [0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1,
            1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1,
            0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1,
            -1.1, 3.1, 2.1, -2.1]
  geometry1.setType(XdmfGeometryType.XYZ())
  geometry1.insertAsFloat64(0, points, 0, 36, 1, 1)

  grid1.setGeometry(geometry1)

  attr1 = XdmfAttribute.New()

  nodeValues = [100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600,
                700]
  attr1.setName("Nodal Attribute")
  attr1.setType(XdmfAttributeType.Scalar())
  attr1.setCenter(XdmfAttributeCenter.Node())
  attr1.insertAsInt32(0, nodeValues, 0, 12, 1, 1)

  grid1.insert(attr1)

  # Grid with the same structure

  grid2 = XdmfUnstructuredGrid.New()

  topology2 = XdmfTopology.New()

  topology2.setType(XdmfTopologyType.Hexahedron())
  topology2.insertAsInt32(0, connectivity, 0, 16, 1, 1)

  grid2.setTopology(topology2)

  geometry2 = XdmfGeometry.New()

  geometry2.setType(XdmfGeometryType.XYZ())
  geometry2.insertAsFloat64(0, points, 0, 36, 1, 1)

  grid2.setGeometry(geometry2)

  attr2 = XdmfAttribute.New()

  attr2.setName("Nodal Attribute")
  attr2.setType(XdmfAttributeType.Scalar())
  attr2.setCenter(XdmfAttributeCenter.Node())
  attr2.insertAsInt32(0, nodeValues, 0, 12, 1, 1)

  grid2.insert(attr2)

  # Grid with structure partially off, difference of 5.

  grid3 = XdmfUnstructuredGrid.New()

  topology3 = XdmfTopology.New()

  topology3.setType(XdmfTopologyType.Hexahedron());
  topology3.insertAsInt32(0, connectivity, 0, 16, 1, 1)

  grid3.setTopology(topology3)

  geometry3 = XdmfGeometry.New();

  pointsdiff = [4.1, 0.1, 1.1, 1.1, 0.1, 3.1, 3.1, 0.1, 2.1, 0.1, 1.1,
                1.1, 1.1, 1.1, 1.1, 6.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1,
                0.1, -1.1, 3.1, 3.1, -4.1, 0.1, 1.1, -1.1, 1.1, 1.1,
                -1.1, 3.1, 2.1, -2.1]
  geometry3.setType(XdmfGeometryType.XYZ());
  geometry3.insertAsFloat64(0, pointsdiff, 0, 36, 1, 1);

  grid3.setGeometry(geometry3)

  attr3 = XdmfAttribute.New()

  attr3.setName("Nodal Attribute")
  attr3.setType(XdmfAttributeType.Scalar())
  attr3.setCenter(XdmfAttributeCenter.Node())
  attr3.insertAsInt32(0, nodeValues, 0, 12, 1, 1)

  grid3.insert(attr3)

  # Make diff checks

  diff = XdmfDiff.New()

  if not diff.compare(grid1, grid2):
    print "equivalent grids are not compared correctly"
  else:
    print "equivalent grids are compared correctly"

  assert(diff.compare(grid1, grid2))

  if diff.compare(grid1, grid3):
    print "dissimilar grids are not compared correctly"
  else:
    print "dissimilar grids are compared correctly"

  assert(not diff.compare(grid1, grid3))

  print "default tolerance = " , str(diff.getAbsoluteTolerance())

  diff.setAbsoluteTolerance(6)

  if not diff.compare(grid1, grid3):
    print "tolerance is not applied correctly"
  else:
    print "tolerance is applied correctly"

  assert(diff.compare(grid1, grid3))
