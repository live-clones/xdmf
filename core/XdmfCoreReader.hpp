#ifndef XDMFCOREREADER_HPP_
#define XDMFCOREREADER_HPP_

// Forward Declarations
class XdmfCoreItemFactory;
class XdmfItem;

// Includes
#include <boost/shared_ptr.hpp>
#include <string>

/**
 * @brief Reads an Xdmf structured file stored on disk into memory.
 *
 * Reads an Xdmf structured file stored on disk into an Xdmf structure in memory.  All light data is parsed in order to create appropriate
 * Xdmf objects.  Heavy data controllers are created and attached to XdmfArrays but no heavy data is read into memory.
 */
class XdmfCoreReader {

public:

	virtual ~XdmfCoreReader();

	/**
	 * Read an Xdmf file from disk into memory.
	 *
	 * @param filePath the path of the Xdmf file to read in from disk.
	 * @return an XdmfItem at the root of the Xdmf tree.
	 */
	virtual boost::shared_ptr<XdmfItem> read(const std::string & filePath) const;

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

	const XdmfCoreReaderImpl * const mImpl;
};

#endif /* XDMFCOREREADER_HPP_ */
