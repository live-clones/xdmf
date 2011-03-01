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
   * @param xmlFilePath the path to the xml file to write to.
   *
   * @return the new XdmfWriter.
   */
  static boost::shared_ptr<XdmfWriter> New(const std::string & xmlFilePath);

  /**
   * Create a new XdmfWriter to write Xdmf data to disk. This will
   * utilize the passed heavy data writer to write any heavy data to
   * disk.
   *
   * @param xmlFilePath the path to the xml file to write to.
   * @param heavyDataWriter the heavy data writer to use when writing.
   *
   * @return the new XdmfWriter.
   */
  static boost::shared_ptr<XdmfWriter> New(const std::string & xmlFilePath, const boost::shared_ptr<XdmfHeavyDataWriter> heavyDataWriter);

  virtual ~XdmfWriter();

  /**
   * Get the absolute path to the XML file on disk this writer is
   * writing to.
   *
   * @return a std::string containing the path to the XML file on disk this
   * writer is writing to.
   */
  std::string getFilePath() const;

  /**
   * Get the heavy data writer that this XdmfWriter uses to write
   * heavy data to disk.
   *
   * @return the requested heavy data writer.
   */
  boost::shared_ptr<XdmfHeavyDataWriter> getHeavyDataWriter();

  /**
   * Get the heavy data writer that this XdmfWriter uses to write
   * heavy data to disk (const version).
   *
   * @return the requested heavy data writer.
   */
  boost::shared_ptr<const XdmfHeavyDataWriter> getHeavyDataWriter() const;

  /**
   * Get the number of values that this writer writes to light data
   * (XML) before switching to a heavy data format.
   *
   * @return an unsigned int containing the number of values.
   */
  unsigned int getLightDataLimit() const;

  /**
   * Get the Mode of operation for this writer.
   *
   * @return the Mode of operation for this writer.
   */
  Mode getMode() const;

  /**
   * Get whether this writer is set to write xpaths.
   *
   * @return bool whether this writer is set to write xpaths.
   */
  bool getWriteXPaths() const;

  /**
   * Set the number of values that this writer writes to light data
   * (XML) before switching to a heavy data format.
   *
   * @param numValues an unsigned int containing the number of values.
   */
  void setLightDataLimit(const unsigned int numValues);

  /**
   * Set the mode of operation for this writer.
   *
   * @param mode the Mode of operation for this writer.
   */
  void setMode(const Mode mode);

  /**
   * Set whether to write xpaths for this writer.
   *
   * @param writeXPaths whether to write xpaths for this writer.
   */
  void setWriteXPaths(const bool writeXPaths = true);

  /**
   * Write an XdmfArray to disk
   *
   * @param array an XdmfArray to write to disk.
   * @param visitor a smart pointer to this visitor --- aids in grid traversal.
   */
  virtual void visit(XdmfArray & array,
                     const boost::shared_ptr<XdmfBaseVisitor> visitor);

  /**
   * Write an XdmfItem to disk
   *
   * @param item an XdmfItem to write to disk.
   * @param visitor a smart pointer to this visitor --- aids in grid traversal.
   */
  virtual void visit(XdmfItem & item,
                     const boost::shared_ptr<XdmfBaseVisitor> visitor);


 protected:

  XdmfWriter(const std::string & xmlFilePath,
             boost::shared_ptr<XdmfHeavyDataWriter> heavyDataWriter);

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
boost::shared_ptr<XdmfHeavyDataWriter>;
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
boost::shared_ptr<const XdmfHeavyDataWriter>;
#endif

#endif /* XDMFWRITER_HPP_ */
