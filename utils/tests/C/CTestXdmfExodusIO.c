#include "XdmfDomain.hpp"
#include "XdmfExodusReader.hpp"
#include "XdmfExodusWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"

#include "string.h"

int main()
{
  int status = 0;

  XDMFEXODUSWRITER * exodusWriter = XdmfExodusWriterNew();
  XDMFUNSTRUCTUREDGRID * hexahedron = XdmfUnstructuredGridNew();
  XdmfUnstructuredGridSetName(hexahedron, "Hexahedron");

  // Set Geometry
  double points[36] = {0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1,
                     1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1,
                     0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1,
                     -1.1, 3.1, 2.1, -2.1};
  XDMFGEOMTERY * hexGeometry = XdmfUnstructuredGridGetGeometry(hexahedron);
  XdmfGeometrySetType(hexGeometry, XDMF_GEOMETRY_TYPE_XYZ);
  XdmfGeometryInsertDataFromPointer(hexGeometry, &points[0], XDMF_ARRAY_TYPE_FLOAT64, 0, 36, 1, 1 &status);

  // Set Topology
  int connectivity[16] = {0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10};
  XDMFTOPOLOGY * hexTopology =  XdmfUnstructuredGridGetTopology(hexahedron);
  XdmfTopologySetType(hexTopology, XDMF_TOPOLOGY_TYPE_HEXAHEDRON);
  XdmfTopologyInsertDataFromPointer(hexTopology, &connectivity[0], XDMF_ARRAY_TYPE_INT32, 0, 16, 1, 1 &status);

  // Add Node Attribute
  XDMFATTRIBUTE * nodalAttribute = XdmfAttributeNew();
  int nodeValues[12] = {100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600,
                      700};
  XdmfAttributeSetName(nodalAttribute, "Nodal Attribute");
  XdmfAttributeSetType(nodalAttribute, XDMF_ATTRIBUTE_TYPE_SCALAR);
  XdmfAttributeSetCenter(nodalAttribute, XDMF_ATTRIBUTE_CENTER_NODE);
  XdmfAttributeInsertDataFromPointer(nodalAttribute, &nodeValues[0], XDMF_ARRAY_TYPE_INT32, 0, 12, 1, 1 &status);

  // Add Cell Attribute
  XDMFATTRIBUTE * cellAttribute = XdmfAttributeNew();
  int cellValues[2] = {100, 200};
  XdmfAttributeSetName(cellAttribute, "Cell Attribute");
  XdmfAttributeSetType(cellAttribute, XDMF_ATTRIBUTE_TYPE_SCALAR);
  XdmfAttributeSetCenter(cellAttribute, XDMF_ATTRIBUTE_CENTER_CELL);
  XdmfAttributeInsertDataFromPointer(cellAttribute, &cellValues[0], XDMF_ARRAY_TYPE_INT32, 0, 2, 1, 1 &status);

  // Add Grid Attribute
  XDMFATTRIBUTE * gridAttribute = XdmfAttributeNew();
  int gridValues[2] = {1, 2};
  XdmfAttributeSetName(gridAttribute, "Grid Attribute");
  XdmfAttributeSetType(gridAttribute, XDMF_ATTRIBUTE_TYPE_VECTOR);
  XdmfAttributeSetCenter(gridAttribute, XDMF_ATTRIBUTE_CENTER_GRID);
  XdmfAttributeInsertDataFromPointer(gridAttribute, &gridValues[0], XDMF_ARRAY_TYPE_INT32, 0, 2, 1, 1 &status);

  // Add Node Set
  XDMFSET * nodeSet = XdmfSetNew();
  int nodeIds[3] = {0, 1, 2};
  XdmfSetSetName(nodeSet, "Node Set");
  XdmfSetSetType(nodeSet, XDMF_SET_TYPE_NODE);
  XdmfSetInsertDataFromPointer(nodeSet, &nodeIds[0], XDMF_ARRAY_TYPE_INT32, 0, 3, 1, 1, &status);

  // Add Node Set Attribute
  double nodeSetAttributeValues[3] = {10, 11, 12};
  XDMFATTRIBUTE * nodeSetAttribute = XdmfAttributeNew();
  XdmfAttributeSetName(nodeSetAttribute, "Node Set Attribute");
  XdmfAttributeSetType(nodeSetAttribute, XDMF_ATTRIBUTE_TYPE_SCALAR);
  XdmfAttributeSetCenter(nodeSetAttribute, XDMF_ATTRIBUTE_CENTER_NODE);
  XdmfAttributeInsertDataFromPointer(nodeSetAttribute, &nodeSetAttributeValues[0], XDMF_ARRAY_TYPE_FLOAT64, 0, 3, 1, 1 &status);
  XdmfSetInsertAttribute(nodeSet, nodeSetAttribute);

  // Add Time
  XDMFTIME * time = XdmfTimeNew(100);
  XdmfUnstructuredGridSetTime(hexahedron, time);

  XdmfUnstructuredGridInsertAttribute(hexahedron, nodalAttribute);
  XdmfUnstructuredGridInsertAttribute(hexahedron, cellAttribute);
  XdmfUnstructuredGridInsertAttribute(hexahedron, gridAttribute);
  XdmfUnstructuredGridInsertAttribute(hexahedron, nodeSet);

  XdmfExodusWriterWrite(exodusWriter, "TestXdmfExodusIO.exo", hexahedron);

  XDMFEXODUSREADER * reader = XdmfExodusReaderNew();
  XDMFUNSTRUCTUREDGRID * grid =
    (XDMFUNSTRUCTUREDGRID *)XdmfExodusReaderRead(reader, "TestXdmfExodusIO.exo");
  XDMFGEOMETRY * readGeometry = XdmfUnstructuredGridGetGeometry(grid);
  XDMFTOPOLOGY * readTopology = XdmfUnstructuredGridGetTopology(grid);
  assert(strcmp(XdmfUnstructuredGridGetName(grid) == XdmfUnstructuredGridGetName(hexahedron)));
  assert(XdmfGeometryGetType(XdmfUnstructuredGridGetGeometry(grid)) ==
         XdmfGeometryGetType(hexGeometry));
  assert(XdmfGeometryGetNumberPoints(XdmfUnstructuredGridGetGeometry(grid)) ==
         XdmfGeometryGetNumberPoints(hexGeometry));
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int geoSize = XdmfGeometryGetSize(hexGeometry);
  for(i = 0; i < geoSize; ++i) {//TODO
    assert(((double *)XdmfArrayGetValue(hexGeometry, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0] ==
           ((double *)XdmfArrayGetValue(readGeometry, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0]);
  }
  assert(XdmfTopologyGetType(XdmfUnstructuredGridGetTopology(grid)) ==
         XdmfTopologyGetType(hexTopology));
  assert(XdmfTopologyGetNumberElements(XdmfUnstructuredGridGetTopology(grid)) ==
         XdmfTopologyGetNumberElements(hexTopology));
  unsigned int toposize = XdmfTopologyGetSize(hexTopology);
  for(i = 0; i < toposize; ++i) {//TODO
    assert(((double *)XdmfArrayGetValue(hexTopology, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0] ==
           ((double *)XdmfArrayGetValue(readTopology, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0]);
  }
  assert(XdmfUnstructuredGridGetNumberAttributes(hexahedron) + 2 == XdmfUnstructuredGridGetNumberAttributes(grid));
  unsigned int numAttr = XdmfUnstructuredGridGetNumberAttributes(hexahedron);
  for(i = 0; i < numAttr; ++i) {//TODO
    XDMFATTRIBUTE * attribute1 = XdmfUnstructuredGridGetAttribute(hexahedron, i);
    if(attribute1->getCenter() != XdmfAttributeCenter::Grid()) {
      XDMFATTRIBUTE * attribute2 = XdmfUnstructuredGridGetAttributeByName(grid, XdmfAttributeGetName(attribute1));
      assert(XdmfAttributeGetCenter(attribute1, &status) == XdmfAttributeGetCenter(attribute2, &status));
      assert(XdmfAttributeGetType(attribute1, &status) == XdmfAttributeGetType(attribute2, &status));
      assert(XdmfAttributeGetSize(attribute1) == XdmfAttributeGetSize(attribute2));
      unsigned int attrSize = XdmfAttributeGetSize(attribute1);
      for (j = 0; j < attrSize; ++j) {
        assert(((double *)XdmfArrayGetValue(attribute1, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0] ==
               ((double *)XdmfArrayGetValue(attribute2, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0]);
      }
    }
  }
}
