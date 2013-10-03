from Xdmf import *

if __name__ == "__main__":
        #Assuming that exampleCollection is a shared pointer to an XdmfGridCollection object

        #//initialization begin

        exampleCollection = XdmfGridCollection.New()

        #//initialization end

        #//setType begin

        exampleCollection.setType(XdmfGridCollectionType.Temporal())

        #//setType end

        #//getType begin

        exampleType = exampleCollection.getType()

        if exampleType == XdmfGridCollectionType.Temporal():
                #do whatever is to be done if the grid collection is temporal

        #//getType end

        #//insert begin

        exampleInformation = XdmfInformation.New()
        newKey = "New Key"
        newValue = "New Value"
        exampleInformation.setKey(newKey)
        exampleInformation.setValue(newValue)
        exampleCollection.insert(exampleInformation)

        #//insert end
