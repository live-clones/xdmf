from Xdmf import *

import timeit
import time
import os

h5file = os.getcwd() + "/timestamptest.h5"
xmffile = os.getcwd() + "/timestamptest.xmf"

if __name__ == "__main__":

	#for later use in determining actual time
	#    print(timeit.Timer(timedWrite.write).timeit(1))

        exampleHeavyWriter = XdmfHDF5Writer.New(h5file)

        #possible options
        exampleHeavyWriter.setReleaseData(True)

        exampleWriter = XdmfWriter.New(xmffile, exampleHeavyWriter)
        exampleWriter.setLightDataLimit(10)


	primaryDomain = XdmfDomain.New()

	numNodes = 30000
	numAttrib = 10
	gridsPerTimestamp = 10
	numTimestamps = 100

	#for 100 time steps
	#each time step is a grid collection
	#base structue is a spacial grid collection full of temporal grid collections

	primaryCollection = XdmfGridCollection.New()

	primaryDomain.insert(primaryCollection)

	startclock = time.clock()

	for i in range(numTimestamps):
		timestampGrid = XdmfGridCollection.New()
		timestampTime = XdmfTime.New(i)
		timestampGrid.setTime(timestampTime)
		#each time stamp has 10 grids
		for j in range(gridsPerTimestamp):
			sectionGrid = XdmfUnstructuredGrid.New()
			sectionTime = XdmfTime.New(i)
			sectionGrid.setTime(sectionTime)
			#each grid has one topo, one geo, and 10 attrib
			sectionGeometry = XdmfGeometry.New()
			sectionGeometry.setType(XdmfGeometryType.XYZ())
			sectionTopology = XdmfTopology.New()
			sectionTopology.setType(XdmfTopologyType.Triangle())
			#Fill each with 30,000 values
			#for k in range(numNodes):
			#	sectionGeometry.pushBackAsFloat32(k)
			#	sectionTopology.pushBackAsFloat32(k)
			sectionGeometry.resizeAsFloat32(30000)
			sectionTopology.resizeAsFloat32(30000)
			sectionGrid.setGeometry(sectionGeometry)
			sectionGrid.setTopology(sectionTopology)
			for k in range(numAttrib):
				sectionAttribute = XdmfAttribute.New()
				#for l in range(numNodes):
				#	sectionAttribute.pushBackAsFloat32(l)
				sectionAttribute.resizeAsFloat32(30000)
				sectionGrid.insert(sectionAttribute)
			timestampGrid.insert(sectionGrid)
			#exampleHeavyWriter.openFile()
			#sectionGrid.accept(exampleHeavyWriter)
			#exampleHeavyWriter.closeFile()
		primaryCollection.insert(timestampGrid)
		writeclock = time.clock()
		exampleHeavyWriter.openFile()
		#primaryDomain.accept(exampleHeavyWriter)
		if i % 10 == 9:
			primaryDomain.accept(exampleWriter)
		else:
			timestampGrid.accept(exampleHeavyWriter)
		exampleHeavyWriter.closeFile()
		print("iteration " + str(i) + " Time = " + str(time.clock() - writeclock))


	exampleHeavyWriter.openFile()
	primaryDomain.accept(exampleWriter)

	exampleHeavyWriter.closeFile()

	print((time.clock() - startclock))

	os.remove(xmffile)
	os.remove(h5file)
