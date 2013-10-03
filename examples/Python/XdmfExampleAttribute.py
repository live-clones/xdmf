from Xdmf import *

if __name__ == "__main__":

        #//initialization begin
        exampleAttribute = XdmfAttribute.New()
        #//initialization end

        #Assuming that exampleAttribute is a shared pointer to an XdmfAttribute object

        #//setCenter begin

        exampleAttribute.setCenter(XdmfAttributeCenter.Node())

        #//setCenter end

        #//setName begin

        newName = "New Name"
        exampleAttribute.setName(newName)

        #//setNAme end

        #//setType begin

        exampleAttribute.setType(XdmfAttributeType.Scalar())

        #//setType end

        #//getCenter begin

        exampleCenter = exampleAttribute.getCenter()

        if exampleCenter == XdmfAttributeCenter.Grid():
                #do whatever is to be done if the center is a grid

        #//getCenter end

        #//getName begin

        exampleName = exampleAttribute.getName()

        #//getName end

        #//getType begin

        exampleType = exampleAttribute.getType()

        if exampleType == XdmfAttributeType.Scalar():
                #do whatever is to be done if the Type is a Scalar

        #//getType end
