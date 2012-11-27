from Xdmf import *

if __name__ == "__main__":
	exampleTopology = XdmfTopology.New()

	exampleTopology.setType(XdmfTopologyType.Pyramid())

	exampleType = exampleTopology.getType()

	numElements = exampleTopology.getNumberElements()
