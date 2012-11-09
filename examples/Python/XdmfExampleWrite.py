from Xdmf import *

if __name__ == "__main__":
	primaryDomain = XdmfDomain.New()
	domaininfo = XdmfInformation.New("Domain", "This is the primary data structure in Xdmf")
	domaininfoinfo = XdmfInformation.New("Information", "Information can have information")
	domaininfo.insert(domaininfoinfo)
	primaryDomain.insert(domaininfo)

	gridHolder = XdmfGridCollection.New()
	gridHolder.setType(XdmfGridCollectionType.Spatial())
	holderInfo = XdmfInformation.New("Grid Collection 1", "This is the main grid collection")
	gridHolder.insert(holderInfo)
	gridHolder.setName("GridCollection Example")


	ungrid = XdmfUnstructuredGrid.New()
	ungrid.setName("Unstructured Grid Example")
	untime = XdmfTime.New(5.0)
	untimeinfo = XdmfInformation.New("Time", "This is the time for the Unstructured Grid")
	untime.insert(untimeinfo)
	ungrid.setTime(untime)
	unglobalID = XdmfAttribute.New()
	unglobalID.setType(XdmfAttributeType.GlobalId())
	unglobalID.setCenter(XdmfAttributeCenter.Node())
	unglobalID.setName("Global Node Equivalencies")
	task1globalnodes = [1, 4, 5, 7, 3, 6]
	unglobalID.insertAsInt32(0, task1globalnodes)
	unglobalIDinfo = XdmfInformation.New("Global Nodes", "This is the global nodes that accociate with the local nodes")
	ungrid.insert(unglobalID)
	unset = XdmfSet.New()
	unsetinfo = XdmfInformation.New("Data Set", "This is a set of arbitrary data")
	unset.insert(unsetinfo)
	unset.setName("Unstructured Grid's Set")
	unset.setType(XdmfSetType.Node())
	unsetattribute = XdmfAttribute.New()
	unsetattribute.setType(XdmfAttributeType.Scalar())
	unsetattribute.setCenter(XdmfAttributeCenter.Node())
	unsetattribute.setName("The Set's attribute")
	unsetattribdata = [1.9, 2.8, 3.7, 4.6, 5.5, 6.4, 7.3, 8.2, 9.1]
	unsetattribute.insertAsFloat64(0, unsetattribdata)
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
	ungeometry.insert(ungeometryinfo)
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
	curvgrid.setName("Curvilinear Grid Example")
	curvgridinfo = XdmfInformation.New("Curvilinear Grid", "This is an example curvilinear grid")
	curvgrid.insert(curvgridinfo)
	curvtime = XdmfTime.New(5.0)
	curvtimeinfo = XdmfInformation.New("Time", "The Time of the Curvilinear Grid")
	curvtime.insert(curvtimeinfo)
	curvgrid.setTime(curvtime)
	curvglobalID = XdmfAttribute.New()
	curvglobalID.setType(XdmfAttributeType.GlobalId())
	curvglobalID.setCenter(XdmfAttributeCenter.Node())
	curvglobalID.setName("Global Node Equivalencies")
	task2globalnodes = [7, 3, 8, 2, 5, 1]
	curvglobalID.insertAsInt32(0, task1globalnodes)
	curvglobalIDinfo = XdmfInformation.New("Global Node Equivalencies", "These are the global nodes that accociate with the local nodes")
	curvglobalID.insert(curvglobalIDinfo)
	curvgrid.insert(curvglobalID)
	curvgeometry = XdmfGeometry.New()
	curvgeometry.setType(XdmfGeometryType.XYZ())
	curvgeometry.setName("Curvilinear Geometry")
	curvgeopoints = [1.1, 1.1, 2.1, 2.1, 1.1, 2.1, 4.1, 1.1, 3.1, 1.1, 2.1, 2.1, 2.1,
		2.1, 2.1, 4.1, 3.1, 3.1, 1.1, 1.1, 0.1, 2.1, 1.1, 0.1, 4.1,
		1.1, -1.1, 1.1, 2.1, 0.1, 1.1, 2.1, -0.1, 4.1, 3.1, -1.1]
	curvgeometry.insertAsFloat64(0, curvgeopoints)
	curvgeometryinfo = XdmfInformation.New("Geometry", "The geometry of the curvilinear grid")
	curvgeometry.insert(curvgeometryinfo)
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
	coordinatecontainer.push_back(rectXarray)
	coordinatecontainer.push_back(rectYarray)
	coordinatecontainer.push_back(rectZarray)
	rectgrid = XdmfRectilinearGrid.New(coordinatecontainer)
	rectgrid.setName("Rectilinear Grid Example")
	rectgridinfo = XdmfInformation.New("Rectilinear Grid", "This is an example of a rectilinear grid")
	rectglobalID = XdmfAttribute.New()
	rectglobalID.setType(XdmfAttributeType.GlobalId())
	rectglobalID.setCenter(XdmfAttributeCenter.Node())
	rectglobalID.setName("Global Node Equivalencies")
	task3globalnodes = [2, 7, 9, 0, 8, 6]
	rectglobalID.insertAsInt32(0, task3globalnodes)
	rectglobalIDinfo = XdmfInformation.New("Global Node Equivalencies", "These are the global nodes that associate with the local nodes")
	rectglobalID.insert(rectglobalIDinfo)
	recttime = XdmfTime.New(5.0)
	recttimeinfo = XdmfInformation.New("Time", "The time of the rectiliniear grid")
	recttime.insert(recttimeinfo)
	rectgrid.setTime(recttime)
	rectgrid.insert(rectglobalID)

	regbrick = XdmfArray.New()
	regbrickvals = [10, 10, 10]
	regbrick.insertAsFloat64(0, regbrickvals)
	regdimensions = XdmfArray.New()
	regdimensionvals = [5, 5, 5]
	regdimensions.insertAsInt32(0, regdimensionvals)
	regorigin = XdmfArray.New()
	regoriginvals = [0, 0, 0]
	regorigin.insertAsFloat64(0, regoriginvals)
	reggrid = XdmfRegularGrid.New(regbrick, regdimensions, regorigin)
	reggrid.setName("Regular Grid Example")
	reggridinfo = XdmfInformation.New("Regular Grid", "This is an example of a regular grid")
	regtime = XdmfTime.New(5.0)
	regtimeinfo = XdmfInformation.New("Time", "This is the time for the regular grid")
	reggrid.setTime(regtime)
	regglobalID = XdmfAttribute.New()
	regglobalID.setType(XdmfAttributeType.GlobalId())
	regglobalID.setCenter(XdmfAttributeCenter.Node())
	regglobalID.setName("Global Node Equivalencies")
	task4globalnodes = [3, 6, 1, 4, 2, 5, 9]
	regglobalID.insertAsInt32(0, task4globalnodes)
	regglobalIDinfo = XdmfInformation.New("Global Node Equivalencies", "These are the global nodes that associate with the local nodes")
	reggrid.insert(regglobalID)

	nodeholder = AttributeVector()
	nodeholder.push_back(unglobalID)
	nodeholder.push_back(curvglobalID)
	nodeholder.push_back(rectglobalID)
	nodeholder.push_back(regglobalID)
	mapcollection = XdmfMap.New(nodeholder)

	ungrid.insert(mapcollection[0])
	curvgrid.insert(mapcollection[1])
	rectgrid.insert(mapcollection[2])
	reggrid.insert(mapcollection[3])

	'''
	the version of XdmfMap.New used here returns a number of maps equal to the number of attributes it was provided with.
	'''
	for insertedmap in mapcollection:
		gridHolder.insert(insertedmap)

	gridHolder.insert(ungrid)
	gridHolder.insert(curvgrid)
	gridHolder.insert(rectgrid)
	gridHolder.insert(reggrid)

	secondaryHolder = XdmfGridCollection.New()
	secondaryHolder.setName("Secondary grid collection")
	gridHolder.insert(secondaryHolder)
	'''
	grid collections can be placed inside other grid collections
	'''

	primaryDomain.insert(gridHolder)
	'''
	grids can be inserted into the domain in the same way as the grid collection
	'''
	primaryDomain.insert(ungrid)
	primaryDomain.insert(curvgrid)
	primaryDomain.insert(rectgrid)
	primaryDomain.insert(reggrid)

	exampleHeavyWriter = XdmfHDF5Writer.New("testoutput.h5")
	exampleWriter = XdmfWriter.New("testoutput.xmf", exampleHeavyWriter)

	
	primaryDomain.accept(exampleWriter)
