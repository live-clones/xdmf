#include "XdmfHDF5Controller.hpp"

int main(int, char **)
{
        //#initialization begin

        std::string newPath = "File path to hdf5 file goes here";
        std::string newSetPath = "path to the set goes here";
        shared_ptr<const XdmfArrayType> readType = XdmfArrayType::Int32();
        std::vector<unsigned int> readStarts;
        //Three dimensions, all starting at index 0
        readStarts.push_back(0);
        readStarts.push_back(0);
        readStarts.push_back(0);
        std::vector<unsigned int> readStrides;
        //Three dimensions, no skipping between reads
        readStrides.push_back(1);
        readStrides.push_back(1);
        readStrides.push_back(1);
        std::vector<unsigned int> readCounts;
        //Three dimensions, reading 10 values from each
        readCounts.push_back(10);
        readCounts.push_back(10);
        readCounts.push_back(10);
        std::vector<unsigned int> readDataSize;
        //Three dimensions, each with a maximum of 20 values
        readDataSize.push_back(20);
        readDataSize.push_back(20);
        readDataSize.push_back(20);
        shared_ptr<XdmfHDF5Controller> exampleController = XdmfHDF5Controller::New(
                newPath,
                newSetPath,
                readType,
                readStarts,
                readStrides,
                readCounts,
                readDataSize);

        //#initialization end

        //#getDataSetPath begin

        std::string exampleSetPath = exampleController->getDataSetPath();

        //#getDataSetPath end

        //#getDataspaceDimensions begin

        std::vector<unsigned int> exampleDataspaceDimensions = exampleController->getDataspaceDimensions();

        //#getDataspaceDimensions end

        //#getStart begin

        std::vector<unsigned int> exampleStart = exampleController->getStart();

        //#getStart end

        //#getStride begin

        std::vector<unsigned int> exampleStride = exampleController->getStride();

        //#getStride end

        //#setMaxOpenedFiles begin

        XdmfHDF5Controller::setMaxOpenedFiles(2);

        //#setMaxOpenedFiles end

        //#getMaxOpenedFiles begin

        unsigned int maxNumOpened = XdmfHDF5Controller::getMaxOpenedFiles();

        //#getMaxOpenedFiles end

        //#closeFiles begin

        XdmfHDF5Controller::closeFiles();

        //#closeFiles end

        return 0;
}
