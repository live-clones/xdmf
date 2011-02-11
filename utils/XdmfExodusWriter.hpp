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
class XdmfTopologyType;
class XdmfUnstructuredGrid;

// Includes
#include <boost/shared_ptr.hpp>
#include <string>

/*!
 * @brief XdmfExodusWriter writes an Xdmf structure in memory to an ExodusII file on disk.
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
     * Write an XdmfGridUnstructured to an ExodusII file.
     *
     * @param filePath of the ExodusII file to write.
     * @param gridToWrite an XdmfGrid to write to ExodusII file format.
     */
    void write(const std::string & filePath, const boost::shared_ptr<XdmfUnstructuredGrid> gridToWrite) const;

protected:

	XdmfExodusWriter();

private:

	/**
	 * PIMPL
	 */
	class XdmfExodusWriterImpl;

	XdmfExodusWriter(const XdmfExodusWriter & exodusWriter);  // Not implemented.
	void operator=(const XdmfExodusWriter & exodusWriter);  // Not implemented.

	/**
	 * Convert an Xdmf topology type to an exodus topology type.
	 *
	 * @param exodusTopologyType a string containing the name of the exodus topology type to convert.
	 * @param pointsPerCell the number of points per cell for the exodus topology type to convert.
	 *
	 * @return the equivalent XdmfTopologyType.
	 */
	std::string xdmfToExodusTopologyType(boost::shared_ptr<const XdmfTopologyType> topologyType) const;

	XdmfExodusWriterImpl * mImpl;
};

#endif /* XDMFEXODUSWRITER_HPP_ */
