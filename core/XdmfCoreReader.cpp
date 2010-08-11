// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <libxml/xpointer.h>
#include <libxml/xmlreader.h>
#include <map>
#include "XdmfCoreItemFactory.hpp"
#include "XdmfCoreReader.hpp"
#include "XdmfItem.hpp"
#include "XdmfSystemUtils.hpp"

/**
 * PIMPL
 */
class XdmfCoreReader::XdmfCoreReaderImpl {

public:

	XdmfCoreReaderImpl(const boost::shared_ptr<const XdmfCoreItemFactory> itemFactory) :
		mItemFactory(itemFactory)
	{
	};

	~XdmfCoreReaderImpl()
	{
	};

	void closeFile()
	{
		xmlXPathFreeContext(mXPathContext);
		xmlFreeDoc(mDocument);
		xmlCleanupParser();
	}

	void openFile(const std::string & filePath)
	{
		mXMLDir = XdmfSystemUtils::getRealPath(filePath);
		size_t index = mXMLDir.find_last_of("/\\");
		if(index != std::string::npos)
		{
		    mXMLDir = mXMLDir.substr(0, index + 1);
		}

		mDocument = xmlReadFile(filePath.c_str(), NULL, 0);

		if(mDocument == NULL)
		{
			assert(false);
		}

		mXPathContext = xmlXPtrNewContext(mDocument, NULL, NULL);
	}

	/**
	 * Constructs XdmfItems for all nodes in currNode's tree.  XdmfItems are constructed by recursively calling this function for all children of currNode.
	 */
	std::vector<boost::shared_ptr<XdmfItem> > read(xmlNodePtr currNode, std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> > & xPathMap) const
	{
		std::vector<boost::shared_ptr<XdmfItem> > myItems;

		while(currNode != NULL)
		{
			if(currNode->type == XML_ELEMENT_NODE)
			{
				xmlChar * xpointer = NULL;
				if(xmlStrcmp(currNode->name, (xmlChar*)"include") == 0)
				{
					// Deal with proper reading of XIncludes
					xmlAttrPtr currAttribute = currNode->properties;
					while(currAttribute != NULL)
					{
						if(xmlStrcmp(currAttribute->name, (xmlChar*)"xpointer") == 0)
						{
							xpointer = currAttribute->children->content;
							break;
						}
					}
					xmlXPathObjectPtr xPathObject = xmlXPtrEval(xpointer, mXPathContext);
					for(unsigned int i=0; i<xPathObject->nodesetval->nodeNr; ++i)
					{
						this->readSingleNode(xPathObject->nodesetval->nodeTab[i], xPathMap, myItems);
					}
					xmlXPathFreeObject(xPathObject);
				}
				else
				{
					// Normal reading
					this->readSingleNode(currNode, xPathMap, myItems);
				}
			}
			currNode = currNode->next;
		}
		return myItems;
	}

	/**
	 * Reads a single xmlNode into an XdmfItem object in memory.  The constructed XdmfItem is added to myItems and an entry is added mapping the xmlNodePtr to the new XdmfItem in the xPathMap.
	 */
	void readSingleNode(const xmlNodePtr currNode, std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> > & xPathMap, std::vector<boost::shared_ptr<XdmfItem> > & myItems) const
	{
		std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> >::const_iterator iter = xPathMap.find(currNode);
		if(iter != xPathMap.end())
		{
			myItems.push_back(iter->second);
		}
		else
		{
			std::map<std::string, std::string> itemProperties;
			if(currNode->children != NULL)
			{
				itemProperties["Content"] = (const char *)currNode->children->content;
				itemProperties["XMLDir"] = mXMLDir;
			}

			xmlAttrPtr currAttribute = currNode->properties;
			while(currAttribute != NULL)
			{
				itemProperties[(const char *)currAttribute->name] = (const char *)currAttribute->children->content;
				currAttribute = currAttribute->next;
			}
			std::vector<boost::shared_ptr<XdmfItem> > childItems = this->read(currNode->children, xPathMap);
			boost::shared_ptr<XdmfItem> newItem = mItemFactory->createItem((const char *)currNode->name, itemProperties);
			if(newItem == NULL)
			{
				assert(false);
			}
			newItem->populateItem(itemProperties, childItems);
			myItems.push_back(newItem);
			xPathMap[currNode] = newItem;
		}
	}

	xmlDocPtr mDocument;
	const boost::shared_ptr<const XdmfCoreItemFactory> mItemFactory;
	std::string mXMLDir;
	xmlXPathContextPtr mXPathContext;
};

XdmfCoreReader::XdmfCoreReader(const boost::shared_ptr<const XdmfCoreItemFactory> itemFactory) :
	mImpl(new XdmfCoreReaderImpl(itemFactory))
{
}

XdmfCoreReader::~XdmfCoreReader()
{
	delete mImpl;
}

boost::shared_ptr<XdmfItem> XdmfCoreReader::read(const std::string & filePath) const
{
	mImpl->openFile(filePath);
	const xmlNodePtr currNode = xmlDocGetRootElement(mImpl->mDocument);
	std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> > xPathMap;
	const std::vector<boost::shared_ptr<XdmfItem> > toReturn = mImpl->read(currNode->children, xPathMap);
	mImpl->closeFile();
	return toReturn[0];
}

std::vector<boost::shared_ptr<XdmfItem> > XdmfCoreReader::read(const std::string & filePath, const std::string & xPath) const
{
	mImpl->openFile(filePath);
	std::vector<boost::shared_ptr<XdmfItem> > toReturn;
	std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> > xPathMap;
	xmlXPathObjectPtr xPathObject = xmlXPathEvalExpression((xmlChar*)xPath.c_str(), mImpl->mXPathContext);
	for(unsigned int i=0; i<xPathObject->nodesetval->nodeNr; ++i)
	{
		mImpl->readSingleNode(xPathObject->nodesetval->nodeTab[i], xPathMap, toReturn);
	}
	xmlXPathFreeObject(xPathObject);
	mImpl->closeFile();
	return toReturn;
}
