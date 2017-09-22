#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfSubset.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include <map>
#include <iostream>

int main(int, char **)
{
  shared_ptr<XdmfArray> referenceArray = XdmfArray::New();

  for (unsigned int i = 0; i < 27; ++i){
    referenceArray->pushBack(i);
  }

  std::vector<unsigned int> initDimensions;
  initDimensions.push_back(3);
  initDimensions.push_back(3);
  initDimensions.push_back(3);

  referenceArray->resize(initDimensions, 0);

  std::vector<unsigned int> newStarts;
  newStarts.push_back(0);
  newStarts.push_back(0);
  newStarts.push_back(0);
  std::vector<unsigned int> newStrides;
  newStrides.push_back(2);
  newStrides.push_back(2);
  newStrides.push_back(2);
  std::vector<unsigned int> newDimensions;
  newDimensions.push_back(2);
  newDimensions.push_back(2);
  newDimensions.push_back(2);

  shared_ptr<XdmfSubset> testSubset = XdmfSubset::New(referenceArray,
                                                      newStarts,
                                                      newStrides,
                                                      newDimensions);

  shared_ptr<XdmfAttribute> subsetHolder = XdmfAttribute::New();
  subsetHolder->setReference(testSubset);
  subsetHolder->setReadMode(XdmfArray::Reference);

  shared_ptr<XdmfWriter> subsetWriter = XdmfWriter::New("subset.xmf");
  subsetHolder->accept(subsetWriter);

  subsetHolder->readReference();
  std::cout << subsetHolder->getValuesString() << std::endl;

  shared_ptr<XdmfReader> subsetReader = XdmfReader::New();
  shared_ptr<XdmfItem> readItem = subsetReader->read("subset.xmf");

  std::cout << readItem->getItemTag() << " ?= Attribute" << std::endl;
  assert(readItem->getItemTag().compare("Attribute") == 0);

  shared_ptr<XdmfAttribute> readSubsetHolder = 
    shared_dynamic_cast<XdmfAttribute>(readItem);

  readSubsetHolder->readReference();

  std::cout << shared_dynamic_cast<XdmfSubset>(readSubsetHolder->getReference())->getReferenceArray()->getValuesString() << std::endl;

  std::cout << readSubsetHolder->getValuesString() << std::endl;

  assert(readSubsetHolder->getValuesString().compare(subsetHolder->getValuesString()) == 0);

  shared_ptr<XdmfArray> referenceArray2 = XdmfArray::New();

  for (unsigned int i = 10; i < 37; ++i) {
    referenceArray2->pushBack(i);
  }

  referenceArray2->resize(initDimensions, 0);

  testSubset->setReferenceArray(referenceArray2);

  std::string changedSubsetOutput = testSubset->read()->getValuesString();

  std::cout << "after changing reference Array: " << changedSubsetOutput 
	    << std::endl;

  assert(changedSubsetOutput.compare("10 12 16 18 28 30 34 36") == 0);

  shared_ptr<XdmfArray> subsetHolder2 = XdmfArray::New();
  subsetHolder2->setReference(testSubset);
  subsetHolder2->setReadMode(XdmfArray::Reference);
  subsetHolder2->accept(subsetWriter);
  subsetHolder2->readReference();

  std::cout << subsetHolder2->getValuesString() << std::endl;

  readItem = subsetReader->read("subset.xmf");

  std::cout << readItem->getItemTag() << " ?= DataItem" << std::endl;

  assert(readItem->getItemTag().compare("DataItem") == 0);

  shared_ptr<XdmfArray> readSubsetHolder2 = 
    shared_dynamic_cast<XdmfArray>(readItem);
  readSubsetHolder2->readReference();
  std::cout <<
    shared_dynamic_cast<XdmfSubset>(readSubsetHolder2->getReference())->getReferenceArray()->getValuesString()
            << std::endl;

  std::cout << readSubsetHolder2->getValuesString() << std::endl;

  assert(readSubsetHolder2->getValuesString().compare(subsetHolder2->getValuesString()) == 0);

  return 0;
}
