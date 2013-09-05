from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        #using XdmfReader since XdmfCoreReader is abstract
        exampleReader = XdmfReader.New()

        #//initialization end

        #//parse begin

        readLight = "<dataitem>1 1 1 1 1 1 3 5 7 4 2</dataitem>"
        exampleItem = exampleReader.parse(readLight)

        #//parse end

        #//readpath begin

        readPath = "your file path here";

        #//readpath end

        #//readroot begin

        exampleItem = exampleReader.read(readPath)

        #//readroot end

        #//readItems begin

        exampleCollection = exampleReader.readItems(readPath)

        #//readItems end

        #//readXPath begin

        readXPath = "your X path here"
        exampleItems = exampleReader.read(readPath, readXPath)

        #//readXPath end
