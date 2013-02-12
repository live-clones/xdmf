from Xdmf import *

if __name__ == "__main__":
	#Assuming that exampleSet is a shared pointer to an XdmfSet object

	exampleSet = XdmfSet.New()

	std::string newName = "New Name";
	exampleSet->setName(newName);

	exampleSet->setType(XdmfSetType::Node());

	exampleName = exampleSet.getName()

	exampleType = exampleSet.getType()

	if exampleType == XdmfSetType.Node():
		#do whatever is to be done if the set is a node
