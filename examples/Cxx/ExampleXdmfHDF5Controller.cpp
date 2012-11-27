#include "XdmfHDF5Controller.hpp"

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

	return 0;
}
