#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfMap.hpp"

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "assert.h"

int main()
{

  void * map = XdmfMapNew();

  int i = 0;
  int j = 0;
  int k = 0;

  int status = 0;

  XdmfMapInsert(map, 1, 0, 8);
  XdmfMapInsert(map, 1, 3, 7);
  XdmfMapInsert(map, 1, 5, 9);
  XdmfMapInsert(map, 2, 4, 6);
  XdmfMapInsert(map, 2, 7, 5);
  XdmfMapInsert(map, 3, 8, 3);
  XdmfMapInsert(map, 3, 2, 4);
  XdmfMapInsert(map, 3, 6, 2);
  XdmfMapInsert(map, 3, 1, 1);

  int numRemoteTask = XdmfMapRetrieveNumberRemoteTaskIds(map);

  printf("%d ?= %d\n", numRemoteTask, 3);

  assert(numRemoteTask == 3);

  int numLocalNode = XdmfMapRetrieveNumberLocalNodeIds(map, 1);

  printf("%d ?= %d\n", numLocalNode, 3);

  assert(numLocalNode == 3);

  numLocalNode = XdmfMapRetrieveNumberLocalNodeIds(map, 2);

  printf("%d ?= %d\n", numLocalNode, 2);

  assert(numLocalNode == 2);

  numLocalNode = XdmfMapRetrieveNumberLocalNodeIds(map, 3);

  printf("%d ?= %d\n", numLocalNode, 4);

  assert(numLocalNode == 4);

  int numRemoteNode = XdmfMapRetrieveNumberRemoteNodeIds(map, 1, 0);

  printf("%d ?= %d\n", numRemoteNode, 1);

  assert(numRemoteNode == 1);

  char * mapName = "Test Name";

  XdmfMapSetName(map, mapName);

  char * outfile = "mapfile.xmf";

  XDMFWRITER * writer = XdmfWriterNew(outfile);

  // Write to File

  XdmfMapAccept(map, (XDMFVISITOR *)XdmfWriterGetHeavyDataWriter(writer, &status), &status);

  XdmfMapAccept(map, (XDMFVISITOR *)writer, &status);

  XdmfMapRelease(map);

  if (XdmfMapIsInitialized(map))
  {
    printf("map is initialized before read\n");
  }
  else
  {
    printf("map is not initialized before read\n");
  }

  assert(!XdmfMapIsInitialized(map));

  XdmfMapRead(map, &status);

  if (XdmfMapIsInitialized(map))
  {
    printf("map is initialized after read\n");
  }
  else
  {
    printf("map is not initialized after read\n");
  }

  assert(XdmfMapIsInitialized(map));

  // Read from File

  void * reader = XdmfReaderNew();

  void * readMap = XdmfReaderRead(reader, outfile, &status);

  if (XdmfMapIsInitialized(readMap))
  {
    printf("map is initialized before read\n");
  }
  else
  {
    printf("map is not initialized before read\n");
  }

  assert(!XdmfMapIsInitialized(readMap));

  XdmfMapRead(readMap, &status);

  if (XdmfMapIsInitialized(readMap))
  {
    printf("map is initialized after read\n");
  }
  else
  {
    printf("map is not initialized after read\n");
  }

  assert(XdmfMapIsInitialized(readMap));

  char * readName = XdmfMapGetName(readMap);

  printf("%s ?= %s\n", readName, "Test Name");

  assert(strcmp(readName, "Test Name") ==  0);

  int * globalNodeIds[3];

  int globalNodes1[5];

  globalNodeIds[0] = globalNodes1;

  globalNodeIds[0][0] = 1;
  globalNodeIds[0][1] = 2;
  globalNodeIds[0][2] = 3;
  globalNodeIds[0][3] = 4;
  globalNodeIds[0][4] = 5;

  int globalNodes2[6];

  globalNodeIds[1] = globalNodes2;

  globalNodeIds[1][0] = 3;
  globalNodeIds[1][1] = 4;
  globalNodeIds[1][2] = 5;
  globalNodeIds[1][3] = 6;
  globalNodeIds[1][4] = 7;
  globalNodeIds[1][5] = 8;

  int globalNodes3[7];

  globalNodeIds[2] = globalNodes3;

  globalNodeIds[2][0] = 9;
  globalNodeIds[2][1] = 8;
  globalNodeIds[2][2] = 7;
  globalNodeIds[2][3] = 6;
  globalNodeIds[2][4] = 5;
  globalNodeIds[2][5] = 4;
  globalNodeIds[2][6] = 3;

  int numIdsOnNode[3] = {5, 6, 7};

  int numIds = 3;

  XDMFMAP ** mapArray = XdmfMapNewFromIdVector(globalNodeIds, numIdsOnNode, numIds);

  int testMapID = 0;

  void * testMap;
  int testNumRemoteTaskIds;
  int * remoteTaskIds;
  int testNumLocalNodeIds;
  int * localNodeIds;
  int testNumRemoteNodeIds;
  int * remoteNodeIds;

  int expectedRemoteTasks[3][2];
  expectedRemoteTasks[0][0] = 1;
  expectedRemoteTasks[0][1] = 2;
  expectedRemoteTasks[1][0] = 0;
  expectedRemoteTasks[1][1] = 2;
  expectedRemoteTasks[2][0] = 0;
  expectedRemoteTasks[2][1] = 1;

  int expectedLocalNodeIds[3][2][6];
  expectedLocalNodeIds[0][0][0] = 2;
  expectedLocalNodeIds[0][0][1] = 3;
  expectedLocalNodeIds[0][0][2] = 4;
  expectedLocalNodeIds[0][1][0] = 2;
  expectedLocalNodeIds[0][1][1] = 3;
  expectedLocalNodeIds[0][1][2] = 4;
  expectedLocalNodeIds[1][0][0] = 0;
  expectedLocalNodeIds[1][0][1] = 1;
  expectedLocalNodeIds[1][0][2] = 2;
  expectedLocalNodeIds[1][1][0] = 0;
  expectedLocalNodeIds[1][1][1] = 1;
  expectedLocalNodeIds[1][1][2] = 2;
  expectedLocalNodeIds[1][1][3] = 3;
  expectedLocalNodeIds[1][1][4] = 4;
  expectedLocalNodeIds[1][1][5] = 5;
  expectedLocalNodeIds[2][0][0] = 4;
  expectedLocalNodeIds[2][0][1] = 5;
  expectedLocalNodeIds[2][0][2] = 6;
  expectedLocalNodeIds[2][1][0] = 1;
  expectedLocalNodeIds[2][1][1] = 2;
  expectedLocalNodeIds[2][1][2] = 3;
  expectedLocalNodeIds[2][1][3] = 4;
  expectedLocalNodeIds[2][1][4] = 5;
  expectedLocalNodeIds[2][1][5] = 6;

  int expectedRemoteNodeIds[3][2][6][1];
  expectedRemoteNodeIds[0][0][0][0] = 0;
  expectedRemoteNodeIds[0][0][1][0] = 1;
  expectedRemoteNodeIds[0][0][2][0] = 2;
  expectedRemoteNodeIds[0][1][0][0] = 6;
  expectedRemoteNodeIds[0][1][1][0] = 5;
  expectedRemoteNodeIds[0][1][2][0] = 4;
  expectedRemoteNodeIds[1][0][0][0] = 2;
  expectedRemoteNodeIds[1][0][1][0] = 3;
  expectedRemoteNodeIds[1][0][2][0] = 4;
  expectedRemoteNodeIds[1][1][0][0] = 6;
  expectedRemoteNodeIds[1][1][1][0] = 5;
  expectedRemoteNodeIds[1][1][2][0] = 4;
  expectedRemoteNodeIds[1][1][3][0] = 3;
  expectedRemoteNodeIds[1][1][4][0] = 2;
  expectedRemoteNodeIds[1][1][5][0] = 1;
  expectedRemoteNodeIds[2][0][0][0] = 4;
  expectedRemoteNodeIds[2][0][1][0] = 3;
  expectedRemoteNodeIds[2][0][2][0] = 2;
  expectedRemoteNodeIds[2][1][0][0] = 5;
  expectedRemoteNodeIds[2][1][1][0] = 4;
  expectedRemoteNodeIds[2][1][2][0] = 3;
  expectedRemoteNodeIds[2][1][3][0] = 2;
  expectedRemoteNodeIds[2][1][4][0] = 1;
  expectedRemoteNodeIds[2][1][5][0] = 0;

  for (testMapID = 0; testMapID < 3; ++testMapID) {

    testMap = mapArray[testMapID];

    printf("Map # %d\n\n", testMapID);

    testNumRemoteTaskIds = XdmfMapRetrieveNumberRemoteTaskIds(testMap);

//    printf("number remote task Ids = %d\n", testNumRemoteTaskIds);

    remoteTaskIds = XdmfMapRetrieveRemoteTaskIds(testMap);

    for (i = 0; i < testNumRemoteTaskIds; ++i) {

      printf("remote task id %d\n", remoteTaskIds[i]);
      assert(expectedRemoteTasks[testMapID][i] == remoteTaskIds[i]);
      testNumLocalNodeIds = XdmfMapRetrieveNumberLocalNodeIds(testMap, remoteTaskIds[i]);

//      printf("number local node Ids = %d\n", testNumLocalNodeIds);

      localNodeIds = XdmfMapRetrieveLocalNodeIds(testMap, remoteTaskIds[i]);

      for (j = 0; j < testNumLocalNodeIds; ++j) {
        printf("\tlocal node id %d\n", localNodeIds[j]);
        assert(localNodeIds[j] == expectedLocalNodeIds[testMapID][i][j]);

        testNumRemoteNodeIds = XdmfMapRetrieveNumberRemoteNodeIds(testMap, remoteTaskIds[i], localNodeIds[j]);

//        printf("\tnumber remote node ids = %d\n", testNumRemoteNodeIds);

        remoteNodeIds = XdmfMapRetrieveRemoteNodeIds(testMap, remoteTaskIds[i], localNodeIds[j]);

        for (k = 0; k < testNumRemoteNodeIds; ++k) {
          printf("\t\tremote node id %d\n", remoteNodeIds[k]);
          assert(remoteNodeIds[k] == expectedRemoteNodeIds[testMapID][i][j][k]);
        }
        free(remoteNodeIds);
      }
      free(localNodeIds);
    }
    free(remoteTaskIds);
  }

/*
XDMF_EXPORT void XdmfMapSetHeavyDataControllers(void * map,
                                                void ** remoteTaskControllers,
                                                int numRemoteTaskControllers,
                                                void ** localNodeControllers,
                                                int numberLocalNodeControllers,
                                                void ** remoteLocalNodeControllers,
                                                int numRemoteLocalNodeControllers,
                                                int passControl);
*/

  return 0;
}
