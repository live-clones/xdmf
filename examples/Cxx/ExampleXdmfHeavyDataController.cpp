#include "XdmfHDF5Controller.hpp"
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"

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
        //Three dimensions, 10 values in each
        readDataSize.push_back(10);
        readDataSize.push_back(10);
        readDataSize.push_back(10);
        shared_ptr<XdmfHDF5Controller> exampleController = XdmfHDF5Controller::New(
                newPath,
                newSetPath,
                readType,
                readStarts,
                readStrides,
                readCounts,
                readDataSize);

        //Using XdmfHDF5Controller since XdmfHeavyDataController is an abstract class

        //#initialization end

        //#getDataSetPath begin

        std::string exampleSetPath = exampleController->getDataSetPath();

        //#getDataSetPath end

        //#getDimensions begin

        std::vector<unsigned int>  exampleDimensions = exampleController->getDimensions();

        //#getDimensions end

        //#getDataspaceDimensions begin

        std::vector<unsigned int> exampleDataspaceDimensions = exampleController->getDataspaceDimensions();

        //#getDataspaceDimensions end

        //#getFilePath begin

        std::string exampleFilePath = exampleController->getFilePath();

        //#getFilePath end

        //#getName begin

        std::string exampleName = exampleController->getName();

        //#getName end

        //#getSize begin

        unsigned int exampleSize = exampleController->getSize();

        //#getSize end

        //#getType begin

        shared_ptr<const XdmfArrayType> exampleType = exampleController->getType();

        //#getType end

        //#read begin

        shared_ptr<XdmfArray> exampleArray = XdmfArray::New();
        exampleController->read(exampleArray);
        //exampleArray now holds the data that exampleController holds.

        //#read end

        //#getStart begin

        std::vector<unsigned int> exampleStart = exampleController->getStart();

        //#getStart end

        //#getStride begin

        std::vector<unsigned int> exampleStride = exampleController->getStride();

        //#getStride end

        //#setArrayOffset begin

        unsigned int newArrayOffset = 5;//default is 0

        exampleController->setArrayOffset(newArrayOffset);

        //#setArrayOffset end

        //#getArrayOffset begin

        unsigned int exampleOffset = exampleController->getArrayOffset();

        //#getArrayOffset end

        return 0;
}
