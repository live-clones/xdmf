#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "assert.h"

int main()
{

  XDMFTOPOLOGY * topology = XdmfTopologyNew();

  int i = 0;
  int status = 0;
  for (i = 0; i < 30; i++) {
    XdmfTopologyPushBack(topology, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  XdmfTopologySetType(topology, XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10, &status);

  XDMFWRITER * writer = XdmfWriterNew("topofile.xmf");

  // Write to File
  XdmfTopologyAccept(topology, (XDMFVISITOR *)writer, &status);
  XdmfWriterFree(writer);
  XdmfTopologyFree(topology);

  // Read from File
  XDMFREADER * reader = XdmfReaderNew();
  XDMFITEM * readItem = XdmfReaderRead(reader, "topofile.xmf", &status);
  XdmfReaderFree(reader);
  XDMFTOPOLOGY * readTopology = XdmfTopologyCast(readItem);
  XdmfItemFree(readItem);

  char * valueString = XdmfTopologyGetItemTag(readTopology);
  printf("%s ?= %s\n", valueString, "Topology");
  assert(strcmp(valueString, "Topology") == 0);
  free(valueString);

  XdmfTopologyRead(readTopology, &status);

  int topotype = XdmfTopologyGetType(readTopology);
  printf("%d ?= %d\n", topotype, XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10);
  assert(topotype == XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10);

  unsigned int numElements = XdmfTopologyGetNumberElements(readTopology, &status);
  printf("array contains %d elements\n", numElements);
  assert(numElements == 3);

  int cellType = XdmfTopologyTypeGetCellType(topotype);
  printf("cell type code = %d\n", cellType);
  assert(cellType == 2);

  unsigned int numEdges = XdmfTopologyTypeGetEdgesPerElement(topotype, &status);
  unsigned int numFaces = XdmfTopologyTypeGetFacesPerElement(topotype, &status);
  unsigned int numNodes = XdmfTopologyTypeGetNodesPerElement(topotype);
  printf("type contains:\n%d edges\n%d faces\n%d nodes\n", numEdges, numFaces, numNodes);
  assert(numEdges == 6);
  assert(numFaces == 4);
  assert(numNodes == 10);

  valueString = XdmfTopologyTypeGetName(topotype);
  printf("Topology type name: %s\n", valueString);
  assert(strcmp("Tetrahedron_10", valueString) == 0);
  free(valueString);

  valueString = XdmfTopologyGetValuesString(readTopology);
  printf("array contains: %s\n", valueString);
  assert(strcmp("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29", valueString) == 0);
  free(valueString);

  XdmfTopologyFree(readTopology);

  return 0;
}
