// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <libxml/tree.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfCoreWriter.hpp"
#include "XdmfItem.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfSystemUtils.hpp"

/**
 * PIMPL
 */
class XdmfCoreWriter::XdmfCoreWriterImpl {

public:

	XdmfCoreWriterImpl(const std::string & xmlFilePath, const boost::shared_ptr<XdmfHDF5Writer> hdf5Writer) :
		mHDF5Writer(hdf5Writer),
		mLightDataLimit(100),
		mMode(Default),
		mXMLCurrentNode(NULL),
		mXMLDocument(NULL),
		mXPathCount(0),
		mXPathString("")
	{
		mXMLFilePath = XdmfSystemUtils::getRealPath(xmlFilePath);
	};

	~XdmfCoreWriterImpl()
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
	unsigned int mLightDataLimit;
	Mode mMode;
	xmlNodePtr mXMLCurrentNode;
	xmlDocPtr mXMLDocument;
	std::string mXMLFilePath;
	std::map<const XdmfItem * const, std::string> mXPath;
	unsigned int mXPathCount;
	std::string mXPathString;
};

XdmfCoreWriter::XdmfCoreWriter(const std::string & xmlFilePath)
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
	mImpl = new XdmfCoreWriterImpl(xmlFilePath, hdf5Writer);
}

XdmfCoreWriter::XdmfCoreWriter(const std::string & xmlFilePath, boost::shared_ptr<XdmfHDF5Writer> hdf5Writer) :
	mImpl(new XdmfCoreWriterImpl(xmlFilePath, hdf5Writer))
{
}

XdmfCoreWriter::~XdmfCoreWriter()
{
	delete mImpl;
}

boost::shared_ptr<XdmfHDF5Writer> XdmfCoreWriter::getHDF5Writer()
{
	return boost::const_pointer_cast<XdmfHDF5Writer>(static_cast<const XdmfCoreWriter &>(*this).getHDF5Writer());
}

boost::shared_ptr<const XdmfHDF5Writer> XdmfCoreWriter::getHDF5Writer() const
{
	return mImpl->mHDF5Writer;
}

std::string XdmfCoreWriter::getFilePath() const
{
	return mImpl->mXMLFilePath;
}

unsigned int XdmfCoreWriter::getLightDataLimit() const
{
	return mImpl->mLightDataLimit;
}

XdmfCoreWriter::Mode XdmfCoreWriter::getMode() const
{
	return mImpl->mMode;
}

void XdmfCoreWriter::moveToLastWrittenNode()
{
	mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->last;
}
void XdmfCoreWriter::moveToParentNode()
{
	mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->parent;
}

void XdmfCoreWriter::setLightDataLimit(const unsigned int numValues)
{
	mImpl->mLightDataLimit = numValues;
}

void XdmfCoreWriter::setMode(const Mode mode)
{
	mImpl->mMode = mode;
}

void XdmfCoreWriter::visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	if(array.getSize() > 0)
	{
		std::stringstream xmlTextValues;

		if(array.getHDF5Controller() && array.getHDF5Controller()->getFilePath().compare(mImpl->mHDF5Writer->getFilePath()) != 0 && mImpl->mMode == Default)
		{
			array.read();
		}

		if(array.getHDF5Controller() || array.getSize() > mImpl->mLightDataLimit)
		{
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
			xmlTextValues << array.getValuesString();
		}

		this->visit(dynamic_cast<XdmfItem &>(array), visitor);
		xmlAddChild(mImpl->mXMLCurrentNode->last, xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
	}
}

void XdmfCoreWriter::visit(XdmfItem & item, const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	if(mImpl->mXPathString.compare("") == 0)
	{
		mImpl->openFile();
	}

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
	}

	mImpl->mXMLCurrentNode = mImpl->mXMLCurrentNode->parent;
	mImpl->mXPathString = parentXPathString;

	if(mImpl->mXPathString.compare("") == 0)
	{
		mImpl->closeFile();
	}
}
