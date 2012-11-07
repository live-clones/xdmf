/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfWriter.hpp                                                      */
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

#ifndef XDMFWRITER_HPP_
#define XDMFWRITER_HPP_

// Forward Declarations
class XdmfArray;
class XdmfHeavyDataWriter;

// Includes
#include "XdmfCore.hpp"
#include "XdmfVisitor.hpp"

/**
 * @brief Traverse the Xdmf graph and write light and heavy data
 * stored to disk.
 *
 * XdmfWriter visits each node of an Xdmf graph structure and writes
 * data to disk. Writing begins by calling the accept() operation on
 * any XdmfItem and supplying this writer as the parameter. The
 * XdmfItem as well as all children attached to the XdmfItem are
 * written to disk. Heavy data is written to a heavy data format using
 * an XdmfHeavyDataWriter and light data is written to XML.
 *
 * By default, the XdmfWriter writes all heavy data to a single heavy
 * data file specified by the XdmfHeavyDataWriter. If a dataset is
 * encountered that resides in a different heavy data file on disk,
 * the dataset is read from disk and written to the new heavy data
 * file. If this is undesired, the XdmfWriter can be set to
 * DistributedHeavyData mode in which the writer will automatically
 * reference any heavy dataset even if it resides in a different file
 * than the one currently being written to.
 */
class XDMFCORE_EXPORT XdmfWriter : public XdmfVisitor,
                                   public Loki::Visitor<XdmfArray> {

public:

  enum Mode {
    Default,
    DistributedHeavyData
  };

  /**
   * Create a new XdmfWriter to write Xdmf data to disk. This will
   * create its own hdf5 writer based on the xmlFileName. For example,
   * if supplied "output.xmf" the created hdf5 writer would write to
   * file "output.h5".
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * std::string outFile = "output file name goes here";
   * shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New(outFile);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * outFile = "output file name goes here"
   * exampleWriter = XdmfWriter.New(outFile)
   * @endcode
   *
   * @param xmlFilePath the path to the xml file to write to.
   *
   * @return the new XdmfWriter.
   */
  static shared_ptr<XdmfWriter> New(const std::string & xmlFilePath);

  /**
   * Create a new XdmfWriter to write Xdmf data to disk. This will
   * utilize the passed heavy data writer to write any heavy data to
   * disk.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * std::string outFile = "output file name goes here";
   * std::string heavyFile = "heavy file name goes here";
   * bool replaceFile = true;
   * shared_ptr<XdmfHDF5Writer> exampleHeavyWriter = XdmfHDF5Writer::New(heavyFile, replaceFile);
   * shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New(outFile, exampleHeavyWriter);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * outFile = "output file name goes here"
   * heavyFile = "heavy file name goes here"
   * replaceFile = True
   * exampleHeavyWriter = XdmfHDF5Writer.New(heavyFile, replaceFile)
   * exampleWriter = XdmfWriter.New(outFile, exampleHeavyWriter)
   * @endcode
   *
   * @param xmlFilePath the path to the xml file to write to.
   * @param heavyDataWriter the heavy data writer to use when writing.
   *
   * @return the new XdmfWriter.
   */
  static shared_ptr<XdmfWriter> New(const std::string & xmlFilePath, 
                                    const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter);

  /**
   * Create a new XdmfWriter to write Xdmf data to disk. This will
   * write heavy data to disk using the passed heavy data writer and
   * will add xml output to the stream.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * filebuf exampleBuffer;
   * exampleBuffer.open("file goes here", ios::out);
   * ostream exampleStream(&exampleBuffer);
   * std::string heavyFile = "heavy file name goes here";
   * bool replaceFile = true;
   * shared_ptr<XdmfHDF5Writer> exampleHeavyWriter = XdmfHDF5Writer::New(heavyFile, replaceFile);
   * shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New(exampleStream, exampleHeavyWriter);
   * @endcode
   *
   * Python: does not curretnly support this version of New
   *
   * @param stream the output stream to write light data to.
   * @param heavyDataWriter the heavy data writer to use when writing.
   *
   * @return the new XdmfWriter;
   */
  static shared_ptr<XdmfWriter> New(std::ostream & stream,
                                    const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter);

  virtual ~XdmfWriter();

  /**
   * Get the absolute path to the XML file on disk this writer is
   * writing to.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * std::string examplePath = exampleWriter->getFilePath();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * '''
   * examplePath = exampleWriter.getFilePath()
   * @endcode
   *
   * @return a std::string containing the path to the XML file on disk this
   * writer is writing to.
   */
  std::string getFilePath() const;

  /**
   * Get the heavy data writer that this XdmfWriter uses to write
   * heavy data to disk.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * shared_ptr<XdmfHeavyDataWriter> exampleHeavyWriter = exampleWriter->getHeavyDataWriter();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * '''
   * exampleHeavyWriter = exampleWriter.getHeavyDataWriter()
   * @endcode
   *
   * @return the requested heavy data writer.
   */
  shared_ptr<XdmfHeavyDataWriter> getHeavyDataWriter();

  /**
   * Get the heavy data writer that this XdmfWriter uses to write
   * heavy data to disk (const version).
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * shared_ptr<const XdmfHeavyDataWriter> exampleHeavyWriter = exampleWriter->getHeavyDataWriter();
   * @endcode
   *
   * Python: Does not support a contant version of this function
   *
   * @return the requested heavy data writer.
   */
  shared_ptr<const XdmfHeavyDataWriter> getHeavyDataWriter() const;

  /**
   * Get the number of values that this writer writes to light data
   * (XML) before switching to a heavy data format.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * unsigned int exampleLimit = exampleWriter->getLightDataLimit();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * '''
   * exampleLimit = exampleWriter.getLightDataLimit()
   * @endcode
   *
   * @return an unsigned int containing the number of values.
   */
  unsigned int getLightDataLimit() const;

  /**
   * Get the Mode of operation for this writer.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * XdmfWriter::Mode testMode = XdmfWriter::Default;
   * //Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * if (exampleWriter->getMode() == testMode)
   * {
   *   //Do whatever is to be done if the mode is default
   * }
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * testMode = XdmfWriter.Default
   * '''
   * Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * '''
   * if exampleWriter.getMode() == testMode:
   *   '''
   *   Do whatever is to be done if the mode is default
   *   '''
   * @endcode
   *
   * @return the Mode of operation for this writer.
   */
  Mode getMode() const;

  /**
   * Get whether this writer is set to write xpaths.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * bool exampleTestPaths = exampleWriter->getWriteXPaths();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * '''
   * exampleTestPaths = exampleWriter.getWriteXPaths()
   * @endcode
   *
   * @return bool whether this writer is set to write xpaths.
   */
  bool getWriteXPaths() const;

  /**
   * Set the number of values that this writer writes to light data
   * (XML) before switching to a heavy data format.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * unsigned int newLimit = 20;
   * //Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * exampleWriter->setLightDataLimit(newLimit);
   * //The writer will now place any data with a number of values over 20 into heavy data
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * newLimit = 20;
   * '''
   * Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * '''
   * exampleWriter.setLightDataLimit(newLimit)
   * '''
   * The writer will now place any data with a number of values over 20 into heavy data
   * '''
   * @endcode
   *
   * @param numValues an unsigned int containing the number of values.
   */
  void setLightDataLimit(const unsigned int numValues);

  /**
   * Set the mode of operation for this writer.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * exampleWriter->setMode(XdmfWriter::Default);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * '''
   * exampleWriter.setMode(XdmfWriter.Default)
   * @endcode
   *
   * @param mode the Mode of operation for this writer.
   */
  void setMode(const Mode mode);

  /**
   * Set whether to write xpaths for this writer.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * exampleWriter->setWriteXPaths(true);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assume that exampleWriter is a shared pointer to a XdmfWriter object
   * '''
   * exampleWriter.setWriteXPaths(True)
   * @endcode
   *
   * @param writeXPaths whether to write xpaths for this writer.
   */
  void setWriteXPaths(const bool writeXPaths = true);

  /**
   * Write an XdmfArray to disk
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Using XdmfAttribute here, but any XdmfArray would work
   * shared_ptr<XdmfAttribute> exampleAttribute = XdmfAttribute::New();
   * exampleAttribute->setCenter(XdmfAttributeCenter::Node());
   * exampleAttribute->setType(XdmfAttributeType::Scalar());
   * exampleAttribute->pushBack(1);
   * exampleAttribute->pushBack(2);
   * exampleAttribute->pushBack(3);
   * exampleAttribute->pushBack(4);
   * exampleAttribute->pushBack(5);
   * exampleAttribute->pushBack(6);
   * std::string outFile = "output file name goes here";
   * shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New(outFile);
   * exampleWriter->visit(exampleAttribute, exampleWriter);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Using XdmfAttribute here, but any XdmfArray would work
   * '''
   * exampleAttribute = XdmfAttribute.New()
   * exampleAttribute.setCenter(XdmfAttributeCenter.Node())
   * exampleAttribute.setType(XdmfAttributeType.Scalar())
   * exampleAttribute.pushBackAsInt32(1)
   * exampleAttribute.pushBackAsInt32(2)
   * exampleAttribute.pushBackAsInt32(3)
   * exampleAttribute.pushBackAsInt32(4)
   * exampleAttribute.pushBackAsInt32(5)
   * exampleAttribute.pushBackAsInt32(6)
   * outFile = "output file name goes here"
   * exampleWriter = XdmfWriter.New(outFile)
   * exampleWriter.visit(exampleAttribute, exampleWriter)
   * @endcode
   *
   * @param array an XdmfArray to write to disk.
   * @param visitor a smart pointer to this visitor --- aids in grid traversal.
   */
  virtual void visit(XdmfArray & array,
                     const shared_ptr<XdmfBaseVisitor> visitor);

  /**
   * Write an XdmfItem to disk
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Using XdmfDomain here, but any XdmfItem would work
   * shared_ptr<XdmfDomain> exampleDomain = XdmfDomain::New();
   * std::string outFile = "output file name goes here";
   * shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New(outFile);
   * exampleWriter->visit(exampleDomain, exampleWriter);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Using XdmfDomain here, but any XdmfItem would work
   * '''
   * exampleDomain = XdmfDomain.New()
   * outFile = "output file name goes here"
   * exampleWriter = XdmfWriter.New(outFile)
   * exampleWriter.visit(exampleDomain, exampleWriter)
   * @endcode
   *
   * @param item an XdmfItem to write to disk.
   * @param visitor a smart pointer to this visitor --- aids in grid traversal.
   */
  virtual void visit(XdmfItem & item,
                     const shared_ptr<XdmfBaseVisitor> visitor);


protected:

  XdmfWriter(const std::string & xmlFilePath,
             shared_ptr<XdmfHeavyDataWriter> heavyDataWriter,
             std::ostream * stream = NULL);

  void setDocumentTitle(std::string title);
  void setVersionString(std::string version);

private:

  /**
   * PIMPL
   */
  class XdmfWriterImpl;

  XdmfWriter(const XdmfWriter &);  // Not implemented.
  void operator=(const XdmfWriter &);  // Not implemented.

  XdmfWriterImpl * mImpl;

};

#ifdef _WIN32
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
shared_ptr<XdmfHeavyDataWriter>;
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
shared_ptr<const XdmfHeavyDataWriter>;
#endif

#endif /* XDMFWRITER_HPP_ */
