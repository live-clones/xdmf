from Xdmf import *

if __name__ == "__main__":
	exampleAttribute = XdmfAttribute.New()

	exampleAttribute.setCenter(XdmfAttributeCenter.Node())
	newName = "New Name"
	exampleAttribute.setName(newName)
	exampleAttribute.setType(XdmfAttributeType.Node())

	exampleCenter = exampleAttribute.getCenter()
	exampleName = exampleAttribute.getName()
	exampleType = exampleAttribute.getType()

