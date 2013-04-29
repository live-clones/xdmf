#include "XdmfHDF5Writer.hpp"

int main(int, char **)
{
	std::string newPath = "Your file path goes here";
	bool replaceOrig = true;
	shared_ptr<XdmfHDF5Writer> exampleWriter = XdmfHDF5Writer::New(newPath, replaceOrig);

	int newFileSizeLimit = 10;//limit of 10 MB

	exampleWriter->setFileSizeLimit(newFileSizeLimit);

	int exampleLimit = exampleWriter->getFileSizeLimit();

	bool newAllow = true;//false is default

	exampleWriter->setAllowSetSplitting(newAllow);

	bool exampleAllowSplitting = exampleWriter->getAllowSetSplitting();

	int newFileIndex = 0;//0 resets to no appended index

	exampleWriter->setFileIndex(newFileIndex);

	int exampleIndex = exampleWriter->getFileIndex();

	int newChunk = 10;//creates blocks in sets of 10 slots

	exampleWriter->setChunkSize(newChunk);

	int exampleChunk = exampleWriter->getChunkSize();

	return 0;
}
