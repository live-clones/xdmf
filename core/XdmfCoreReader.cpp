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

	XdmfCoreReaderImpl(const boost::shared_ptr<XdmfCoreItemFactory> itemFactory) :
		mItemFactory(itemFactory)
	{
	};

	~XdmfCoreReaderImpl()
	{
	};

	std::vector<boost::shared_ptr<XdmfItem> > read(xmlNodePtr currNode, bool ignoreSiblings = false)
	{
		std::vector<boost::shared_ptr<XdmfItem> > myItems;

		while(currNode != NULL)
		{
			if(currNode->type == XML_ELEMENT_NODE)
			{
				xmlChar * xpointer = NULL;
				if(xmlStrcmp(currNode->name, (xmlChar*)"include") == 0)
				{
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
						std::vector<boost::shared_ptr<XdmfItem> > newItems = this->read(xPathObject->nodesetval->nodeTab[i], true);
						myItems.insert(myItems.end(), newItems.begin(), newItems.end());
					}
					xmlXPathFreeObject(xPathObject);
				}
				else
				{
					std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> >::const_iterator iter = mXPathMap.find(currNode);
					if(iter != mXPathMap.end())
					{
						myItems.push_back(iter->second);
					}
					else
					{
						std::map<std::string, std::string> itemProperties;
						if(currNode->children != NULL)
						{
							itemProperties["Content"] = (const char *)currNode->children->content;
						}
						else
						{
							itemProperties["Content"] = "";
						}
						xmlAttrPtr currAttribute = currNode->properties;
						while(currAttribute != NULL)
						{
							itemProperties[(const char *)currAttribute->name] = (const char *)currAttribute->children->content;
							currAttribute = currAttribute->next;
						}
						std::vector<boost::shared_ptr<XdmfItem> > childItems = this->read(currNode->children, ignoreSiblings);
						boost::shared_ptr<XdmfItem> newItem = mItemFactory->createItem((const char *)currNode->name, itemProperties);
						newItem->populateItem(itemProperties, childItems);
						myItems.push_back(newItem);
						mXPathMap[currNode] = newItem;
					}
				}
			}
			if(ignoreSiblings)
			{
				currNode = NULL;
			}
			else
			{
				currNode = currNode->next;
			}
		}
		return myItems;
	}

	xmlXPathContextPtr mXPathContext;
	std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> > mXPathMap;

private:

	const boost::shared_ptr<XdmfCoreItemFactory> mItemFactory;
};

XdmfCoreReader::XdmfCoreReader(const boost::shared_ptr<XdmfCoreItemFactory> itemFactory) :
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
	xmlDocPtr document;
	xmlNodePtr currNode;

	document = xmlReadFile(filePath.c_str(), NULL, 0);
	mImpl->mXPathContext = xmlXPtrNewContext(document, NULL, NULL);
	if(document == NULL)
	{
		assert(false);
	}
	currNode = xmlDocGetRootElement(document);

	std::vector<boost::shared_ptr<XdmfItem> > toReturn = mImpl->read(currNode->children);

	mImpl->mXPathMap.clear();
	xmlXPathFreeContext(mImpl->mXPathContext);
	xmlFreeDoc(document);
	xmlCleanupParser();

	return toReturn[0];
}
