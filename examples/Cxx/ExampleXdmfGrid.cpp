#include "XdmfDomain.hpp"

int main(int, char **)
{
	shared_ptr<XdmfUnstructuredGrid> exampleGrid = XdmfUnstructuredGrid::New();

	//Using an unstructured grid since XdmfGrid is an abstract class
	//Assumming that exampleGrid is a shared pointer to an XdmfUnstructuredGrid object
	
	std::string newName = "New Name";
	exampleGrid->setName(newName);

	shared_ptr<XdmfTime> newTime = XdmfTime::New(20.0);
	exampleGrid->setTime(newTime);

	shared_ptr< XdmfTime> exampleTime = exampleGrid->getTime();

	shared_ptr<const XdmfTime> exampleTimeConst = exampleGrid->getTime();

	std::string exampleName = exampleGrid->getName();

	shared_ptr<const XdmfGeometry> exampleGeometry = exampleGrid->getGeometry();

	shared_ptr<const XdmfTopology> exampleTopology = exampleGrid->getTopology();

	return 0;
}
