#include <cmath>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGeometryConverter.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfWriter.hpp"

double Pi = 3.1415926535897932384626433832795;

int main(int, char **)
{
  shared_ptr<XdmfGeometryConverter> converter =
    XdmfGeometryConverter::New();

  shared_ptr<XdmfGeometry> cartesianGeo = XdmfGeometry::New();

  cartesianGeo->setType(XdmfGeometryType::XYZ());

  for (unsigned int i = 0; i < 3; ++i)
  {
    cartesianGeo->pushBack(1);
  }

  shared_ptr<XdmfGeometry> resultSphericalGeo = converter->convertToSpherical(cartesianGeo);

  shared_ptr<XdmfGeometry> sphericalGeo = XdmfGeometry::New();

  sphericalGeo->setType(XdmfGeometryType::Spherical());

  sphericalGeo->pushBack(std::sqrt(3));
  sphericalGeo->pushBack(std::asin(std::sqrt((double)2/3))); // should be equal to std::acos(1/std::sqrt(3))
//  sphericalGeo->pushBack(std::acos(1/std::sqrt(3))); // should be equal to std::asin(std::sqrt((double)2/3))
  sphericalGeo->pushBack(Pi/4);

  std::cout << "Tolerance 10^-15\nresult\n" << resultSphericalGeo->getValuesString() << "\n?=\ncompare to\n" << sphericalGeo->getValuesString() << std::endl;

  for (unsigned int i = 0; i < sphericalGeo->getSize(); ++i)
  {
    double val1 = resultSphericalGeo->getValue<double>(i);
    double val2 = sphericalGeo->getValue<double>(i);
    val1 = std::floor(val1 * std::pow((double)10, (double)15) + 0.5) / std::pow((double)10, (double)15);
    val2 = std::floor(val2 * std::pow((double)10, (double)15) + 0.5) / std::pow((double)10, (double)15);
    std::cout << std::fixed << std::setprecision(15) << val1 << " ?= " << std::fixed << std::setprecision(15) << val2 << std::endl;
    assert(val1 == val2);
  }

  shared_ptr<XdmfGeometry> resultCartesianGeo = converter->convertToCartesian(sphericalGeo);

  std::cout << resultCartesianGeo->getValuesString() << "\n?=\n" << cartesianGeo->getValuesString() << std::endl;

  for (unsigned int i = 0; i < cartesianGeo->getSize(); ++i)
  {
    double val1 = resultCartesianGeo->getValue<double>(i);
    double val2 = cartesianGeo->getValue<double>(i);
    val1 = std::floor(val1 * std::pow((double)10, (double)15) + 0.5) / std::pow((double)10, (double)15);
    val2 = std::floor(val2 * std::pow((double)10, (double)15) + 0.5) / std::pow((double)10, (double)15);
    std::cout << std::fixed << std::setprecision(15) << val1 << " ?= " << std::fixed << std::setprecision(15) << val2 << std::endl;
    assert(val1 == val2);
  }

  // Convert in place for geometries with a lot of references

  shared_ptr<XdmfGeometry> convertedToSpherical = XdmfGeometry::New();

  convertedToSpherical->setType(XdmfGeometryType::XYZ());

  for (unsigned int i = 0; i < 3; ++i)
  {
    convertedToSpherical->pushBack(1);
  }

  converter->convertToSphericalOverwrite(convertedToSpherical);

  for (unsigned int i = 0; i < sphericalGeo->getSize(); ++i)
  {
    double val1 = convertedToSpherical->getValue<double>(i);
    double val2 = sphericalGeo->getValue<double>(i);
    val1 = std::floor(val1 * std::pow((double)10, (double)15) + 0.5) / std::pow((double)10, (double)15);
    val2 = std::floor(val2 * std::pow((double)10, (double)15) + 0.5) / std::pow((double)10, (double)15);
    std::cout << std::fixed << std::setprecision(15) << val1 << " ?= " << std::fixed << std::setprecision(15) << val2 << std::endl;
    assert(val1 == val2);
  }

  shared_ptr<XdmfGeometry> convertedToCartesian = XdmfGeometry::New();

  convertedToCartesian->setType(XdmfGeometryType::Spherical());

  convertedToCartesian->pushBack(std::sqrt(3));
  convertedToCartesian->pushBack(std::asin(std::sqrt((double)2/3))); // should be equal to std::acos(1/std::sqrt(3))
//  sphericalGeo->pushBack(std::acos(1/std::sqrt(3))); // should be equal to std::asin(std::sqrt((double)2/3))
  convertedToCartesian->pushBack(Pi/4);

  converter->convertToCartesianOverwrite(convertedToCartesian);

  for (unsigned int i = 0; i < cartesianGeo->getSize(); ++i)
  {
    double val1 = convertedToCartesian->getValue<double>(i);
    double val2 = cartesianGeo->getValue<double>(i);
    val1 = std::floor(val1 * std::pow((double)10, (double)15) + 0.5) / std::pow((double)10, (double)15);
    val2 = std::floor(val2 * std::pow((double)10, (double)15) + 0.5) / std::pow((double)10, (double)15);
    std::cout << std::fixed << std::setprecision(15) << val1 << " ?= " << std::fixed << std::setprecision(15) << val2 << std::endl;
    assert(val1 == val2);
  }

  return 0;
}
