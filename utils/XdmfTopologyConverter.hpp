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

// C Compatible Includes
#include "XdmfUtils.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfHeavyDataWriter.hpp"

#ifdef __cplusplus

// Forward Declarations
//class XdmfHeavyDataWriter;
class XdmfTopologyType;
//class XdmfUnstructuredGrid;

// Includes
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
 *   Hexahedron to Hexahedron_27
 *   Hexahedron to Hexahedron_64
 *   Hexahedron to Hexahedron_125
 *   Hexahedron to Hexahedron_216
 *   Hexahedron to Hexahedron_343
 *   Hexahedron to Hexahedron_512
 *   Hexahedron to Hexahedron_729
 *   Hexahedron to Hexahedron_1000
 *   Hexahedron to Hexahedron_1331
 *   Hexahedron to Hexahedron_Spectral_64
 *   Hexahedron to Hexahedron_Spectral_125
 *   Hexahedron to Hexahedron_Spectral_216
 *   Hexahedron to Hexahedron_Spectral_343
 *   Hexahedron to Hexahedron_Spectral_512
 *   Hexahedron to Hexahedron_Spectral_729
 *   Hexahedron to Hexahedron_Spectral_1000
 *   Hexahedron to Hexahedron_Spectral_1331
 *   Hexahedron_64 to Hexahedron
 *   Hexahedron_125 to Hexahedron
 *   Hexahedron_216 to Hexahedron
 *   Hexahedron_343 to Hexahedron
 *   Hexahedron_512 to Hexahedron
 *   Hexahedron_729 to Hexahedron
 *   Hexahedron_1000 to Hexahedron
 *   Hexahedron_1331 to Hexahedron
 *   Hexahedron_Spectral_64 to Hexahedron
 *   Hexahedron_Spectral_125 to Hexahedron
 *   Hexahedron_Spectral_216 to Hexahedron
 *   Hexahedron_Spectral_343 to Hexahedron
 *   Hexahedron_Spectral_512 to Hexahedron
 *   Hexahedron_Spectral_729 to Hexahedron
 *   Hexahedron_Spectral_1000 to Hexahedron
 *   Hexahedron_Spectral_1331 to Hexahedron
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
   * @param heavyDataWriter an heavy data writer to write the converted mesh
   * to. If no heavyDataWriter is specified, all mesh data will remain in
   * memory.
   *
   * @return the converted unstructured grid.
   */
  shared_ptr<XdmfUnstructuredGrid>
  convert(const shared_ptr<XdmfUnstructuredGrid> gridToConvert,
          const shared_ptr<const XdmfTopologyType> topologyType,
          const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter = shared_ptr<XdmfHeavyDataWriter>()) const;

  /**
   * Gets all faces within the given topology. Removing duplicates.
   *
   * @param     convertedTopology       The topology to be deconstructed
   * @return                            A topology containing the faces from the deconstructed topology
   */
  shared_ptr<XdmfTopology>
  getExternalFaces(const shared_ptr<XdmfTopology> convertedTopology);

  XdmfTopologyConverter(const XdmfTopologyConverter &);

protected:

  XdmfTopologyConverter();

private:

  void insertInHash(std::vector<long> nodes,
                    std::vector<std::vector<std::vector<long> > > & hash,
                    unsigned int numCornerNodes);

  void operator=(const XdmfTopologyConverter &);  // Not implemented.

};

#endif

#ifdef __cplusplus
extern "C" {
#endif

// C wrappers go here

struct XDMFTOPOLOGYCONVERTER; // Simply as a typedef to ensure correct typing
typedef struct XDMFTOPOLOGYCONVERTER XDMFTOPOLOGYCONVERTER;

XDMFUTILS_EXPORT XDMFTOPOLOGYCONVERTER * XdmfTopologyConverterNew();

XDMFUTILS_EXPORT XDMFUNSTRUCTUREDGRID * XdmfTopologyConverterConvert(XDMFTOPOLOGYCONVERTER * converter,
                                                                     XDMFUNSTRUCTUREDGRID * gridToConvert,
                                                                     int topologytype,
                                                                     XDMFHEAVYDATAWRITER * heavyDataWriter);

XDMFUTILS_EXPORT XDMFTOPOLOGY * XdmfTopologyConverterGetExternalFaces(XDMFTOPOLOGYCONVERTER * converter,
                                                                      XDMFTOPOLOGY * convertedTopology);

XDMFUTILS_EXPORT void XdmfTopologyConverterFree(XDMFTOPOLOGYCONVERTER * converter);

#ifdef __cplusplus
}
#endif

#endif /* XDMFTOPOLOGYCONVERTER_HPP_ */
