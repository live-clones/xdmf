from Xdmf import *

if __name__ == "__main__":
	exampleMap = XdmfMap.New()

	#Assumming that exampleMap is a shared pointer to a XdmfMap object

	#create attributes for each task id
	#the index of the node id in the attribute is the local node id
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

	#insert the attributes into a vector
	#the id of the attribute in the vector is equal to the task id
	testVector = AttributeVector()
	testVector.push_back(map1Attribute)
	testVector.push_back(map2Attribute)

	newRemoteTaskID = 4
	newLocalNodeID = 7
	newRemoteLocalNodeID = 3
	exampleMap.insert(newRemoteTaskID, newLocalNodeID, newRemoteLocalNodeID)
	#This inserts an entry of (4, 7, 3) into the map

	exampleMapVector = XdmfMap.New(testVector)
	#returns a vector of maps that holds the equivalencies for the nodes provided
	#for example if Attribute 1 had globalNodeID 3 at localNodeID 2
	#and Attribute 3 had globalNodeID 3 at localNodeID 5
	#then map 1 would have an entry of (3, 5, 2)
	#and map 3 would have an entry of (1, 2, 5)
	#The entries are formatted (remoteTaskID, remoteLocalNodeID, localNodeID)

	newTaskMap = XdmfMapMap()
	newNodeIdMap = XdmfMapNodeIdMap()
	newNodeIdMap[2] = (3, 6, 8)
	newNodeIdMap[3] = (3,)
	newNodeIdMap[4] = (7,9)
	#newNodeIdMap now contains the following
	#(2, 3)
	#(2, 6)
	#(2, 8)
	#(3, 3)
	#(4, 7)
	#(4, 9)
	secondNodeIdMap = XdmfMapNodeIdMap()
	secondNodeIdMap[5] = (3, 6, 8)
	secondNodeIdMap[7] = (3,)
	secondNodeIdMap[9] = (7,9)
	#secondNodeIdMap now contains the following
	#(5, 3)
	#(5, 6)
	#(5, 8)
	#(7, 3)
	#(9, 7)
	#(9, 9)
	newTaskMap[1] = newNodeIdMap
	newTaskMap[2] = secondNodeIdMap
	exampleMap = XdmfMap.New()
	exampleMap.setMap(newTaskMap)
	#(1, 2, 3)
	#(1, 2, 6)
	#(1, 2, 8)
	#(1, 3, 3)
	#(1, 4, 7)
	#(1, 4, 9)
	#(2, 5, 3)
	#(2, 5, 6)
	#(2, 5, 8)
	#(2, 7, 3)
	#(2, 9, 7)
	#(2, 9, 9)
	#Is now stored in exampleMap

	newName = "New Name"
	exampleMap.setName(newName)

	exampleName = exampleMap.getName()

	#getMap
	#Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
	#(1, 1, 9)
	#(1, 2, 8)
	#(2, 3, 7)
	#(2, 4, 6)
	#(3, 5, 5)
	#(3, 6, 4)
	taskIDMap = exampleMap.getMap()
	i = 0
	for val in taskIDMap:
		print val
		i = i + 1
		if i == taskIDMap.size():
			break
	#This prints out all the task IDs
	#unless the break is called on the last iteration the program will fail because of an issue with SWIG
	nodeIDMap = taskIDMap[1]
	#nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
	#(1, 9)
	#(2, 8)
	i = 0
	for val in nodeIDMap:
		print val
		i = i + 1
		if i == nodeIDMap.size():
			break
	#This prints out all the local node IDs
	#unless the break is called on the last iteration the program will fail because of an issue with SWIG
	for val in nodeIDMap[1]:
		print val
	#prints out all the remote node values associated with taskID 1 and localNode 1


	nodeIDMap = exampleMap.getRemoteNodeIds(1)
	#nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
	#(1, 9)
	#(2, 8)
	i = 0
	for val in nodeIDMap:
		print val
		i = i + 1
		if i == nodeIDMap.size():
			break
	#This prints out all the local node IDs
	#unless the break is called on the last iteration the program will fail because of an issue with SWIG
	for val in nodeIDMap[1]:
		print val
	#prints out all the remote node values associated with taskID 1 and localNode 1

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
