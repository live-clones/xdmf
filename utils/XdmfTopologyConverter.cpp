/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfTopologyConverter.cpp                                           */
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

#include <cmath>
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfHeavyDataWriter.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyConverter.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfError.hpp"

//
// local methods
//
namespace {

  void handleSetConversion(const shared_ptr<XdmfUnstructuredGrid> gridToConvert,
			   const shared_ptr<XdmfUnstructuredGrid> toReturn,
			   const std::map<unsigned int, unsigned int> & oldIdToNewId,
			   const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter) 
  {

    for(unsigned int i=0; i<gridToConvert->getNumberSets(); ++i) {
      const shared_ptr<XdmfSet> set = gridToConvert->getSet(i);
      const shared_ptr<const XdmfSetType> setType = set->getType();
      if(setType == XdmfSetType::Cell()) {
	toReturn->insert(set);
      }
      else if(setType == XdmfSetType::Node()) {
	bool releaseSet = false;
	if(!set->isInitialized()) {
	  set->read();
	  releaseSet = true;
	}
	shared_ptr<XdmfSet> toReturnSet = XdmfSet::New();
	toReturnSet->setName(set->getName());
	toReturnSet->setType(set->getType());
	toReturnSet->initialize(set->getArrayType(),
				set->getSize());
	  
	for(unsigned int i=0; i<set->getSize(); ++i) {
	  const unsigned int nodeId = set->getValue<unsigned int>(i);
	  std::map<unsigned int, unsigned int>::const_iterator iter =
	    oldIdToNewId.find(nodeId);
	  if(iter == oldIdToNewId.end()) {
	    XdmfError::message(XdmfError::FATAL, 
			       "Error converting hex node id set to hex_27 "
			       "node id set.");
	  }
	  toReturnSet->insert(i, iter->second);
	}
	if(releaseSet) {
	  set->release();
	}
	  
	toReturn->insert(toReturnSet);
	  
	if(heavyDataWriter) {
	  toReturnSet->accept(heavyDataWriter);
	  toReturnSet->release();
	}
      }
    }
  }

  // Classes that perform topology conversions. Converter is the root
  // base class.  Tessellator is a subclass of Converter that deals
  // with cases where the mesh only needs to be tessellated to carry
  // out the conversion (e.g. Hexahedron_64 to Hexahedron).

  class Converter {

  public:

    Converter()
    {
    }

    virtual ~Converter()
    {
    }

    virtual shared_ptr<XdmfUnstructuredGrid>
    convert(const shared_ptr<XdmfUnstructuredGrid> gridToConvert,
            const shared_ptr<const XdmfTopologyType> topologyType,
            const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter) const = 0;

  protected:

    struct PointComparison {

      static double epsilon() { return 1e-6; };

      bool
      operator()(const std::vector<double> & point1,
                 const std::vector<double> & point2) const
      {
        for(unsigned int i=0; i<3; ++i) {
          if(fabs(point1[i] - point2[i]) > epsilon()) {
            return point1[i] < point2[i];
          }
        }
        return false;
      }
    };

    unsigned int
    insertPointWithoutCheck(const std::vector<double> & newPoint,
                            const shared_ptr<XdmfTopology> & newConnectivity,
                            const shared_ptr<XdmfGeometry> & newPoints) const
    {
      const unsigned int index = newPoints->getSize() / 3;
      newConnectivity->pushBack(index);
      newPoints->pushBack(newPoint[0]);
      newPoints->pushBack(newPoint[1]);
      newPoints->pushBack(newPoint[2]);
      return index;
    }

    unsigned int
    insertPointWithCheck(const std::vector<double> & newPoint,
                         std::map<std::vector<double>, unsigned int, PointComparison> & coordToIdMap,
                         const shared_ptr<XdmfTopology> & newConnectivity,
                         const shared_ptr<XdmfGeometry> & newPoints) const
    {
      std::map<std::vector<double>, unsigned int, PointComparison>::const_iterator iter =
        coordToIdMap.find(newPoint);
      if(iter == coordToIdMap.end()) {
        // Not inserted before
        coordToIdMap[newPoint] = newPoints->getSize() / 3;;
        return insertPointWithoutCheck(newPoint, newConnectivity, newPoints);
      }
      else {
        const unsigned int index = iter->second;
        newConnectivity->pushBack(index);
        return index;
      }
    }

  };

  class Tessellator : public Converter {

  public:

    virtual ~Tessellator()
    {
    }

    shared_ptr<XdmfUnstructuredGrid>
    convert(const shared_ptr<XdmfUnstructuredGrid> gridToConvert,
            const shared_ptr<const XdmfTopologyType> topologyType,
            const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter) const
    {
      shared_ptr<XdmfUnstructuredGrid> toReturn =
        XdmfUnstructuredGrid::New();
      toReturn->setName(gridToConvert->getName());
      toReturn->setGeometry(gridToConvert->getGeometry());

      if(heavyDataWriter) {
        if(!toReturn->getGeometry()->isInitialized()) {
          toReturn->getGeometry()->read();
        }
        toReturn->getGeometry()->accept(heavyDataWriter);
        toReturn->getGeometry()->release();
      }

      bool releaseTopology;
      if(!gridToConvert->getTopology()->isInitialized()) {
        gridToConvert->getTopology()->read();
        releaseTopology = true;
      }

      this->tesselateTopology(gridToConvert->getTopology(),
                              toReturn->getTopology());

      if(releaseTopology) {
        gridToConvert->getTopology()->release();
      }

      if(heavyDataWriter) {
        toReturn->getTopology()->accept(heavyDataWriter);
        toReturn->getTopology()->release();
      }

      for(unsigned int i=0; i<gridToConvert->getNumberAttributes(); ++i) {
        shared_ptr<XdmfAttribute> currAttribute =
          gridToConvert->getAttribute(i);
        shared_ptr<XdmfAttribute> createdAttribute =
          shared_ptr<XdmfAttribute>();
        if(currAttribute->getCenter() == XdmfAttributeCenter::Node()) {
          createdAttribute = currAttribute;
        }
        else if(currAttribute->getCenter() == XdmfAttributeCenter::Cell()) {
          bool releaseAttribute = false;
          if(!currAttribute->isInitialized()) {
            currAttribute->read();
            releaseAttribute = true;
          }

          createdAttribute = XdmfAttribute::New();
          createdAttribute->setName(currAttribute->getName());
          createdAttribute->setType(currAttribute->getType());
          createdAttribute->setCenter(currAttribute->getCenter());
          createdAttribute->initialize(currAttribute->getArrayType(),
                                       currAttribute->getSize() * mNumTesselations);
          for(unsigned int j=0; j<currAttribute->getSize(); ++j) {
            createdAttribute->insert(j * mNumTesselations,
                                     currAttribute,
                                     j,
                                     mNumTesselations,
                                     1,
                                     0);
          }

          if(releaseAttribute) {
            currAttribute->release();
          }
        }
        if(createdAttribute) {
          toReturn->insert(createdAttribute);
          if(heavyDataWriter) {
            if(!createdAttribute->isInitialized()) {
              createdAttribute->read();
            }
            createdAttribute->accept(heavyDataWriter);
            createdAttribute->release();
          }
        }
      }
      return toReturn;
    }

    virtual void
    tesselateTopology(shared_ptr<XdmfTopology> topologyToConvert,
                      shared_ptr<XdmfTopology> topologyToReturn) const = 0;

  protected:

    Tessellator(const unsigned int numTesselations) :
      mNumTesselations(numTesselations)
    {
    }

    const unsigned int mNumTesselations;

  };

  class HexahedronToHexahedron27 : public Converter {

  public:

    HexahedronToHexahedron27()
    {
    }

    virtual ~HexahedronToHexahedron27()
    {
    }

    void
    calculateMidPoint(std::vector<double> & result,
		      const std::vector<double> & point1,
		      const std::vector<double> & point2) const
    {
      for (int i=0; i<3; i++)
        result[i] = (point1[i]+point2[i]) / 2.0;
    }

    shared_ptr<XdmfUnstructuredGrid>
    convert(const shared_ptr<XdmfUnstructuredGrid> gridToConvert,
            const shared_ptr<const XdmfTopologyType> topologyType,
            const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter) const
    {

      shared_ptr<XdmfUnstructuredGrid> toReturn = XdmfUnstructuredGrid::New();
      toReturn->setName(gridToConvert->getName());

      shared_ptr<XdmfGeometry> geometry = gridToConvert->getGeometry();
      shared_ptr<XdmfGeometry> toReturnGeometry = toReturn->getGeometry();

      toReturnGeometry->setType(geometry->getType());
      toReturnGeometry->initialize(geometry->getArrayType());

      bool releaseGeometry = false;
      if(!geometry->isInitialized()) {
	geometry->read();
	releaseGeometry = true;
      }

      shared_ptr<XdmfTopology> topology = gridToConvert->getTopology();
      shared_ptr<XdmfTopology> toReturnTopology = toReturn->getTopology();

      toReturn->getTopology()->setType(topologyType);
      toReturnTopology->initialize(topology->getArrayType());
      toReturnTopology->reserve(topologyType->getNodesPerElement() *
				topology->getNumberElements());

      bool releaseTopology = false;
      if(!topology->isInitialized()) {
	topology->read();
	releaseTopology = true;
      }

      std::map<std::vector<double>, unsigned int, PointComparison> coordToIdMap;
      std::map<unsigned int, unsigned int> oldIdToNewId;

      // allocate storage for values used in loop
      unsigned int zeroIndex;
      unsigned int oneIndex;
      unsigned int twoIndex;
      unsigned int threeIndex;
      unsigned int fourIndex;
      unsigned int fiveIndex;
      unsigned int sixIndex;
      unsigned int sevenIndex;
      std::vector<double> elementCorner0(3);
      std::vector<double> elementCorner1(3);
      std::vector<double> elementCorner2(3);
      std::vector<double> elementCorner3(3);
      std::vector<double> elementCorner4(3);
      std::vector<double> elementCorner5(3);
      std::vector<double> elementCorner6(3);
      std::vector<double> elementCorner7(3);
      std::vector<double> newPoint(3);

      unsigned int offset = 0;
      unsigned int newIndex = 0;
      for(unsigned int elem = 0; elem<topology->getNumberElements(); ++elem) {

	// get indices of coner vertices of the element
	zeroIndex = topology->getValue<unsigned int>(offset++);
	oneIndex = topology->getValue<unsigned int>(offset++);
	twoIndex = topology->getValue<unsigned int>(offset++);
	threeIndex = topology->getValue<unsigned int>(offset++);
	fourIndex = topology->getValue<unsigned int>(offset++);
	fiveIndex = topology->getValue<unsigned int>(offset++);
	sixIndex = topology->getValue<unsigned int>(offset++);
	sevenIndex = topology->getValue<unsigned int>(offset++);

	// get locations of corner vertices of the element
	geometry->getValues(zeroIndex * 3,
			    &(elementCorner0[0]),
			    3);
	geometry->getValues(oneIndex * 3,
			    &(elementCorner1[0]),
			    3);
	geometry->getValues(twoIndex * 3,
			    &(elementCorner2[0]),
			    3);
	geometry->getValues(threeIndex * 3,
			    &(elementCorner3[0]),
			    3);
	geometry->getValues(fourIndex * 3,
			    &(elementCorner4[0]),
			    3);
	geometry->getValues(fiveIndex * 3,
			    &(elementCorner5[0]),
			    3);
	geometry->getValues(sixIndex * 3,
			    &(elementCorner6[0]),
			    3);
	geometry->getValues(sevenIndex * 3,
			    &(elementCorner7[0]),
			    3);

	// insert corner points
	newIndex = this->insertPointWithCheck(elementCorner0,
					      coordToIdMap,
					      toReturnTopology,
					      toReturnGeometry);
	oldIdToNewId[zeroIndex] = newIndex;
	newIndex = this->insertPointWithCheck(elementCorner1,
					      coordToIdMap,
					      toReturnTopology,
					      toReturnGeometry);
	oldIdToNewId[oneIndex] = newIndex;
	newIndex = this->insertPointWithCheck(elementCorner2,
					      coordToIdMap,
					      toReturnTopology,
					      toReturnGeometry);
	oldIdToNewId[twoIndex] = newIndex;
	newIndex = this->insertPointWithCheck(elementCorner3,
					      coordToIdMap,
					      toReturnTopology,
					      toReturnGeometry);
	oldIdToNewId[threeIndex] = newIndex;
	newIndex = this->insertPointWithCheck(elementCorner4,
					      coordToIdMap,
					      toReturnTopology,
					      toReturnGeometry);
	oldIdToNewId[fourIndex] = newIndex;
	newIndex = this->insertPointWithCheck(elementCorner5,
					      coordToIdMap,
					      toReturnTopology,
					      toReturnGeometry);
	oldIdToNewId[fiveIndex] = newIndex;
	newIndex = this->insertPointWithCheck(elementCorner6,
					      coordToIdMap,
					      toReturnTopology,
					      toReturnGeometry);
	oldIdToNewId[sixIndex] = newIndex;
	newIndex = this->insertPointWithCheck(elementCorner7,
					      coordToIdMap,
					      toReturnTopology,
					      toReturnGeometry);
	oldIdToNewId[sevenIndex] = newIndex;

	// insert additional points
	calculateMidPoint(newPoint,
			  elementCorner0,
			  elementCorner1);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner1,
			  elementCorner2);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner2,
			  elementCorner3);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner0,
			  elementCorner3);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner4,
			  elementCorner5);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner5,
			  elementCorner6);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner6,
			  elementCorner7);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner4,
			  elementCorner7);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner0,
			  elementCorner4);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner1,
			  elementCorner5);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner2,
			  elementCorner6);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner3,
			  elementCorner7);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner0,
			  elementCorner7);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner1,
			  elementCorner6);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner1,
			  elementCorner4);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner2,
			  elementCorner7);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner0,
			  elementCorner2);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner4,
			  elementCorner6);
	this->insertPointWithCheck(newPoint,
				   coordToIdMap,
				   toReturnTopology,
				   toReturnGeometry);
	calculateMidPoint(newPoint,
			  elementCorner0,
			  elementCorner6);
	this->insertPointWithoutCheck(newPoint,
				      toReturnTopology,
				      toReturnGeometry);
       
      }
 
      if(releaseTopology) {
	topology->release();
      }
      if(releaseGeometry) {
	geometry->release();
      }
       
      if(heavyDataWriter) {
	toReturnTopology->accept(heavyDataWriter);
	toReturnTopology->release();
	toReturnGeometry->accept(heavyDataWriter);
	toReturnGeometry->release();
      }

      handleSetConversion(gridToConvert,
			  toReturn,
			  oldIdToNewId,
			  heavyDataWriter);
       
      return toReturn;
      
    }
    
  };

  template <unsigned int ORDER, bool ISSPECTRAL>
  class HexahedronToHighOrderHexahedron : public Converter {

  public:

    HexahedronToHighOrderHexahedron()
    {
    }

    virtual ~HexahedronToHighOrderHexahedron()
    {
    }

    void
    calculateIntermediatePoint(std::vector<double> & result,
                               const std::vector<double> & point1,
                               const std::vector<double> & point2,
                               int index,
                               bool spectral) const
    {
      const double scalar = points[index];
      for (int i=0; i<3; i++)
        result[i] = point1[i]+scalar*(point2[i]-point1[i]);
    }

    shared_ptr<XdmfUnstructuredGrid>
    convert(const shared_ptr<XdmfUnstructuredGrid> gridToConvert,
            const shared_ptr<const XdmfTopologyType> topologyType,
            const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter) const
    {

      shared_ptr<XdmfUnstructuredGrid> toReturn = XdmfUnstructuredGrid::New();
      toReturn->setName(gridToConvert->getName());

      shared_ptr<XdmfGeometry> geometry = gridToConvert->getGeometry();
      shared_ptr<XdmfGeometry> toReturnGeometry = toReturn->getGeometry();

      toReturnGeometry->setType(geometry->getType());
      toReturnGeometry->initialize(geometry->getArrayType());

      bool releaseGeometry = false;
      if(!geometry->isInitialized()) {
        geometry->read();
        releaseGeometry = true;
      }

      shared_ptr<XdmfTopology> topology = gridToConvert->getTopology();
      shared_ptr<XdmfTopology> toReturnTopology = toReturn->getTopology();

      toReturn->getTopology()->setType(topologyType);
      toReturnTopology->initialize(topology->getArrayType());
      toReturnTopology->reserve(topologyType->getNodesPerElement() *
                                topology->getNumberElements());

      bool releaseTopology = false;
      if(!topology->isInitialized()) {
        topology->read();
        releaseTopology = true;
      }

      std::map<std::vector<double>, unsigned int, PointComparison> coordToIdMap;
      std::map<unsigned int, unsigned int> oldIdToNewId;

      // allocate storage for values used in loop
      unsigned int zeroIndex;
      unsigned int oneIndex;
      unsigned int twoIndex;
      unsigned int threeIndex;
      unsigned int fourIndex;
      unsigned int fiveIndex;
      unsigned int sixIndex;
      unsigned int sevenIndex;
      std::vector<double> elementCorner0(3);
      std::vector<double> elementCorner1(3);
      std::vector<double> elementCorner2(3);
      std::vector<double> elementCorner3(3);
      std::vector<double> elementCorner4(3);
      std::vector<double> elementCorner5(3);
      std::vector<double> elementCorner6(3);
      std::vector<double> elementCorner7(3);
      std::vector<double> planeCorner0(3);
      std::vector<double> planeCorner1(3);
      std::vector<double> planeCorner2(3);
      std::vector<double> planeCorner3(3);
      std::vector<double> lineEndPoint0(3);
      std::vector<double> lineEndPoint1(3);
      std::vector<double> point(3);

      unsigned int offset = 0;
      for(unsigned int elem = 0; elem<topology->getNumberElements(); ++elem) {

        // get indices of coner vertices of the element
        zeroIndex = topology->getValue<unsigned int>(offset++);
        oneIndex = topology->getValue<unsigned int>(offset++);
        twoIndex = topology->getValue<unsigned int>(offset++);
        threeIndex = topology->getValue<unsigned int>(offset++);
        fourIndex = topology->getValue<unsigned int>(offset++);
        fiveIndex = topology->getValue<unsigned int>(offset++);
        sixIndex = topology->getValue<unsigned int>(offset++);
        sevenIndex = topology->getValue<unsigned int>(offset++);

        // get locations of corner vertices of the element
        geometry->getValues(zeroIndex * 3,
                            &(elementCorner0[0]),
                            3);
        geometry->getValues(oneIndex * 3,
                            &(elementCorner1[0]),
                            3);
        geometry->getValues(twoIndex * 3,
                            &(elementCorner2[0]),
                            3);
        geometry->getValues(threeIndex * 3,
                            &(elementCorner3[0]),
                            3);
        geometry->getValues(fourIndex * 3,
                            &(elementCorner4[0]),
                            3);
        geometry->getValues(fiveIndex * 3,
                            &(elementCorner5[0]),
                            3);
        geometry->getValues(sixIndex * 3,
                            &(elementCorner6[0]),
                            3);
        geometry->getValues(sevenIndex * 3,
                            &(elementCorner7[0]),
                            3);

        // loop over i, j, k directions of element isolation i, j, and
        // k planes
        for(unsigned int i=0; i<mNodesPerEdge; ++i){
          // calculate corners of i plane
          calculateIntermediatePoint(planeCorner0,
                                     elementCorner0,
                                     elementCorner1,
                                     i,
                                     true);
          calculateIntermediatePoint(planeCorner1,
                                     elementCorner4,
                                     elementCorner5,
                                     i,
                                     true);
          calculateIntermediatePoint(planeCorner2,
                                     elementCorner3,
                                     elementCorner2,
                                     i,
                                     true);
          calculateIntermediatePoint(planeCorner3,
                                     elementCorner7,
                                     elementCorner6,
                                     i,
                                     true);

          for(unsigned int j=0; j<mNodesPerEdge; ++j) {
            // calculate endpoints of j slice of i plane
            calculateIntermediatePoint(lineEndPoint0,
                                       planeCorner0,
                                       planeCorner2,
                                       j,
                                       true);
            calculateIntermediatePoint(lineEndPoint1,
                                       planeCorner1,
                                       planeCorner3,
                                       j,
                                       true);

            for(unsigned int k=0; k<mNodesPerEdge; ++k) {
              // calculate point to add to mesh
              calculateIntermediatePoint(point,
                                         lineEndPoint0,
                                         lineEndPoint1,
                                         k,
                                         true);
              if((i == 0 || i == ORDER) ||
                 (j == 0 || j == ORDER) ||
                 (k == 0 || k == ORDER)) {
                const unsigned int newIndex = 
                  this->insertPointWithCheck(point,
                                             coordToIdMap,
                                             toReturnTopology,
                                             toReturnGeometry);
                if((i == 0 || i == ORDER) &&
                   (j == 0 || j == ORDER) &&
                   (k == 0 || k == ORDER)) {
                  if(i == 0) {
                    if(j == 0) {
                      if(k == 0) {
                        oldIdToNewId[zeroIndex] = newIndex;
                      }
                      else {
                        oldIdToNewId[fourIndex] = newIndex;
                      }
                    }
                    else if(k == 0) {
                      oldIdToNewId[threeIndex] = newIndex;
                    }
                    else {
                      oldIdToNewId[sevenIndex] = newIndex;
                    }
                  }
                  else {
                    if(j == 0) {
                      if(k == 0) {
                        oldIdToNewId[oneIndex] = newIndex;
                      }
                      else {
                        oldIdToNewId[fiveIndex] = newIndex;
                      }
                    }
                    else if(k == 0) {
                      oldIdToNewId[twoIndex] = newIndex;
                    }
                    else {
                      oldIdToNewId[sixIndex] = newIndex;
                    }
                  }
                }
              }
              else {
                this->insertPointWithoutCheck(point,
                                              toReturnTopology,
                                              toReturnGeometry);
              }
            }
          }
        }
      }

      if(releaseTopology) {
        topology->release();
      }
      if(releaseGeometry) {
        geometry->release();
      }

      if(heavyDataWriter) {
        toReturnTopology->accept(heavyDataWriter);
        toReturnTopology->release();
        toReturnGeometry->accept(heavyDataWriter);
        toReturnGeometry->release();
      }

      handleSetConversion(gridToConvert,
			  toReturn,
			  oldIdToNewId,
			  heavyDataWriter);

      return toReturn;
    }

  private:
    static const unsigned int mNodesPerEdge = ORDER + 1;
    static const double points[];

  };

  template <>
  const double HexahedronToHighOrderHexahedron<3, true>::points[] = {
    0.0,
    0.5-0.1*sqrt(5.0),
    0.5+0.1*sqrt(5.0),
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<4, true>::points[] = {
    0.0,
    0.5-sqrt(21.0)/14.0,
    0.5,
    0.5+sqrt(21.0)/14.0,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<5, true>::points[] = {
    0.0,
    0.5-sqrt((7.0+2.0*sqrt(7.0))/84.0),
    0.5-sqrt((7.0-2.0*sqrt(7.0))/84.0),
    0.5+sqrt((7.0-2.0*sqrt(7.0))/84.0),
    0.5+sqrt((7.0+2.0*sqrt(7.0))/84.0),
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<6, true>::points[] = {
    0.0,
    0.5-sqrt((15.0+2.0*sqrt(15.0))/132.0),
    0.5-sqrt((15.0-2.0*sqrt(15.0))/132.0),
    0.5,
    0.5+sqrt((15.0-2.0*sqrt(15.0))/132.0),
    0.5+sqrt((15.0+2.0*sqrt(15.0))/132.0),
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<7, true>::points[] = {
    0.0,
    0.064129925745196714,
    0.20414990928342885,
    0.39535039104876057,
    0.60464960895123943,
    0.79585009071657109,
    0.93587007425480329,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<8, true>::points[] = {
    0.0,
    0.050121002294269912,
    0.16140686024463108,
    0.31844126808691087,
    0.5,
    0.68155873191308913,
    0.83859313975536898,
    0.94987899770573003,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<9, true>::points[] = {
    0.0,
    0.040233045916770627,
    0.13061306744724743,
    0.26103752509477773,
    0.4173605211668065,
    0.58263947883319345,
    0.73896247490522227,
    0.86938693255275257,
    0.95976695408322943,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<10, true>::points[] = {
    0.0,
    0.032999284795970474,
    0.10775826316842779,
    0.21738233650189748,
    0.35212093220653029,
    0.5,
    0.64787906779346971,
    0.78261766349810258,
    0.89224173683157226,
    0.96700071520402953,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<3, false>::points[] = {
    0.0,
    1.0/3.0,
    2.0/3.0,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<4, false>::points[] = {
    0.0,
    0.25,
    0.5,
    0.75,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<5, false>::points[] = {
    0.0,
    0.2,
    0.4,
    0.6,
    0.8,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<6, false>::points[] = {
    0.0,
    1.0/6.0,
    1.0/3.0,
    0.5,
    2.0/3.0,
    5.0/6.0,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<7, false>::points[] = {
    0.0,
    1.0/7.0,
    2.0/7.0,
    3.0/7.0,
    4.0/7.0,
    5.0/7.0,
    6.0/7.0,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<8, false>::points[] = {
    0.0,
    0.125,
    0.25,
    0.375,
    0.5,
    0.625,
    0.75,
    0.875,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<9, false>::points[] = {
    0.0,
    1.0/9.0,
    2.0/9.0,
    1.0/3.0,
    4.0/9.0,
    5.0/9.0,
    2.0/3.0,
    7.0/9.0,
    8.0/9.0,
    1.0
  };

  template <>
  const double HexahedronToHighOrderHexahedron<10, false>::points[] = {
    0.0,
    0.1,
    0.2,
    0.3,
    0.4,
    0.5,
    0.6,
    0.7,
    0.8,
    0.9,
    1.0
  };

  template <unsigned int ORDER>
  class HighOrderHexahedronToHexahedron : public Tessellator {

  public:

    HighOrderHexahedronToHexahedron() :
      Tessellator(ORDER * ORDER * ORDER)
    {
    }

    void
    tesselateTopology(shared_ptr<XdmfTopology> topologyToConvert,
                      shared_ptr<XdmfTopology> topologyToReturn) const
    {
      topologyToReturn->setType(XdmfTopologyType::Hexahedron());
      topologyToReturn->initialize(topologyToConvert->getArrayType(),
                                   8 * ORDER * ORDER * ORDER * topologyToConvert->getNumberElements());

      unsigned int newIndex = 0;
      int indexA = 0;
      int indexB = mNodesPerEdge * mNodesPerEdge;
      int indexC = mNodesPerEdge * mNodesPerEdge + mNodesPerEdge;
      int indexD = mNodesPerEdge;
      for(unsigned int i=0; i<topologyToConvert->getNumberElements(); ++i) {
        for(unsigned int j=0; j<ORDER; ++j) {
          for(unsigned int k=0; k<ORDER; ++k) {
            for(unsigned int l=0; l<ORDER; ++l){
              topologyToReturn->insert(newIndex++, topologyToConvert, indexA++);
              topologyToReturn->insert(newIndex++, topologyToConvert, indexB++);
              topologyToReturn->insert(newIndex++, topologyToConvert, indexC++);
              topologyToReturn->insert(newIndex++, topologyToConvert, indexD++);
              topologyToReturn->insert(newIndex++, topologyToConvert, indexA);
              topologyToReturn->insert(newIndex++, topologyToConvert, indexB);
              topologyToReturn->insert(newIndex++, topologyToConvert, indexC);
              topologyToReturn->insert(newIndex++, topologyToConvert, indexD);
            }
            indexA++;
            indexB++;
            indexC++;
            indexD++;
          }
          indexA += mNodesPerEdge;
          indexB += mNodesPerEdge;
          indexC += mNodesPerEdge;
          indexD += mNodesPerEdge;
        }
        indexA += mNodesPerEdge * mNodesPerEdge;
        indexB += mNodesPerEdge * mNodesPerEdge;
        indexC += mNodesPerEdge * mNodesPerEdge;
        indexD += mNodesPerEdge * mNodesPerEdge;
      }
    }

  private:
    static const unsigned int mNodesPerEdge = (ORDER + 1);

  };

}

shared_ptr<XdmfTopologyConverter>
XdmfTopologyConverter::New()
{
  shared_ptr<XdmfTopologyConverter> p(new XdmfTopologyConverter());
  return p;
}

XdmfTopologyConverter::XdmfTopologyConverter()
{
}

XdmfTopologyConverter::~XdmfTopologyConverter()
{
}

shared_ptr<XdmfUnstructuredGrid>
XdmfTopologyConverter::convert(const shared_ptr<XdmfUnstructuredGrid> gridToConvert,
                               const shared_ptr<const XdmfTopologyType> topologyType,
                               unsigned int options,
                               const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter) const
{
  // Make sure geometry and topology are non null
  if(!(gridToConvert->getGeometry() && gridToConvert->getTopology()))
    XdmfError::message(XdmfError::FATAL,
                       "Current grid's geometry or topology is null "
                       "in XdmfTopologyConverter::convert");

  shared_ptr<const XdmfTopologyType> topologyTypeToConvert =
    gridToConvert->getTopology()->getType();
  if(topologyTypeToConvert == topologyType) {
    // No conversion necessary
    return gridToConvert;
  }

  if(gridToConvert->getGeometry()->getType() != XdmfGeometryType::XYZ()) {
    XdmfError::message(XdmfError::FATAL,
                       "Grid to convert's type is not 'XYZ' in "
                       "XdmfTopologyConverter::convert");
  }

  Converter * converter = NULL;
  if(topologyTypeToConvert == XdmfTopologyType::Hexahedron()) {
    if(topologyType == XdmfTopologyType::Hexahedron_27()) {
      converter = new HexahedronToHexahedron27();
    }
    if(topologyType == XdmfTopologyType::Hexahedron_64()) {
      if(options == 1) {
        converter = new HexahedronToHighOrderHexahedron<3, true>();
      }
      else {
        converter = new HexahedronToHighOrderHexahedron<3, false>();
      }
    }
    else if(topologyType == XdmfTopologyType::Hexahedron_125()) {
      if(options == 1) {
        converter = new HexahedronToHighOrderHexahedron<4, true>();
      }
      else {
        converter = new HexahedronToHighOrderHexahedron<4, false>();
      }
    }
    else if(topologyType == XdmfTopologyType::Hexahedron_216()) {
      if(options == 1) {
        converter = new HexahedronToHighOrderHexahedron<5, true>();
      }
      else {
        converter = new HexahedronToHighOrderHexahedron<5, false>();
      }
    }
    else if(topologyType == XdmfTopologyType::Hexahedron_343()) {
      if(options == 1) {
        converter = new HexahedronToHighOrderHexahedron<6, true>();
      }
      else {
        converter = new HexahedronToHighOrderHexahedron<6, false>();
      }
    }
    else if(topologyType == XdmfTopologyType::Hexahedron_512()) {
      if(options == 1) {
        converter = new HexahedronToHighOrderHexahedron<7, true>();
      }
      else {
        converter = new HexahedronToHighOrderHexahedron<7, false>();
      }
    }
    else if(topologyType == XdmfTopologyType::Hexahedron_729()) {
      if(options == 1) {
        converter = new HexahedronToHighOrderHexahedron<8, true>();
      }
      else {
        converter = new HexahedronToHighOrderHexahedron<8, false>();
      }
    }
    else if(topologyType == XdmfTopologyType::Hexahedron_1000()) {
      if(options == 1) {
        converter = new HexahedronToHighOrderHexahedron<9, true>();
      }
      else {
        converter = new HexahedronToHighOrderHexahedron<9, false>();
      }
    }
    else if(topologyType == XdmfTopologyType::Hexahedron_1331()) {
      if(options == 1) {
        converter = new HexahedronToHighOrderHexahedron<10, true>();
      }
      else {
        converter = new HexahedronToHighOrderHexahedron<10, false>();
      }
    }
  }
  else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_64()) {
    if(topologyType == XdmfTopologyType::Hexahedron()) {
      converter = new HighOrderHexahedronToHexahedron<3>();
    }
  }
  else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_125()) {
    if(topologyType == XdmfTopologyType::Hexahedron()) {
      converter = new HighOrderHexahedronToHexahedron<4>();
    }
  }
  else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_216()) {
    if(topologyType == XdmfTopologyType::Hexahedron()) {
      converter = new HighOrderHexahedronToHexahedron<5>();
    }
  }
  else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_343()) {
    if(topologyType == XdmfTopologyType::Hexahedron()) {
      converter = new HighOrderHexahedronToHexahedron<6>();
    }
  }
  else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_512()) {
    if(topologyType == XdmfTopologyType::Hexahedron()) {
      converter = new HighOrderHexahedronToHexahedron<7>();
    }
  }
  else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_729()) {
    if(topologyType == XdmfTopologyType::Hexahedron()) {
      converter = new HighOrderHexahedronToHexahedron<8>();
    }
  }
  else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_1000()) {
    if(topologyType == XdmfTopologyType::Hexahedron()) {
      converter = new HighOrderHexahedronToHexahedron<9>();
    }
  }
  else if(topologyTypeToConvert == XdmfTopologyType::Hexahedron_1331()) {
    if(topologyType == XdmfTopologyType::Hexahedron()) {
      converter = new HighOrderHexahedronToHexahedron<10>();
    }
  }

  if(converter) {
    if(heavyDataWriter) {
      heavyDataWriter->openFile();
    }

    shared_ptr<XdmfUnstructuredGrid> toReturn =
      converter->convert(gridToConvert,
                         topologyType,
                         heavyDataWriter);

    if(heavyDataWriter) {
      heavyDataWriter->closeFile();
    }

    delete converter;
    return toReturn;
  }
  else {
    XdmfError::message(XdmfError::FATAL,
                       "Cannot convert topology type in "
                       "XdmfTopologyConverter::convert");
  }

  // not reached
  return shared_ptr<XdmfUnstructuredGrid>();

}
