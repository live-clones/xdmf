#include "XdmfRectilinearGrid.hpp"
#include "XdmfArray.hpp"

int main(int, char **)
{
	int size = 2;

	//Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid

	//#initvalues begin

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

	//#initvalues end

	if (size==2)
	{

	//#initialization2 begin

	shared_ptr<XdmfRectilinearGrid> exampleGrid = XdmfRectilinearGrid::New(pointsXArray, pointsYArray);

	//#initialization2 end

	//#getCoodinatessingle begin

	shared_ptr<XdmfArray> readPointsX = exampleGrid->getCoordinates(0);
	shared_ptr<XdmfArray> readPointsY = exampleGrid->getCoordinates(1);

	//#getCoordinatessingle end

	//#getCoordinatessingleconst begin

	shared_ptr<const XdmfArray> readPointsXConst = exampleGrid->getCoordinates(0);
        shared_ptr<const XdmfArray> readPointsYConst = exampleGrid->getCoordinates(1);

	//#getCoordinatessingleconst end

	}
        else if (size==3)
        {

	//#initialization3 begin

	shared_ptr<XdmfRectilinearGrid> exampleGrid = XdmfRectilinearGrid::New(pointsXArray, pointsYArray, pointsZArray);

	//#initialization3 end

	//#setCoordinatessingle begin

	exampleGrid->setCoordinates(0, pointsXArray);

	//#setCoordinatessingle end

        }
        else //mutable size
        {

	//#initializationvector begin

	std::vector<shared_ptr<XdmfArray> > pointsCollector;
	pointsCollector.push_back(pointsXArray);
	pointsCollector.push_back(pointsYArray);
	pointsCollector.push_back(pointsZArray);
	shared_ptr<XdmfRectilinearGrid> exampleGrid = XdmfRectilinearGrid::New(pointsCollector);

	//#initializationvector end

	//#setCoordinatesvector begin

	exampleGrid->setCoordinates(pointsCollector);

	//#setCoordinatesvector end

	//#getDimensions begin

	shared_ptr<XdmfArray> exampleDimensions = exampleGrid->getDimensions();

	//#getDimensions end

	//#getDimensionsconst begin

	shared_ptr<const XdmfArray> exampleDimensionsConst = exampleGrid->getDimensions();

	//#getDimensionsconst end

	//#getCoordinatesvector begin

	std::vector<shared_ptr<XdmfArray> > exampleCoordinates = exampleGrid->getCoordinates();

	//#getCoordinatesvector end

	//#getCoordinatesvectorconst begin

	const std::vector<shared_ptr<XdmfArray> > exampleCoordinatesConst = exampleGrid->getCoordinates();

	//#getCoordinatesvectorconst end

        }
	return 0;
}
