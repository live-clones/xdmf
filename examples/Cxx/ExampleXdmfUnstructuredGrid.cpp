#include "XdmfUnstructuredGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfGeometry.hpp"

int main(int, char **)
{
	//#initialization begin

	shared_ptr<XdmfUnstructuredGrid> exampleGrid = XdmfUnstructuredGrid::New();

	//#initialization end

	//#initializationregular begin

	double newBrickX = 0.0;
	double newBrickY = 0.0;
	unsigned int newPointsX = 5;
	unsigned int newPointsY = 5;
	double newOriginX = 20.0;
	double newOriginY = 20.0;
	shared_ptr<XdmfRegularGrid> baseGrid = XdmfRegularGrid::New(newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY);
	shared_ptr<XdmfUnstructuredGrid> regGeneratedGrid = XdmfUnstructuredGrid::New(baseGrid);

	//#initializationregular end

	//#setGeometry begin

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

	//#setGeometry end

	//#setTopology begin

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

	//#setTopology end

	//#getGeometry begin

	shared_ptr<XdmfGeometry> exampleGeometry = exampleGrid->getGeometry();

	//#getGeometry end

	//#getTopology begin

	shared_ptr<XdmfTopology> exampleTopology = exampleGrid->getTopology();

	//#getTopology end

	return 0;
}
