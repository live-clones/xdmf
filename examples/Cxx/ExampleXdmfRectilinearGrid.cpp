#include "XdmfRectilinearGrid.hpp"
#include "XdmfArray.hpp"

int main(int, char **)
{
	int size = 2;

	//Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid

        shared_ptr<XdmfArray> pointsXArray = XdmfArray::New();
        pointsXArray->pushBack(5);
        pointsXArray->pushBack(6);
        pointsXArray->pushBack(7);
        pointsXArray->pushBack(8);
        pointsXArray->pushBack(9);
        pointsXArray->pushBack(10);
        shared_ptr<XdmfArray> pointsYArray = XdmfArray::New();
        pointsYArray->pushBack(3);
        pointsYArray->pushBack(6);
        pointsYArray->pushBack(4);
        pointsYArray->pushBack(8);
        pointsYArray->pushBack(7);
        pointsYArray->pushBack(10);
        shared_ptr<XdmfArray> pointsZArray = XdmfArray::New();
        pointsZArray->pushBack(3);
        pointsZArray->pushBack(9);
        pointsZArray->pushBack(4);
        pointsZArray->pushBack(5);
        pointsZArray->pushBack(7);
        pointsZArray->pushBack(2);

	if (size==2)
	{

	shared_ptr<XdmfRectilinearGrid> exampleGrid = XdmfRectilinearGrid::New(pointsXArray, pointsYArray);
	shared_ptr<XdmfArray> readPointsX = exampleGrid->getCoordinates(0);
	shared_ptr<XdmfArray> readPointsY = exampleGrid->getCoordinates(1);
	shared_ptr<const XdmfArray> readPointsXConst = exampleGrid->getCoordinates(0);
        shared_ptr<const XdmfArray> readPointsYConst = exampleGrid->getCoordinates(1);

	}
        else if (size==3)
        {

	shared_ptr<XdmfRectilinearGrid> exampleGrid = XdmfRectilinearGrid::New(pointsXArray, pointsYArray, pointsZArray);
	exampleGrid->setCoordinates(0, pointsXArray);

        }
        else //mutable size
        {

	std::vector<shared_ptr<XdmfArray> > pointsCollector;
	pointsCollector.push_back(pointsXArray);
	pointsCollector.push_back(pointsYArray);
	pointsCollector.push_back(pointsZArray);
	shared_ptr<XdmfRectilinearGrid> exampleGrid = XdmfRectilinearGrid::New(pointsCollector);
	exampleGrid->setCoordinates(pointsCollector);
	shared_ptr<XdmfArray> exampleDimensions = exampleGrid->getDimensions();
	shared_ptr<const XdmfArray> exampleDimensionsConst = exampleGrid->getDimensions();
	std::vector<shared_ptr<XdmfArray> > exampleCoordinates = exampleGrid->getCoordinates();
	const std::vector<shared_ptr<XdmfArray> > exampleCoordinatesConst = exampleGrid->getCoordinates();

        }
	return 0;
}
