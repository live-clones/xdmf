/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHeavyDataWriter.hpp                                             */
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

#ifndef XDMFHEAVYDATAWRITER_HPP_
#define XDMFHEAVYDATAWRITER_HPP_

// Forward Declarations
class XdmfArray;

// Includes
#include <string>
#include "XdmfCore.hpp"
#include "XdmfVisitor.hpp"

/**
 * @brief Traverses the Xdmf graph and writes heavy data stored in
 * XdmfArrays to heavy data files on disk.
 *
 * This is an abstract base class to support the writing of different
 * heavy data formats.
 *
 * XdmfHeavyDataWriter traverses an Xdmf graph structure and writes
 * data stored in XdmfArrays to heavy data files on disk. Writing
 * begins by calling the accept() operation on any XdmfItem and
 * supplying this writer as the parameter. The writer will write all
 * XdmfArrays under the XdmfItem to a heavy data file on disk. It will
 * also attach an XdmfHeavyDataController to all XdmfArrays that it
 * writes to disk.
 *
 * There are three modes of operation for this writer:
 *   Default - All initialized XdmfArrays are written to new heavy datasets
 *             regardless of whether they are attached to another heavy
 *             dataset on disk via an XdmfHeavyDataController.
 *   Overwrite - If an initialized XdmfArray is attached to an heavy dataset
 *               via an XdmfHeavyDataController the writer will write values
 *               to that location, overwriting all previous written values.
 *   Append - If an initialized XdmfArray is attached to an heavy dataset via
 *            an XdmfHeavyDataController the writer will append the values to
 *            the end of the dataset on disk.
 */
class XDMFCORE_EXPORT XdmfHeavyDataWriter : public XdmfVisitor,
                                            public Loki::Visitor<XdmfArray> {

public:

  enum Mode {
    Default,
    Overwrite,
    Append
  };

  virtual ~XdmfHeavyDataWriter() = 0;

  /**
   * Close file. This is only needed when the file is opened manually
   * through openFile().
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.
   * exampleWriter->closeFile();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.
   * '''
   * exampleWriter.closeFile()
   * @endcode
   */
  virtual void closeFile() = 0;

  /**
   * Get the path to the heavy data file on disk this writer is writing to.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that examplewriter is a shared poinnter to a XdmfHDF5Writer.
   * std::string examplePath = exampleWriter->getFilePath();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that examplewriter is a shared poinnter to a XdmfHDF5Writer.
   * '''
   * examplePath = exampleWriter.getFilePath()
   * @endcode
   *
   * @return a std::string containing the path to the heavy file on disk this
   * writer is writing to.
   */
  std::string getFilePath() const;

  /**
   * Get the Mode of operation for this writer.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * XdmfHeavyDataWriter::Mode exampleMode = XdmfHeavyDataWriter::Default;
   * //Assuming that exampleWriter is a shared pointer to a XdmfHDF5Writer
   * if (exampleWriter->getMode() == exampleMode)
   * {
   *   //Do whatever is to be done if the mode is default
   * }
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * exampleMode = XdmfHeavyDataWriter.Default
   * '''
   * Assuming that exampleWriter is a shared pointer to a XdmfHDF5Writer
   * '''
   * if exampleWriter.getMode() == exampleMode:
   *   '''
   *   Do whatever is to be done if the mode is default
   *   '''
   * @endcode
   *
   * @return the Mode of operation for this writer.
   */
  Mode getMode() const;

  /**
   * Get whether to release data from memory after writing to disk.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.
   * bool testRelease = exampleWriter->getReleaseData();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.
   * '''
   * testRelease = exampleWriter.getReleaseData()
   * @endcode
   *
   * @return true if data is freed after writing
   */
  bool getReleaseData() const;

  /**
   * Open file for writing. This is an optional command that can
   * improve performance for some writers when writing many datasets
   * to a single file. User must call closeFile() after completing
   * output.
   *
   * By default, heavy data files are open and closed before and after
   * writing each dataset to ensure that other writers have access to
   * the file (we never know whether we will be writing to the file
   * again). This is expensive in some cases, but is always
   * safe. Opening the file once and writing many datasets may result
   * in improved performance, but the user must tell the writer when
   * to open and close the file.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.
   * exampleWriter->openFile();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.
   * '''
   * exampleWriter.openFile()
   * @endcode
   */
  virtual void openFile() = 0;

  /**
   * Set the mode of operation for this writer.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assuming that exampleWriter is a shared pointer to a XdmfHDF5Writer
   * exampleWriter->setMode(XdmfHeavyDataWriter::Default);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assuming that exampleWriter is a shared pointer to a XdmfHDF5Writer
   * '''
   * exampleWriter.setMode(XdmfHeavyDataWriter.Default)
   * @endcode
   *
   * @param mode the Mode of operation for this writer.
   */
  void setMode(const Mode mode);

  /**
   * Set whether to release data from memory after writing to disk.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.
   * exampleWriter->setReleaseData(true);
   * //Sets the writer to release data after writing
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.
   * '''
   * exampleWriter.setReleaseData(True)
   * '''
   * Sets the writer to release data after writing
   * '''
   * @endcode
   *
   * @param releaseData true if data should be freed after writing
   */
  void setReleaseData(const bool releaseData = true);

  /**
   * Write an XdmfArray to heavy data file on disk.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared pointer to an XdmfHDF5Writer.
   * shared_ptr<XdmfArray> exampleArray = XdmfArray::New();
   * exampleArray->pushBack(1);
   * exampleArray->pushBack(2);
   * exampleArray->pushBack(3);
   * exampleArray->pushBack(4);
   * exampleArray->pushBack(5);
   * exampleWriter->visit(exampleArray, exampleWriter);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared pointer to an XdmfHDF5Writer.
   * '''
   * exampleArray = XdmfArray.New()
   * exampleArray.pushBackAsInt32(1)
   * exampleArray.pushBackAsInt32(2)
   * exampleArray.pushBackAsInt32(3)
   * exampleArray.pushBackAsInt32(4)
   * exampleArray.pushBackAsInt32(5)
   * exampleWriter.visit(exampleArray, exampleWriter)
   * @endcode
   *
   * @param array an XdmfArray to write to heavy data.
   * @param visitor a smart pointer to this visitor --- aids in grid traversal.
   */
  virtual void visit(XdmfArray & array,
                     const shared_ptr<XdmfBaseVisitor> visitor) = 0;

protected:

  XdmfHeavyDataWriter();
  XdmfHeavyDataWriter(const std::string & filePath);

  int mDataSetId;
  std::string mFilePath;
  Mode mMode;
  bool mReleaseData;

private:

  XdmfHeavyDataWriter(const XdmfHeavyDataWriter &);  // Not implemented.
  void operator=(const XdmfHeavyDataWriter &);  // Not implemented.

};

#endif /* XDMFHEAVYDATAWRITER_HPP_ */
