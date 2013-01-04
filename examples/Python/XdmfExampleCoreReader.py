from Xdmf import *

if __name__ == "__main__":
	#using XdmfReader since XdmfCoreReader is abstract
	exampleReader = XdmfReader.New()


	readLight = "<dataitem>1 1 1 1 1 1 3 5 7 4 2</dataitem>"
	exampleItem = exampleReader.parse(readLight)

	readPath = "your file path here";
	exampleItem = exampleReader.read(readPath)
	exampleCollection = exampleReader.readItems(readPath)

	readXPath = "your X path here"
	exampleItems = exampleReader.read(readPath, readXPath)
