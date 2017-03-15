/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfDiff.cpp                                                        */
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
#include <iomanip>
#include <iostream>
#include <assert.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfDiff.hpp"
#include "XdmfError.hpp"
#include "XdmfVisitor.hpp"

//
// local methods
//
namespace {

  /**
   * @brief Internal class that traverses all items in xdmf hierarchy and
   * adds them to mItems.
   */
  class XdmfDiffVisitor : public XdmfVisitor {

  public:

    static shared_ptr<XdmfDiffVisitor>
    New()
    {
      shared_ptr<XdmfDiffVisitor> p(new XdmfDiffVisitor());
      return p;
    };

    ~XdmfDiffVisitor()
    {
    };

    void
    visit(XdmfItem & item,
          const shared_ptr<XdmfBaseVisitor> visitor)
    {
      mItems.push_back(&item);
      item.traverse(visitor);
    }

    std::vector<XdmfItem *> mItems;

  protected:

    XdmfDiffVisitor()
    {
    }

  };

  template <typename T>
  void 
  diffArrays(XdmfArray * array1,
             XdmfArray * array2,
             const double absoluteTolerance,
             bool & returnValue) {
    bool releaseArray1 = false;
    if(!array1->isInitialized()) {
      array1->read();
      releaseArray1 = true;
    }
    bool releaseArray2 = false;
    if(!array2->isInitialized()) {
      array2->read();
      releaseArray2 = true;
    }

    const unsigned int size = array1->getSize();
    for(unsigned int i=0; i<size; ++i) {
      const T array1Value = array1->getValue<T>(i);
      const T array2Value = array2->getValue<T>(i);
      const T difference =
        static_cast<T>(array1Value > array2Value ?
                         array1Value-array2Value : array2Value-array1Value);

      if(difference > absoluteTolerance) {
        std::cout << "At Index " << i << " | Expected: "
                  << array1Value << " | Got: " << array2Value << std::endl;
        returnValue = false;
      }
    }

    if(releaseArray1) {
      array1->release();
    }
    if(releaseArray2) {
      array2->release();
    }
  }

}

shared_ptr<XdmfDiff>
XdmfDiff::New()
{
  shared_ptr<XdmfDiff> p(new XdmfDiff());
  return p;
}

XdmfDiff::XdmfDiff() :
  mAbsoluteTolerance(0.0)
{
}

XdmfDiff::XdmfDiff(const XdmfDiff &diffRef) :
  mAbsoluteTolerance(diffRef.mAbsoluteTolerance)
{
}

XdmfDiff::~XdmfDiff()
{
}

bool
XdmfDiff::compare(const shared_ptr<XdmfItem> item1,
                  const shared_ptr<XdmfItem> item2) const
{

  bool returnValue = true;

  shared_ptr<XdmfDiffVisitor> diffVisitor1 = XdmfDiffVisitor::New();
  shared_ptr<XdmfDiffVisitor> diffVisitor2 = XdmfDiffVisitor::New();

  item1->accept(diffVisitor1);
  item2->accept(diffVisitor2);

  assert(diffVisitor1->mItems.size() == diffVisitor2->mItems.size());

  for(std::vector<XdmfItem *>::size_type i=0;
      i<diffVisitor1->mItems.size();
      ++i) {

    XdmfItem * item1 = diffVisitor1->mItems[i];
    XdmfItem * item2 = diffVisitor2->mItems[i];

    // compare item tags
    const std::string itemTag1 = item1->getItemTag();
    const std::string itemTag2 = item2->getItemTag();

    if(itemTag1.compare(itemTag2) != 0) {
      std::cout << "Error: Expected " << itemTag1 << " and got " << itemTag2
                << ". The files are not structured the same" << std::endl;
      return false;
    }

    // compare item properties
    const std::map<std::string, std::string> itemProperties1 =
      item1->getItemProperties();
    const std::map<std::string, std::string> itemProperties2 =
      item2->getItemProperties();

    assert(itemProperties1.size() == itemProperties2.size());

    for(std::map<std::string, std::string>::const_iterator iter1 =
          itemProperties1.begin();
        iter1 != itemProperties1.end();
        ++iter1) {
      const std::string & key = iter1->first;
      const std::map<std::string, std::string>::const_iterator iter2 =
        itemProperties2.find(key);
      if(iter2 == itemProperties2.end()) {
        std::cout << "Error: Expected " << key << " and did not find corresponding key "
                  << "files are not structured the same" << std::endl;
        assert(false);
      }
      const std::string & value1 = iter1->second;
      const std::string & value2 = iter2->second;
      if(value1.compare(value2) != 0) {
        std::cout << "Error: For: " << itemTag1 << " | Key: " << key
                  << "Expected: " << value1 << " | Got: " << value2
                  << std::endl;
        returnValue = false;
      }
    }

    // compare array values
    XdmfArray * array1 = dynamic_cast<XdmfArray *>(item1);

    if(array1) {
      XdmfArray * array2 = dynamic_cast<XdmfArray *>(item2);
      if(array1->getSize() != array2->getSize()) {
        std::cout << "Error: For: " << itemTag1
                  << " Different number of values stored. Expected: "
                  << array1->getSize() << " | Got: " << array2->getSize()
                  << std::endl;
        returnValue = false;
      }
      const shared_ptr<const XdmfArrayType> arrayType =
        array1->getArrayType();
      if(arrayType != array2->getArrayType()) {
        std::cout << "Error: For: " << itemTag1
                  << " Different type of values stored. Expected: "
                  << arrayType->getName() << " | Got: "
                  << array2->getArrayType()->getName() << std::endl;
        returnValue = false;
      }
      if(arrayType == XdmfArrayType::Int8()) {
        diffArrays<char>(array1, 
                         array2, 
                         mAbsoluteTolerance, 
                         returnValue);
      }
      else if(arrayType == XdmfArrayType::Int16()) {
        diffArrays<short>(array1, 
                          array2, 
                          mAbsoluteTolerance,
                          returnValue);
      }
      else if(arrayType == XdmfArrayType::Int32()) {
        diffArrays<int>(array1, 
                        array2, 
                        mAbsoluteTolerance,
                        returnValue);
      }
      else if(arrayType == XdmfArrayType::Int64()) {
        diffArrays<long>(array1, 
                         array2, 
                         mAbsoluteTolerance, 
                         returnValue);
      }
      else if(arrayType == XdmfArrayType::UInt8()) {
        diffArrays<unsigned char>(array1, 
                                  array2, 
                                  mAbsoluteTolerance,
                                  returnValue);
      }
      else if(arrayType == XdmfArrayType::UInt16()) {
        diffArrays<unsigned short>(array1, 
                                   array2, 
                                   mAbsoluteTolerance,
                                   returnValue);
      }
      else if(arrayType == XdmfArrayType::UInt32()) {
        diffArrays<unsigned int>(array1, 
                                 array2, 
                                 mAbsoluteTolerance,
                                 returnValue);
      }
      else if(arrayType == XdmfArrayType::Float32()) {
        diffArrays<float>(array1, 
                          array2, 
                          mAbsoluteTolerance,
                          returnValue);
      }
      else if(arrayType == XdmfArrayType::Float64()) {
        std::cout << std::setprecision(15);
        diffArrays<double>(array1, 
                           array2, 
                           mAbsoluteTolerance,
                           returnValue);
      }
      else {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Array Types match, but type is not supported.");
      }
    }
  }
  return returnValue;
}

double
XdmfDiff::getAbsoluteTolerance() const
{
  return mAbsoluteTolerance;
}

void
XdmfDiff::setAbsoluteTolerance(const double absoluteTolerance)
{
  mAbsoluteTolerance = absoluteTolerance;
}

// C Wrappers

XDMFDIFF *
XdmfDiffNew()
{
  shared_ptr<XdmfDiff> generatedDiff = XdmfDiff::New();
  return (XDMFDIFF *)((void *)(new XdmfDiff(*generatedDiff.get())));
}

int
XdmfDiffCompare(XDMFDIFF * diff, XDMFITEM * item1, XDMFITEM * item2)
{
  shared_ptr<XdmfItem> tempItem1 = shared_ptr<XdmfItem>((XdmfItem *)item1, XdmfNullDeleter());
  shared_ptr<XdmfItem> tempItem2 = shared_ptr<XdmfItem>((XdmfItem *)item2, XdmfNullDeleter());
  return ((XdmfDiff *)diff)->compare(tempItem1, tempItem2);
}

double
XdmfDiffGetAbsoluteTolerance(XDMFDIFF * diff)
{
  return ((XdmfDiff *)diff)->getAbsoluteTolerance();
}

void
XdmfDiffSetAbsoluteTolerance(XDMFDIFF * diff, double tolerance)
{
  ((XdmfDiff *)diff)->setAbsoluteTolerance(tolerance);
}

void
XdmfDiffFree(XDMFDIFF * diff)
{
  if (diff != NULL) {
    delete ((XdmfDiff *)diff);
    diff = NULL;
  }
}
