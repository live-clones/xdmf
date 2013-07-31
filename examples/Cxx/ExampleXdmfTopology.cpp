#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

int main(int, char **)
{
	//#initialization begin

	shared_ptr<XdmfTopology> exampleTopology = XdmfTopology::New();

	//#initialization end

	//#setType begin

	exampleTopology->setType(XdmfTopologyType::Pyramid());

	//#setType end

	//#getType begin

	shared_ptr<const XdmfTopologyType> exampleType = exampleTopology->getType();

	//#getType end

	//#getNumberElements begin

	unsigned int numElements = exampleTopology->getNumberElements();

	//#getNumberElements end

	return 0;
}
