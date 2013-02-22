import Xdmf
import timeit
import os
import time

numberArrays = 10000
arraySize = 10000

class TimedWrite():
    def __init__(self):
        self.domain = Xdmf.XdmfDomain.New()
        grid = Xdmf.XdmfUnstructuredGrid.New()
        self.domain.insert(grid)
                
        for i in range(0, numberArrays):
            attribute = Xdmf.XdmfAttribute.New()
            attribute.resizeAsFloat64(arraySize)
            grid.insert(attribute)

        hdf5Writer = Xdmf.XdmfHDF5Writer.New("/usr/var/tmp/ajburns/timing.h5")
        hdf5Writer.setFileSizeLimit(10000)
        self.writer = Xdmf.XdmfWriter.New("/usr/var/tmp/ajburns/timing.xmf",
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
