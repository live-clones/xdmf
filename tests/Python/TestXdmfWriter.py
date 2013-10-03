from Xdmf import *

if __name__ == "__main__":

    writer = XdmfWriter.New("output.xmf")
    writer.setLightDataLimit(10)

    heavyFile = "testoutput.h5"
    replaceFile = True
    exampleHeavyWriter = XdmfHDF5Writer.New(heavyFile, replaceFile)
    exampleWriter = XdmfWriter.New("testoutput.xmf", exampleHeavyWriter)

    grid = XdmfUnstructuredGrid.New()
    grid.setName("test")
    points = [0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1, 1.1, 1.1,
              1.1, 1.1, 3.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1, 0.1, -1.1, 3.1,
              0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1, -1.1, 3.1, 2.1, -2.1]

    testArray = XdmfArray.New()

    dimensionVector = UInt32Vector()
    dimensionVector.push_back(3)
    dimensionVector.push_back(12)

    testArray.initialize(XdmfArrayType.Float64(), dimensionVector)
#    testArray.initialize(XdmfArrayType.Float64(), 36)

    testVals = [float(piece) for piece in testArray.getValuesString().split()]
    print testVals
    print "These values are not exact because of the way that python handles floats"

    testArray.insertAsFloat64(0, points)

    arrayDimensions = testArray.getDimensions()

    for val in arrayDimensions:
      print val

    testVals = [float(piece) for piece in testArray.getValuesString().split()]
    print testVals
    print "These values are not exact because of the way that python handles floats"

    grid.getGeometry().setType(XdmfGeometryType.XYZ())
    grid.getGeometry().insert(0, testArray, 0, 36, 1, 1);
# or insert points directly
#    grid.getGeometry().insertAsFloat64(0, points)

    connectivity = [0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10]
    grid.getTopology().setType(XdmfTopologyType.Hexahedron())
    grid.getTopology().insertAsInt32(0, connectivity)

    nodalAttribute = XdmfAttribute.New()
    nodalAttribute.setName("Nodal Attribute")
    nodalAttribute.setType(XdmfAttributeType.Scalar())
    nodalAttribute.setCenter(XdmfAttributeCenter.Node())
    nodeVals = [100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600, 700]
    nodalAttribute.insertAsFloat64(0, nodeVals)

    cellAttribute = XdmfAttribute.New()
    cellAttribute.setName("Cell Attribute")
    cellAttribute.setType(XdmfAttributeType.Scalar())
    cellAttribute.setCenter(XdmfAttributeCenter.Cell())
    cellVals = [100, 200]
    cellAttribute.insertAsFloat64(0, cellVals)

    map1Attribute = XdmfAttribute.New()
    map1Attribute.setName("Test Attribute")
    map1Attribute.setType(XdmfAttributeType.Scalar())
    map1Attribute.setCenter(XdmfAttributeCenter.Node())
    map1Vals = [1,2,3,4,5,7,9]
    map1Attribute.insertAsInt32(0, map1Vals)

    map2Attribute = XdmfAttribute.New()
    map2Attribute.setName("Test Attribute")
    map2Attribute.setType(XdmfAttributeType.Scalar())
    map2Attribute.setCenter(XdmfAttributeCenter.Node())
    map2Vals = [9,8,7,4,3]
    map2Attribute.insertAsInt32(0, map2Vals)

    testVector = AttributeVector()
    testVector.push_back(map1Attribute)
    testVector.push_back(map2Attribute)

    testMap = XdmfMap.New(testVector)

    taskIDMap = testMap[0].getMap()
    nodeIDMap = taskIDMap[1]

    print type(taskIDMap)

    i = 0
    for val in taskIDMap:
      print val
      i = i + 1
      if i == taskIDMap.size():
        break

    i = 0
    for val in nodeIDMap:
      print val
      i = i + 1
      if i == nodeIDMap.size():
        break

    for val in nodeIDMap[3]:
      print val

    newTaskMap = XdmfMapMap()

    newNodeIdMap = XdmfMapNodeIdMap()

    newNodeIdMap[0] = (1,)
    newNodeIdMap[1] = (5,)
    newNodeIdMap[2] = (3,)
    newNodeIdMap[3] = (6,)
    newNodeIdMap[4] = (2,)

    newTaskMap[1] = newNodeIdMap

    testSetMap = XdmfMap.New()
    testSetMap.setMap(newTaskMap)

    grid.insert(nodalAttribute)
    grid.insert(cellAttribute)
    grid.insert(testSetMap)
    for insertedMap in testMap:
      grid.insert(insertedMap)


    xPoints = XdmfArray.New()
    xPoints.pushBackAsInt32(1)
    xPoints.pushBackAsInt32(2)
    xPoints.pushBackAsInt32(3)
    xPoints.pushBackAsInt32(4)
    xPoints.pushBackAsInt32(5)

    yPoints = XdmfArray.New()
    yPoints.pushBackAsInt32(1)
    yPoints.pushBackAsInt32(2)
    yPoints.pushBackAsInt32(3)
    yPoints.pushBackAsInt32(4)
    yPoints.pushBackAsInt32(5)

    zPoints = XdmfArray.New()
    zPoints.pushBackAsInt32(1)
    zPoints.pushBackAsInt32(2)
    zPoints.pushBackAsInt32(3)
    zPoints.pushBackAsInt32(4)
    zPoints.pushBackAsInt32(5)

    arrayVector = ArrayVector()
    arrayVector.push_back(xPoints)
    arrayVector.push_back(yPoints)
    arrayVector.push_back(zPoints)

    rectGrid = XdmfRectilinearGrid.New(arrayVector)


    domain = XdmfDomain.New()
    domain.insert(grid)
    domain.insert(rectGrid)
    domain.accept(writer)
    domain.accept(exampleWriter)
