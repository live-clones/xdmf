/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfTopologyConverter.hpp                                           */
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

#ifndef XDMFTOPOLOGYCONVERTER_HPP_
#define XDMFTOPOLOGYCONVERTER_HPP_

// Forward Declarations
class XdmfHeavyDataWriter;
class XdmfTopologyType;
class XdmfUnstructuredGrid;

// Includes
#include "XdmfUtils.hpp"
#include "XdmfSharedPtr.hpp"

/**
 * @brief Converts an unstructured grid to different topology types.
 *
 * Attributes and sets attached to the grid are adjusted to remain
 * valid for the new topology type.
 *
 * When converting from a lower order topology to a higher order
 * topology type (e.g. Hexahedron to Hexahedron_64) additional points
 * are added to the mesh, no additional elements are added. When
 * converting from a higher order topology to a lower order topology
 * type (e.h. Hexahedron_64 to Hexahedron) elements are tesselated to
 * form the new topology, no additional points are added.
 *
 * Currently supported conversions:
 *   Hexahedron to Hexahedron_64
 *   Hexahedron to Hexahedron_125
 *   Hexahedron to Hexahedron_216
 *   Hexahedron to Hexahedron_343
 *   Hexahedron to Hexahedron_512
 *   Hexahedron to Hexahedron_729
 *   Hexahedron to Hexahedron_1000
 *   Hexahedron to Hexahedron_1331
 *   Hexahedron_64 to Hexahedron
 *   Hexahedron_125 to Hexahedron
 *   Hexahedron_216 to Hexahedron
 *   Hexahedron_343 to Hexahedron
 *   Hexahedron_512 to Hexahedron
 *   Hexahedron_729 to Hexahedron
 *   Hexahedron_1000 to Hexahedron
 *   Hexahedron_1331 to Hexahedron
 *
 * Additional options:
 *   When generating high order hexahedrons an option of 0 adds nodes at
 *   equal intervals inside the element. An option of 1 places nodes at
 *   Guass Lobatto Legendre points.
 */
class XDMFUTILS_EXPORT XdmfTopologyConverter {

public:

  /**
   * Create a new XdmfTopologyConverter.
   *
   * @return constructed XdmfTopologyConverter.
   */
  static shared_ptr<XdmfTopologyConverter> New();

  virtual ~XdmfTopologyConverter();

  /**
   * Converts an unstructured grid to a different topology type
   *
   * @param gridToConvert the unstructured grid to convert to a different
   * topology
   * @param topologyType the topology type to convert to.
   * @param options additional options used when converting.
   * @param heavyDataWriter an heavy data writer to write the converted mesh
   * to. If no heavyDataWriter is specified, all mesh data will remain in
   * memory.
   *
   * @return the converted unstructured grid.
   */
  shared_ptr<XdmfUnstructuredGrid>
  convert(const shared_ptr<XdmfUnstructuredGrid> gridToConvert,
          const shared_ptr<const XdmfTopologyType> topologyType,
          unsigned int options = 0,
          const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter = shared_ptr<XdmfHeavyDataWriter>()) const;

protected:

  XdmfTopologyConverter();

private:

  XdmfTopologyConverter(const XdmfTopologyConverter &);  // Not implemented.
  void operator=(const XdmfTopologyConverter &);  // Not implemented.

};

#endif /* XDMFTOPOLOGYCONVERTER_HPP_ */
