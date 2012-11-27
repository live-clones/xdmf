#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

int main(int, char **)
{
	shared_ptr<XdmfTopology> exampleTopology = XdmfTopology::New();

	exampleTopology->setType(XdmfTopologyType::Pyramid());

	shared_ptr<const XdmfTopologyType> exampleType = exampleTopology->getType();

	unsigned int numElements = exampleTopology->getNumberElements();

	return 0;
}
