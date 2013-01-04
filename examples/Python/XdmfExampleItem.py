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

	#Using a shared pointer to an XdmfDomain object as an example

	exampleDomain = XdmfDomain.New()
	exampleTag = exampleDomain.getItemTag()
	propertyMap = exampleDomain.getItemProperties()
	writePath = "file path here"
	exampleWriter = XdmfWriter.New(writepath)
	exampleDomain.traverse(exampleWriter)
