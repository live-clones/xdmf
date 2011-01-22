#ifndef XDMFHEAVYDATACONTROLLER_HPP_
#define XDMFHEAVYDATACONTROLLER_HPP_

// Forward Declarations
class XdmfArray;
class XdmfArrayType;

// Includes
#include <boost/shared_ptr.hpp>
#include <string>
#include "XdmfCore.hpp"

/**
 * @brief Couples an XdmfArray with heavy data stored on disk.
 * This is an abstract base class to support the reading of different heavy
 * data formats.
 *
 * Serves as an interface between data stored in XdmfArrays and data stored on
 * disk. When an Xdmf file is read from or written to disk an
 * XdmfHeavyController is attached to XdmfArrays. This allows data to be
 * released from memory but still be accessible or have its location written
 * to light data.
 */
class XDMFCORE_EXPORT XdmfHeavyDataController {

 public:

  virtual ~XdmfHeavyDataController() = 0;

  /**
   * Get the path of the data set within the heavy data file owned by this
   * controller.
   * For "/home/output.h5:/foo/data" this is "/foo/data"
   *
   * @return a std::string containing the path of the data set.
   */
  std::string getDataSetPath() const;

  /**
   * Get the absolute path to the heavy data file on disk where the data set
   * owned by this controller resides.
   * For "/home/output.h5:/foo/data" this is "/home/output.h5"
   *
   * @return a std::string containing the path to the heavy data file.
   */
  std::string getFilePath() const;

  /**
   * Get the name of this heavy data format. E.g. "HDF" for hdf5 format.
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
   * Get the array type of the heavy data set owned by this controller.
   *
   * @return an XdmfArrayType containing the array type of the heavy data set.
   */
  boost::shared_ptr<const XdmfArrayType> getType() const;

  /**
   * Read data owned by this controller on disk into the passed XdmfArray.
   *
   * @param array and XdmfArray to read data into.
   */
  virtual void read(XdmfArray * const array) = 0;

 protected:

  XdmfHeavyDataController(const std::string & filePath,
                          const std::string & dataSetPath,
                          const unsigned int size,
                          const boost::shared_ptr<const XdmfArrayType> type);

  std::string mDataSetPath;
  std::string mFilePath;
  unsigned int mSize;
  boost::shared_ptr<const XdmfArrayType> mType;

 private:

  XdmfHeavyDataController(const XdmfHeavyDataController&);  // Not implemented.
  void operator=(const XdmfHeavyDataController &);  // Not implemented.

};

#ifdef _WIN32
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
boost::shared_ptr<const XdmfArrayType>;
#endif

#endif /* XDMFHEAVYDATACONTROLLER_HPP_ */
