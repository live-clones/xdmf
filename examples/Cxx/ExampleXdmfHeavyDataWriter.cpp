#include "XdmfDomain.hpp"

int main(int, char **)
{
	std::string newPath = "Your file path goes here";
	bool replaceOrig = true;
	shared_ptr<XdmfHDF5Writer> exampleWriter = XdmfHDF5Writer::New(newPath, replaceOrig);

	//Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.

	exampleWriter->openFile();

	exampleWriter->closeFile();

	std::string examplePath = exampleWriter->getFilePath();

	XdmfHeavyDataWriter::Mode exampleMode = XdmfHeavyDataWriter::Default;

	if (exampleWriter->getMode() == exampleMode)
	{
		//Do whatever is to be done if the mode is default
	}

	bool testRelease = exampleWriter->getReleaseData();

	exampleWriter->setMode(XdmfHeavyDataWriter::Default);

	exampleWriter->setReleaseData(true);
	//Sets the writer to release data after writing

	shared_ptr<XdmfArray> exampleArray = XdmfArray::New();
	exampleArray->pushBack(1);
	exampleArray->pushBack(2);
	exampleArray->pushBack(3);
	exampleArray->pushBack(4);
	exampleArray->pushBack(5);
	exampleWriter->visit(exampleArray, exampleWriter);

	return 0;
}
