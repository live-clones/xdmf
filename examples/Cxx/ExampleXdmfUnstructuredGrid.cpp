#include "XdmfUnstructuredGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfGeometry.hpp"

int main(int, char **)
{
	//Assuming that exampleGrid is a shared pointer to an XdmfUnstructuredGrid object

	shared_ptr<XdmfUnstructuredGrid> exampleGrid = XdmfUnstructuredGrid::New();

	double newBrickX = 0.0;
	double newBrickY = 0.0;
	unsigned int newPointsX = 5;
	unsigned int newPointsY = 5;
	double newOriginX = 20.0;
	double newOriginY = 20.0;
	shared_ptr<XdmfRegularGrid> baseGrid = XdmfRegularGrid::New(newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY);
	shared_ptr<XdmfUnstructuredGrid> regGeneratedGrid = XdmfUnstructuredGrid::New(baseGrid);

	shared_ptr<XdmfGeometry> newGeometry = XdmfGeometry::New();
	newGeometry->setType(XdmfGeometryType::XYZ());
	newGeometry->pushBack(1);
	newGeometry->pushBack(2);
	newGeometry->pushBack(3);
	newGeometry->pushBack(4);
	newGeometry->pushBack(5);
	newGeometry->pushBack(6);
	newGeometry->pushBack(7);
	newGeometry->pushBack(8);
	newGeometry->pushBack(9);
	exampleGrid->setGeometry(newGeometry);

	shared_ptr<XdmfTopology> newTopology = XdmfTopology::New();
	newTopology->setType(XdmfTopologyType::Triangle());
	newTopology->pushBack(1);
	newTopology->pushBack(2);
	newTopology->pushBack(3);
	newTopology->pushBack(4);
	newTopology->pushBack(5);
	newTopology->pushBack(6);
	newTopology->pushBack(7);
	newTopology->pushBack(8);
	newTopology->pushBack(9);
	exampleGrid->setTopology(newTopology);

	shared_ptr<XdmfGeometry> exampleGeometry = exampleGrid->getGeometry();
	shared_ptr<XdmfTopology> exampleTopology = exampleGrid->getTopology();

	return 0;
}
