from Xdmf import *

if __name__ == "__main__":
        #Assume that exampleItem is a shared pointer to the ParentClass object
        #Using an XdmfInformation as an example because all XdmfItems have XdmfInformation as a child class

        exampleItem = XdmfInformation.New("Parent", "This is a parent information")
        addChild = XdmfInformation.New("Child", "This is a child information")

        exampleItem.insert(addChild)

        getIndex = 0
        exampleChild = exampleItem.getInformation(getIndex)

        findingInfo = "Find this"
        exampleStringChild = exampleItem.getInformation(findingInfo)

        exampleSize = exampleItem.getNumberInformations()

        removeIndex = 0
        exampleItem.removeInformation(removeIndex)

        removeInfo = "Remove this"
        exampleItem.removeInformation(removeInfo)

        #//initialization begin

        #Using a shared pointer to an XdmfDomain object as an example

        exampleDomain = XdmfDomain.New()

        #//initialization end

        #//getItemTag begin

        exampleTag = exampleDomain.getItemTag()

        #//getItemTag end

        #//getItemProperties begin

        propertyMap = exampleDomain.getItemProperties()

        #//getItemProperties end

        #//traverse begin

        writePath = "file path here"
        exampleWriter = XdmfWriter.New(writepath)
        exampleDomain.traverse(exampleWriter)

        #//traverse end
