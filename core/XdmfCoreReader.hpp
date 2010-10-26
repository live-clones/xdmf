#ifndef XDMFCOREREADER_HPP_
#define XDMFCOREREADER_HPP_

// Forward Declarations
class XdmfCoreItemFactory;
class XdmfItem;

// Includes
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

/**
 * @brief Reads an Xdmf structured file stored on disk into memory.
 *
 * Reads an Xdmf structured file stored on disk into an Xdmf structure in memory.  All light data is parsed in order to create appropriate
 * Xdmf objects.  Heavy data controllers are created and attached to XdmfArrays but no heavy data is read into memory.
 *
 * XdmfCoreReader is an abstract base class.
 */
class XdmfCoreReader {

public:

	virtual ~XdmfCoreReader() = 0;

	/**
	 * Read an Xdmf file from disk into memory.
	 *
	 * @param filePath the path of the Xdmf file to read in from disk.
	 * @return an XdmfItem at the root of the Xdmf tree.
	 */
	virtual boost::shared_ptr<XdmfItem> read(const std::string & filePath) const;

	/**
	 * Read an Xdmf file from disk into memory.
	 *
	 * @param filePath the path of the Xdmf file to read in from disk.
	 * @return a vector of XdmfItems at the root of the Xdmf tree.
	 */
	virtual std::vector<boost::shared_ptr<XdmfItem> > readItems(const std::string & filePath) const;

	/**
	 * Read part of an Xdmf file from disk into memory.
	 *
	 * @param filePath the path of the Xdmf file to read in from disk.
	 * @param xPath an XPath corresponding to the portion of the file to read.
	 * @return a vector of XdmfItems that are included in the XPath.
	 */
	virtual std::vector<boost::shared_ptr<XdmfItem> > read(const std::string & filePath, const std::string & xPath) const;

	std::vector<boost::shared_ptr<XdmfItem> > readPathObjects(const std::string & xPath) const;

protected:

	/**
	 * Constructor
	 *
	 * @param itemFactory an XdmfCoreItemFactory to construct XdmfItems for a specific Xdmf language.
	 */
	XdmfCoreReader(const boost::shared_ptr<const XdmfCoreItemFactory> itemFactory);

private:

	/**
	 * PIMPL
	 */
	class XdmfCoreReaderImpl;

	XdmfCoreReader(const XdmfCoreReader & reader);  // Not implemented.
	void operator=(const XdmfCoreReader & reader);  // Not implemented.

	XdmfCoreReaderImpl * const mImpl;
};

#endif /* XDMFCOREREADER_HPP_ */
