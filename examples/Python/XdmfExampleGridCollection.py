from Xdmf import *

if __name__ == "__main__":
	exampleCollection = XdmfGridCollection.New()

	exampleCollection.setType(XdmfGridCollectionType.Temporal())

	exampleType = exampleCollection.getType()

	exampleInformation = XdmfInformation.New()
	newKey = "New Key"
	newValue = "New Value"
	exampleInformation.setKey(newKey)
	exampleInformation.setValue(newValue)
	exampleCollection.insert(exampleInformation)
