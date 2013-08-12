#include "XdmfDomain.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"

int main(int, char **)
{
	//#initialization begin

	std::string outFile = "output file name goes here";
	shared_ptr<XdmfWriter> writerExample = XdmfWriter::New(outFile);

	//#initialization end

	//#heavyinitialization begin

	std::string heavyFile = "heavy file name goes here";
	bool replaceFile = true;
	shared_ptr<XdmfHDF5Writer> exampleHeavyWriter = XdmfHDF5Writer::New(heavyFile, replaceFile);
	shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New(outFile, exampleHeavyWriter);

	//#heavyinitialization end

	//#bufferinitialization begin

	filebuf exampleBuffer;
	exampleBuffer.open("file goes here", ios::out);
	ostream exampleStream(&exampleBuffer);
	shared_ptr<XdmfWriter> exampleBufferWriter = XdmfWriter::New(exampleStream, exampleHeavyWriter);

	//#bufferinitialization end

	//#getFilePath begin

	std::string examplePath = exampleWriter->getFilePath();

	//#getFilePath

	//#getHeavyDataWriter begin

	shared_ptr<XdmfHeavyDataWriter> exampleHeavyWriter = exampleWriter->getHeavyDataWriter();

	//#getHeavyDataWriter end

	//#getHeavyDataWriterconst begin

	shared_ptr<const XdmfHeavyDataWriter> exampleHeavyWriterConst = exampleWriter->getHeavyDataWriter();

	//#getHeavyDataWriterconst end

	//#setHeavyDataWriter begin

	exampleWriter->setHeavyDataWriter(exampleHeavyWriter);

	//#setHeavyDataWriter end

	//#getLightDataLimit begin

	unsigned int exampleLimit = exampleWriter->getLightDataLimit();

	//#getLightDataLimit end

	//#getMode begin

	XdmfWriter::Mode testMode = XdmfWriter::Default;
	if (exampleWriter->getMode() == testMode)
	{
		//Do whatever is to be done if the mode is default
	}

	//#getMode end

	//#getWriteXPaths begin

	bool exampleTestPaths = exampleWriter->getWriteXPaths();

	//#getWriteXPaths end

	//#getXPathParse begin

	bool exampleXPathParse = exampleWriter->getXPathParse();

	//#getXPathParse end

	//#setLightDataLimit begin

	unsigned int newLimit = 20;
	exampleWriter->setLightDataLimit(newLimit);
	//The writer will now place any data with a number of values over 20 into heavy data

	//#setLightDataLimit end

	//#setMode begin

	exampleWriter->setMode(XdmfWriter::Default);

	//#setMode end

	//#setWriteXPaths begin

	exampleWriter->setWriteXPaths(true);

	//#setWriteXPaths end

	//#setXPathParse begin

	exampleWriter->setXPathParse(true);

	//#setXPathParse end

	//#visitarray begin

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
	shared_ptr<XdmfWriter> exampleArrayWriter = XdmfWriter::New(attributeFile);
	exampleArrayWriter->visit(exampleAttribute, exampleWriter);

	//#visitarray end

	//#visititem begin

	//Using XdmfDomain here, but any XdmfItem would work
	shared_ptr<XdmfDomain> exampleDomain = XdmfDomain::New();
	std::string domainFile = "output file name goes here";
	shared_ptr<XdmfWriter> exampleDomainWriter = XdmfWriter::New(domainFile);
	exampleDomainWriter->visit(exampleDomain, exampleWriter);

	//#visititem end

	return 0;
}
