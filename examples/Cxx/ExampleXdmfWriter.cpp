#include "XdmfDomain.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"

int main(int, char **)
{
	std::string outFile = "output file name goes here";
	shared_ptr<XdmfWriter> writerExample = XdmfWriter::New(outFile);

	std::string heavyFile = "heavy file name goes here";
	bool replaceFile = true;
	shared_ptr<XdmfHDF5Writer> exampleHeavyWriter = XdmfHDF5Writer::New(heavyFile, replaceFile);
	shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New(outFile, exampleHeavyWriter);

	filebuf exampleBuffer;
	exampleBuffer.open("file goes here", ios::out);
	ostream exampleStream(&exampleBuffer);
	shared_ptr<XdmfWriter> exampleBufferWriter = XdmfWriter::New(exampleStream, exampleHeavyWriter);

	//Assume that exampleWriter is a shared pointer to a XdmfWriter object

	std::string examplePath = exampleWriter->getFilePath();

	shared_ptr<XdmfHeavyDataWriter> exampleHeavyWriter = exampleWriter->getHeavyDataWriter();

	shared_ptr<const XdmfHeavyDataWriter> exampleHeavyWriterConst = exampleWriter->getHeavyDataWriter();

	exampleWriter->setHeavyDataWriter(exampleHeavyWriter);

	unsigned int exampleLimit = exampleWriter->getLightDataLimit();

	XdmfWriter::Mode testMode = XdmfWriter::Default;
	if (exampleWriter->getMode() == testMode)
	{
		//Do whatever is to be done if the mode is default
	}

	bool exampleTestPaths = exampleWriter->getWriteXPaths();

	bool exampleXPathParse = exampleWriter->getXPathParse();

	unsigned int newLimit = 20;
	exampleWriter->setLightDataLimit(newLimit);
	//The writer will now place any data with a number of values over 20 into heavy data

	exampleWriter->setMode(XdmfWriter::Default);

	exampleWriter->setWriteXPaths(true);

	exampleWriter->setXPathParse(true);

	//Using XdmfAttribute here, but any XdmfArray would work
	shared_ptr<XdmfAttribute> exampleAttribute = XdmfAttribute::New();
	exampleAttribute->setCenter(XdmfAttributeCenter::Node());
	exampleAttribute->setType(XdmfAttributeType::Scalar());
	exampleAttribute->pushBack(1);
	exampleAttribute->pushBack(2);
	exampleAttribute->pushBack(3);
	exampleAttribute->pushBack(4);
	exampleAttribute->pushBack(5);
	exampleAttribute->pushBack(6);
	std::string attributeFile = "output file name goes here";
	shared_ptr<XdmfWriter> exampleArrayWriter = XdmfWriter::New(outFile);
	exampleArrayWriter->visit(exampleAttribute, exampleWriter);

	//Using XdmfDomain here, but any XdmfItem would work
	shared_ptr<XdmfDomain> exampleDomain = XdmfDomain::New();
	std::string domainFile = "output file name goes here";
	shared_ptr<XdmfWriter> exampleDomainWriter = XdmfWriter::New(outFile);
	exampleDomainWriter->visit(exampleDomain, exampleWriter);

	return 0;
}
