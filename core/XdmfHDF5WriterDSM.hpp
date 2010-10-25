#ifndef XDMFHDF5WRITERDSM_HPP_
#define XDMFHDF5WRITERDSM_HPP_

// Forward Declarations
class H5FDdsmBuffer;

// Includes
#include "XdmfHDF5Writer.hpp"

/**
 * @brief Traverse the Xdmf graph and write heavy data stored in XdmfArrays to a DSM buffer.
 *
 * XdmfHDF5WriterDSM traverses an Xdmf graph structure and writes data stored in XdmfArrays to a DSM buffer.
 * Writing begins by calling the accept() operation on any XdmfItem and supplying this writer
 * as the parameter.  The writer will write all XdmfArrays under the XdmfItem to a DSM Buffer
 * It will also attach an XdmfHDF5Controller to all XdmfArrays that it writes to disk.
 *
 * This writer supports all heavy data writing modes listed in XdmfHeavyDataWriter.
 */
class XdmfHDF5WriterDSM : public XdmfHDF5Writer {

public:

	/**
	 * Construct XdmfHDF5WriterDSM
	 *
	 * @param filePath the location of the hdf5 file to output to on disk.
	 * @param dsmBuffer the dsm buffer to write to.
	 * @return new XdmfHDF5WriterDSM.
	 */
	static boost::shared_ptr<XdmfHDF5WriterDSM> New(const std::string & filePath, H5FDdsmBuffer * const dsmBuffer);

	virtual ~XdmfHDF5WriterDSM();

	void visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfHDF5WriterDSM(const std::string & filePath, H5FDdsmBuffer * const dsmBuffer);
	virtual boost::shared_ptr<XdmfHDF5Controller> createHDF5Controller(const std::string & hdf5FilePath, const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type);

private:

	XdmfHDF5WriterDSM(const XdmfHDF5WriterDSM & hdf5Writer);  // Not implemented.
	void operator=(const XdmfHDF5WriterDSM & hdf5Writer);  // Not implemented.

	H5FDdsmBuffer * mDSMBuffer;

};

#endif /* XDMFHDF5WRITERDSM_HPP_ */
