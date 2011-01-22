#ifndef XDMFEXODUSWRITER_HPP_
#define XDMFEXODUSWRITER_HPP_

// Forward Declarations
class XdmfUnstructuredGrid;

// Includes
#include <boost/shared_ptr.hpp>
#include <string>

/*!
 * @brief XdmfExodusWriter writes an Xdmf structure in memory to an ExodusII
 * file on disk.
 */
class XdmfExodusWriter {

public:

  /**
   * Create a new XdmfExodusReader.
   *
   * @return constructed XdmfExodusReader.
   */
  static boost::shared_ptr<XdmfExodusWriter> New();

  virtual ~XdmfExodusWriter();

  /*!
   * Write an XdmfUnstructuredGrid to an ExodusII file.
   *
   * @param filePath of the ExodusII file to write.
   * @param gridToWrite an XdmfUnstructuredGrid to write to ExodusII file
   * format.
   */
  void write(const std::string & filePath,
             const boost::shared_ptr<XdmfUnstructuredGrid> gridToWrite) const;

protected:

  XdmfExodusWriter();

private:

  XdmfExodusWriter(const XdmfExodusWriter &);  // Not implemented.
  void operator=(const XdmfExodusWriter &);  // Not implemented.

};

#endif /* XDMFEXODUSWRITER_HPP_ */
