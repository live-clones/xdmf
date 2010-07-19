// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <libxml/tree.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfItem.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"

/**
 * PIMPL
 */
class XdmfWriter::XdmfWriterImpl {

public:

	XdmfWriterImpl(const std::string & xmlFilePath, const boost::shared_ptr<XdmfHDF5Writer> hdf5Writer) :
		mHDF5Writer(hdf5Writer),
		mLightDataLimit(100),
		mMode(Default),
		mXMLCurrentNode(NULL),
		mXMLDocument(NULL),
		mXMLFilePath(xmlFilePath),
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
	unsigned int mLightDataLimit;
	Mode mMode;
	xmlNodePtr mXMLCurrentNode;
	xmlDocPtr mXMLDocument;
	std::string mXMLFilePath;
	std::map<const XdmfItem * const, std::string> mXPath;
	unsigned int mXPathCount;
	std::string mXPathString;
};

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
	std::cout << "Created XdmfWriter " << this << std::endl;
}

XdmfWriter::XdmfWriter(const std::string & xmlFilePath, boost::shared_ptr<XdmfHDF5Writer> hdf5Writer) :
	mImpl(new XdmfWriterImpl(xmlFilePath, hdf5Writer))
{
	std::cout << "Created XdmfWriter " << this << std::endl;
}

XdmfWriter::~XdmfWriter()
{
	delete mImpl;
	std::cout << "Deleted XdmfWriter " << this << std::endl;
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

void XdmfWriter::setLightDataLimit(const unsigned int numValues)
{
	mImpl->mLightDataLimit = numValues;
}

void XdmfWriter::setMode(const Mode mode)
{
	mImpl->mMode = mode;
}

void XdmfWriter::visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor)
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
			std::string contentVal = array.getHDF5Controller()->getDataSetPath();
			if(size_t colonLocation = contentVal.find(":") != std::string::npos)
			{
				if(size_t fileDir = contentVal.substr(0, colonLocation).find_last_of("/\\") != std::string::npos)
				{
					// Absolute Path
					std::string cwd = XdmfObject::getCWD();
					if(size_t relPathBegin = contentVal.find(cwd) != std::string::npos)
					{
						// Substitute Relative Path
						xmlTextValues << contentVal.substr(cwd.size() + 1, contentVal.size() - cwd.size());
					}
					else
					{
						// Write Absolute Path
						xmlTextValues << contentVal;
					}
				}
				else
				{
					// Relative Path
					xmlTextValues << contentVal;
				}
			}
			else
			{
				assert(false);
			}
		}
		else
		{
			xmlTextValues << array.getValuesString();
		}

		this->visit(dynamic_cast<XdmfItem &>(array), visitor);
		xmlAddChild(mImpl->mXMLCurrentNode->last, xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
	}
}

void XdmfWriter::visit(XdmfItem & item, const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	if(mImpl->mXPathString.compare("") == 0)
	{
		mImpl->openFile();
	}

	mImpl->mXPathCount++;

	xmlNodePtr parentNode = mImpl->mXMLCurrentNode;
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

	mImpl->mXMLCurrentNode = parentNode;
	mImpl->mXPathString = parentXPathString;

	if(mImpl->mXPathString.compare("") == 0)
	{
		mImpl->closeFile();
	}
}
