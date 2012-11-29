/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5ControllerDSM.cpp                                           */
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

#ifndef XDMFHDF5CONTROLLERDSM_HPP_
#define XDMFHDF5CONTROLLERDSM_HPP_

// Forward Declarations
class H5FDdsmBuffer;

// Includes
#include "XdmfCore.hpp"
#include "XdmfHDF5Controller.hpp"

/**
 * @brief Couples an XdmfArray with HDF5 data stored in a DSM buffer.
 *
 * Serves as an interface between data stored in XdmfArrays and data
 * stored in DSM buffers.  When an Xdmf file is read from or written
 * to a DSM buffer an XdmfHDF5ControllerDSM is attached to XdmfArrays.
 * This allows data to be released from memory but still be accessible
 * or have its location written to light data.
 */
class XDMFCORE_EXPORT XdmfHDF5ControllerDSM : public XdmfHDF5Controller {

public:

  virtual ~XdmfHDF5ControllerDSM();

  /**
   * Create a new controller for an DSM data set.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * std::string newPath = "Your file path goes here";
   * std::string newSetPath = "The data set path goes here";
   * shared_ptr<const XdmfArrayType> newType = XdmfArrayType::Int32();
   * std::vector<unsigned int> readStarts;
   * //Three dimensions, all starting at 0
   * readStarts.push_back(0);
   * readStarts.push_back(0);
   * readStarts.push_back(0);
   * std::vector<unsigned int> readStrides;
   * //Three dimensions, all of them skip no values
   * readStrides.push_back(1);
   * readStrides.push_back(1);
   * readStrides.push_back(1);
   * std::vector<unsigned int> readCounts;
   * //Three dimensions, read 10 values from all of them
   * readCounts.push_back(10);
   * readCounts.push_back(10);
   * readCounts.push_back(10);
   * //assume newDsmBuffer is a pointer to the dsm buffer where the data is stored
   * shared_ptr<XdmfHDF5ControllerDSM> exampleController = XdmfHDF5ControllerDSM::New(
   *   newPath,
   *   newSetPath,
   *   newType,
   *   readStarts,
   *   readStrides,
   *   readCounts,
   *   newDsmBuffer);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * newPath = "Your file path goes here"
   * newSetPath = "The data set path goes here"
   * newType = XdmfArrayType.Int32()
   * readStarts = UInt32Vector()
   * '''
   * Three dimensions, all starting at 0
   * '''
   * readStarts.push_back(0)
   * readStarts.push_back(0)
   * readStarts.push_back(0)
   * readStrides = UInt32Vector()
   * '''
   * Three dimensions, all of them skip no values
   * '''
   * readStrides.push_back(1)
   * readStrides.push_back(1)
   * readStrides.push_back(1)
   * readCounts = UInt32Vector()
   * '''
   * Three dimensions, read 10 values from all of them
   * '''
   * readCounts.push_back(10)
   * readCounts.push_back(10)
   * readCounts.push_back(10)
   * '''
   * assume newDsmBuffer is a pointer to the dsm buffer where the data is stored
   * '''
   * exampleController = XdmfHDF5ControllerDSM.New(
   *   newPath,
   *   newSetPath,
   *   newType,
   *   readStarts,
   *   readStrides,
   *   readCounts,
   *   newDsmBuffer)
   * @endcode
   *
   * @param	hdf5FilePath	The path to the hdf5 file that the controller will be accessing
   * @param	dataSetPath	The location within the file of the data the controller with be accessing
   * @param	type		The data type of the data Ex: XdmfArrayType::Int32()
   * @param	start		A vector of the start indexes for all dimensions of the data
   * @param	stride		A vector of the distance between reads for all dimensions of the data
   * @param	count		A vector of the number of values read from all dimensions of the data
   * @param	dsmBuffer	A pointer to the dsm buffer
   */
  static shared_ptr<XdmfHDF5ControllerDSM>
  New(const std::string & hdf5FilePath,
      const std::string & dataSetPath,
      const shared_ptr<const XdmfArrayType> type,
      const std::vector<unsigned int> & start,
      const std::vector<unsigned int> & stride,
      const std::vector<unsigned int> & dimensions,
      const std::vector<unsigned int> & dataspaceDimensions,
      H5FDdsmBuffer * const dsmBuffer);

  std::string getName() const;

  void read(XdmfArray * const array);

protected:

  XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                        const std::string & dataSetPath,
                        const shared_ptr<const XdmfArrayType> type,
                        const std::vector<unsigned int> & start,
                        const std::vector<unsigned int> & stride,
                        const std::vector<unsigned int> & dimensions,
                        const std::vector<unsigned int> & dataspaceDimensions,
                        H5FDdsmBuffer * const dsmBuffer);

private:

  XdmfHDF5ControllerDSM(const XdmfHDF5Controller &);  // Not implemented.
  void operator=(const XdmfHDF5Controller &);  // Not implemented.

  H5FDdsmBuffer * mDSMBuffer;
};

#endif /* XDMFHDF5CONTROLLER_HPP_ */
