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
class XdmfInformation;
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
 * An infinite loop is possible if an XdmfItem somehow ends up as its own child,
 * either directly or by way of another Xdmf Item.
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline outFile
   * @skipline New
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline outFile
   * @skipline New
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline outFile
   * @skipline heavyFile
   * @until exampleWriter
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline outFile
   * @skipline heavyFile
   * @until exampleWriter
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline heavyFile
   * @until exampleHeavyWriter
   * @skipline filebuf
   * @until exampleBufferWriter
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline getFilePath
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline Assume
   * @skipline getFilePath
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline getHeavyDataWriter
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline Assume
   * @skipline getHeavyDataWriter
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline exampleHeavyWriterConst
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline getLightDataLimit
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline Assume
   * @skipline getLightDataLimit
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline testMode
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline Assume
   * @skipline testMode
   * @until default
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline getWriteXPaths
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline Assume
   * @skipline getWriteXPaths
   *
   * @return bool whether this writer is set to write xpaths.
   */
  bool getWriteXPaths() const;

  /**
   * Get whether this writer is set to parse xpaths from information.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline getXPathParse
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline Assume
   * @skipline getXPathParse
   *
   * @return bool whether this writer is set to write xpaths.
   */
  bool getXPathParse() const;

  /**
   * Set the number of values that this writer writes to light data
   * (XML) before switching to a heavy data format.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline newLimit
   * @until //
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline newLimit
   * @until values
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline setMode
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline Assume
   * @skipline setMode
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline setWriteXPaths
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline Assume
   * @skipline setWriteXPaths
   *
   * @param writeXPaths whether to write xpaths for this writer.
   */
  void setWriteXPaths(const bool writeXPaths = true);

  /**
   * Set whether to parse xpaths from infomation for this writer.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline Assume
   * @skipline setXPathParse
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline Assume
   * @skipline setXPathParse
   *
   * @param xPathParse whether to write xpaths for this writer.
   */
  void setXPathParse(const bool xPathParse = true);

  /**
   * Write an XdmfArray to disk
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline XdmfAttribute
   * @skipline visit
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline XdmfAttribute
   * @skipline visit
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
   * @dontinclude ExampleXdmfWriter.cpp
   * @skipline XdmfDomain
   * @skipline visit
   *
   * Python
   *
   * @dontinclude XdmfExampleWriter.py
   * @skipline XdmfDomain
   * @skipline visit
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
