from Xdmf import *

if __name__ == "__main__":

        #//getDimensions begin

        exampleDimensions = XdmfGeometryType.XYZ().getDimensions()

        #//getDimensions end

        #//getName begin

        exampleName = XdmfGeometryType.XYZ().getName()

        #//getName end

        #//getType begin

        exampleGeometry = XdmfGeometry.New()

        exampleType = exampleGeometry.getType()

        if exampleType == XdmfGeometryType.XYZ():
                #do whatever is to be done if the geometry is XYZ

        #//getType end
