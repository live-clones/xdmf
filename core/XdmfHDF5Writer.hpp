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
class XdmfArray;
class XdmfArrayType;
class XdmfHDF5Controller;

// Includes
#include "XdmfCore.hpp"
#include "XdmfHeavyDataWriter.hpp"

/**
 * @brief Traverse the Xdmf graph and write heavy data stored in
 * XdmfArrays to HDF5 on disk.
 *
 * XdmfHDF5Writer traverses an Xdmf graph structure and writes data
 * stored in XdmfArrays to HDF5. Writing begins by calling the
 * accept() operation on any XdmfItem and supplying this writer as the
 * parameter. The writer will write all XdmfArrays under the XdmfItem
 * to an hdf5 file on disk. It will also attach an XdmfHDF5Controller
 * to all XdmfArrays that it writes to disk.
 *
 * This writer supports all heavy data writing modes listed in
 * XdmfHeavyDataWriter.
 */
class XDMFCORE_EXPORT XdmfHDF5Writer : public XdmfHeavyDataWriter {

public:

  /**
   * Construct XdmfHDF5Writer.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfHDF5Writer.cpp
   * @skipline newPath
   * @until XdmfHDF5Writer
   *
   * Python
   *
   * @dontinclude XdmfExampleHDF5Writer.py
   * @skipline newPath
   * @until XdmfHDF5Writer
   *
   * @param filePath the location of the hdf5 file to output to on disk.
   * @param clobberFile whether to overwrite the previous file if it exists.
   *
   * @return new XdmfHDF5Writer.
   */
  static shared_ptr<XdmfHDF5Writer> New(const std::string & filePath,
                                        const bool clobberFile = false);

  virtual ~XdmfHDF5Writer();


  /**
   * Sets the file size limit of the HDF5 files produced by the writer in MB. Overflow is pushed to a new HDF5 file.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfHDF5Writer.cpp
   * @skipline newPath
   * @until setFileSizeLimit
   *
   * Python
   *
   * @dontinclude XdmfExampleHDF5Writer.py
   * @skipline newPath
   * @until setFileSizeLimit
   *
   * @param	newSize	The size limit in MB
   */
  void setFileSizeLimit(int newSize);

  /**
   * Gets the file size limit of the HDF5 files produced by the writer in MB. Overflow is pushed to a new HDF5 file.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfHDF5Writer.cpp
   * @skipline newPath
   * @until New
   * @skipline exampleLimit
   *
   * Python
   *
   * @dontinclude XdmfExampleHDF5Writer.py
   * @skipline newPath
   * @until New
   * @skipline exampleLimit
   *
   * @return	The size limit in MB
   */
  int getFileSizeLimit();

  virtual void closeFile();

  virtual void openFile();

  using XdmfHeavyDataWriter::visit;
  virtual void visit(XdmfArray & array,
                     const shared_ptr<XdmfBaseVisitor> visitor);

  virtual void visit(XdmfItem & item,
                     const shared_ptr<XdmfBaseVisitor> visitor);

protected:

  XdmfHDF5Writer(const std::string & filePath);

  /**
   * Create a new HDF5 Controller that is able to read in after being
   * written by this writer.
   *
   * @param hdf5FilePath the location of the hdf5 file the data set resides in.
   * @param dataSetPath the location of the dataset within the hdf5 file.
   * @param type the data type of the dataset to read.
   * @param start the offset of the starting element in each dimension in
   * the hdf5 data set.
   * @param stride the number of elements to move in each dimension from the
   * hdf5 data set.
   * @param dimensions the number of elements to select in each
   * dimension from the hdf5 data set. (size in each dimension)
   * @param dataspaceDimensions the number of elements in the entire
   * hdf5 data set (may be larger that dimensions if using
   * hyperslabs).
   *
   * @return new HDF5 Controller.
   */
  virtual shared_ptr<XdmfHDF5Controller>
  createHDF5Controller(const std::string & hdf5FilePath,
                       const std::string & dataSetPath,
                       const shared_ptr<const XdmfArrayType> type,
                       const std::vector<unsigned int> & start,
                       const std::vector<unsigned int> & stride,
                       const std::vector<unsigned int> & dimensions,
                       const std::vector<unsigned int> & dataspaceDimensions);

  /**
   * Open hdf5 file with a fapl.
   *
   * @param fapl the file access property list for the hdf5 file.
   */
  void openFile(const int fapl);

  /**
   * Write the XdmfArray to a hdf5 file.
   *
   * @param array an XdmfArray to write to hdf5.
   * @param fapl the file access property list for the hdf5 file on disk.
   */
  void write(XdmfArray & array, const int fapl);

private:

  /**
   * PIMPL
   */
  class XdmfHDF5WriterImpl;

  XdmfHDF5Writer(const XdmfHDF5Writer &);  // Not implemented.
  void operator=(const XdmfHDF5Writer &);  // Not implemented.

  XdmfHDF5WriterImpl * mImpl;

};

#endif /* XDMFHDF5WRITER_HPP_ */
