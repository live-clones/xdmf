from Xdmf import *

if __name__ == "__main__":
	#//initialization begin

	exampleTopology = XdmfTopology.New()

	#//initialization end

	#//setType begin

	exampleTopology.setType(XdmfTopologyType.Pyramid())

	#//setType end

	#//getType begin

	exampleType = exampleTopology.getType()

	#//getType end

	#//getNumberElements begin

	numElements = exampleTopology.getNumberElements()

	#//getNumberElements end
