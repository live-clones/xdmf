// Kenneth Leiter
// Xdmf Smart Pointer Test

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

	std::vector<boost::shared_ptr<XdmfItem> > read(xmlNodePtr currNode) const
	{
		std::vector<boost::shared_ptr<XdmfItem> > myItems;

		while(currNode != NULL)
		{
			if(currNode->type == XML_ELEMENT_NODE)
			{
				std::map<std::string, std::string> itemProperties;
				std::cout << currNode->name << std::endl;
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
				std::vector<boost::shared_ptr<XdmfItem> > childItems = this->read(currNode->children);
				boost::shared_ptr<XdmfItem> newItem = mItemFactory->createItem((const char *)currNode->name, itemProperties);
				newItem->populateItem(itemProperties, childItems);
				myItems.push_back(newItem);
			}
			currNode = currNode->next;
		}
		return myItems;
	}

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
	if(document == NULL)
	{
		assert(false);
	}
	currNode = xmlDocGetRootElement(document);

	std::vector<boost::shared_ptr<XdmfItem> > toReturn = mImpl->read(currNode->children);

	xmlFreeDoc(document);
	xmlCleanupParser();

	return toReturn[0];
}
