from Xdmf import *

if __name__ == "__main__":
	#Assuming that exampleCollection is a shared pointer to an XdmfGridCollection object

	exampleCollection = XdmfGridCollection.New()

	exampleCollection.setType(XdmfGridCollectionType.Temporal())

	exampleType = exampleCollection.getType()

	if exampleType == XdmfGridCollectionType.Temporal():
		#do whatever is to be done if the grid collection is temporal

	exampleInformation = XdmfInformation.New()
	newKey = "New Key"
	newValue = "New Value"
	exampleInformation.setKey(newKey)
	exampleInformation.setValue(newValue)
	exampleCollection.insert(exampleInformation)
