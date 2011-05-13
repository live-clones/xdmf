/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5WriterDSM.hpp                                               */
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

#ifndef XDMFHDF5WRITERDSM_HPP_
#define XDMFHDF5WRITERDSM_HPP_

// Forward Declarations
class H5FDdsmBuffer;

// Includes
#include "XdmfHDF5Writer.hpp"

/**
 * @brief Traverse the Xdmf graph and write heavy data stored in
 * XdmfArrays to a DSM buffer.
 *
 * XdmfHDF5WriterDSM traverses an Xdmf graph structure and writes data
 * stored in XdmfArrays to a DSM buffer. Writing begins by calling the
 * accept() operation on any XdmfItem and supplying this writer as the
 * parameter. The writer will write all XdmfArrays under the XdmfItem
 * to a DSM Buffer. It will also attach an XdmfHDF5ControllerDSM to
 * all XdmfArrays.
 *
 * This writer supports all heavy data writing modes listed in
 * XdmfHeavyDataWriter.
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
  static shared_ptr<XdmfHDF5WriterDSM>
  New(const std::string & filePath,
      H5FDdsmBuffer * const dsmBuffer);

  virtual ~XdmfHDF5WriterDSM();

  void visit(XdmfArray & array,
             const shared_ptr<XdmfBaseVisitor> visitor);

protected:

  XdmfHDF5WriterDSM(const std::string & filePath,
                    H5FDdsmBuffer * const dsmBuffer);

  virtual shared_ptr<XdmfHDF5Controller>
  createHDF5Controller(const std::string & hdf5FilePath,
                       const std::string & dataSetPath,
                       const shared_ptr<const XdmfArrayType> type,
                       const std::vector<unsigned int> & start,
                       const std::vector<unsigned int> & stride,
                       const std::vector<unsigned int> & count);

private:

  XdmfHDF5WriterDSM(const XdmfHDF5WriterDSM &);  // Not implemented.
  void operator=(const XdmfHDF5WriterDSM &);  // Not implemented.

  H5FDdsmBuffer * mDSMBuffer;

};

#endif /* XDMFHDF5WRITERDSM_HPP_ */
