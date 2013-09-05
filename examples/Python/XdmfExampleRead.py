from Xdmf import *

if __name__ == "__main__":
        exampleReader = XdmfReader.New()

        '''
        This is assuming that the read item is an XdmfDomain object
        '''
        primaryDomain = exampleReader.read("testoutput.xmf")
        outputInformation = primaryDomain.getInformation(0)
        print "Key: " + outputInformation.getKey() + "\nValue: " + outputInformation.getValue()
        print "The Domain's tag is: " + primaryDomain.getItemTag()

        gridHolder = primaryDomain.getGridCollection(0)

        print "The Grid Collection's tag is: " + gridHolder.getItemTag()
        print "The Grid Collection's name is: " + gridHolder.getName()
        outputInformation = gridHolder.getInformation(0)
        print "Key: " + outputInformation.getKey() + "\nValue: " + outputInformation.getValue()

        for property in gridHolder.getItemProperties():
                print property + ": " + gridHolder.getItemProperties()[property]

        if gridHolder.getType() == XdmfGridCollectionType.Spatial():
                print "This is a spatial grid collection"
        else:
                print "This is not a spatial grid collection"

        i = 0
        outstring = ""
        while i < gridHolder.getNumberMaps():
                readMap = gridHolder.getMap(i)
                if not readMap.isInitialized():
                        readMap.read()
                outstring = outstring + "Map # " + str(i) + "\n"
                taskIDMap = readMap.getMap()
                j = 0
                for task in taskIDMap:
                        nodeIDmap = taskIDMap[task]
                        k = 0
                        for node in nodeIDmap:
                                remoteIDmap = nodeIDmap[node]
                                for remote in remoteIDmap:
                                        outstring = outstring + "taskID: " + str(task) + "\tlocalnodeID: " + str(node) +"\tremotenodeID: " + str(remote) + "\n"
                                k = k + 1
                                if k == nodeIDmap.size():
                                        break
                        j = j + 1
                        if j == taskIDMap.size():
                                break
                i = i + 1
        print outstring

        print "Unstructured Grid"
        ungrid = gridHolder.getUnstructuredGrid(0)
        print "The Unstructured Grid's tag is: " + ungrid.getItemTag()
        print "The Unstructured Grid's name is: " + ungrid.getName()
        for property in ungrid.getItemProperties():
                print property + ": " + ungrid.getItemProperties()[property]
        print "The Unstructured Grid's time is: " + str(ungrid.getTime().getValue())
        i = 0
        outstring = ""
        while i < ungrid.getNumberMaps():
                readMap = ungrid.getMap(i)
                if not readMap.isInitialized():
                        readMap.read()
                print "Map # " + str(i)
                taskIDMap = readMap.getMap()
                j = 0
                for task in taskIDMap:
                        nodeIDmap = taskIDMap[task]
                        k = 0
                        for node in nodeIDmap:
                                remoteIDmap = nodeIDmap[node]
                                for remote in remoteIDmap:
                                        outstring = outstring + "taskID: " + str(task) + "\tlocalnodeID: " + str(node) +"\tremotenodeID: " + str(remote) + "\n"
                                k = k + 1
                                if k == nodeIDmap.size():
                                        break
                        j = j + 1
                        if j == taskIDMap.size():
                                break
                i = i + 1
        i = 0
        print outstring
        while i < ungrid.getNumberSets():
                readSet = ungrid.getSet(i)
                if not readSet.isInitialized():
                        readSet.read()
                print "Set # " + str(i)
                print readSet.getName()
                if readSet.getType() == XdmfSetType.Node():
                        print "This set is a node"
                else:
                        print "This set is not a node"
                outputInformation = readSet.getInformation(0)
                print "Key: " + outputInformation.getKey() + "\nValue: " + outputInformation.getValue()
                print readSet.getValuesString()
                j=0
                while j < readSet.getNumberAttributes():
                        readAttribute = readSet.getAttribute(j)
                        if not readAttribute.isInitialized():
                                readAttribute.read()
                        print "Set Attribute # " + str(j)
                        print readAttribute.getName()
                        if readAttribute.getType() == XdmfAttributeType.Scalar():
                                print "This attribute is a scalar"
                        else:
                                print "This attribute is not a scalar"
                        if readAttribute.getCenter() == XdmfAttributeCenter.Node():
                                print "This attribute is a node"
                        else:
                                print "This attribute is not a node"
                        print readAttribute.getValuesString()
                        j = j + 1
                i = i + 1
        i = 0
        while i < ungrid.getNumberAttributes():
                readAttribute = ungrid.getAttribute(i)
                if not readAttribute.isInitialized():
                        readAttribute.read()
                print "Attribute # " + str(i)
                print readAttribute.getName()
                if readAttribute.getType() == XdmfAttributeType.Scalar():
                        print "This attribute is a scalar"
                else:
                        print "This attribute is not a scalar"
                if readAttribute.getCenter() == XdmfAttributeCenter.Node():
                        print "This attribute is a node"
                else:
                        print "This attribute is not a node"
                print readAttribute.getValuesString()
                i = i + 1

        print "Unstructured Topology"
        untopology = ungrid.getTopology()
        if not untopology.isInitialized():
                untopology.read()
        print "The topology's tag: " + untopology.getItemTag()
        if untopology.getType() == XdmfTopologyType.Hexahedron():
                print "This topology is a hexahedron"
        else:
                print "This topology is not a hexahedron"
        print "Contains " + str(untopology.getNumberElements()) + " elements"
        print "Contains the values: " + untopology.getValuesString()

        print "Unstructured Geometry"
        ungeometry = ungrid.getGeometry()
        if not ungeometry.isInitialized():
                ungeometry.read()
        print "The geometry's tag: " +ungeometry.getItemTag()
        if ungeometry.getType() == XdmfGeometryType.XYZ():
                print "This geometry is XYZ"
        else:
                print "This geometry is not XYZ"
        outputInformation = ungeometry.getInformation(0)
        print "Key: " + outputInformation.getKey() + "\nValue: " + outputInformation.getValue()
        print "Contains " + str(ungeometry.getNumberPoints()) + " points"
        print "Contains the values: " + ungeometry.getValuesString()


        print "Curvilinear Grid"
        curvgrid = gridHolder.getCurvilinearGrid(0)
        print "The Curvilinear Grid's tag is: " + curvgrid.getItemTag()
        print "The Curvilinear Grid's name is: " + curvgrid.getName()
        for property in curvgrid.getItemProperties():
                print property + ": " + curvgrid.getItemProperties()[property]
        outputInformation = curvgrid.getInformation(0)
        print "Key: " + outputInformation.getKey() + "\nValue: " + outputInformation.getValue()
        print "The Curvilinear Grid's time is: " + str(curvgrid.getTime().getValue())
        i = 0
        outstring = ""
        while i < curvgrid.getNumberMaps():
                readMap = curvgrid.getMap(i)
                if not readMap.isInitialized():
                        readMap.read()
                print "Map # " + str(i)
                taskIDMap = readMap.getMap()
                j = 0
                for task in taskIDMap:
                        nodeIDmap = taskIDMap[task]
                        k = 0
                        for node in nodeIDmap:
                                remoteIDmap = nodeIDmap[node]
                                for remote in remoteIDmap:
                                        outstring = outstring + "taskID: " + str(task) + "\tlocalnodeID: " + str(node) +"\tremotenodeID: " + str(remote) + "\n"
                                k = k + 1
                                if k == nodeIDmap.size():
                                        break
                        j = j + 1
                        if j == taskIDMap.size():
                                break
                i = i + 1
        i = 0
        print outstring
        while i < curvgrid.getNumberSets():
                readSet = curvgrid.getSet(i)
                if not readSet.isInitialized():
                        readSet.read()
                print "Set # " + str(i)
                print readSet.getName()
                if readSet.getType() == XdmfSetType.Node():
                        print "This set is a node"
                else:
                        print "This set is not a node"
                print readSet.getValuesString()
                j=0
                while j < readSet.getNumberAttributes():
                        readAttribute = readSet.getAttribute(j)
                        if not readAttribute.isInitialized():
                                readAttribute.read()
                        print "Set Attribute # " + str(j)
                        print readAttribute.getName()
                        if readAttribute.getType() == XdmfAttributeType.Scalar():
                                print "This attribute is a scalar"
                        else:
                                print "This attribute is not a scalar"
                        if readAttribute.getCenter() == XdmfAttributeCenter.Node():
                                print "This attribute is a node"
                        else:
                                print "This attribute is not a node"
                        print readAttribute.getValuesString()
                        j = j + 1
                i = i + 1
        i = 0
        while i < curvgrid.getNumberAttributes():
                readAttribute = curvgrid.getAttribute(i)
                if not readAttribute.isInitialized():
                        readAttribute.read()
                print "Attribute # " + str(i)
                print readAttribute.getName()
                if readAttribute.getType() == XdmfAttributeType.Scalar():
                        print "This attribute is a scalar"
                else:
                        print "This attribute is not a scalar"
                if readAttribute.getCenter() == XdmfAttributeCenter.Node():
                        print "This attribute is a node"
                else:
                        print "This attribute is not a node"
                outputInformation = readAttribute.getInformation(0)
                print "Key: " + outputInformation.getKey() + "\nValue: " + outputInformation.getValue()
                print readAttribute.getValuesString()
                i = i + 1

        print "Curvilinear Dimensions"
        curvdimensions = curvgrid.getDimensions()
        if not curvdimensions.isInitialized():
                curvdimensions.read()
        print "The dimensions' tag: " + curvdimensions.getItemTag()
        print "Contains the values: " + curvdimensions.getValuesString()

        print "Curvilinear Geometry"
        curvgeometry = curvgrid.getGeometry()
        if not curvgeometry.isInitialized():
                curvgeometry.read()
        print "The geometry's tag: " + curvgeometry.getItemTag()
        if curvgeometry.getType() == XdmfGeometryType.XYZ():
                print "This geometry is XYZ"
        else:
                print "This geometry is not XYZ"
        outputInformation = curvgeometry.getInformation(0)
        print "Key: " + outputInformation.getKey() + "\nValue: " + outputInformation.getValue()
        print "Contains " + str(curvgeometry.getNumberPoints()) + " points"
        print "Contains the values: " + curvgeometry.getValuesString()


        print "Rectilinear Grid"
        rectgrid = gridHolder.getRectilinearGrid(0)
        print "The Rectilinear Grid's tag is: " + rectgrid.getItemTag()
        print "The Rectilinear Grid's name is: " + rectgrid.getName()
        for property in rectgrid.getItemProperties():
                print property + ": " + rectgrid.getItemProperties()[property]
        print "The Rectilinear Grid's time is: " + str(rectgrid.getTime().getValue())
        i = 0
        outstring = ""
        while i < rectgrid.getNumberMaps():
                readMap = rectgrid.getMap(i)
                if not readMap.isInitialized():
                        readMap.read()
                print "Map # " + str(i)
                taskIDMap = readMap.getMap()
                j = 0
                for task in taskIDMap:
                        nodeIDmap = taskIDMap[task]
                        k = 0
                        for node in nodeIDmap:
                                remoteIDmap = nodeIDmap[node]
                                for remote in remoteIDmap:
                                        outstring = outstring + "taskID: " + str(task) + "\tlocalnodeID: " + str(node) +"\tremotenodeID: " + str(remote) + "\n"
                                k = k + 1
                                if k == nodeIDmap.size():
                                        break
                        j = j + 1
                        if j == taskIDMap.size():
                                break
                i = i + 1
        i = 0
        print outstring
        while i < rectgrid.getNumberSets():
                readSet = rectgrid.getSet(i)
                if not readSet.isInitialized():
                        readSet.read()
                print "Set # " + str(i)
                print readSet.getName()
                if readSet.getType() == XdmfSetType.Node():
                        print "This set is a node"
                else:
                        print "This set is not a node"
                print readSet.getValuesString()
                j=0
                while j < readSet.getNumberAttributes():
                        readAttribute = readSet.getAttribute(j)
                        if not readAttribute.isInitialized():
                                readAttribute.read()
                        print "Set Attribute # " + str(j)
                        print readAttribute.getName()
                        if readAttribute.getType() == XdmfAttributeType.Scalar():
                                print "This attribute is a scalar"
                        else:
                                print "This attribute is not a scalar"
                        if readAttribute.getCenter() == XdmfAttributeCenter.Node():
                                print "This attribute is a node"
                        else:
                                print "This attribute is not a node"
                        print readAttribute.getValuesString()
                        j = j + 1
                i = i + 1
        i = 0
        while i < rectgrid.getNumberAttributes():
                readAttribute = rectgrid.getAttribute(i)
                if not readAttribute.isInitialized():
                        readAttribute.read()
                print "Attribute # " + str(i)
                print readAttribute.getName()
                if readAttribute.getType() == XdmfAttributeType.Scalar():
                        print "This attribute is a scalar"
                else:
                        print "This attribute is not a scalar"
                if readAttribute.getCenter() == XdmfAttributeCenter.Node():
                        print "This attribute is a node"
                else:
                        print "This attribute is not a node"
                outputInformation = readAttribute.getInformation(0)
                print "Key: " + outputInformation.getKey() + "\nValue: " + outputInformation.getValue()
                print readAttribute.getValuesString()
                i = i + 1

        print "Rectilinear Dimensions"
        rectdimensions = rectgrid.getDimensions()
        if not rectdimensions.isInitialized():
                rectdimensions.read()
        print "The dimensions' tag: " + rectdimensions.getItemTag()
        print "Contains the values: " + rectdimensions.getValuesString()

        print "Rectilinear Coordinates"
        rectcoordinates = rectgrid.getCoordinates()
        print "Contains the values: "
        for coordinateaxes in rectcoordinates:
                if not coordinateaxes.isInitialized():
                        coordinateaxes.read()
                print coordinateaxes.getValuesString()

        print "Regular Grid"
        reggrid = gridHolder.getRegularGrid(0)
        print "The Regular Grid's tag is: " + reggrid.getItemTag()
        print "The Regular Grid's name is: " + reggrid.getName()
        for property in reggrid.getItemProperties():
                print property + ": " + reggrid.getItemProperties()[property]
        print "The Regular Grid's time is: " + str(reggrid.getTime().getValue())
        i = 0
        outstring = ""
        while i < reggrid.getNumberMaps():
                readMap = reggrid.getMap(i)
                if not readMap.isInitialized():
                        readMap.read()
                print "Map # " + str(i)
                taskIDMap = readMap.getMap()
                j = 0
                for task in taskIDMap:
                        nodeIDmap = taskIDMap[task]
                        k = 0
                        for node in nodeIDmap:
                                remoteIDmap = nodeIDmap[node]
                                for remote in remoteIDmap:
                                        outstring = outstring + "taskID: " + str(task) + "\tlocalnodeID: " + str(node) +"\tremotenodeID: " + str(remote) + "\n"
                                k = k + 1
                                if k == nodeIDmap.size():
                                        break
                        j = j + 1
                        if j == taskIDMap.size():
                                break
                i = i + 1
        i = 0
        print outstring
        while i < reggrid.getNumberSets():
                readSet = reggrid.getSet(i)
                if not readSet.isInitialized():
                        readSet.read()
                print "Set # " + str(i)
                print readSet.getName()
                if readSet.getType() == XdmfSetType.Node():
                        print "This set is a node"
                else:
                        print "This set is not a node"
                print readSet.getValuesString()
                j=0
                while j < readSet.getNumberAttributes():
                        readAttribute = readSet.getAttribute(j)
                        if not readAttribute.isInitialized():
                                readAttribute.read()
                        print "Set Attribute # " + str(j)
                        print readAttribute.getName()
                        if readAttribute.getType() == XdmfAttributeType.Scalar():
                                print "This attribute is a scalar"
                        else:
                                print "This attribute is not a scalar"
                        if readAttribute.getCenter() == XdmfAttributeCenter.Node():
                                print "This attribute is a node"
                        else:
                                print "This attribute is not a node"
                        print readAttribute.getValuesString()
                        j = j + 1
                i = i + 1
        i = 0
        while i < reggrid.getNumberAttributes():
                readAttribute = reggrid.getAttribute(i)
                if not readAttribute.isInitialized():
                        readAttribute.read()
                print "Attribute # " + str(i)
                print readAttribute.getName()
                if readAttribute.getType() == XdmfAttributeType.Scalar():
                        print "This attribute is a scalar"
                else:
                        print "This attribute is not a scalar"
                if readAttribute.getCenter() == XdmfAttributeCenter.Node():
                        print "This attribute is a node"
                else:
                        print "This attribute is not a node"
                print readAttribute.getValuesString()
                i = i + 1

        print "Regular Brick Size"
        regbricksize = reggrid.getBrickSize()
        if not regbricksize.isInitialized():
                regbricksize.read()
        print "The brick's tag: " + regbricksize.getItemTag()
        print "Contains the values: " + regbricksize.getValuesString()

        print "Regular Number of Points"
        regnumpoints = reggrid.getDimensions()
        if not regnumpoints.isInitialized():
                regnumpoints.read()
        print "The dimensions' tag: " + regnumpoints.getItemTag()
        print "Contains the values: " + regnumpoints.getValuesString()

        print "Regular Origin"
        regorigin = reggrid.getOrigin()
        if not regorigin.isInitialized():
                regorigin.read()
        print "The origin's tag: " + regorigin.getItemTag()
        print "Contains the values: " + regorigin.getValuesString()
