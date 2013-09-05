from Xdmf import *

if __name__ == "__main__":

        #//initialization begin

        exampleGeometry = XdmfGeometry.New()

        #//initialization end

        #Assuming that exampleGeometry is a shared pointer to a XdmfGeometry object

        #//setType begin

        exampleGeometry.setType(XdmfGeometryType.XYZ())

        #//setType end

        #//getType begin

        exampleType = exampleGeometry.getType()

        #//getType end

        #//getNumberPoints begin

        numPoints = exampleGeometry.getNumberPoints()

        #//getNumberPoints end
