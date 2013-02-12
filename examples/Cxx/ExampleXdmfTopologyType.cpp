#include "XdmfTopologyType.hpp"
#include "XdmfTopology.hpp"

int main(int, char **)
{
	unsigned int exampleID = XdmfTopologyType::Triangle()->getID();
	shared_ptr<XdmfTopology> createdTopology = XdmfTopology::New();
	createdTopology->setType(XdmfTopologyType::New(exampleID));

	XdmfTopologyType::CellType exampleType = XdmfTopologyType::Linear;
	//Assuming that exampleTopology is a shared pointer to a filled XdmfTopology object
	if (exampleType == exampleTopology->getCellType())
	{
		//Do whatever is to be done if the cell type is linear
	}

	unsigned int numEdges = XdmfTopologyType::Triangle()->getEdgesPerElement();

	unsigned int numFaces = XdmfTopologyType::Triangle()->getFacesPerElement();

	unsigned int holdID = XdmfTopologyType::Triangle()->getID();

	std::string exampleName = XdmfTopologyType::Triangle()->getName();

	unsigned int numNodes = XdmfTopologyType::Triangle()->getNodesPerElement();

	if (createdTopology->getType() == XdmfTopologyType::Triangle())
	{
		//Do whatever is to be done if the type is Triangle
	}

	return 0;
}
