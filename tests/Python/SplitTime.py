from Xdmf import *
import XdmfCore
import timeit
import os
import time

numberArrays = 10000
arraySize = 100

class TimedWrite():
    def __init__(self):
        self.domain = XdmfDomain.New()
        grid = XdmfUnstructuredGrid.New()
        self.domain.insert(grid)
        dimensionArray = UInt32Vector()
	dimensionArray.push_back(arraySize)
	dimensionArray.push_back(arraySize)
        for i in range(0, numberArrays):
            attribute = XdmfAttribute.New()
            attribute.resizeAsFloat64(dimensionArray)
            grid.insert(attribute)

        hdf5Writer = XdmfHDF5Writer.New("/usr/var/tmp/ajburns/timing.h5")
        hdf5Writer.setFileSizeLimit(10000)
        self.writer = XdmfWriter.New("/usr/var/tmp/ajburns/timing.xmf",
                                     hdf5Writer)
	self.writer.setLightDataLimit(10)

    def write(self):
        self.domain.accept(self.writer)
   
if __name__ == "__main__":
    timedWrite = TimedWrite()
    startclock = time.clock()
    print timeit.Timer(timedWrite.write).timeit(1)
    print (time.clock() - startclock)
    os.remove("/usr/var/tmp/ajburns/timing.xmf")
    os.remove("/usr/var/tmp/ajburns/timing.h5")
