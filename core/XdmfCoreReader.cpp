// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <libxml/xpointer.h>
#include <libxml/xmlreader.h>
#include "XdmfCoreItemFactory.hpp"
#include "XdmfCoreReader.hpp"
#include "XdmfItem.hpp"

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

	/**
	 * Constructs XdmfItems for all nodes in currNode's tree.  XdmfItems are constructed by recursively calling this function for all children of currNode.
	 */
	std::vector<boost::shared_ptr<XdmfItem> > read(xmlNodePtr currNode, const xmlXPathContextPtr xPathContext, std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> > & xPathMap, const std::string & xmlDir) const
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
					xmlXPathObjectPtr xPathObject = xmlXPtrEval(xpointer, xPathContext);
					for(unsigned int i=0; i<xPathObject->nodesetval->nodeNr; ++i)
					{
						this->readSingleNode(xPathObject->nodesetval->nodeTab[i], xPathContext, xPathMap, myItems, xmlDir);
					}
					xmlXPathFreeObject(xPathObject);
				}
				else
				{
					// Normal reading
					this->readSingleNode(currNode, xPathContext, xPathMap, myItems, xmlDir);
				}
			}
			currNode = currNode->next;
		}
		return myItems;
	}

private:

	/**
	 * Reads a single xmlNode into an XdmfItem object in memory.  The constructed XdmfItem is added to myItems and an entry is added mapping the xmlNodePtr to the new XdmfItem in the xPathMap.
	 */
	void readSingleNode(const xmlNodePtr currNode, const xmlXPathContextPtr xPathContext, std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> > & xPathMap, std::vector<boost::shared_ptr<XdmfItem> > & myItems, const std::string & xmlDir) const
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
				itemProperties["XMLDir"] = xmlDir;
			}

			xmlAttrPtr currAttribute = currNode->properties;
			while(currAttribute != NULL)
			{
				itemProperties[(const char *)currAttribute->name] = (const char *)currAttribute->children->content;
				currAttribute = currAttribute->next;
			}
			std::vector<boost::shared_ptr<XdmfItem> > childItems = this->read(currNode->children, xPathContext, xPathMap, xmlDir);
			boost::shared_ptr<XdmfItem> newItem = mItemFactory->createItem((const char *)currNode->name, itemProperties);
			newItem->populateItem(itemProperties, childItems);
			myItems.push_back(newItem);
			xPathMap[currNode] = newItem;
		}
	}

	const boost::shared_ptr<const XdmfCoreItemFactory> mItemFactory;
};

XdmfCoreReader::XdmfCoreReader(const boost::shared_ptr<const XdmfCoreItemFactory> itemFactory) :
	mImpl(new XdmfCoreReaderImpl(itemFactory))
{
	std::cout << "Created XdmfReader " << this << std::endl;
}

XdmfCoreReader::~XdmfCoreReader()
{
	delete mImpl;
	std::cout << "Deleted XdmfReader " << this << std::endl;
}

boost::shared_ptr<XdmfItem> XdmfCoreReader::read(const std::string & filePath) const
{
	std::string xmlDir = XdmfObject::getRealPath(filePath);
	size_t index = xmlDir.find_last_of("/\\");
	if(index != std::string::npos)
	{
	    xmlDir = xmlDir.substr(0, index + 1);
	}

	const xmlDocPtr document = xmlReadFile(filePath.c_str(), NULL, 0);
	const xmlXPathContextPtr xPathContext = xmlXPtrNewContext(document, NULL, NULL);
	if(document == NULL)
	{
		assert(false);
	}
	const xmlNodePtr currNode = xmlDocGetRootElement(document);

	std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> > xPathMap;
	const std::vector<boost::shared_ptr<XdmfItem> > toReturn = mImpl->read(currNode->children, xPathContext, xPathMap, xmlDir);
	xmlXPathFreeContext(xPathContext);
	xmlFreeDoc(document);
	xmlCleanupParser();

	return toReturn[0];
}
