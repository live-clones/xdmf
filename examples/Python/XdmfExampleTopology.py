from Xdmf import *

if __name__ == "__main__":
	exampleTopology = XdmfTopology.New()

	#Assuming that exampleTopology is a shared pointer to an XdmfTopology object

	exampleTopology.setType(XdmfTopologyType.Pyramid())

	exampleType = exampleTopology.getType()

	numElements = exampleTopology.getNumberElements()
