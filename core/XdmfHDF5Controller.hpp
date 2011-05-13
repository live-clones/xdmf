/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5Controller.hpp                                              */
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

#ifndef XDMFHDF5CONTROLLER_HPP_
#define XDMFHDF5CONTROLLER_HPP_

// Includes
#include "XdmfCore.hpp"
#include "XdmfHeavyDataController.hpp"

/**
 * @brief Couples an XdmfArray with HDF5 data stored on disk.
 *
 * Serves as an interface between data stored in XdmfArrays and data
 * stored in hdf5 files on disk. When an Xdmf file is read from or
 * written to disk an XdmfHDF5Controller is attached to
 * XdmfArrays. This allows data to be released from memory but still
 * be accessible or have its location written to light data.
 */
class XDMFCORE_EXPORT XdmfHDF5Controller : public XdmfHeavyDataController {

public:

  virtual ~XdmfHDF5Controller();

  /**
   * Create a new controller for an hdf5 data set on disk.
   *
   * @param hdf5FilePath the location of the hdf5 file the data set resides in.
   * @param dataSetPath the location of the dataset within the hdf5 file.
   * @param type the data type of the dataset to read.
   * @param start the offset of the starting element in each dimension in
   * the hdf5 data set.
   * @param stride the number of elements to move in each dimension from the
   * hdf5 data set.
   * @param count the number of elements to select in each dimension from the
   * hdf5 data set. (size in each dimension)
   *
   * @return new HDF5 Controller.
   */
  static shared_ptr<XdmfHDF5Controller>
  New(const std::string & hdf5FilePath,
      const std::string & dataSetPath,
      const shared_ptr<const XdmfArrayType> type,
      const std::vector<unsigned int> & start,
      const std::vector<unsigned int> & stride,
      const std::vector<unsigned int> & count);

  virtual std::string getName() const;

  virtual void read(XdmfArray * const array);

protected:

  XdmfHDF5Controller(const std::string & hdf5FilePath,
                     const std::string & dataSetPath,
                     const shared_ptr<const XdmfArrayType> type,
                     const std::vector<unsigned int> & start,
                     const std::vector<unsigned int> & stride,
                     const std::vector<unsigned int> & count);

  void read(XdmfArray * const array, const int fapl);

private:

  XdmfHDF5Controller(const XdmfHDF5Controller &);  // Not implemented.
  void operator=(const XdmfHDF5Controller &);  // Not implemented.

  const std::vector<unsigned int> mStart;
  const std::vector<unsigned int> mStride;

};

#endif /* XDMFHDF5CONTROLLER_HPP_ */
