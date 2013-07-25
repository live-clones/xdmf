/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfExodusWriter.hpp                                                */
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

#ifndef XDMFEXODUSWRITER_HPP_
#define XDMFEXODUSWRITER_HPP_

// Forward Declarations
class XdmfGridCollection;
class XdmfUnstructuredGrid;

// Includes
#include <string>
#include "XdmfUtils.hpp"
#include "XdmfSharedPtr.hpp"

/**
 * @brief Writes an Xdmf structure in memory to an ExodusII file on
 * disk.
 */
class XDMFUTILS_EXPORT XdmfExodusWriter {

public:

  /**
   * Create a new XdmfExodusReader.
   *
   * @return constructed XdmfExodusReader.
   */
  static shared_ptr<XdmfExodusWriter> New();

  virtual ~XdmfExodusWriter();

  /**
   * Write an XdmfUnstructuredGrid to an ExodusII file.
   *
   * @param filePath of the ExodusII file to write.
   * @param grid an XdmfUnstructuredGrid to write to ExodusII file
   * format.
   */
  void write(const std::string & filePath,
             const shared_ptr<XdmfUnstructuredGrid> grid) const;

  /**
   * Write an XdmfGridCollection to an ExodusII file.
   *
   * @param filePath of the ExodusII file to write.
   * @param grid an XdmfGridCollection to write to ExodusII file
   * format.
   */
  void write(const std::string & filePath,
             const shared_ptr<XdmfGridCollection> grid) const;


protected:

  XdmfExodusWriter();

private:

  XdmfExodusWriter(const XdmfExodusWriter &);  // Not implemented.
  void operator=(const XdmfExodusWriter &);  // Not implemented.

};

#endif /* XDMFEXODUSWRITER_HPP_ */
