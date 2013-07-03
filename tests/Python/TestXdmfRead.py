from Xdmf import *

if __name__ == "__main__":

    fileName ="TestXdmfRead.xmf"

    # create a simple empty file
    domain = XdmfDomain.New()
    collection = XdmfGridCollection.New()
    grid = XdmfUnstructuredGrid.New()
    attribute1 = XdmfAttribute.New()
    attribute2 = XdmfAttribute.New()
    information = XdmfInformation.New()

    domain.insert(collection)
    collection.insert(grid)
    grid.insert(attribute1)
    grid.insert(attribute2)
    grid.insert(information)
    
    writer = XdmfWriter.New(fileName)
    domain.accept(writer)

    # read file using XPaths and verify downcasts to appropriate XdmfItems
    reader = XdmfReader.New()

    domain = reader.read(fileName, "/Xdmf/Domain")
    assert(len(domain) == 1)
    assert(isinstance(domain[0], XdmfDomain))

    collection = reader.read(fileName, "/Xdmf/Domain/Grid")
    assert(len(collection) == 1)
    assert(isinstance(collection[0], XdmfGridCollection))
    
    grid = reader.read(fileName, "/Xdmf/Domain/Grid/Grid")
    assert(len(grid) == 1)
    assert(isinstance(grid[0], XdmfUnstructuredGrid))

    attributes = reader.read(fileName, "/Xdmf/Domain/Grid/Grid/Attribute")
    assert(len(attributes) == 2)
    assert(isinstance(attributes[0], XdmfAttribute))
    assert(isinstance(attributes[1], XdmfAttribute))

    information = reader.read(fileName, "/Xdmf/Domain/Grid/Grid/Information")
    assert(len(information) == 1)
    assert(isinstance(information[0], XdmfInformation))

