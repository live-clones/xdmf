/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfExodusReader.hpp                                                */
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

#ifndef XDMFEXODUSREADER_HPP_
#define XDMFEXODUSREADER_HPP_

// Forward Declarations
class XdmfHeavyDataWriter;
class XdmfUnstructuredGrid;

// Includes
#include <string>
#include "XdmfUtils.hpp"
#include "XdmfSharedPtr.hpp"

/**
 * @brief Reads an ExodusII file from disk into an Xdmf structure in
 * memory.
 */
class XDMFUTILS_EXPORT XdmfExodusReader {

public:

  /**
   * Create a new XdmfExodusReader.
   *
   * @return constructed XdmfExodusReader.
   */
  static shared_ptr<XdmfExodusReader> New();

  virtual ~XdmfExodusReader();

  /**
   * Read the contents of an ExodusII file from disk into an Xdmf
   * structure in memory.
   *
   * @param fileName containing the path of the exodus file to read.
   * @param heavyDataWriter an XdmfHeavyDataWriter to write the mesh to. If no
   * heavyDataWriter is specified, all mesh data will remain in memory.
   *
   * @return an unstructured grid containing the mesh stored in the ExodusII
   * file.
   */
  shared_ptr<XdmfUnstructuredGrid>
  read(const std::string & fileName,
       const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter = shared_ptr<XdmfHeavyDataWriter>()) const;

protected:

  XdmfExodusReader();

private:

  XdmfExodusReader(const XdmfExodusReader &);  // Not implemented.
  void operator=(const XdmfExodusReader &);  // Not implemented.

};

#endif /* XDMFEXODUSREADER_HPP_ */
