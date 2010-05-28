// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <libxml/tree.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfItem.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"

/**
 * PIMPL
 */
class XdmfWriter::XdmfWriterImpl {

public:

	XdmfWriterImpl(const std::string & xmlFilePath, const boost::shared_ptr<XdmfHDF5Writer> hdf5Writer) :
		mHDF5Writer(hdf5Writer),
		mLightDataLimit(100),
		mXMLFilePath(xmlFilePath),
		mTraverseLevel(0)
	{
	};
	~XdmfWriterImpl()
	{
	};
	boost::shared_ptr<XdmfHDF5Writer> mHDF5Writer;
	unsigned int mTraverseLevel;
	unsigned int mLightDataLimit;
	xmlDocPtr mXMLDocument;
	xmlNodePtr mXMLCurrentNode;
	std::string mXMLFilePath;
};

XdmfWriter::XdmfWriter(const std::string & xmlFilePath)
{
	std::stringstream heavyFileName;
	size_t extension = xmlFilePath.rfind(".");
	if(extension != std::string::npos)
	{
		heavyFileName << xmlFilePath.substr(0, extension) << ".h5";
	}
	else
	{
		heavyFileName << xmlFilePath << ".h5";
	}
	boost::shared_ptr<XdmfHDF5Writer> hdf5Writer = XdmfHDF5Writer::New(heavyFileName.str());
	mImpl = new XdmfWriterImpl(xmlFilePath, hdf5Writer);
	std::cout << "Created XdmfWriter " << this << std::endl;
}

XdmfWriter::XdmfWriter(const std::string & xmlFilePath, boost::shared_ptr<XdmfHDF5Writer> hdf5Writer) :
	mImpl(new XdmfWriterImpl(xmlFilePath, hdf5Writer))
{
	std::cout << "Created XdmfWriter " << this << std::endl;
}

XdmfWriter::~XdmfWriter()
{
	delete mImpl;
	std::cout << "Deleted XdmfWriter " << this << std::endl;
}

void XdmfWriter::closeFile()
{
	xmlSaveFormatFile(mImpl->mXMLFilePath.c_str(), mImpl->mXMLDocument, 1);
	xmlFreeDoc(mImpl->mXMLDocument);
	xmlCleanupParser();
}

unsigned int XdmfWriter::getLightDataLimit() const
{
	return mImpl->mLightDataLimit;
}

void XdmfWriter::openFile()
{
	mImpl->mXMLDocument = xmlNewDoc((xmlChar*)"1.0");
	mImpl->mXMLCurrentNode = xmlNewNode(NULL, (xmlChar*)"Xdmf");
	xmlDocSetRootElement(mImpl->mXMLDocument, mImpl->mXMLCurrentNode);
}

void XdmfWriter::setLightDataLimit(const unsigned int numValues)
{
	mImpl->mLightDataLimit = numValues;
}

void XdmfWriter::visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	std::stringstream xmlTextValues;
	if(array.getHDF5Controller() || array.getSize() > mImpl->mLightDataLimit)
	{
		mImpl->mHDF5Writer->visit(array, mImpl->mHDF5Writer);
		xmlTextValues << mImpl->mHDF5Writer->getLastWrittenDataSet();
	}
	else
	{
		xmlTextValues << array.getValuesString();
	}

	this->visit(dynamic_cast<XdmfItem &>(array), visitor);
	xmlAddChild(mImpl->mXMLCurrentNode->children, xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
}

void XdmfWriter::visit(XdmfItem & item, const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	if(mImpl->mTraverseLevel == 0)
	{
		this->openFile();
	}
	mImpl->mTraverseLevel++;
	xmlNodePtr parentNode = mImpl->mXMLCurrentNode;
	mImpl->mXMLCurrentNode = xmlNewChild(mImpl->mXMLCurrentNode, NULL, (xmlChar*)item.getItemTag().c_str(), NULL);
	const std::map<std::string, std::string> itemProperties = item.getItemProperties();
	for(std::map<std::string, std::string>::const_iterator iter = itemProperties.begin(); iter != itemProperties.end(); ++iter)
	{
		xmlNewProp(mImpl->mXMLCurrentNode, (xmlChar*)iter->first.c_str(), (xmlChar*)iter->second.c_str());
	}
	item.traverse(visitor);
	mImpl->mXMLCurrentNode = parentNode;
	mImpl->mTraverseLevel--;
	if(mImpl->mTraverseLevel == 0)
	{
		this->closeFile();
	}
}
