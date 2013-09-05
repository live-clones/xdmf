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

        //#initializationdim2 start

        unsigned int newPointsX = 5;
        unsigned int newPointsY = 5;
        shared_ptr<XdmfCurvilinearGrid> exampleGrid = XdmfCurvilinearGrid::New(newPointsX, newPointsY);

        //#initializationdim2 end

        }
        else if (size = 3)
        {

        //#initializationdim3 start

        unsigned int newPointsX = 5;
        unsigned int newPointsY = 5;
        unsigned int newPointsZ = 5;
        shared_ptr<XdmfCurvilinearGrid> exampleGrid = XdmfCurvilinearGrid::New(newPointsX, newPointsY, newPointsZ);

        //#initializationdim3 end

        //#setDimensions begin

        shared_ptr<XdmfArray> newPoints = XdmfArray::New();
        newPoints->pushBack(10);
        newPoints->pushBack(10);
        newPoints->pushBack(10);
        exampleGrid->setDimensions(newPoints);

        //#setDimensions end

        //#getDimensions begin

        shared_ptr<XdmfArray> exampleDimensions = exampleGrid->getDimensions();

        //#getDimensions end

        //#getDimensionsconst begin

        shared_ptr<const XdmfArray> exampleDimensions = exampleGrid->getDimensions();

        //#getDimensionsconst end

        //#setGeometry begin

        shared_ptr<XdmfArray> newData = XdmfArray::New();
        newData->pushBack(5);
        newData->pushBack(5);
        newData->pushBack(5);
        shared_ptr<XdmfGeometry> newGeometry = XdmfGeometry::New();
        newGeometry->setType(XdmfGeometryType::XYZ());
        // Start index is 0, 3 values are passed, stride for both arrays is 1
        newGeometry->insert(0, newData, 0, 3, 1, 1);
        exampleGrid->setGeometry(newGeometry);

        //#setGeometry end

        //#getGeometry begin

        shared_ptr<XdmfGeometry> exampleGeometry = exampleGrid->getGeometry();

        //#getGeometry end

        }
        else // via array
        {

        //#initializationvector start

        shared_ptr<XdmfArray> newPoints = XdmfArray::New();
        newPoints->pushBack(5);
        newPoints->pushBack(5);
        newPoints->pushBack(5);
        shared_ptr<XdmfCurvilinearGrid> exampleGrid = XdmfCurvilinearGrid::New(newPoints);

        //#initializationvector end

        }


        return 0;
}
