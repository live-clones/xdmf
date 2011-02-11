/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5Writer.hpp                                                  */
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

#ifndef XDMFHDF5WRITER_HPP_
#define XDMFHDF5WRITER_HPP_

// Forward Declarations
class XdmfArrayType;
class XdmfHDF5Controller;

// Includes
#include "XdmfCore.hpp"
#include "XdmfHeavyDataWriter.hpp"

/**
 * @brief Traverse the Xdmf graph and write heavy data stored in XdmfArrays to HDF5 on disk.
 *
 * XdmfHDF5Writer traverses an Xdmf graph structure and writes data stored in XdmfArrays to HDF5.
 * Writing begins by calling the accept() operation on any XdmfItem and supplying this writer
 * as the parameter.  The writer will write all XdmfArrays under the XdmfItem to an hdf5 file on
 * disk.  It will also attach an XdmfHDF5Controller to all XdmfArrays that it writes to disk.
 *
 * This writer supports all heavy data writing modes listed in XdmfHeavyDataWriter.
 */
class XDMFCORE_EXPORT XdmfHDF5Writer : public XdmfHeavyDataWriter {

public:

	/**
	 * Construct XdmfHDF5Writer
	 *
	 * @param filePath the location of the hdf5 file to output to on disk.
	 * @return new XdmfHDF5Writer
	 */
	static boost::shared_ptr<XdmfHDF5Writer> New(const std::string & filePath);

	virtual ~XdmfHDF5Writer();

	virtual void visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfHDF5Writer(const std::string & filePath);

	/**
	 * Create a new HDF5 Controller that is able to read in a dataset after being written by this writer.
	 *
	 * @param hdf5FilePath the location of the hdf5 file the data set resides in.
	 * @param dataSetPath the location of the dataset within the hdf5 file.
	 * @param size the size of the dataset to read.
	 * @param type the data type of the dataset to read.
	 *
	 * @return new HDF5 Controller.
	 */
	virtual boost::shared_ptr<XdmfHDF5Controller> createHDF5Controller(const std::string & hdf5FilePath, const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type);

	/**
	 * Write the XdmfArray to a hdf5 file.
	 *
	 * @param array an XdmfArray to write to hdf5.
	 * @param fapl the file access property list for the hdf5 file on disk.
	 */
	void write(XdmfArray & array, const int fapl);

private:

	XdmfHDF5Writer(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.
	void operator=(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.
};

#endif /* XDMFHDF5WRITER_HPP_ */
