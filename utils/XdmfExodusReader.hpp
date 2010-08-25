#ifndef XDMFEXODUSREADER_HPP_
#define XDMFEXODUSREADER_HPP_

// Forward Declarations
class XdmfGrid;
class XdmfHDF5Writer;
class XdmfTopologyType;

// Includes
#include <boost/shared_ptr.hpp>
#include <string>

/*!
 * @brief XdmfExodusReader reads an ExodusII file from disk into an Xdmf structure in memory.
 */
class XdmfExodusReader {

public:

	/**
	 * Create a new XdmfExodusReader.
	 *
	 * @return constructed XdmfExodusReader.
	 */
	static boost::shared_ptr<XdmfExodusReader> New();

	virtual ~XdmfExodusReader();

    /*!
     * Read the contents of an ExodusII file from disk into an Xdmf structure in memory..
     *
     * @param fileName containing the path of the exodus file to read.
     * @param heavyDataWriter an XdmfHDF5Writer to write the mesh to.  If no heavyDataWriter is specified, all mesh data will remain in memory.
     *
     * @return XdmfGrid containing the mesh stored in the ExodusII file.
     */
	boost::shared_ptr<XdmfGrid> read(const std::string & fileName, const boost::shared_ptr<XdmfHDF5Writer> heavyDataWriter = boost::shared_ptr<XdmfHDF5Writer>()) const;

protected:

	XdmfExodusReader();

private:

	XdmfExodusReader(const XdmfExodusReader & exodusReader);  // Not implemented.
	void operator=(const XdmfExodusReader & exodusReader);  // Not implemented.

	/**
	 * Convert an Exodus topology type to an XdmfTopologyType.
	 *
	 * @param exodusTopologyType a string containing the name of the exodus topology type to convert.
	 * @param pointsPerCell the number of points per cell for the exodus topology type to convert.
	 *
	 * @return the equivalent XdmfTopologyType.
	 */
	boost::shared_ptr<const XdmfTopologyType> exodusToXdmfTopologyType(std::string exodusTopologyType, const int pointsPerCell) const;
};

#endif /* XDMFEXODUSREADER_HPP_ */
