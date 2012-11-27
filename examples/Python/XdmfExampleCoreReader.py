from Xdmf import *

if __name__ == "__main__":
	#using XdmfReader since XdmfCoreReader is abstract
	exampleReader = XdmfReader.New()


	readLight = "your light data here"
	exampleItem = reader->parse(readLight)

	readPath = "your file path here";
	exampleItem = reader->read(readPath)
	exampleCollection = reader.readItems(readPath)

	readXPath = "your X path here"
	exampleItems = reader.read(readPath, readXPath)
