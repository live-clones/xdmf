#include "XdmfArray.hpp"
#include "XdmfRegularGrid.hpp"

int main(int, char **)
{
	int size = 2;

	//Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object

	//#initvalue begin

	double newBrickX = 20.0;
        unsigned int newPointsX = 5;
        double newOriginX = 0;
	double newBrickY = 20.0;
	unsigned int newPointsY = 5;
	double newOriginY = 0;
	double newBrickZ = 20.0;
	unsigned int newPointsZ = 5;
	double newOriginZ = 0;

	//#initvalue end

	if (size==2)
	{

	//#initialization2 begin

	shared_ptr<XdmfRegularGrid> exampleGrid = XdmfRegularGrid::New(
		newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY);

	//#initialization2 end

	//#getBrickSize begin

	shared_ptr<XdmfArray> exampleBrick = exampleGrid->getBrickSize();

	//#getBrickSize end

	//#getBrickSizeconst begin

	shared_ptr<const XdmfArray> exampleBrickConst = exampleGrid->getBrickSize();

	//#getBrickSizeconst end

	//#getDimensions begin

	shared_ptr<XdmfArray> exampleDimensions = exampleGrid->getDimensions();

	//#getDimensions end

	//#getDimensionsconst begin

	shared_ptr<const XdmfArray> exampleDimensionsConst = exampleGrid->getDimensions();

	//#getDimensionsconst end

	//#getOrigin begin

	shared_ptr<XdmfArray> exampleOrigin = exampleGrid->getOrigin();

	//#getOrigin end

	//#getOriginconst begin

	shared_ptr<const XdmfArray> exampleOriginConst = exampleGrid->getOrigin();

	//#getOriginconst end

	}
	else if (size==3)
	{

	//#initialization3 begin

	shared_ptr<XdmfRegularGrid> exampleGrid = XdmfRegularGrid::New(
		newBrickX, newBrickY, newBrickZ, newPointsX, newPointsY, newPointsZ, newOriginX, newOriginY, newOriginZ);

	//#initialization3 end

	}
	else //mutable size
	{

	//#initializationvector begin

	shared_ptr<XdmfArray> newBrickSize = XdmfArray::New();
	newBrickSize->pushBack(20.0);
	newBrickSize->pushBack(21.0);
	newBrickSize->pushBack(22.0);
	shared_ptr<XdmfArray> newNumPoints = XdmfArray::New();
	newNumPoints->pushBack(5);
	newNumPoints->pushBack(6);
	newNumPoints->pushBack(7);
	shared_ptr<XdmfArray> newGridOrigin = XdmfArray::New();
	newGridOrigin->pushBack(0.0);
	newGridOrigin->pushBack(1.0);
	newGridOrigin->pushBack(2.0);
	shared_ptr<XdmfRegularGrid> exampleGrid = XdmfRegularGrid::New(newBrickSize, newNumPoints, newGridOrigin);

	//#initializationvector end

	//#setBrickSize begin

	exampleGrid->setBrickSize(newBrickSize);

	//#setBrickSize end

	//#setDimensions begin

	exampleGrid->setDimensions(newNumPoints);

	//#setDimensions end

	//#setOrigin begin

	exampleGrid->setOrigin(newGridOrigin);

	//#setOrigin end

	}

	return 0;
}
