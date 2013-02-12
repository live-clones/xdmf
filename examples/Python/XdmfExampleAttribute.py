from Xdmf import *

if __name__ == "__main__":
	exampleAttribute = XdmfAttribute.New()

	#Assuming that exampleAttribute is a shared pointer to an XdmfAttribute object

	exampleAttribute.setCenter(XdmfAttributeCenter.Node())
	newName = "New Name"
	exampleAttribute.setName(newName)
	exampleAttribute.setType(XdmfAttributeType.Scalar())

	exampleCenter = exampleAttribute.getCenter()

	if exampleCenter == XdmfAttributeCenter.Grid():
		#do whatever is to be done if the center is a grid

	exampleName = exampleAttribute.getName()
	exampleType = exampleAttribute.getType()

	if exampleType == XdmfAttributeType.Scalar():
		#do whatever is to be done if the Type is a Scalar
