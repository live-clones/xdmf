#include "XdmfDomain.hpp"
#include "XdmfExodusReader.hpp"
#include "XdmfExodusWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"

#include "assert.h"
#include "stdlib.h"
#include "string.h"

int main()
{
  int status = 0;

  XDMFEXODUSWRITER * exodusWriter = XdmfExodusWriterNew();
  XDMFUNSTRUCTUREDGRID * hexahedron = XdmfUnstructuredGridNew();
  XdmfUnstructuredGridSetName(hexahedron, "Hexahedron", &status);

  // Set Geometry
  double points[36] = {0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1,
                     1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1,
                     0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1,
                     -1.1, 3.1, 2.1, -2.1};
  XDMFGEOMETRY * hexGeometry = XdmfUnstructuredGridGetGeometry(hexahedron);
  XdmfGeometrySetType(hexGeometry, XDMF_GEOMETRY_TYPE_XYZ, &status);
  XdmfGeometryInsertDataFromPointer(hexGeometry, &points[0], XDMF_ARRAY_TYPE_FLOAT64, 0, 36, 1, 1, &status);

  // Set Topology
  int connectivity[16] = {0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10};
  XDMFTOPOLOGY * hexTopology =  XdmfUnstructuredGridGetTopology(hexahedron);
  XdmfTopologySetType(hexTopology, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, &status);
  XdmfTopologyInsertDataFromPointer(hexTopology, &connectivity[0], XDMF_ARRAY_TYPE_INT32, 0, 16, 1, 1, &status);

  // Add Node Attribute
  XDMFATTRIBUTE * nodalAttribute = XdmfAttributeNew();
  int nodeValues[12] = {100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600,
                      700};
  XdmfAttributeSetName(nodalAttribute, "Nodal Attribute", &status);
  XdmfAttributeSetType(nodalAttribute, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(nodalAttribute, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfAttributeInsertDataFromPointer(nodalAttribute, &nodeValues[0], XDMF_ARRAY_TYPE_INT32, 0, 12, 1, 1, &status);

  // Add Cell Attribute
  XDMFATTRIBUTE * cellAttribute = XdmfAttributeNew();
  int cellValues[2] = {100, 200};
  XdmfAttributeSetName(cellAttribute, "Cell Attribute", &status);
  XdmfAttributeSetType(cellAttribute, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(cellAttribute, XDMF_ATTRIBUTE_CENTER_CELL, &status);
  XdmfAttributeInsertDataFromPointer(cellAttribute, &cellValues[0], XDMF_ARRAY_TYPE_INT32, 0, 2, 1, 1, &status);

  // Add Grid Attribute
  XDMFATTRIBUTE * gridAttribute = XdmfAttributeNew();
  int gridValues[2] = {1, 2};
  XdmfAttributeSetName(gridAttribute, "Grid Attribute", &status);
  XdmfAttributeSetType(gridAttribute, XDMF_ATTRIBUTE_TYPE_VECTOR, &status);
  XdmfAttributeSetCenter(gridAttribute, XDMF_ATTRIBUTE_CENTER_GRID, &status);
  XdmfAttributeInsertDataFromPointer(gridAttribute, &gridValues[0], XDMF_ARRAY_TYPE_INT32, 0, 2, 1, 1, &status);

  // Add Node Set
  XDMFSET * nodeSet = XdmfSetNew();
  int nodeIds[3] = {0, 1, 2};
  XdmfSetSetName(nodeSet, "Node Set", &status);
  XdmfSetSetType(nodeSet, XDMF_SET_TYPE_NODE, &status);
  XdmfSetInsertDataFromPointer(nodeSet, &nodeIds[0], XDMF_ARRAY_TYPE_INT32, 0, 3, 1, 1, &status);

  // Add Node Set Attribute
  double nodeSetAttributeValues[3] = {10, 11, 12};
  XDMFATTRIBUTE * nodeSetAttribute = XdmfAttributeNew();
  XdmfAttributeSetName(nodeSetAttribute, "Node Set Attribute", &status);
  XdmfAttributeSetType(nodeSetAttribute, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(nodeSetAttribute, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfAttributeInsertDataFromPointer(nodeSetAttribute, &nodeSetAttributeValues[0], XDMF_ARRAY_TYPE_FLOAT64, 0, 3, 1, 1, &status);
  XdmfSetInsertAttribute(nodeSet, nodeSetAttribute, 0);
  XdmfAttributeFree(nodeSetAttribute);

  // Add Time
  XDMFTIME * time = XdmfTimeNew(100);
  XdmfUnstructuredGridSetTime(hexahedron, time, 0);
  XdmfTimeFree(time);

  XdmfUnstructuredGridInsertAttribute(hexahedron, nodalAttribute, 0);
  XdmfUnstructuredGridInsertAttribute(hexahedron, cellAttribute, 0);
  XdmfUnstructuredGridInsertAttribute(hexahedron, gridAttribute, 0);
  XdmfUnstructuredGridInsertSet(hexahedron, nodeSet, 0);

  XdmfAttributeFree(nodalAttribute);
  XdmfAttributeFree(cellAttribute);
  XdmfAttributeFree(gridAttribute);
  XdmfSetFree(nodeSet);

  XdmfExodusWriterWriteGrid(exodusWriter, 
			    "TestXdmfExodusIO.exo", 
			    hexahedron,
			    &status);
  XdmfExodusWriterFree(exodusWriter);

  XDMFEXODUSREADER * reader = XdmfExodusReaderNew();
  XDMFUNSTRUCTUREDGRID * grid =
    (XDMFUNSTRUCTUREDGRID *)XdmfExodusReaderRead(reader, 
						 "TestXdmfExodusIO.exo",
						 NULL);
  XdmfExodusReaderFree(reader);

  char * readName = XdmfUnstructuredGridGetName(grid);
  char * gridName = XdmfUnstructuredGridGetName(hexahedron);
  assert(strcmp(gridName, readName) == 0);
  free(readName);
  free(gridName);

  XDMFGEOMETRY * readGeometry = XdmfUnstructuredGridGetGeometry(grid);

  int geometryType = XdmfGeometryGetType(hexGeometry);
  int readGeometryType = XdmfGeometryGetType(readGeometry);
  assert(readGeometryType == geometryType);

  int numberPoints = XdmfGeometryGetNumberPoints(hexGeometry);
  int readNumberPoints = XdmfGeometryGetNumberPoints(readGeometry);
  assert(readNumberPoints == numberPoints);

  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int geoSize = XdmfGeometryGetSize(hexGeometry);

  for(i = 0; i < geoSize; ++i) {

    double * val = 
      (double *)XdmfGeometryGetValue(hexGeometry, 
				     i, 
				     XDMF_ARRAY_TYPE_FLOAT64, 
				     &status);

    double * readVal = 
      (double *)XdmfGeometryGetValue(readGeometry, 
				     i, 
				     XDMF_ARRAY_TYPE_FLOAT64, 
				     &status);
    
    assert(*val == *readVal);
    
    free(val);
    free(readVal);
    
  }

  XdmfGeometryFree(readGeometry);

  XDMFTOPOLOGY * readTopology = XdmfUnstructuredGridGetTopology(grid);

  int topologyType = XdmfTopologyGetType(hexTopology);
  int readTopologyType = XdmfTopologyGetType(readTopology);
  assert(readTopologyType == topologyType);

  int numberElements = XdmfTopologyGetNumberElements(hexTopology, &status);
  int readNumberElements = XdmfTopologyGetNumberElements(readTopology, &status);
  assert(readNumberElements == numberElements);

  unsigned int toposize = XdmfTopologyGetSize(hexTopology);

  for(i = 0; i < toposize; ++i) {

    double * val = 
      (double *)XdmfTopologyGetValue(hexTopology, 
				     i, 
				     XDMF_ARRAY_TYPE_FLOAT64, 
				     &status);

    double * readVal = 
      (double *)XdmfTopologyGetValue(readTopology, 
				     i, 
				     XDMF_ARRAY_TYPE_FLOAT64, 
				     &status);
    
    assert(*val == *readVal);
    
    free(val);
    free(readVal);
    
  }
  
  XdmfTopologyFree(readTopology);
  
  unsigned int numberAttributes = 
    XdmfUnstructuredGridGetNumberAttributes(hexahedron) + 2;
  unsigned int readAttributes = XdmfUnstructuredGridGetNumberAttributes(grid);

  assert(numberAttributes == readAttributes);

  unsigned int numAttr = XdmfUnstructuredGridGetNumberAttributes(hexahedron);

  for(i = 0; i < numAttr; ++i) {

    XDMFATTRIBUTE * attribute1 = 
      XdmfUnstructuredGridGetAttribute(hexahedron, i);
    
    int attributeCenter = XdmfAttributeGetCenter(attribute1);

    if(attributeCenter != XDMF_ATTRIBUTE_CENTER_GRID) {

      char * name = XdmfAttributeGetName(attribute1);

      XDMFATTRIBUTE * attribute2 = 
	XdmfUnstructuredGridGetAttributeByName(grid, name);
      
      free(name);

      int center = XdmfAttributeGetCenter(attribute1);
      int readCenter = XdmfAttributeGetCenter(attribute2);
      assert(center == readCenter);

      int type = XdmfAttributeGetType(attribute1);
      int readType = XdmfAttributeGetType(attribute2);
      assert(type == readType);

      unsigned int size = XdmfAttributeGetSize(attribute1);
      unsigned int readSize = XdmfAttributeGetSize(attribute2);
      assert(size == readSize);
      
      unsigned int attrSize = XdmfAttributeGetSize(attribute1);
      for (j = 0; j < attrSize; ++j) {

	double * val = 
	  (double *)XdmfAttributeGetValue(attribute1, 
					  i, 
					  XDMF_ARRAY_TYPE_FLOAT64, 
					  &status);

	double * readVal = 
	  (double *) XdmfAttributeGetValue(attribute2, 
					   i, 
					   XDMF_ARRAY_TYPE_FLOAT64, 
					   &status);
	
	assert(*val == *readVal);
	
	free(readVal);
	free(val);

      }

      XdmfAttributeFree(attribute2);

    }

    XdmfAttributeFree(attribute1);

  }

  XdmfGridFree(hexahedron);
  XdmfGridFree(grid);

  XdmfTopologyFree(hexTopology);
  XdmfTopologyFree(hexGeometry);

  return 0;

}
