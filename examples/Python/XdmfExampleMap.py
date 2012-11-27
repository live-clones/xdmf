from Xdmf import *

if __name__ == "__main__":
	exampleMap = XdmfMap.New()

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

	newRemoteTaskID = 4
	newLocalNodeID = 7
	newRemoteLocalNodeID = 3
	exampleMap.insert(newRemoteTaskID, newLocalNodeID, newRemoteLocalNodeID)

	exampleMapVector = XdmfMap.New(testVector)

	newTaskMap = XdmfMapMap()
	newNodeIdMap = XdmfMapNodeIdMap()
	newNodeIdMap[2] = (3, 6, 8)
	newNodeIdMap[3] = (3,)
	newNodeIdMap[4] = (7,9)
	secondNodeIdMap = XdmfMapNodeIdMap()
	secondNodeIdMap[5] = (3, 6, 8)
	secondNodeIdMap[7] = (3,)
	secondNodeIdMap[9] = (7,9)
	newTaskMap[1] = newNodeIdMap
	newTaskMap[2] = secondNodeIdMap
	exampleMap = XdmfMap.New()
	exampleMap.setMap(newTaskMap)

	newName = "New Name"
	exampleMap.setName(newName)

	exampleName = exampleMap.getName()

	taskIDMap = exampleMap.getMap()
	i = 0
	for val in taskIDMap:
		print val
		i = i + 1
		if i == taskIDMap.size():
			break
	nodeIDMap = taskIDMap[1]
	i = 0
	for val in nodeIDMap:
		print val
		i = i + 1
		if i == nodeIDMap.size():
			break
	for val in nodeIDMap[1]:
		print val


	nodeIDMap = exampleMap.getRemoteNodeIds(1)
	i = 0
	for val in nodeIDMap:
		print val
		i = i + 1
		if i == nodeIDMap.size():
			break
	for val in nodeIDMap[1]:
		print val

	if not(exampleMap.isInitialized()):
		exampleMap.read()

	exampleMap.release()

	hdf5FilePath = "The HDF5 file path goes here"
	hdf5SetPath = "The HDF5 set path goes here"
	startIndex = 0#start at the beginning
	readStride = 1#read all values
	readNumber = 10#read 10 values
	newRemoteTaskController = XdmfHDF5Controller.New(
		hdf5FilePath, hdf5SetPath, XdmfArrayType.Int32(),
		startIndex, readStride, readNumber)
	hdf5FilePath = "The HDF5 file path for the local nodes goes here"
	hdf5SetPath = "The HDF5 set path for the local nodes goes here"
	newLocalNodeController = XdmfHDF5Controller.New(
		hdf5FilePath, hdf5SetPath, XdmfArrayType.Int32(),
		startIndex, readStride, readNumber)
	hdf5FilePath = "The HDF5 file path for the remote local nodes goes here"
	hdf5SetPath = "The HDF5 set path for the remote local nodes goes here"
	newRemoteLocalNodeController = XdmfHDF5Controller.New(
		hdf5FilePath, hdf5SetPath, XdmfArrayType.Int32(),
		startIndex, readStride, readNumber)
	exampleMap = XdmfMap.New()
	exampleMap.setHeavyDataControllers(newRemoteTaskController, newLocalNodeController, newRemoteLocalNodeController)
