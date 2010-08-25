// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <libxml/tree.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfItem.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfSystemUtils.hpp"
#include "XdmfWriter.hpp"

/**
 * PIMPL
 */
class XdmfWriter::XdmfWriterImpl {

public:

	XdmfWriterImpl(const std::string & xmlFilePath, const boost::shared_ptr<XdmfHDF5Writer> hdf5Writer) :
		mHDF5Writer(hdf5Writer),
		mLastXPathed(false),
		mLightDataLimit(100),
		mMode(Default),
		mWriteXPaths(true),
		mXMLCurrentNode(NULL),
		mXMLDocument(NULL),
		mXMLFilePath(XdmfSystemUtils::getRealPath(xmlFilePath)),
		mXPathCount(0),
		mXPathString("")
	{
	};

	~XdmfWriterImpl()
	{
	};

	void closeFile()
	{
		mXPath.clear();
		xmlSaveFormatFile(mXMLFilePath.c_str(), mXMLDocument, 1);
		xmlFreeDoc(mXMLDocument);
		xmlCleanupParser();
	};

	void openFile()
	{
		mXMLDocument = xmlNewDoc((xmlChar*)"1.0");
		mXMLCurrentNode = xmlNewNode(NULL, (xmlChar*)"Xdmf");
		xmlNewProp(mXMLCurrentNode, (xmlChar*)"xmlns:xi", (xmlChar*)"http://www.w3.org/2001/XInclude");
		xmlNewProp(mXMLCurrentNode, (xmlChar*)"Version", (xmlChar*)"2.0");
		xmlDocSetRootElement(mXMLDocument, mXMLCurrentNode);
	}

	boost::shared_ptr<XdmfHDF5Writer> mHDF5Writer;
	bool mLastXPathed;
	unsigned int mLightDataLimit;
	Mode mMode;
	bool mWriteXPaths;
	xmlNodePtr mXMLCurrentNode;
	xmlDocPtr mXMLDocument;
	std::string mXMLFilePath;
	std::map<const XdmfItem * const, std::string> mXPath;
	unsigned int mXPathCount;
	std::string mXPathString;
};

boost::shared_ptr<XdmfWriter> XdmfWriter::New(const std::string & xmlFilePath)
{
	boost::shared_ptr<XdmfWriter> p(new XdmfWriter(xmlFilePath));
	return p;
}

boost::shared_ptr<XdmfWriter> XdmfWriter::New(const std::string & xmlFilePath, const boost::shared_ptr<XdmfHDF5Writer> hdf5Writer)
{
	boost::shared_ptr<XdmfWriter> p(new XdmfWriter(xmlFilePath, hdf5Writer));
	return p;
}

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
}

XdmfWriter::XdmfWriter(const std::string & xmlFilePath, boost::shared_ptr<XdmfHDF5Writer> hdf5Writer) :
	mImpl(new XdmfWriterImpl(xmlFilePath, hdf5Writer))
{
}

XdmfWriter::~XdmfWriter()
{
	delete mImpl;
}

boost::shared_ptr<XdmfHDF5Writer> XdmfWriter::getHDF5Writer()
{
	return boost::const_pointer_cast<XdmfHDF5Writer>(static_cast<const XdmfWriter &>(*this).getHDF5Writer());
}

boost::shared_ptr<const XdmfHDF5Writer> XdmfWriter::getHDF5Writer() const
{
	return mImpl->mHDF5Writer;
}

std::string XdmfWriter::getFilePath() const
{
	return mImpl->mXMLFilePath;
}

unsigned int XdmfWriter::getLightDataLimit() const
{
	return mImpl->mLightDataLimit;
}

XdmfWriter::Mode XdmfWriter::getMode() const
{
	return mImpl->mMode;
}

bool XdmfWriter::getWriteXPaths() const
{
	return mImpl->mWriteXPaths;
}

void XdmfWriter::setLightDataLimit(const unsigned int numValues)
{
	mImpl->mLightDataLimit = numValues;
}

void XdmfWriter::setMode(const Mode mode)
{
	mImpl->mMode = mode;
}

void XdmfWriter::setWriteXPaths(const bool writeXPaths)
{
	mImpl->mWriteXPaths = writeXPaths;
}

void XdmfWriter::visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	bool isSubclassed = array.getItemTag().compare(XdmfArray::ItemTag) != 0;

	if(isSubclassed)
	{
		this->visit(dynamic_cast<XdmfItem &>(array), visitor);
	}

	if(array.size() > 0 && !(mImpl->mLastXPathed && isSubclassed))
	{
		std::stringstream xmlTextValues;

		// Take care of writing to single HDF5 file (Default behavior)
		if(!array.isInitialized() && array.getHDF5Controller() && array.getHDF5Controller()->getFilePath().compare(mImpl->mHDF5Writer->getFilePath()) != 0 && mImpl->mMode == Default)
		{
			array.read();
		}

		if(array.getHDF5Controller() || array.size() > mImpl->mLightDataLimit)
		{
			// Write values to HDF5
			mImpl->mHDF5Writer->visit(array, mImpl->mHDF5Writer);

			std::string hdf5Path = array.getHDF5Controller()->getFilePath();
			size_t index = hdf5Path.find_last_of("/\\");
			if(index != std::string::npos)
			{
				std::string hdf5Dir = hdf5Path.substr(0, index + 1);
				if(mImpl->mXMLFilePath.find(hdf5Dir) == 0)
				{
					hdf5Path = hdf5Path.substr(hdf5Dir.size(), hdf5Path.size() - hdf5Dir.size());
				}
			}

			xmlTextValues << hdf5Path << ":" << array.getHDF5Controller()->getDataSetPath();
		}
		else
		{
			// Write values to XML
			xmlTextValues << array.getValuesString();
		}

		bool oldWriteXPaths = mImpl->mWriteXPaths;
		mImpl->mWriteXPaths = false;

		// Write XML (metadata) description
		if(isSubclassed)
		{
			boost::shared_ptr<XdmfArray> arrayToWrite = XdmfArray::New();
			array.swap(arrayToWrite);
			mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->last;
			this->visit(dynamic_cast<XdmfItem &>(*arrayToWrite.get()), visitor);
			xmlAddChild(mImpl->mXMLCurrentNode->last, xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
			mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->parent;
			array.swap(arrayToWrite);
		}
		else
		{
			this->visit(dynamic_cast<XdmfItem &>(array), visitor);
			xmlAddChild(mImpl->mXMLCurrentNode->last, xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
		}

		mImpl->mWriteXPaths = oldWriteXPaths;
	}
}

void XdmfWriter::visit(XdmfItem & item, const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	if(mImpl->mXPathString.compare("") == 0)
	{
		mImpl->openFile();
	}

	if(mImpl->mWriteXPaths)
	{
		mImpl->mXPathCount++;

		std::string parentXPathString = mImpl->mXPathString;

		std::stringstream newXPathString;
		newXPathString << mImpl->mXPathString << "/" << mImpl->mXPathCount;
		mImpl->mXPathString = newXPathString.str();

		std::map<const XdmfItem * const, std::string>::const_iterator iter = mImpl->mXPath.find(&item);
		if(iter != mImpl->mXPath.end())
		{
			// Inserted before --- just xpath location of previously written node
			mImpl->mXMLCurrentNode = xmlNewChild(mImpl->mXMLCurrentNode, NULL, (xmlChar*)"xi:include", NULL);
			xmlNewProp(mImpl->mXMLCurrentNode, (xmlChar*)"xpointer", (xmlChar*)iter->second.c_str());
			mImpl->mLastXPathed = true;
		}
		else
		{
			// Not inserted before --- need to write all data and traverse.
			mImpl->mXMLCurrentNode = xmlNewChild(mImpl->mXMLCurrentNode, NULL, (xmlChar*)item.getItemTag().c_str(), NULL);
			std::stringstream xPathProp;
			xPathProp << "element(/1" << mImpl->mXPathString << ")";
			mImpl->mXPath[&item] = xPathProp.str();
			const std::map<std::string, std::string> itemProperties = item.getItemProperties();
			for(std::map<std::string, std::string>::const_iterator iter = itemProperties.begin(); iter != itemProperties.end(); ++iter)
			{
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
		mImpl->mXMLCurrentNode = xmlNewChild(mImpl->mXMLCurrentNode, NULL, (xmlChar*)item.getItemTag().c_str(), NULL);
		const std::map<std::string, std::string> itemProperties = item.getItemProperties();
		for(std::map<std::string, std::string>::const_iterator iter = itemProperties.begin(); iter != itemProperties.end(); ++iter)
		{
			xmlNewProp(mImpl->mXMLCurrentNode, (xmlChar*)iter->first.c_str(), (xmlChar*)iter->second.c_str());
		}
		item.traverse(visitor);
	}

	mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->parent;

	if(mImpl->mXPathString.compare("") == 0)
	{
		mImpl->closeFile();
	}
}
