/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHeavyDataController.hpp                                         */
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

#ifndef XDMFHEAVYDATACONTROLLER_HPP_
#define XDMFHEAVYDATACONTROLLER_HPP_

// Forward Declarations
class XdmfArray;
class XdmfArrayType;

// Includes
#include <string>
#include <vector>
#include "XdmfCore.hpp"
#include "XdmfSharedPtr.hpp"

/**
 * @brief Couples an XdmfArray with heavy data stored on disk.
 *
 * This is an abstract base class to support the reading of different
 * heavy data formats.
 *
 * Serves as an interface between data stored in XdmfArrays and data
 * stored on disk. When an Xdmf file is read from or written to disk
 * an XdmfHeavyController is attached to XdmfArrays. This allows data
 * to be released from memory but still be accessible or have its
 * location written to light data.
 */
class XDMFCORE_EXPORT XdmfHeavyDataController {

public:

  virtual ~XdmfHeavyDataController() = 0;

  /**
   * Get the path of the data set within the heavy data file owned by
   * this controller.
   * For "/home/output.h5:/foo/data" this is "/foo/data"
   *
   * @return a std::string containing the path of the data set.
   */
  std::string getDataSetPath() const;

  /**
   * Get the dimensions of the dataspace owned by this
   * controller. This is the dimension of the entire heavy dataset,
   * which may be larger than the dimensions of the array (if reading
   * a piece of a larger dataset).
   *
   * @return a vector containing the size in each dimension of the dataspace
   * owned by this controller.
   */
  std::vector<unsigned int> getDataspaceDimensions() const;

  /**
   * Get the dimensions of the heavy data set owned by this controller.
   *
   * @return a vector containing the size in each dimension of the heavy data
   * set owned by this controller.
   */
  std::vector<unsigned int> getDimensions() const;

  /**
   * Get the absolute path to the heavy data file on disk where the
   * data set owned by this controller resides.
   * For "/home/output.h5:/foo/data" this is "/home/output.h5"
   *
   * @return a std::string containing the path to the heavy data file.
   */
  std::string getFilePath() const;

  /**
   * Get the name of this heavy data format. E.g. "HDF" for hdf5
   * format.
   *
   * @return std::string containing the name of this heavy data format
   */
  virtual std::string getName() const = 0;

  /**
   * Get the size of the heavy data set owned by this controller.
   *
   * @return a int containing the size of the heavy data set.
   */
  unsigned int getSize() const;

  /**
   * Get the start index of the heavy data set owned by this controller.
   *
   * @return a vector containing the start index in each dimension of
   * the heavy data set owned by this controller.
   */
  std::vector<unsigned int> getStart() const;

  /**
   * Get the stride of the heavy data set owned by this controller.
   *
   * @return a vector containing the stride in each dimension of the
   * heavy data set owned by this controller.
   */
  std::vector<unsigned int> getStride() const;

  /**
   * Get the array type of the heavy data set owned by this
   * controller.
   *
   * @return an XdmfArrayType containing the array type of the heavy data set.
   */
  shared_ptr<const XdmfArrayType> getType() const;

  /**
   * Read data owned by this controller on disk into the passed
   * XdmfArray.
   *
   * @param array and XdmfArray to read data into.
   */
  virtual void read(XdmfArray * const array) = 0;

protected:

  XdmfHeavyDataController(const std::string & filePath,
                          const std::string & dataSetPath,
                          const shared_ptr<const XdmfArrayType> type,
                          const std::vector<unsigned int> & start,
                          const std::vector<unsigned int> & stride,
                          const std::vector<unsigned int> & dimensions,
                          const std::vector<unsigned int> & dataspaceDimensions);

  const std::string mDataSetPath;
  const std::vector<unsigned int> mDataspaceDimensions;
  const std::vector<unsigned int> mDimensions;
  const std::string mFilePath;
  const std::vector<unsigned int> mStart;
  const std::vector<unsigned int> mStride;
  const shared_ptr<const XdmfArrayType> mType;

private:

  XdmfHeavyDataController(const XdmfHeavyDataController&);  // Not implemented.
  void operator=(const XdmfHeavyDataController &);  // Not implemented.

};

#ifdef _WIN32
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
shared_ptr<const XdmfArrayType>;
#endif

#endif /* XDMFHEAVYDATACONTROLLER_HPP_ */
