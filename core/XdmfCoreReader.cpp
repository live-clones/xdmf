/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfCoreReader.cpp                                                  */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#include <libxml/xpointer.h>
#include <libxml/xmlreader.h>
#include <map>
#include <sstream>
#include <cstring>
#include "XdmfCoreItemFactory.hpp"
#include "XdmfCoreReader.hpp"
#include "XdmfItem.hpp"
#include "XdmfSystemUtils.hpp"
#include "XdmfError.hpp"

/**
 * PIMPL
 */
class XdmfCoreReader::XdmfCoreReaderImpl {

public:

  XdmfCoreReaderImpl(const boost::shared_ptr<const XdmfCoreItemFactory> itemFactory,
                     const XdmfCoreReader * const coreReader) :
    mCoreReader(coreReader),
    mItemFactory(itemFactory)
  {
  };

  ~XdmfCoreReaderImpl()
  {
  };

  void
  closeFile()
  {
    mXPathMap.clear();
    xmlXPathFreeContext(mXPathContext);
    xmlFreeDoc(mDocument);
    xmlCleanupParser();
  }

  void
  openFile(const std::string & filePath)
  {
    mXMLDir = XdmfSystemUtils::getRealPath(filePath);
    size_t index = mXMLDir.find_last_of("/\\");
    if(index != std::string::npos) {
      mXMLDir = mXMLDir.substr(0, index + 1);
    }

    mDocument = xmlReadFile(filePath.c_str(), NULL, 0);

    if(mDocument == NULL) {
      XdmfError::message(XdmfError::FATAL, "xmlReadFile could not read "+filePath+" in XdmfCoreReader::XdmfCoreReaderImpl::openFile");
    }

    mXPathContext = xmlXPtrNewContext(mDocument, NULL, NULL);
    mXPathMap.clear();
  }

  /**
   * Constructs XdmfItems for all nodes in currNode's tree.
   * XdmfItems are constructed by recursively calling this function for all
   * children of currNode.
   */
  std::vector<boost::shared_ptr<XdmfItem> >
  read(xmlNodePtr currNode)
  {
    std::vector<boost::shared_ptr<XdmfItem> > myItems;

    while(currNode != NULL) {
      if(currNode->type == XML_ELEMENT_NODE) {
        xmlChar * xpointer = NULL;
        if(xmlStrcmp(currNode->name, (xmlChar*)"include") == 0) {
          // Deal with proper reading of XIncludes
          xmlAttrPtr currAttribute = currNode->properties;
          while(currAttribute != NULL) {
            if(xmlStrcmp(currAttribute->name, (xmlChar*)"xpointer") == 0) {
              xpointer = currAttribute->children->content;
              break;
            }
          }
          if(xpointer) {
            xmlXPathObjectPtr xPathObject =
              xmlXPtrEval(xpointer, mXPathContext);
            if(xPathObject && xPathObject->nodesetval) {
              for(int i=0; i<xPathObject->nodesetval->nodeNr; ++i) {
                this->readSingleNode(xPathObject->nodesetval->nodeTab[i],
                                     myItems);
              }
            }
            xmlXPathFreeObject(xPathObject);
          }
        }
        else {
          // Normal reading
          this->readSingleNode(currNode, myItems);
        }
      }
      currNode = currNode->next;
    }
    return myItems;
  }

  /**
   * Reads a single xmlNode into an XdmfItem object in memory. The constructed
   * XdmfItem is added to myItems and an entry is added mapping the xmlNodePtr
   * to the new XdmfItem in the mXPathMap.
   */
  void
  readSingleNode(const xmlNodePtr currNode,
                 std::vector<boost::shared_ptr<XdmfItem> > & myItems)
  {
    std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> >::const_iterator iter =
      mXPathMap.find(currNode);
    if(iter != mXPathMap.end()) {
      myItems.push_back(iter->second);
    }
    else {
      std::map<std::string, std::string> itemProperties;

      xmlNodePtr childNode = currNode->children;
      while(childNode != NULL) {
        if(childNode->type == XML_TEXT_NODE && childNode->content) {
          const char * content = (char*)childNode->content;

          // determine if content is whitespace
          const size_t contentSize = std::strlen(content);
          bool whitespace = true;

          for(int i=0; i<contentSize; ++i) {
            if(!isspace(content[i])) {
              whitespace = false;
              break;
            }
          }

          if(!whitespace) {
            itemProperties["Content"] = content;
            itemProperties["XMLDir"] = mXMLDir;
            break;
          }
        }
        childNode = childNode->next;
      }
      xmlAttrPtr currAttribute = currNode->properties;
      while(currAttribute != NULL) {
        itemProperties[(const char *)currAttribute->name] =
          (const char *)currAttribute->children->content;
        currAttribute = currAttribute->next;
      }

      std::vector<boost::shared_ptr<XdmfItem> > childItems =
        this->read(currNode->children);
      boost::shared_ptr<XdmfItem> newItem =
        mItemFactory->createItem((const char *)currNode->name,
                                 itemProperties,
                                 childItems);
      if(newItem == NULL) {
        XdmfError::message(XdmfError::FATAL, "mItemFactory failed to createItem in XdmfCoreReader::XdmfCoreReaderImpl::readSingleNode");
      }

      newItem->populateItem(itemProperties, childItems, mCoreReader);
      myItems.push_back(newItem);
      mXPathMap[currNode] = newItem;
    }
  }

  void
  readPathObjects(const std::string & xPath,
                  std::vector<boost::shared_ptr<XdmfItem> > & myItems)
  {
    xmlXPathObjectPtr xPathObject =
      xmlXPathEvalExpression((xmlChar*)xPath.c_str(), mXPathContext);
    if(xPathObject && xPathObject->nodesetval) {
      for(int i=0; i<xPathObject->nodesetval->nodeNr; ++i) {
        this->readSingleNode(xPathObject->nodesetval->nodeTab[i], myItems);
      }
    }
    xmlXPathFreeObject(xPathObject);
  }

  xmlDocPtr mDocument;
  const XdmfCoreReader * const mCoreReader;
  const boost::shared_ptr<const XdmfCoreItemFactory> mItemFactory;
  std::string mXMLDir;
  xmlXPathContextPtr mXPathContext;
  std::map<xmlNodePtr, boost::shared_ptr<XdmfItem> > mXPathMap;
};

XdmfCoreReader::XdmfCoreReader(const boost::shared_ptr<const XdmfCoreItemFactory> itemFactory) :
  mImpl(new XdmfCoreReaderImpl(itemFactory, this))
{
}

XdmfCoreReader::~XdmfCoreReader()
{
  delete mImpl;
}

std::vector<boost::shared_ptr<XdmfItem> >
XdmfCoreReader::readItems(const std::string & filePath) const
{
  mImpl->openFile(filePath);
  const xmlNodePtr currNode = xmlDocGetRootElement(mImpl->mDocument);
  const std::vector<boost::shared_ptr<XdmfItem> > toReturn =
    mImpl->read(currNode->children);
  mImpl->closeFile();
  return toReturn;
}

boost::shared_ptr<XdmfItem>
XdmfCoreReader::read(const std::string & filePath) const
{
  const std::vector<boost::shared_ptr<XdmfItem> > toReturn =
    readItems(filePath);
  if (toReturn.size() == 0) {
    return(boost::shared_ptr<XdmfItem>());
  }
  return(toReturn[0]);
}

std::vector<boost::shared_ptr<XdmfItem> >
XdmfCoreReader::read(const std::string & filePath,
                     const std::string & xPath) const
{
  mImpl->openFile(filePath);
  std::vector<boost::shared_ptr<XdmfItem> > toReturn =
    this->readPathObjects(xPath);
  mImpl->closeFile();
  return toReturn;
}

std::vector<boost::shared_ptr<XdmfItem> >
XdmfCoreReader::readPathObjects(const std::string & xPath) const
{
  std::vector<boost::shared_ptr<XdmfItem> > toReturn;
  mImpl->readPathObjects(xPath, toReturn);
  return toReturn;
}

