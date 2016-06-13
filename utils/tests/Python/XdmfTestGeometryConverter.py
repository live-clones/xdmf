from Xdmf import *
from XdmfUtils import *
from math import *

if __name__ == "__main__":
  Pi = 3.1415926535897932384626433832795

  converter = XdmfGeometryConverter.New()

  cartesianGeo = XdmfGeometry.New()

  cartesianGeo.setType(XdmfGeometryType.XYZ())

  for i in range(3):
    cartesianGeo.pushBackAsFloat64(1)

  resultSphericalGeo = converter.convertToSpherical(cartesianGeo)

  sphericalGeo = XdmfGeometry.New()

  sphericalGeo.setType(XdmfGeometryType.Spherical())

  sphericalGeo.pushBackAsFloat64(sqrt(3));
  sphericalGeo.pushBackAsFloat64(asin(sqrt(2.0/3.0))); # should be equal to acos(1/sqrt(3))
  sphericalGeo.pushBackAsFloat64(Pi/4);

  print "Tolerance 10^-15\nresult\n" , resultSphericalGeo.getValuesString(), "\n?=\ncompare to\n", sphericalGeo.getValuesString()

  for i in range(sphericalGeo.getSize()):
    val1 = resultSphericalGeo.getValueAsFloat64(i)
    val2 = sphericalGeo.getValueAsFloat64(i)
    val1 = floor(val1 * pow(10.0, 15.0) + 0.5) / pow(10.0, 15.0);
    val2 = floor(val2 * pow(10.0, 15.0) + 0.5) / pow(10.0, 15.0);
    print val1, " ?= ", val2
    assert(val1 == val2)

  resultCartesianGeo = converter.convertToCartesian(sphericalGeo)

  print resultCartesianGeo.getValuesString(), "\n?=\n", cartesianGeo.getValuesString()

  for i in range(cartesianGeo.getSize()):
    val1 = resultCartesianGeo.getValueAsFloat64(i);
    val2 = cartesianGeo.getValueAsFloat64(i);
    val1 = floor(val1 * pow(10.0, 15.0) + 0.5) / pow(10.0, 15.0)
    val2 = floor(val2 * pow(10.0, 15.0) + 0.5) / pow(10.0, 15.0)
    print val1, " ?= ", val2
    assert(val1 == val2)

  # Convert in place for geometries with a lot of references

  convertedToSpherical = XdmfGeometry.New()

  convertedToSpherical.setType(XdmfGeometryType.XYZ())

  for i in range(3):
    convertedToSpherical.pushBackAsFloat64(1)

  converter.convertToSphericalOverwrite(convertedToSpherical)

  for i in range(sphericalGeo.getSize()):
    val1 = convertedToSpherical.getValueAsFloat64(i)
    val2 = sphericalGeo.getValueAsFloat64(i)
    val1 = floor(val1 * pow(10.0, 15.0) + 0.5) / pow(10.0, 15.0)
    val2 = floor(val2 * pow(10.0, 15.0) + 0.5) / pow(10.0, 15.0)
    print val1, " ?= ", val2
    assert(val1 == val2)

  convertedToCartesian = XdmfGeometry.New()

  convertedToCartesian.setType(XdmfGeometryType.Spherical())

  convertedToCartesian.pushBackAsFloat64(sqrt(3))
  convertedToCartesian.pushBackAsFloat64(asin(sqrt(2.0/3))) # should be equal to acos(1/sqrt(3))
  convertedToCartesian.pushBackAsFloat64(Pi/4)

  converter.convertToCartesianOverwrite(convertedToCartesian)

  for i in range(cartesianGeo.getSize()):
    val1 = convertedToCartesian.getValueAsFloat64(i)
    val2 = cartesianGeo.getValueAsFloat64(i)
    val1 = floor(val1 * pow(10.0, 15.0) + 0.5) / pow(10.0, 15.0)
    val2 = floor(val2 * pow(10.0, 15.0) + 0.5) / pow(10.0, 15.0)
    print val1, " ?= ", val2
    assert(val1 == val2)
