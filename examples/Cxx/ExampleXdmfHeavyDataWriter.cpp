#include "XdmfDomain.hpp"

int main(int, char **)
{
	//#initialization begin

	std::string newPath = "Your file path goes here";
	bool replaceOrig = true;
	shared_ptr<XdmfHDF5Writer> exampleWriter = XdmfHDF5Writer::New(newPath, replaceOrig);

	//using an XdmfHDF5Writer because XdmfHeavyWriter is abstract

	//#initialization end

	//#openFile begin

	exampleWriter->openFile();

	//#openFile end

	//#closeFile begin

	exampleWriter->closeFile();

	//#closeFile end

	//#getFilePath begin

	std::string examplePath = exampleWriter->getFilePath();

	//#getFilePath end

	//#getMode begin

	XdmfHeavyDataWriter::Mode exampleMode = XdmfHeavyDataWriter::Default;

	if (exampleWriter->getMode() == exampleMode)
	{
		//Do whatever is to be done if the mode is default
	}

	//#getMode end

	//#getReleaseData begin

	bool testRelease = exampleWriter->getReleaseData();

	//#getReleaseData end

	//#setMode begin

	exampleWriter->setMode(XdmfHeavyDataWriter::Default);

	//#setMode end

	//#setReleaseData begin

	exampleWriter->setReleaseData(true);
	//Sets the writer to release data after writing

	//#setReleaseData end

	//#setFileSizeLimit begin

	int newFileSizeLimit = 10;
	//limit of 10 MB

	exampleWriter->setFileSizeLimit(newFileSizeLimit);

	//#setFileSizeLimit end

	//#getFileSizeLimit begin

	int exampleLimit = exampleWriter->getFileSizeLimit();

	//#getFileSizeLimit end

	//#getFileOverhead begin

	unsigned int exampleOverhead = exampleWriter->getFileOverhead();

	//#getFileOverhead end

	//#setAllowSetSplitting begin

	bool newAllow = true;
	//false is default

	exampleWriter->setAllowSetSplitting(newAllow);

	//#setAllowSetSplitting end

	//#getAllowSetSplitting begin

	bool exampleAllowSplitting = exampleWriter->getAllowSetSplitting();

	//#getAllowSetSplitting end

	//#setFileIndex begin

	int newFileIndex = 0;
	//0 resets to no appended index

	exampleWriter->setFileIndex(newFileIndex);

	//#setFileIndex end

	//#getFileIndex begin

	int exampleIndex = exampleWriter->getFileIndex();

	//#getFileIndex end

	//#visit begin

	shared_ptr<XdmfArray> exampleArray = XdmfArray::New();
	exampleArray->pushBack(1);
	exampleArray->pushBack(2);
	exampleArray->pushBack(3);
	exampleArray->pushBack(4);
	exampleArray->pushBack(5);
	exampleWriter->visit(exampleArray, exampleWriter);

	//#visit end

	return 0;
}
