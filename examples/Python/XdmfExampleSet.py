from Xdmf import *

if __name__ == "__main__":
	exampleSet = XdmfSet.New()

	std::string newName = "New Name";
	exampleSet->setName(newName);

	exampleSet->setType(XdmfSetType::Node());

	exampleName = exampleSet.getName()

	exampleType = exampleSet.getType()
