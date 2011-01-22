#ifndef XDMFHDF5CONTROLLER_HPP_
#define XDMFHDF5CONTROLLER_HPP_

// Includes
#include "XdmfCore.hpp"
#include "XdmfHeavyDataController.hpp"

/**
 * @brief Couples an XdmfArray with HDF5 data stored on disk.
 *
 * Serves as an interface between data stored in XdmfArrays and data stored in
 * hdf5 files on disk. When an Xdmf file is read from or written to disk an
 * XdmfHDF5Controller is attached to XdmfArrays. This allows data to be
 * released from memory but still be accessible or have its location written
 * to light data.
 */
class XDMFCORE_EXPORT XdmfHDF5Controller : public XdmfHeavyDataController {

 public:

  virtual ~XdmfHDF5Controller();

  /**
   * Create a new controller for an hdf5 data set on disk.
   *
   * @param hdf5FilePath the location of the hdf5 file the data set resides in.
   * @param dataSetPath the location of the dataset within the hdf5 file.
   * @param size the size of the dataset to read.
   * @param type the data type of the dataset to read.
   *
   * @return new HDF5 Controller.
   */
  static boost::shared_ptr<XdmfHDF5Controller>
  New(const std::string & hdf5FilePath,
      const std::string & dataSetPath,
      const unsigned int size,
      const boost::shared_ptr<const XdmfArrayType> type);

  virtual std::string getName() const;

  virtual void read(XdmfArray * const array);

 protected:

  XdmfHDF5Controller(const std::string & hdf5FilePath,
                     const std::string & dataSetPath,
                     const unsigned int size,
                     const boost::shared_ptr<const XdmfArrayType> type);

  void read(XdmfArray * const array, const int fapl);

 private:

  XdmfHDF5Controller(const XdmfHDF5Controller &);  // Not implemented.
  void operator=(const XdmfHDF5Controller &);  // Not implemented.

};

#endif /* XDMFHDF5CONTROLLER_HPP_ */
