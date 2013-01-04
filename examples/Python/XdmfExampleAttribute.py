from Xdmf import *

if __name__ == "__main__":
	exampleAttribute = XdmfAttribute.New()

	#Assuming that exampleAttribute is a shared pointer to an XdmfAttribute object

	exampleAttribute.setCenter(XdmfAttributeCenter.Node())
	newName = "New Name"
	exampleAttribute.setName(newName)
	exampleAttribute.setType(XdmfAttributeType.Scalar())

	exampleCenter = exampleAttribute.getCenter()
	exampleName = exampleAttribute.getName()
	exampleType = exampleAttribute.getType()

