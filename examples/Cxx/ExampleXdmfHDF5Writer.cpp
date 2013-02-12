#include "XdmfHDF5Writer.hpp"

int main(int, char **)
{
	std::string newPath = "Your file path goes here";
	bool replaceOrig = true;
	shared_ptr<XdmfHDF5Writer> exampleWriter = XdmfHDF5Writer::New(newPath, replaceOrig);

	int newFileSizeLimit = 10;//limit of 10 MB

	exampleWriter->setFileSizeLimit(newFileSizeLimit);

	int exampleLimit = exampleWriter->getFileSizeLimit();

	return 0;
}
