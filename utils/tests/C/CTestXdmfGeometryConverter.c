#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGeometryConverter.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfWriter.hpp"

#include "stdio.h"
#include "math.h"
#include "assert.h"

double Pi = 3.1415926535897932384626433832795;

int main()
{
  int status = 0;
  unsigned int i = 0;

  XDMFGEOMETRYCONVERTER * converter = XdmfGeometryConverterNew();

  XDMFGEOMETRY * cartesianGeo = XdmfGeometryNew();

  XdmfGeometrySetType(cartesianGeo, XDMF_GEOMETRY_TYPE_XYZ, &status);

  for (i = 0; i < 3; ++i)
  {
    double pushval = 1;
    XdmfGeometryPushBack(cartesianGeo, &pushval, XDMF_ARRAY_TYPE_FLOAT64, &status);
  }

  XDMFGEOMETRY * resultSphericalGeo = XdmfGeometryConverterConvertToSpherical(converter, cartesianGeo);

  XDMFGEOMETRY * sphericalGeo = XdmfGeometryNew();

  XdmfGeometrySetType(sphericalGeo, XDMF_GEOMETRY_TYPE_SPHERICAL, &status);

  double value = sqrt(3);

  XdmfGeometryPushBack(sphericalGeo, &value, XDMF_ARRAY_TYPE_FLOAT64, &status);

  value = asin(sqrt((double)2/3));

  XdmfGeometryPushBack(sphericalGeo, &value, XDMF_ARRAY_TYPE_FLOAT64, &status); // should be equal to std::acos(1/std::sqrt(3))

  value = Pi/4;

  XdmfGeometryPushBack(sphericalGeo, &value, XDMF_ARRAY_TYPE_FLOAT64, &status);

  printf("Tolerance 10^-15\nresult\n%s\n?=\ncompare to\n%s\n", XdmfGeometryGetValuesString(resultSphericalGeo), XdmfGeometryGetValuesString(sphericalGeo));

  for (i = 0; i < XdmfGeometryGetSize(sphericalGeo); ++i)
  {
    double val1 = ((double *)XdmfGeometryGetValue(resultSphericalGeo, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
    double val2 = ((double *)XdmfGeometryGetValue(sphericalGeo, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
    val1 = floor(val1 * pow((double)10, (double)15) + 0.5) / pow((double)10, (double)15);
    val2 = floor(val2 * pow((double)10, (double)15) + 0.5) / pow((double)10, (double)15);
    printf("%4.15f ?= %4.15f\n", val1, val2);
    assert(val1 == val2);
  }

  XDMFGEOMETRY * resultCartesianGeo = XdmfGeometryConverterConvertToCartesian(converter, sphericalGeo);

  printf("%s\n?=\n%s\n", XdmfGeometryGetValuesString(resultCartesianGeo), XdmfGeometryGetValuesString(cartesianGeo));

  for (i = 0; i < XdmfGeometryGetSize(cartesianGeo); ++i)
  {
    double val1 = ((double *)XdmfGeometryGetValue(resultCartesianGeo, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
    double val2 = ((double *)XdmfGeometryGetValue(cartesianGeo, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
    val1 = floor(val1 * pow((double)10, (double)15) + 0.5) / pow((double)10, (double)15);
    val2 = floor(val2 * pow((double)10, (double)15) + 0.5) / pow((double)10, (double)15);
    printf("%4.15f ?= %4.15f\n", val1, val2);
    assert(val1 == val2);
  }

  // Convert in place for geometries with a lot of references

  XDMFGEOMETRY * convertedToSpherical = XdmfGeometryNew();

  XdmfGeometrySetType(convertedToSpherical, XDMF_GEOMETRY_TYPE_XYZ, &status);

  for (i = 0; i < 3; ++i)
  {
    double pushval = 1;
    XdmfGeometryPushBack(convertedToSpherical, &pushval, XDMF_ARRAY_TYPE_FLOAT64, &status);
  }

  XdmfGeometryConverterConvertToSphericalOverwrite(converter, convertedToSpherical);

  for (i = 0; i < XdmfGeometryGetSize(sphericalGeo); ++i)
  {
    double val1 = ((double *)XdmfGeometryGetValue(convertedToSpherical, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
    double val2 = ((double *)XdmfGeometryGetValue(sphericalGeo, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
    val1 = floor(val1 * pow((double)10, (double)15) + 0.5) / pow((double)10, (double)15);
    val2 = floor(val2 * pow((double)10, (double)15) + 0.5) / pow((double)10, (double)15);
    printf("%4.15f ?= %4.15f\n", val1, val2);
    assert(val1 == val2);
  }

  XDMFGEOMETRY * convertedToCartesian = XdmfGeometryNew();

  XdmfGeometrySetType(convertedToCartesian, XDMF_GEOMETRY_TYPE_SPHERICAL, &status);

  value = sqrt(3);

  XdmfGeometryPushBack(convertedToCartesian, &value, XDMF_ARRAY_TYPE_FLOAT64, &status);

  value = asin(sqrt((double)2/3));

  XdmfGeometryPushBack(convertedToCartesian, &value, XDMF_ARRAY_TYPE_FLOAT64, &status); // should be equal to std::acos(1/std::sqrt(3))

  value = Pi/4;

  XdmfGeometryPushBack(convertedToCartesian, &value, XDMF_ARRAY_TYPE_FLOAT64, &status);

  XdmfGeometryConverterConvertToCartesianOverwrite(converter, convertedToCartesian);

  for (i = 0; i < XdmfGeometryGetSize(cartesianGeo); ++i)
  {
    double val1 = ((double *)XdmfGeometryGetValue(convertedToCartesian, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
    double val2 = ((double *)XdmfGeometryGetValue(cartesianGeo, i, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
    val1 = floor(val1 * pow((double)10, (double)15) + 0.5) / pow((double)10, (double)15);
    val2 = floor(val2 * pow((double)10, (double)15) + 0.5) / pow((double)10, (double)15);
    printf("%4.15f ?= %4.15f\n", val1, val2);
    assert(val1 == val2);
  }

  return 0;
}
