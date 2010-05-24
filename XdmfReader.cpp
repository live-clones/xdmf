// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfArray.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfTopology.hpp"


#include <libxml/xmlreader.h>
#include <map>
#include "XdmfReader.hpp"

/**
 * PIMPL
 */
class XdmfReader::XdmfReaderImpl {

public:

	XdmfReaderImpl()
	{
	};

	~XdmfReaderImpl()
	{
	};

	boost::shared_ptr<XdmfItem> constructItem(const std::string & itemTag, const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems) const
	{
		boost::shared_ptr<XdmfItem> newItem;
		if(itemTag.compare(XdmfArray::ItemTag) == 0)
		{
			newItem = XdmfArray::New();
		}
		else if(itemTag.compare(XdmfAttribute::ItemTag) == 0)
		{
			newItem = XdmfAttribute::New();
		}
		else if(itemTag.compare(XdmfDomain::ItemTag) == 0)
		{
			newItem = XdmfDomain::New();
		}
		else if(itemTag.compare(XdmfGeometry::ItemTag) == 0)
		{
			newItem = XdmfGeometry::New();
		}
		else if(itemTag.compare(XdmfGrid::ItemTag) == 0)
		{
			newItem = XdmfGrid::New();
		}
		else if(itemTag.compare(XdmfTopology::ItemTag) == 0)
		{
			newItem = XdmfTopology::New();
		}
		else
		{
			assert(false);
		}
		newItem->populateItem(itemProperties, childItems);
		return newItem;
	}

	std::vector<boost::shared_ptr<XdmfItem> > read(xmlNodePtr currNode) const
	{
		std::vector<boost::shared_ptr<XdmfItem> > myItems;

		while(currNode != NULL)
		{
			if(currNode->type == XML_ELEMENT_NODE)
			{
				std::map<std::string, std::string> itemProperties;
				itemProperties["Content"] = (const char *)currNode->children->content;
				xmlAttrPtr currAttribute = currNode->properties;
				while(currAttribute != NULL)
				{
					itemProperties[(const char *)currAttribute->name] = (const char *)currAttribute->children->content;
					currAttribute = currAttribute->next;
				}
				std::vector<boost::shared_ptr<XdmfItem> > childItems = this->read(currNode->children);
				myItems.push_back(this->constructItem((const char *)currNode->name, itemProperties, childItems));
			}
			currNode = currNode->next;
		}
		return myItems;
	}
};

XdmfReader::XdmfReader() :
	mImpl(new XdmfReaderImpl())
{
	std::cout << "Created XdmfReader " << this << std::endl;
}

XdmfReader::~XdmfReader()
{
	delete mImpl;
	std::cout << "Deleted XdmfReader " << this << std::endl;
}

boost::shared_ptr<XdmfDomain> XdmfReader::read(const std::string & filePath) const
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

	return boost::shared_dynamic_cast<XdmfDomain>(toReturn[0]);
}


