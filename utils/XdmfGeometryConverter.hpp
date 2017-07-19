/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfGeometryConverter.hpp                                           */
/*                                                                           */
/*  Author:                                                                  */
/*     Andrew Burns                                                          */
/*     andrew.j.burns2@us.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2014 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef XDMFGEOMETRYCONVERTER_HPP_
#define XDMFGEOMETRYCONVERTER_HPP_

// C Compatible Includes
#include "XdmfUtils.hpp"
#include "XdmfGeometry.hpp"

#ifdef __cplusplus

// Forward Declarations
//class XdmfGeometry;

// Includes
#include "XdmfSharedPtr.hpp"

/**
 * @brief Converts an geometry to a different coordinate system.
 *
 * Converts between the Cartesian and Spherical coordinate systems.
 * Maintains number of dimensions, datatype converted to double
 * precision floating point (XdmfArrayType::Float64()).
 *
 * The cmath library is used for the conversions so angles
 * are treated as radians.
 */
class XDMFUTILS_EXPORT XdmfGeometryConverter {

public:

  /**
   * Create a new XdmfGeometryConverter.
   *
   * @return    constructed XdmfGeometryConverter.
   */
  static shared_ptr<XdmfGeometryConverter> New();

  virtual ~XdmfGeometryConverter();

  /**
   * Converts the provided geometry to Cartesian coordinates.
   *
   * @param     geometryToConvert       The geometry to be converted
   *                                    to Cartesian coordinates.
   *
   * @return                            The geometry equivalent for
   *                                    the Cartesian Coordinate system.
   */
  shared_ptr<XdmfGeometry>
  convertToCartesian(const shared_ptr<XdmfGeometry> & geometryToConvert) const;

  /**
   * Converts the provided geometry to spherical coordinates.
   *
   * @param     geometryToConvert       The geometry to be converted
   *                                    to spherical coordinates.
   *
   * @return                            The geometry equivalent for
   *                                    the Spherical Coordinate system.
   */
  shared_ptr<XdmfGeometry>
  convertToSpherical(const shared_ptr<XdmfGeometry> & geometryToConvert) const;

 /**
   * Converts the provided geometry to Cartesian coordinates. This version
   * overwrites the data in the geometry provided instead of returning a new one.
   *
   * @param     geometryToConvert       The geometry to be converted
   *                                    to Cartesian coordinates.
   */
  void
  convertToCartesianOverwrite(shared_ptr<XdmfGeometry> & geometryToConvert) const;

  /**
   * Converts the provided geometry to spherical coordinates.  This version
   * overwrites the data in the geometry provided instead of returning a new one.
   *
   * @param     geometryToConvert       The geometry to be converted
   *                                    to spherical coordinates.
   */
  void
  convertToSphericalOverwrite(shared_ptr<XdmfGeometry> & geometryToConvert) const;

  shared_ptr<XdmfGeometry>
  zeroOrigin(const shared_ptr<XdmfGeometry> & geometryToConvert);

  void
  zeroOriginOverwrite(shared_ptr<XdmfGeometry> & geometryToConvert);

  XdmfGeometryConverter(const XdmfGeometryConverter &);

protected:

  XdmfGeometryConverter();

private:

  void operator=(const XdmfGeometryConverter &);  // Not implemented.

};

#endif

#ifdef __cplusplus
extern "C" {
#endif

// C wrappers go here

struct XDMFGEOMETRYCONVERTER; // Simply as a typedef to ensure correct typing
typedef struct XDMFGEOMETRYCONVERTER XDMFGEOMETRYCONVERTER;

XDMFUTILS_EXPORT XDMFGEOMETRYCONVERTER * XdmfGeometryConverterNew();

XDMFUTILS_EXPORT XDMFGEOMETRY * XdmfGeometryConverterConvertToCartesian(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert);

XDMFUTILS_EXPORT XDMFGEOMETRY * XdmfGeometryConverterConvertToSpherical(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert);

XDMFUTILS_EXPORT void XdmfGeometryConverterConvertToCartesianOverwrite(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert);

XDMFUTILS_EXPORT void XdmfGeometryConverterConvertToSphericalOverwrite(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert);

XDMFUTILS_EXPORT XDMFGEOMETRY * XdmfGeometryConverterZeroOrigin(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert);

XDMFUTILS_EXPORT void XdmfGeometryConverterZeroOriginOverwrite(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert);

XDMFUTILS_EXPORT void XdmfGeometryConverterFree(XDMFGEOMETRYCONVERTER * converter);

#ifdef __cplusplus
}
#endif

#endif /* XDMFGEOMETRYCONVERTER_HPP_ */
