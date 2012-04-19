/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfWriter.cpp                                                      */
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

#include <fstream>
#include <libxml/tree.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfHeavyDataWriter.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfItem.hpp"
#include "XdmfSystemUtils.hpp"
#include "XdmfWriter.hpp"
#include "XdmfVersion.hpp"

/**
 * PIMPL
 */
class XdmfWriter::XdmfWriterImpl {

public:

  XdmfWriterImpl(const std::string & xmlFilePath,
                 const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter,
                 std::ostream * stream) :
    mDepth(0),
    mDocumentTitle("Xdmf"),
    mHeavyDataWriter(heavyDataWriter),
    mLastXPathed(false),
    mLightDataLimit(100),
    mMode(Default),
    mStream(stream),
    mWriteXPaths(true),
    mXMLCurrentNode(NULL),
    mXMLDocument(NULL),
    mXMLFilePath(XdmfSystemUtils::getRealPath(xmlFilePath)),
    mXPathCount(0),
    mXPathString(""),
    mVersionString(XdmfVersion.getShort())
  {
  };

  ~XdmfWriterImpl()
  {
  };

  void
  closeFile()
  {
    mXPath.clear();

    std::ofstream fileStream;
    if(!mStream) {
      fileStream.open(mXMLFilePath.c_str());
      mStream = &fileStream;
    }

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBuffer * outputBuffer = xmlOutputBufferCreateBuffer(buffer,
                                                                 NULL);
    xmlSaveFormatFileTo(outputBuffer,
                        mXMLDocument,
                        "utf-8",
                        1);
    *mStream << buffer->content;
    xmlBufferFree(buffer);
    
    if(fileStream.is_open()) {
      fileStream.close();
      mStream = NULL;
    }
    
    xmlFreeDoc(mXMLDocument);
    xmlCleanupParser();
    
    if(mHeavyDataWriter->getMode() == XdmfHeavyDataWriter::Default) {
      mHeavyDataWriter->closeFile();
    }
  };

  void
  openFile()
  {
    mXMLDocument = xmlNewDoc((xmlChar*)"1.0");
    mXMLCurrentNode = xmlNewNode(NULL, (xmlChar*)mDocumentTitle.c_str());
    xmlNewProp(mXMLCurrentNode,
               (xmlChar*)"xmlns:xi",
               (xmlChar*)"http://www.w3.org/2001/XInclude");
    xmlNewProp(mXMLCurrentNode,
               (xmlChar*)"Version",
               (xmlChar*)mVersionString.c_str());
    xmlDocSetRootElement(mXMLDocument, mXMLCurrentNode);
    if(mHeavyDataWriter->getMode() == XdmfHeavyDataWriter::Default) {
      mHeavyDataWriter->openFile();
    }
  }

  int mDepth;
  std::string mDocumentTitle;
  shared_ptr<XdmfHeavyDataWriter> mHeavyDataWriter;
  bool mLastXPathed;
  unsigned int mLightDataLimit;
  Mode mMode;
  std::ostream * mStream;
  bool mWriteXPaths;
  xmlNodePtr mXMLCurrentNode;
  xmlDocPtr mXMLDocument;
  std::string mXMLFilePath;
  std::map<const XdmfItem * const, std::string> mXPath;
  unsigned int mXPathCount;
  std::string mXPathString;
  std::string mVersionString;

};

shared_ptr<XdmfWriter>
XdmfWriter::New(const std::string & xmlFilePath)
{
  std::stringstream heavyFileName;
  size_t extension = xmlFilePath.rfind(".");
  if(extension != std::string::npos) {
    heavyFileName << xmlFilePath.substr(0, extension) << ".h5";
  }
  else {
    heavyFileName << xmlFilePath << ".h5";
  }
  shared_ptr<XdmfHDF5Writer> hdf5Writer = 
    XdmfHDF5Writer::New(heavyFileName.str());
  shared_ptr<XdmfWriter> p(new XdmfWriter(xmlFilePath, hdf5Writer));
  return p;
}

shared_ptr<XdmfWriter>
XdmfWriter::New(const std::string & xmlFilePath,
                const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter)
{
  shared_ptr<XdmfWriter> p(new XdmfWriter(xmlFilePath,
                                          heavyDataWriter));
  return p;
}

shared_ptr<XdmfWriter> 
XdmfWriter::New(std::ostream & stream,
                const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter)
{
  shared_ptr<XdmfWriter> p(new XdmfWriter("",
                                          heavyDataWriter,
                                          &stream));
  return p;
}

XdmfWriter::XdmfWriter(const std::string & xmlFilePath,
                       shared_ptr<XdmfHeavyDataWriter> heavyDataWriter,
                       std::ostream * stream) :
  mImpl(new XdmfWriterImpl(xmlFilePath, 
                           heavyDataWriter,
                           stream))
{
}

XdmfWriter::~XdmfWriter()
{
  delete mImpl;
}

shared_ptr<XdmfHeavyDataWriter>
XdmfWriter::getHeavyDataWriter()
{
  return boost::const_pointer_cast<XdmfHeavyDataWriter>
    (static_cast<const XdmfWriter &>(*this).getHeavyDataWriter());
}

shared_ptr<const XdmfHeavyDataWriter>
XdmfWriter::getHeavyDataWriter() const
{
  return mImpl->mHeavyDataWriter;
}

std::string
XdmfWriter::getFilePath() const
{
  return mImpl->mXMLFilePath;
}

unsigned int
XdmfWriter::getLightDataLimit() const
{
  return mImpl->mLightDataLimit;
}

XdmfWriter::Mode
XdmfWriter::getMode() const
{
  return mImpl->mMode;
}

bool
XdmfWriter::getWriteXPaths() const
{
  return mImpl->mWriteXPaths;
}

void
XdmfWriter::setDocumentTitle(std::string title)
{
  mImpl->mDocumentTitle = title;
}

void
XdmfWriter::setLightDataLimit(const unsigned int numValues)
{
  mImpl->mLightDataLimit = numValues;
}

void
XdmfWriter::setMode(const Mode mode)
{
  mImpl->mMode = mode;
}

void
XdmfWriter::setVersionString(std::string version)
{
  mImpl->mVersionString = version;
}

void
XdmfWriter::setWriteXPaths(const bool writeXPaths)
{
  mImpl->mWriteXPaths = writeXPaths;
}

void
XdmfWriter::visit(XdmfArray & array,
                  const shared_ptr<XdmfBaseVisitor> visitor)
{
  if (mImpl->mDepth == 0) {
    mImpl->openFile();
  }
  mImpl->mDepth++;

  bool isSubclassed = array.getItemTag().compare(XdmfArray::ItemTag) != 0;

  if(isSubclassed) {
    this->visit(dynamic_cast<XdmfItem &>(array), visitor);
  }

  if(array.getSize() > 0 && !(mImpl->mLastXPathed && isSubclassed)) {
    std::stringstream xmlTextValues;

    // Take care of writing to single heavy data file (Default behavior)
    if(!array.isInitialized() && array.getHeavyDataController() &&
       array.getHeavyDataController()->getFilePath().compare(mImpl->mHeavyDataWriter->getFilePath()) != 0 &&
       mImpl->mMode == Default) {
      array.read();
    }

    if(array.getHeavyDataController() ||
       array.getSize() > mImpl->mLightDataLimit) {
      // Write values to heavy data
      mImpl->mHeavyDataWriter->visit(array, mImpl->mHeavyDataWriter);

      std::string heavyDataPath =
        array.getHeavyDataController()->getFilePath();
      size_t index = heavyDataPath.find_last_of("/\\");
      if(index != std::string::npos) {
        std::string heavyDataDir = heavyDataPath.substr(0, index + 1);
        if(mImpl->mXMLFilePath.find(heavyDataDir) == 0) {
          heavyDataPath =
            heavyDataPath.substr(heavyDataDir.size(),
                                 heavyDataPath.size() - heavyDataDir.size());
        }
      }

      xmlTextValues << heavyDataPath << ":"
                    << array.getHeavyDataController()->getDataSetPath();
    }
    else {
      // Write values to XML
      xmlTextValues << array.getValuesString();
    }

    bool oldWriteXPaths = mImpl->mWriteXPaths;
    mImpl->mWriteXPaths = false;

    // Write XML (metadata) description
    if(isSubclassed) {
      shared_ptr<XdmfArray> arrayToWrite = XdmfArray::New();
      array.swap(arrayToWrite);
      mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->last;
      this->visit(dynamic_cast<XdmfItem &>(*arrayToWrite.get()), visitor);
      xmlAddChild(mImpl->mXMLCurrentNode->last,
                  xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
      mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->parent;
      array.swap(arrayToWrite);
    }
    else {
      this->visit(dynamic_cast<XdmfItem &>(array), visitor);
      xmlAddChild(mImpl->mXMLCurrentNode->last,
                  xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
    }

    mImpl->mWriteXPaths = oldWriteXPaths;
  }

  mImpl->mDepth--;
  if(mImpl->mDepth <= 0) {
    mImpl->closeFile();
  }
}

void
XdmfWriter::visit(XdmfItem & item,
                  const shared_ptr<XdmfBaseVisitor> visitor)
{

  if (mImpl->mDepth == 0) {
    mImpl->openFile();
  }
  mImpl->mDepth++;

  std::string tag = item.getItemTag();
  if (tag.length() == 0) {
    item.traverse(visitor);
  }
  else {
    if(mImpl->mWriteXPaths) {
      mImpl->mXPathCount++;

      std::string parentXPathString = mImpl->mXPathString;

      std::stringstream newXPathString;
      newXPathString << mImpl->mXPathString << "/" << mImpl->mXPathCount;
      mImpl->mXPathString = newXPathString.str();

      std::map<const XdmfItem * const, std::string>::const_iterator iter =
        mImpl->mXPath.find(&item);
      if(iter != mImpl->mXPath.end()) {
        // Inserted before --- just xpath location of previously written node
        mImpl->mXMLCurrentNode = xmlNewChild(mImpl->mXMLCurrentNode,
                                             NULL,
                                             (xmlChar*)"xi:include",
                                             NULL);
        xmlNewProp(mImpl->mXMLCurrentNode,
                   (xmlChar*)"xpointer",
                   (xmlChar*)iter->second.c_str());
        mImpl->mLastXPathed = true;
      }
      else {
        // Not inserted before --- need to write all data and traverse.
        mImpl->mXMLCurrentNode = xmlNewChild(mImpl->mXMLCurrentNode,
                                             NULL,
                                             (xmlChar *)tag.c_str(),
                                             NULL);
        std::stringstream xPathProp;
        xPathProp << "element(/1" << mImpl->mXPathString << ")";
        mImpl->mXPath[&item] = xPathProp.str();
        const std::map<std::string, std::string> & itemProperties =
          item.getItemProperties();
        for(std::map<std::string, std::string>::const_iterator iter =
              itemProperties.begin();
            iter != itemProperties.end();
            ++iter) {
          xmlNewProp(mImpl->mXMLCurrentNode, (xmlChar*)iter->first.c_str(), (xmlChar*)iter->second.c_str());
        }
        unsigned int parentCount = mImpl->mXPathCount;
        mImpl->mXPathCount = 0;
        item.traverse(visitor);
        mImpl->mXPathCount = parentCount;
        mImpl->mLastXPathed = false;
      }

      mImpl->mXPathString = parentXPathString;
    }
    else
      {
        // Not inserted before --- need to write all data and traverse.
        mImpl->mXMLCurrentNode = xmlNewChild(mImpl->mXMLCurrentNode,
                                             NULL,
                                             (xmlChar*)tag.c_str(),
                                             NULL);
        const std::map<std::string, std::string> itemProperties =
          item.getItemProperties();
        for(std::map<std::string, std::string>::const_iterator iter =
              itemProperties.begin();
            iter != itemProperties.end();
            ++iter) {
          xmlNewProp(mImpl->mXMLCurrentNode,
                     (xmlChar*)iter->first.c_str(),
                     (xmlChar*)iter->second.c_str());
        }
        item.traverse(visitor);
      }

    mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->parent;
  }

  mImpl->mDepth--;
  if(mImpl->mDepth <= 0) {
    mImpl->closeFile();
  }
}
