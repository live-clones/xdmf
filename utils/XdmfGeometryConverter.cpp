/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfTopologyConverter.cpp                                           */
/*                                                                           */
/*  Author:                                                                  */
/*     Andrew Burns                                                          */
/*     andrew.j.burns2@us.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2013 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#include <cmath>
#include <map>
#include <iostream>
#include <vector>
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfHeavyDataWriter.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryConverter.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfError.hpp"

shared_ptr<XdmfGeometryConverter>
XdmfGeometryConverter::New()
{
  shared_ptr<XdmfGeometryConverter> p(new XdmfGeometryConverter());
  return p;
}

XdmfGeometryConverter::XdmfGeometryConverter()
{
}

XdmfGeometryConverter::XdmfGeometryConverter(const XdmfGeometryConverter &)
{
}

XdmfGeometryConverter::~XdmfGeometryConverter()
{
}

shared_ptr<XdmfGeometry>
XdmfGeometryConverter::convertToCartesian(const shared_ptr<XdmfGeometry> & geometryToConvert) const
{
  if (geometryToConvert->getType() == XdmfGeometryType::NoGeometryType())
  {
    XdmfError::message(XdmfError::FATAL,
                         "Error: Converted Geometry size doesn't have a geometry type set");
  }

  if (geometryToConvert->getType() == XdmfGeometryType::XY() ||
      geometryToConvert->getType() == XdmfGeometryType::XYZ())
  {
    // Already in Cartesian form
    return geometryToConvert;
  }
  else
  {
    shared_ptr<XdmfGeometry> returnGeometry = XdmfGeometry::New();

    if (!geometryToConvert->isInitialized()) {
      geometryToConvert->read();
    }

    if (geometryToConvert->getSize()%geometryToConvert->getType()->getDimensions() != 0)
    {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Converted Geometry size doesn't match number of dimensions");
    }
    else
    {
      if (geometryToConvert->getType()->getDimensions() == 2)
      {
        // Polar to XY
        returnGeometry->setType(XdmfGeometryType::XY());
        if (geometryToConvert->getSize() < 2)
        {
          return returnGeometry;
        }
        std::vector<double> origin = geometryToConvert->getOrigin();
        returnGeometry->setOrigin(origin);
        unsigned int i = 0;
        while(i+1 < geometryToConvert->getSize())
        {
          double radius = geometryToConvert->getValue<double>(i);
          double inclination = geometryToConvert->getValue<double>(i+1);

          double x = (radius * std::sin(inclination));
          double y = (radius * std::cos(inclination));
          returnGeometry->pushBack(x);
          returnGeometry->pushBack(y);
          i = i + 2;
        }
      }
      else if (geometryToConvert->getType()->getDimensions() == 3)
      {
        // Spherical to XYZ
        returnGeometry->setType(XdmfGeometryType::XYZ());
        if (geometryToConvert->getSize() < 3)
        {
          return returnGeometry;
        }
        std::vector<double> origin = geometryToConvert->getOrigin();
        returnGeometry->setOrigin(origin);
        unsigned int i = 0;
        while(i+2 < geometryToConvert->getSize())
        {
          double radius = geometryToConvert->getValue<double>(i);
          double inclination = geometryToConvert->getValue<double>(i+1);
          double azimuth = geometryToConvert->getValue<double>(i+2);


          double x = (radius * std::sin(inclination) * std::cos(azimuth));
          double y = (radius * std::sin(inclination) * std::sin(azimuth));
          double z = (radius * std::cos(inclination));

          returnGeometry->pushBack(x);
          returnGeometry->pushBack(y);
          returnGeometry->pushBack(z);
          i = i + 3;
        }
      }
      else
      {
        XdmfError::message(XdmfError::FATAL,
                         "Error: Converted Geometry has an invalid number of dimensions");
      }
    }
    return returnGeometry;
  }
}

shared_ptr<XdmfGeometry>
XdmfGeometryConverter::convertToSpherical(const shared_ptr<XdmfGeometry> & geometryToConvert) const
{
  if (geometryToConvert->getType() == XdmfGeometryType::NoGeometryType())
  {
    XdmfError::message(XdmfError::FATAL,
                       "Error: Converted Geometry size doesn't have a geometry type set");
  }

  if (geometryToConvert->getType() == XdmfGeometryType::Polar() ||
      geometryToConvert->getType() == XdmfGeometryType::Spherical())
  {
    // Already in Spherical form
    return geometryToConvert;
  }
  else
  {
    shared_ptr<XdmfGeometry> returnGeometry = XdmfGeometry::New();

    if (!geometryToConvert->isInitialized()) {
      geometryToConvert->read();
    }

    if (geometryToConvert->getSize()%geometryToConvert->getType()->getDimensions() != 0)
    {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Converted Geometry size doesn't match number of dimensions"); 
    }
    else
    {
      if (geometryToConvert->getType()->getDimensions() == 2)
      {
        // XY to Polar
        returnGeometry->setType(XdmfGeometryType::Polar());
        // insert origin
        std::vector<double> origin = geometryToConvert->getOrigin();
        returnGeometry->setOrigin(origin); 
        unsigned int i = 0;
        while(i+1 < geometryToConvert->getSize())
        {
          double x = geometryToConvert->getValue<double>(i);
          double y = geometryToConvert->getValue<double>(i+1);

          double radius = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
          double inclination = std::acos(x/radius);
          returnGeometry->pushBack(radius);
          returnGeometry->pushBack(inclination);
          i = i + 2;
        }
      }
      else if (geometryToConvert->getType()->getDimensions() == 3)
      {
        // XYZ to Spherical
        returnGeometry->setType(XdmfGeometryType::Spherical());
        // insert origin
        std::vector<double> origin = geometryToConvert->getOrigin();
        returnGeometry->setOrigin(origin);
        unsigned int i = 0;
        while(i+2 < geometryToConvert->getSize())
        {
          double x = geometryToConvert->getValue<double>(i);
          double y = geometryToConvert->getValue<double>(i+1);
          double z = geometryToConvert->getValue<double>(i+2);

          double radius = std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
          double inclination = std::acos(z/radius);
          double azimuth = std::atan(y/x);
          returnGeometry->pushBack(radius);
          returnGeometry->pushBack(inclination);
          returnGeometry->pushBack(azimuth);
          i = i + 3;
        }
      }
      else
      {
        XdmfError::message(XdmfError::FATAL,
                         "Error: Converted Geometry has an invalid number of dimensions");
      }
    }
    return returnGeometry;
  }
}

void
XdmfGeometryConverter::convertToCartesianOverwrite(shared_ptr<XdmfGeometry> & geometryToConvert) const
{
  if (!(geometryToConvert->getType() == XdmfGeometryType::XY() ||
        geometryToConvert->getType() == XdmfGeometryType::XYZ()))
  {
    shared_ptr<XdmfGeometry> tempGeometry = convertToCartesian(geometryToConvert);
    // Change the type of the geometry provided
    geometryToConvert->setType(tempGeometry->getType());
    // Move values over to the original to complete the conversion
    geometryToConvert->swap(tempGeometry);
  }
}

void
XdmfGeometryConverter::convertToSphericalOverwrite(shared_ptr<XdmfGeometry> & geometryToConvert) const
{
  if (!(geometryToConvert->getType() == XdmfGeometryType::Polar() ||
        geometryToConvert->getType() == XdmfGeometryType::Spherical()))
  {
    shared_ptr<XdmfGeometry> tempGeometry = convertToSpherical(geometryToConvert);
    // Change the type of the geometry provided
    geometryToConvert->setType(tempGeometry->getType());
    // Move values over to the original to complete the conversion
    geometryToConvert->swap(tempGeometry);
  }
}

shared_ptr<XdmfGeometry>
XdmfGeometryConverter::zeroOrigin(const shared_ptr<XdmfGeometry> & geometryToConvert)
{
  shared_ptr<XdmfGeometry> returnGeometry = shared_ptr<XdmfGeometry>();
  std::vector<double> origin = geometryToConvert->getOrigin();
  if (origin.size() < geometryToConvert->getType()->getDimensions())
  {
    while (origin.size() < geometryToConvert->getType()->getDimensions())
    {
      origin.push_back(0.0);
    }
  }
  if (geometryToConvert->getType() == XdmfGeometryType::XY() ||
      geometryToConvert->getType() == XdmfGeometryType::XYZ())
  {
    returnGeometry = XdmfGeometry::New();
    returnGeometry->setType(geometryToConvert->getType());
    returnGeometry->initialize(geometryToConvert->getArrayType(), geometryToConvert->getDimensions());
    // Simply add the origin as an offset.
    unsigned int originIndex = 0;
    for (unsigned int i = 0; i < geometryToConvert->getSize(); ++i)
    {
      returnGeometry->insert(i, (geometryToConvert->getValue<double>(i) + origin[originIndex]));
      originIndex = (originIndex + 1) % geometryToConvert->getType()->getDimensions();
    }
  }
  else if (geometryToConvert->getType() == XdmfGeometryType::Polar() ||
           geometryToConvert->getType() == XdmfGeometryType::Spherical())
  {
    // The spherical version needs to convert to cartesian and back.
    returnGeometry = convertToSpherical(zeroOrigin(convertToCartesian(geometryToConvert)));
  }
  return returnGeometry;
}

void
XdmfGeometryConverter::zeroOriginOverwrite(shared_ptr<XdmfGeometry> & geometryToConvert)
{
  shared_ptr<XdmfGeometry> tempGeometry = zeroOrigin(geometryToConvert);
  geometryToConvert->swap(tempGeometry);;
}

// C Wrappers

XDMFGEOMETRYCONVERTER *
XdmfGeometryConverterNew()
{
  shared_ptr<XdmfGeometryConverter> * p = 
    new shared_ptr<XdmfGeometryConverter>(XdmfGeometryConverter::New());
  return (XDMFGEOMETRYCONVERTER *) p;
}

XDMFGEOMETRY * XdmfGeometryConverterConvertToCartesian(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert)
{
  shared_ptr<XdmfGeometryConverter> & refConverter = 
    *(shared_ptr<XdmfGeometryConverter> *)(converter);
  shared_ptr<XdmfGeometry> & refGeometry = 
    *(shared_ptr<XdmfGeometry> *)(geometryToConvert);
  shared_ptr<XdmfGeometry> * p = 
    new shared_ptr<XdmfGeometry>(refConverter->convertToCartesian(refGeometry));
  return (XDMFGEOMETRY *) p;
}

XDMFGEOMETRY * XdmfGeometryConverterConvertToSpherical(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert)
{
  shared_ptr<XdmfGeometryConverter> & refConverter = 
    *(shared_ptr<XdmfGeometryConverter> *)(converter);
  shared_ptr<XdmfGeometry> & refGeometry = 
    *(shared_ptr<XdmfGeometry> *)(geometryToConvert);
  shared_ptr<XdmfGeometry> * p = 
    new shared_ptr<XdmfGeometry>(refConverter->convertToSpherical(refGeometry));
  return (XDMFGEOMETRY *) p;
}

void XdmfGeometryConverterConvertToCartesianOverwrite(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert)
{
  shared_ptr<XdmfGeometryConverter> & refConverter = 
    *(shared_ptr<XdmfGeometryConverter> *)(converter);
  shared_ptr<XdmfGeometry> & refGeometry = 
    *(shared_ptr<XdmfGeometry> *)(geometryToConvert);
  refConverter->convertToCartesianOverwrite(refGeometry);
}

void XdmfGeometryConverterConvertToSphericalOverwrite(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert)
{
  shared_ptr<XdmfGeometryConverter> & refConverter = 
    *(shared_ptr<XdmfGeometryConverter> *)(converter);
  shared_ptr<XdmfGeometry> & refGeometry = 
    *(shared_ptr<XdmfGeometry> *)(geometryToConvert);
  refConverter->convertToSphericalOverwrite(refGeometry);
}

XDMFGEOMETRY * XdmfGeometryConverterZeroOrigin(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert)
{
  shared_ptr<XdmfGeometryConverter> & refConverter = 
    *(shared_ptr<XdmfGeometryConverter> *)(converter);
  shared_ptr<XdmfGeometry> & refGeometry = 
    *(shared_ptr<XdmfGeometry> *)(geometryToConvert);
  shared_ptr<XdmfGeometry> * p = 
    new shared_ptr<XdmfGeometry>(refConverter->zeroOrigin(refGeometry));
  return (XDMFGEOMETRY *) p;
}

void XdmfGeometryConverterZeroOriginOverwrite(XDMFGEOMETRYCONVERTER * converter, XDMFGEOMETRY * geometryToConvert)
{
  shared_ptr<XdmfGeometryConverter> & refConverter = 
    *(shared_ptr<XdmfGeometryConverter> *)(converter);
  shared_ptr<XdmfGeometry> & refGeometry = 
    *(shared_ptr<XdmfGeometry> *)(geometryToConvert);
  refConverter->zeroOriginOverwrite(refGeometry);
}

void
XdmfGeometryConverterFree(XDMFGEOMETRYCONVERTER * converter)
{
  if (converter != NULL) {
    delete (shared_ptr<XdmfGeometryConverter> *)converter;
    converter = NULL;
  }
}
