#include "XdmfDomain.hpp"

int main(int, char **)
{
        //#initialization begin

        shared_ptr<XdmfUnstructuredGrid> exampleGrid = XdmfUnstructuredGrid::New();

        //Using an unstructured grid since XdmfGrid is an abstract class

        //#initialization end
        
        //#setName begin

        std::string newName = "New Name";
        exampleGrid->setName(newName);

        //#setName end

        //#setTime begin

        shared_ptr<XdmfTime> newTime = XdmfTime::New(20.0);
        exampleGrid->setTime(newTime);

        //#setTime end

        //#getTime begin

        shared_ptr< XdmfTime> exampleTime = exampleGrid->getTime();

        //#getTime end

        //#getTimeconst begin

        shared_ptr<const XdmfTime> exampleTimeConst = exampleGrid->getTime();

        //#getTimeconst end

        //#getName begin

        std::string exampleName = exampleGrid->getName();

        //#getName end

        //#getGeometry begin

        shared_ptr<const XdmfGeometry> exampleGeometry = exampleGrid->getGeometry();

        //#getGeometry end

        //#getTopology begin

        shared_ptr<const XdmfTopology> exampleTopology = exampleGrid->getTopology();

        //#getTopology end

        //#setGridController begin

        shared_ptr<XdmfGridController> newGridController = XdmfGridController::New("gridFile.xmf", "/Xdmf/Domain/Grid[1]");

        exampleGrid->setGridController(gridController);

        //#setGridController end

        //#getGridController begin

        shared_ptr<XdmfGridController> exampleGridController = exampleGrid->getGridController();

        //#getGridController end

        //#read begin

        exampleGrid->read();

        //#read end

        //#release begin

        exampleGrid->release();

        //#release end


        return 0;
}
