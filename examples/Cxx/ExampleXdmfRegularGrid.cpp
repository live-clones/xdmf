#include "XdmfArray.hpp"
#include "XdmfRegularGrid.hpp"

int main(int, char **)
{
	int size = 2;

	double newBrickX = 20.0;
        unsigned int newPointsX = 5;
        double newOriginX = 0;
	double newBrickY = 20.0;
	unsigned int newPointsY = 5;
	double newOriginY = 0;
	double newBrickZ = 20.0;
	unsigned int newPointsZ = 5;
	double newOriginZ = 0;

	if (size==2)
	{

	shared_ptr<XdmfRegularGrid> exampleGrid = XdmfRegularGrid::New(
		newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY);
	shared_ptr<XdmfArray> exampleBrick = exampleGrid->getBrickSize();
	shared_ptr<const XdmfArray> exampleBrickConst = exampleGrid->getBrickSize();
	shared_ptr<XdmfArray> exampleDimensions = exampleGrid->getDimensions();
	shared_ptr<const XdmfArray> exampleDimensionsConst = exampleGrid->getDimensions();
	shared_ptr<XdmfArray> exampleOrigin = exampleGrid->getOrigin();
	shared_ptr<const XdmfArray> exampleOriginConst = exampleGrid->getOrigin();

	}
	else if (size==3)
	{

	shared_ptr<XdmfRegularGrid> exampleGrid = XdmfRegularGrid::New(
		newBrickX, newBrickY, newBrickZ, newPointsX, newPointsY, newPointsZ, newOriginX, newOriginY, newOriginZ);

	}
	else //mutable size
	{

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

	exampleGrid->setBrickSize(newBrickSize);
	exampleGrid->setDimensions(newNumPoints);
	exampleGrid->setOrigin(newGridOrigin);

	}

	return 0;
}
