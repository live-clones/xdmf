#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfGridTemplate.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include <iostream>

int main(int, char **)
{
  unsigned int arraySize = 12;
  unsigned int numSteps = 3;

  std::cout << "HDF5" << std::endl;

  shared_ptr<XdmfGridTemplate> temp = XdmfGridTemplate::New();

  shared_ptr<XdmfUnstructuredGrid> ungrid = XdmfUnstructuredGrid::New();

  shared_ptr<XdmfTopology> topo = XdmfTopology::New();

  topo->initialize(XdmfArrayType::Float64(), 12);

  shared_ptr<XdmfGeometry> geo = XdmfGeometry::New();

  geo->initialize(XdmfArrayType::Float64(), 36);

  ungrid->setTopology(topo);
  ungrid->setGeometry(geo);

  shared_ptr<XdmfAttribute> attr = XdmfAttribute::New();

  attr->initialize(XdmfArrayType::Float64(), 12);

  attr->release();

  shared_ptr<XdmfAttribute> attr2 = XdmfAttribute::New();

  attr2->initialize(XdmfArrayType::Float64(), 12);

  attr2->release();

  shared_ptr<XdmfAttribute> attr3 = XdmfAttribute::New();

  attr3->initialize(XdmfArrayType::Float64(), 12);

  attr3->release();

  ungrid->insert(attr);
  ungrid->insert(attr2);
  ungrid->insert(attr3);

  shared_ptr<XdmfTime> time = XdmfTime::New(0.0);

  ungrid->setTime(time);

  shared_ptr<XdmfWriter> writer = XdmfWriter::New("gridtemplate.xmf");

  shared_ptr<XdmfHeavyDataWriter> heavyWriter = writer->getHeavyDataWriter();

  temp->setHeavyDataWriter(heavyWriter);

  temp->setBase(ungrid);

  for (int iteration = 1; iteration <= numSteps; ++iteration)
  {
    time->setValue(10.0 * iteration);
    for (unsigned int i = 0; i < arraySize; ++i)
    {
      attr->insert<double>(i, 1.0 * iteration);
      attr2->insert<double>(i, 2.0 * iteration);
      attr3->insert<double>(i, 3.0 * iteration);
    }

    temp->addStep();

    temp->clearStep();
  }

  assert(temp->getNumberSteps() == numSteps);

  for (int iteration = 0; iteration < numSteps; ++iteration)
  {
    temp->setStep(iteration);

    attr->read();
    attr2->read();
    attr3->read();

    std::cout << "Time:" << time->getValue() << std::endl;

    assert(time->getValue() == 10.0 * (1 + iteration));

    std::cout << attr->getValuesString() << std::endl;
    std::cout << attr2->getValuesString() << std::endl;
    std::cout << attr3->getValuesString() << std::endl;


    for (unsigned int i = 0; i < arraySize; ++i)
    {
      assert(attr->getValue<double>(i) ==  1.0 * (1 + iteration));
      assert(attr2->getValue<double>(i) == 2.0 * (1 + iteration));
      assert(attr3->getValue<double>(i) == 3.0 * (1 + iteration));
    }

    temp->clearStep();
  }

  temp->accept(writer);

  shared_ptr<XdmfReader> reader = XdmfReader::New();

  shared_ptr<XdmfGridTemplate> readTemp = shared_dynamic_cast<XdmfGridTemplate>(reader->read("gridtemplate.xmf"));

  assert(readTemp);

  shared_ptr<XdmfUnstructuredGrid> readungrid = shared_dynamic_cast<XdmfUnstructuredGrid>(readTemp->getBase());

  std::cout << "?" << readungrid << std::endl;

  assert(readungrid);

  shared_ptr<XdmfTime> readTime = readungrid->getTime();

  shared_ptr<XdmfAttribute> readAttr = readungrid->getAttribute(0);
  shared_ptr<XdmfAttribute> readAttr2 = readungrid->getAttribute(1);
  shared_ptr<XdmfAttribute> readAttr3 = readungrid->getAttribute(2);

  std::cout << readTemp->getNumberSteps() << " ?= " << numSteps << std::endl;

  assert(readTemp->getNumberSteps() == numSteps);

  for (int iteration = 0; iteration < numSteps; ++iteration)
  {
    readTemp->setStep(iteration);

    readAttr->read();
    readAttr2->read();
    readAttr3->read();

    std::cout << "Time:" << readTime->getValue() << std::endl;

    assert(readTime->getValue() == 10.0 * (1 + iteration));

    std::cout << readAttr->getValuesString() << std::endl;
    std::cout << readAttr2->getValuesString() << std::endl;
    std::cout << readAttr3->getValuesString() << std::endl;

    for (unsigned int i = 0; i < arraySize; ++i)
    {
      assert(readAttr->getValue<double>(i) ==  1.0 * (1 + iteration));
      assert(readAttr2->getValue<double>(i) == 2.0 * (1 + iteration));
      assert(readAttr3->getValue<double>(i) == 3.0 * (1 + iteration));
    }

    readTemp->clearStep();
  }

  // Without writing to hdf5

  std::cout << "XML" << std::endl;

  shared_ptr<XdmfGridTemplate> temp2 = XdmfGridTemplate::New();

  shared_ptr<XdmfUnstructuredGrid> ungrid2 = XdmfUnstructuredGrid::New();

  shared_ptr<XdmfTopology> topo2 = XdmfTopology::New();

  topo2->initialize(XdmfArrayType::Float64(), 12);

  shared_ptr<XdmfGeometry> geo2 = XdmfGeometry::New();

  geo2->initialize(XdmfArrayType::Float64(), 36);

  ungrid2->setTopology(topo2);
  ungrid2->setGeometry(geo2);

  shared_ptr<XdmfAttribute> secondattr = XdmfAttribute::New();

  secondattr->initialize(XdmfArrayType::Float64(), 12);

  secondattr->release();

  shared_ptr<XdmfAttribute> secondattr2 = XdmfAttribute::New();

  secondattr2->initialize(XdmfArrayType::Float64(), 12);

  secondattr2->release();

  shared_ptr<XdmfAttribute> secondattr3 = XdmfAttribute::New();

  secondattr3->initialize(XdmfArrayType::Float64(), 12);

  secondattr3->release();

  ungrid2->insert(secondattr);
  ungrid2->insert(secondattr2);
  ungrid2->insert(secondattr3);

  shared_ptr<XdmfTime> time2 = XdmfTime::New(0.0);

  ungrid2->setTime(time2);

  shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("gridtemplate2.xmf");

  temp2->setBase(ungrid2);

  for (int iteration = 1; iteration <= numSteps; ++iteration)
  {
    time2->setValue(10.0 * iteration);
    for (unsigned int i = 0; i < arraySize; ++i)
    {
      secondattr->insert<double>(i, 1.0 * iteration);
      secondattr2->insert<double>(i, 2.0 * iteration);
      secondattr3->insert<double>(i, 3.0 * iteration);
    }

    temp2->addStep();

    temp2->clearStep();
  }

  assert(temp2->getNumberSteps() == numSteps);

  for (int iteration = 0; iteration < numSteps; ++iteration)
  {
    temp2->setStep(iteration);

    secondattr->read();
    secondattr2->read();
    secondattr3->read();

    std::cout << "Time:" << time2->getValue() << std::endl;

    assert(time2->getValue() == 10.0 * (1 + iteration));

    std::cout << secondattr->getValuesString() << std::endl;
    std::cout << secondattr2->getValuesString() << std::endl;
    std::cout << secondattr3->getValuesString() << std::endl;


    for (unsigned int i = 0; i < arraySize; ++i)
    {
      assert(secondattr->getValue<double>(i) ==  1.0 * (1 + iteration));
      assert(secondattr2->getValue<double>(i) == 2.0 * (1 + iteration));
      assert(secondattr3->getValue<double>(i) == 3.0 * (1 + iteration));
    }

    temp2->clearStep();
  }

  temp2->accept(writer2);

  shared_ptr<XdmfGridTemplate> readTemp2 = shared_dynamic_cast<XdmfGridTemplate>(reader->read("gridtemplate2.xmf"));

  assert(readTemp2);

  shared_ptr<XdmfUnstructuredGrid> readungrid2 = shared_dynamic_cast<XdmfUnstructuredGrid>(readTemp2->getBase());

  std::cout << "?" << readungrid2 << std::endl;

  assert(readungrid2);

  shared_ptr<XdmfTime> secondreadTime = readungrid2->getTime();

  shared_ptr<XdmfAttribute> secondreadAttr = readungrid2->getAttribute(0);
  shared_ptr<XdmfAttribute> secondreadAttr2 = readungrid2->getAttribute(1);
  shared_ptr<XdmfAttribute> secondreadAttr3 = readungrid2->getAttribute(2);

  std::cout << readTemp2->getNumberSteps() << " ?= " << numSteps << std::endl;

  assert(readTemp2->getNumberSteps() == numSteps);

  for (int iteration = 0; iteration < numSteps; ++iteration)
  {
    readTemp2->setStep(iteration);

    secondreadAttr->read();
    secondreadAttr2->read();
    secondreadAttr3->read();

    std::cout << "Time:" << secondreadTime->getValue() << std::endl;

    assert(secondreadTime->getValue() == 10.0 * (1 + iteration));

    std::cout << secondreadAttr->getValuesString() << std::endl;
    std::cout << secondreadAttr2->getValuesString() << std::endl;
    std::cout << secondreadAttr3->getValuesString() << std::endl;


    for (unsigned int i = 0; i < arraySize; ++i)
    {
      assert(secondreadAttr->getValue<double>(i) ==  1.0 * (1 + iteration));
      assert(secondreadAttr2->getValue<double>(i) == 2.0 * (1 + iteration));
      assert(secondreadAttr3->getValue<double>(i) == 3.0 * (1 + iteration));
    }

    readTemp2->clearStep();
  }

  return 0;
}
