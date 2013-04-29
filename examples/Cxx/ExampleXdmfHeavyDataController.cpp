#include "XdmfHDF5Controller.hpp"
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"

int main(int, char **)
{
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
	shared_ptr<XdmfHDF5Controller> exampleController = XdmfHDF5Controller::New(
		newPath,
		newSetPath,
		readType,
		readStarts,
		readStrides,
		readCounts);


	//Assuming that exampleController is a shared pointer to an XdmfHDF5Controller object
	//Since XdmfHeavyDataController is an abstract class
	std::string exampleSetPath = exampleController->getDataSetPath();	

	std::vector<unsigned int>  exampleDimensions = exampleController->getDimensions();

	std::string exampleFilePath = exampleController->getFilePath();

	std::string exampleName = exampleController->getName();

	unsigned int exampleSize = exampleController->getSize();

	shared_ptr<const XdmfArrayType> exampleType = exampleController->getType();

	shared_ptr<XdmfArray> exampleArray = XdmfArray::New();
	exampleController->read(exampleArray);
	//exampleArray now holds the data that exampleController holds.

        std::vector<unsigned int> exampleStart = exampleController->getStart();

        std::vector<unsigned int> exampleStride = exampleController->getStride();

	unsigned int newArrayOffset = 5;//default is 0

	exampleController->setArrayOffset(newArrayOffset);

	unsigned int exampleOffset = exampleController->getArrayOffset();

	return 0;
}
