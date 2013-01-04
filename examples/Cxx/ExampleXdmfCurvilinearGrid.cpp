#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfArray.hpp"

int main(int, char **)
{
	int size = 2;

	//Assuming that exampleGrid is a shared pointer to an XdmfCurvilinearGrid object

	if (size == 2)
        {

	unsigned int newPointsX = 5;
	unsigned int newPointsY = 5;
	shared_ptr<XdmfCurvilinearGrid> exampleGrid = XdmfCurvilinearGrid::New(newPointsX, newPointsY);
	shared_ptr<XdmfArray> exampleDimensions = exampleGrid->getDimensions();

        }
	else if (size = 3)
	{

	unsigned int newPointsX = 5;
	unsigned int newPointsY = 5;
	unsigned int newPointsZ = 5;
	shared_ptr<XdmfCurvilinearGrid> exampleGrid = XdmfCurvilinearGrid::New(newPointsX, newPointsY, newPointsZ);
	shared_ptr<const XdmfArray> exampleDimensions = exampleGrid->getDimensions();
	shared_ptr<XdmfArray> newPoints = XdmfArray::New();
	newPoints->pushBack(10);
	newPoints->pushBack(10);
	newPoints->pushBack(10);
	exampleGrid->setDimensions(newPoints);
	shared_ptr<XdmfArray> newData = XdmfArray::New();
	newData->pushBack(5);
	newData->pushBack(5);
	newData->pushBack(5);
	shared_ptr<XdmfGeometry> newGeometry = XdmfGeometry::New();
	newGeometry->setType(XdmfGeometryType::XYZ());
	newGeometry->insert(0, newData, 0, 3, 1, 1);//Start index is 0, 3 values are passed, stride for both arrays is 1
	exampleGrid->setGeometry(newGeometry);
	shared_ptr<XdmfGeometry> exampleGeometry = exampleGrid->getGeometry();

	}
	else // via array
	{

	shared_ptr<XdmfArray> newPoints = XdmfArray::New();
	newPoints->pushBack(5);
	newPoints->pushBack(5);
	newPoints->pushBack(5);
	shared_ptr<XdmfCurvilinearGrid> exampleGrid = XdmfCurvilinearGrid::New(newPoints);

	}


	return 0;
}
