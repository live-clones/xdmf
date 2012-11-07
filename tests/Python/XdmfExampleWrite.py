from Xdmf import *

if __name__ == "__main__":
	primaryDomain = XdmfDomain.New()

	gridHolder = XdmfGridCollection.New()
	gridHolder.setType(XdmfGridCollectionType.Spatial())
	holderInfo = XdmfInformation.New("Grid Collection 1", "This is the main grid collection")
	gridHolder.insert(holderInfo)


	ungrid = XdmfUnstructuredGrid.New()
	ungrid.setName("Unstructured Grid Example")
	untime = XdmfTime.New(5.0)
	untimeinfo = XdmfInformation("Time", "This is the time for the Unstructured Grid")
	untime.insert(untimeinfo)
	ungrid.setTime(untime)
	unglobalID = XdmfAttribute.New()
	unglobalID.setType(XdmfAttributeType.GlobalID())
	unglobalID.setCenter(XdmfAttributeCenter.Node())
	unglobalID.setName("Global Node Equivalencies")
	task1globalnodes = [1, 4, 5, 7, 3, 6]
	unglobalID.insertAsInt32(0, task1globalnodes)
	unglobalIDinfo = XdmfInformation("Global Nodes", "This is the global nodes that accociate with the local nodes")
	ungrid.insert(unglobalID)
	unset = XdmfSet.New()
	unsetinfo = XdmfInformation("Data Set", "This is a set of arbitrary data")
	unset.insert(unsetinfo)
	unset.setName("Unstructured Grid's Set")
	unset.setType(XdmfSetType.Node())
	unsetattribute = XdmfAttribute.New()
	unsetattribute.setType(XdmfAttributeType.Scalar())
	unsetattribute.setCenter(XdmfAttributeCenter.Node())
	unsetattribute.setName("The Set's attribute")
	unsetattribdata = [1.9, 2.8, 3.7, 4.6, 5.5, 6.4, 7.3, 8.2, 9.1]
	unsetattribute.insertAsFloat64(unsetattribdata)
	unset.insert(unsetattribute)
	unsetdata = [5.1, 4.2, 3.3, 2.4, 1.5]
	unset.insertAsFloat64(0, unsetdata)
	ungrid.insert(unset)
	ungeometry = XdmfGeometry.New()
	ungeometry.setType(XdmfGeometryType.XYZ())
	ungeometry.setName("Unstructured Geometry")
	ungeopoints = [0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1, 1.1, 1.1,
		1.1, 1.1, 3.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1, 0.1, -1.1, 3.1,
		0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1, -1.1, 3.1, 2.1, -2.1]
	ungeometry.insertAsFloat64(0, ungeopoints)
	ungeometryinfo = XdmfInformation.New("Geometry", "This is the geometry associated with the unstructured grid")
	ungeometry.insert(ungemetryinfo)
	ungrid.setGeometry(ungeometry)
	untopology = XdmfTopology.New()
	untopology.setType(XdmfTopologyType.Hexahedron())
	untopology.setName("Unstructured Topology")
	untopovalues = [0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10]
	untopology.insertAsInt32(0, untopovalues)
	untopologyinfo = XdmfInformation.New("Topology", "This is the topology associated with the unstructured grid")
	ungrid.setTopology(untopology)


	curvdimensions = XdmfArray.New()
	curvdimensions.pushBackAsInt32(12)
	curvdimensions.pushBackAsInt32(12)
	curvdimensions.pushBackAsInt32(12)
	curvgrid = XdmfCurvilinearGrid.New(curvdimensions)
	curvtime = XdmfTime.New(5.0)
	curvgrid.setTime(curvtime)
	curvglobalID = XmdfAttribute.New()
	curvglobalID.setType(XdmfAttributeType.GlobalID())
	curvglobalID.setCenter(XdmfAttributeCenter.Node())
	curvglobalID.setName("Global Node Equivalencies")
	task2globalnodes = [7, 3, 8, 2, 5, 1]
	curvglobalID.insertAsInt32(0, task1globalnodes)
	curvgrid.insert(curvglobalID)
	curvgeometry = XdmfGeometry.New()
	curvgeometry.setType(XdmfGeometryType.XYZ())
	curvgeometry.setName("Curvilinear Geometry")
	curvgeopoints = [1.1, 1.1, 2.1, 2.1, 1.1, 2.1, 4.1, 1.1, 3.1, 1.1, 2.1, 2.1, 2.1,
		2.1, 2.1, 4.1, 3.1, 3.1, 1.1, 1.1, 0.1, 2.1, 1.1, 0.1, 4.1,
		1.1, -1.1, 1.1, 2.1, 0.1, 1.1, 2.1, -0.1, 4.1, 3.1, -1.1]
	curvgeometry.insertAsFloat64(0, curvgeopoints)
	curvgrid.setGeometry(curvgeometry)


	rectXcoordinates = [1.1, 1.1, 2.1, 2.1, 1.1, 2.1, 4.1, 1.1, 3.1, 1.1, 2.1, 2.1]
	rectYcoordinates = [2.1, 2.1, 2.1, 4.1, 3.1, 3.1, 1.1, 1.1, 0.1, 2.1, 1.1, 0.1]
	rectZcoordinates = [4.1, 1.1, -1.1, 1.1, 2.1, 0.1, 1.1, 2.1, -0.1, 4.1, 3.1, -1.1]
	rectXarray = XdmfArray.New()
	rectXarray.insertAsFloat64(0, rectXcoordinates)
	rectYarray = XdmfArray.New()
	rectYarray.insertAsFloat64(0, rectYcoordinates)
	rectZarray = XdmfArray.New()
	rectZarray.insertAsFloat64(0, rectZcoordinates)
	coordinatecontainer = ArrayVector()
	coordinatecontainer.pushback(rectXarray)
	coordinatecontainer.pushback(rectYarray)
	coordinatecontainer.pushback(rectZarray)
	rectgrid = XdmfRectilinearGrid.New(coordinatecontainer)
	rectglobalID = XdmfAttribute.New()
	rectglobalID.setType(XdmfAttributeType.GlobalID())
	rectglobalID.setCenter(XdmfAttributeCenter.Node())
	rectglobalID.setName("Global Node Equivalencies")
	task3globalnodes = [2, 7, 9, 0, 8, 6]
	rectglobalID.insertAsInt(0, task3globalnodes)
	recttime = XdmfTime.New(5.0)
	rectgrid.setTime(recttime)
	rectgrid.insert(rectglobalID)
