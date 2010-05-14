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

	XdmfWriterImpl() :
		mHDF5Writer(XdmfHDF5Writer::New("output.h5")),
		mLightDataLimit(100),
		mXMLDocument(xmlNewDoc((xmlChar*)"1.0")),
		mXMLCurrentNode(xmlNewNode(NULL, (xmlChar*)"Xdmf"))
	{
		xmlDocSetRootElement(mXMLDocument, mXMLCurrentNode);
	};
	~XdmfWriterImpl()
	{
		xmlSaveFormatFile("output.xmf", mXMLDocument, 1);
		xmlFreeDoc(mXMLDocument);
		xmlCleanupParser();
	};
	boost::shared_ptr<XdmfHDF5Writer> mHDF5Writer;
	std::string mHeavyFileName;
	unsigned int mLightDataLimit;
	xmlDocPtr mXMLDocument;
	xmlNodePtr mXMLCurrentNode;

};

XdmfWriter::XdmfWriter() :
	mImpl(new XdmfWriterImpl())
{
	std::cout << "Created XdmfWriter " << this << std::endl;
}

XdmfWriter::~XdmfWriter()
{
	delete mImpl;
	std::cout << "Deleted XdmfWriter " << this << std::endl;
}

unsigned int XdmfWriter::getLightDataLimit() const
{
	return mImpl->mLightDataLimit;
}

void XdmfWriter::setLightDataLimit(unsigned int numValues)
{
	mImpl->mLightDataLimit = numValues;
}

void XdmfWriter::visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	this->visit(dynamic_cast<XdmfItem &>(array), visitor);

	xmlNodePtr parentNode = mImpl->mXMLCurrentNode;
	mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->children;

	std::stringstream xmlTextValues;
	if(array.getSize() > mImpl->mLightDataLimit)
	{
		xmlTextValues << mImpl->mHDF5Writer->visit(array, mImpl->mHDF5Writer);
	}
	else
	{
		xmlTextValues << array.getValuesString();
	}

	xmlAddChild(mImpl->mXMLCurrentNode, xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
	mImpl->mXMLCurrentNode = parentNode;
}

void XdmfWriter::visit(XdmfItem & item, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	xmlNodePtr parentNode = mImpl->mXMLCurrentNode;
	mImpl->mXMLCurrentNode = xmlNewChild(mImpl->mXMLCurrentNode, NULL, (xmlChar*)item.getItemTag().c_str(), NULL);
	const std::map<std::string, std::string> itemProperties = item.getItemProperties();
	for(std::map<std::string, std::string>::const_iterator iter = itemProperties.begin(); iter != itemProperties.end(); ++iter)
	{
		xmlNewProp(mImpl->mXMLCurrentNode, (xmlChar*)iter->first.c_str(), (xmlChar*)iter->second.c_str());
	}
	mImpl->mHDF5Writer->pushDataHierarchy(item);
	item.traverse(visitor);
	mImpl->mHDF5Writer->popDataHierarchy();
	mImpl->mXMLCurrentNode = parentNode;
}
