from Xdmf import *

if __name__ == "__main__":
	#//initialization begin

	exampleSet = XdmfSet.New()

	#//initialization end

	#//setName begin

	std::string newName = "New Name";
	exampleSet->setName(newName);

	#//setName end

	#//setType begin

	exampleSet->setType(XdmfSetType::Node());

	#//setType end

	#//getName begin

	exampleName = exampleSet.getName()

	#//getName end

	#//getType begin

	exampleType = exampleSet.getType()

	if exampleType == XdmfSetType.Node():
		#do whatever is to be done if the set is a node

	#//getType end
