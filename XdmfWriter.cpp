// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfItem.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"

XdmfWriter::XdmfWriter() :
	mHDF5Writer(XdmfHDF5Writer::New()),
	mLightDataLimit(100),
	mXMLDocument(xmlNewDoc((xmlChar*)"1.0")),
	mXMLCurrentNode(xmlNewNode(NULL, (xmlChar*)"Xdmf"))
{
	xmlDocSetRootElement(mXMLDocument, mXMLCurrentNode);
	std::cout << "Created XdmfWriter " << this << std::endl;
}

XdmfWriter::~XdmfWriter()
{
	xmlSaveFormatFile("output.xmf", mXMLDocument, 1);
	xmlFreeDoc(mXMLDocument);
	xmlCleanupParser();
	std::cout << "Deleted XdmfWriter " << this << std::endl;
}

unsigned int XdmfWriter::getLightDataLimit() const
{
	return mLightDataLimit;
}

void XdmfWriter::setLightDataLimit(unsigned int numValues)
{
	mLightDataLimit = numValues;
}

void XdmfWriter::visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	this->visit(dynamic_cast<XdmfItem &>(array), visitor);

	xmlNodePtr parentNode = mXMLCurrentNode;
	mXMLCurrentNode = mXMLCurrentNode->children;

	std::stringstream xmlTextValues;
	if(array.getSize() > mLightDataLimit)
	{
		xmlTextValues << mHDF5Writer->visit(array, mHDF5Writer);
	}
	else
	{
		xmlTextValues << array.getValuesString();
	}

	xmlAddChild(mXMLCurrentNode, xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
	mXMLCurrentNode = parentNode;
}

void XdmfWriter::visit(XdmfItem & item, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	xmlNodePtr parentNode = mXMLCurrentNode;
	mXMLCurrentNode = xmlNewChild(mXMLCurrentNode, NULL, (xmlChar*)item.getItemTag().c_str(), NULL);
	const std::map<std::string, std::string> itemProperties = item.getItemProperties();
	for(std::map<std::string, std::string>::const_iterator iter = itemProperties.begin(); iter != itemProperties.end(); ++iter)
	{
		xmlNewProp(mXMLCurrentNode, (xmlChar*)iter->first.c_str(), (xmlChar*)iter->second.c_str());
	}
	mHDF5Writer->pushDataHierarchy(item);
	item.traverse(visitor);
	mHDF5Writer->popDataHierarchy();
	mXMLCurrentNode = parentNode;
}
