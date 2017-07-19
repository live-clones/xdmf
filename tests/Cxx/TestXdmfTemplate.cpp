#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfTemplate.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include <iostream>


#include "stdio.h"

int main(int, char **)
{
  unsigned int arraySize = 12;
  unsigned int numSteps = 5;

  std::cout << "HDF5" << std::endl;

  shared_ptr<XdmfTemplate> temp = XdmfTemplate::New();

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

  shared_ptr<XdmfWriter> writer = XdmfWriter::New("template.xmf");

  shared_ptr<XdmfHeavyDataWriter> heavyWriter = writer->getHeavyDataWriter();

  temp->setHeavyDataWriter(heavyWriter);

  temp->setBase(ungrid);

  for (unsigned int iteration = 1; iteration <= numSteps; ++iteration)
  {
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

  for (unsigned int iteration = 0; iteration < numSteps; ++iteration)
  {
    temp->setStep(iteration);

    attr->read();
    attr2->read();
    attr3->read();

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

  shared_ptr<XdmfTemplate> readTemp = shared_dynamic_cast<XdmfTemplate>(reader->read("template.xmf"));

  shared_ptr<XdmfUnstructuredGrid> readungrid = shared_dynamic_cast<XdmfUnstructuredGrid>(readTemp->getBase());

  std::cout << "?" << readungrid << std::endl;

  assert(readungrid);

  shared_ptr<XdmfAttribute> readAttr = readungrid->getAttribute(0);
  shared_ptr<XdmfAttribute> readAttr2 = readungrid->getAttribute(1);
  shared_ptr<XdmfAttribute> readAttr3 = readungrid->getAttribute(2);

  std::cout << readTemp->getNumberSteps() << " ?= " << numSteps << std::endl;

  assert(readTemp->getNumberSteps() == numSteps);

  for (unsigned int iteration = 0; iteration < numSteps; ++iteration)
  {
    readTemp->setStep(iteration);

    readAttr->read();
    readAttr2->read();
    readAttr3->read();

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

  shared_ptr<XdmfTemplate> temp2 = XdmfTemplate::New();

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

  shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("template2.xmf");

  temp2->setBase(ungrid2);

  for (unsigned int iteration = 1; iteration <= numSteps; ++iteration)
  {
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

  for (unsigned int iteration = 0; iteration < numSteps; ++iteration)
  {
    temp2->setStep(iteration);

    secondattr->read();
    secondattr2->read();
    secondattr3->read();

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

  shared_ptr<XdmfTemplate> readTemp2 = shared_dynamic_cast<XdmfTemplate>(reader->read("template2.xmf"));

  shared_ptr<XdmfUnstructuredGrid> readungrid2 = shared_dynamic_cast<XdmfUnstructuredGrid>(readTemp2->getBase());

  std::cout << "?" << readungrid2 << std::endl;

  assert(readungrid2);

  shared_ptr<XdmfAttribute> secondreadAttr = readungrid2->getAttribute(0);
  shared_ptr<XdmfAttribute> secondreadAttr2 = readungrid2->getAttribute(1);
  shared_ptr<XdmfAttribute> secondreadAttr3 = readungrid2->getAttribute(2);

  std::cout << readTemp2->getNumberSteps() << " ?= " << numSteps << std::endl;

  assert(readTemp2->getNumberSteps() == numSteps);

  for (unsigned int iteration = 0; iteration < numSteps; ++iteration)
  {
    readTemp2->setStep(iteration);

    secondreadAttr->read();
    secondreadAttr2->read();
    secondreadAttr3->read();

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

  readTemp2->removeStep(2);

  assert(readTemp2->getNumberSteps() == numSteps - 1);

  unsigned int offset = 0;

  for (unsigned int iteration = 0; iteration < numSteps-1; ++iteration)
  {
    if (iteration == 2)
    {
      offset++;
    }
    readTemp2->setStep(iteration);

    secondreadAttr->read();
    secondreadAttr2->read();
    secondreadAttr3->read();

    std::cout << secondreadAttr->getValuesString() << std::endl;
    std::cout << secondreadAttr2->getValuesString() << std::endl;
    std::cout << secondreadAttr3->getValuesString() << std::endl;


    for (unsigned int i = 0; i < arraySize; ++i)
    {
      assert(secondreadAttr->getValue<double>(i) ==  1.0 * (1 + iteration + offset));
      assert(secondreadAttr2->getValue<double>(i) == 2.0 * (1 + iteration + offset));
      assert(secondreadAttr3->getValue<double>(i) == 3.0 * (1 + iteration + offset));
    }

    readTemp2->clearStep();
  }

  // Testing Append mode

  shared_ptr<XdmfTemplate> appendtemp = XdmfTemplate::New();

  shared_ptr<XdmfUnstructuredGrid> appendungrid = XdmfUnstructuredGrid::New();

  shared_ptr<XdmfTopology> appendtopo = XdmfTopology::New();

  appendtopo->initialize(XdmfArrayType::Float64(), 12);

  shared_ptr<XdmfGeometry> appendgeo = XdmfGeometry::New();

  appendgeo->initialize(XdmfArrayType::Float64(), 36);

  appendungrid->setTopology(appendtopo);
  appendungrid->setGeometry(appendgeo);

  shared_ptr<XdmfAttribute> appendattr = XdmfAttribute::New();

  appendattr->initialize(XdmfArrayType::Float64(), 12);

  appendattr->release();

  shared_ptr<XdmfAttribute> appendattr2 = XdmfAttribute::New();

  appendattr2->initialize(XdmfArrayType::Float64(), 12);

  appendattr2->release();

  shared_ptr<XdmfAttribute> appendattr3 = XdmfAttribute::New();

  appendattr3->initialize(XdmfArrayType::Float64(), 12);

  appendattr3->release();

  appendungrid->insert(appendattr);
  appendungrid->insert(appendattr2);
  appendungrid->insert(appendattr3);

  shared_ptr<XdmfWriter> writer3 = XdmfWriter::New("template3.xmf");
  writer3->setLightDataLimit(2);

  shared_ptr<XdmfHeavyDataWriter> heavyWriter3 = writer3->getHeavyDataWriter();

  heavyWriter3->setMode(XdmfHeavyDataWriter::Append);

  appendtemp->setHeavyDataWriter(heavyWriter3);

  appendtemp->setBase(appendungrid);

  for (unsigned int iteration = 1; iteration <= numSteps; ++iteration)
  {
    for (unsigned int i = 0; i < arraySize; ++i)
    {
      appendattr->insert<double>(i, 1.0 * iteration);
      appendattr2->insert<double>(i, 2.0 * iteration);
      appendattr3->insert<double>(i, 3.0 * iteration);
    }

    appendtemp->addStep();

    appendtemp->clearStep();
  }

  std::cout << "writing" << std::endl;

  heavyWriter3->setMode(XdmfHeavyDataWriter::Default);

  appendtemp->accept(writer3); 

  std::cout << "reading" << std::endl;

  shared_ptr<XdmfTemplate> appendreadTemp = shared_dynamic_cast<XdmfTemplate>(reader->read("template3.xmf"));

  shared_ptr<XdmfUnstructuredGrid> appendreadungrid = shared_dynamic_cast<XdmfUnstructuredGrid>(appendreadTemp->getBase());

  std::cout << "?" << appendreadungrid << std::endl;

  assert(appendreadungrid);

  shared_ptr<XdmfAttribute> appendreadAttr = appendreadungrid->getAttribute(0);
  shared_ptr<XdmfAttribute> appendreadAttr2 = appendreadungrid->getAttribute(1);
  shared_ptr<XdmfAttribute> appendreadAttr3 = appendreadungrid->getAttribute(2);

  std::cout << appendreadTemp->getNumberSteps() << " ?= " << numSteps << std::endl;

  assert(appendreadTemp->getNumberSteps() == numSteps);

  for (unsigned int iteration = 0; iteration < numSteps; ++iteration)
  {
    appendreadTemp->setStep(iteration);

    appendreadAttr->read();
    appendreadAttr2->read();
    appendreadAttr3->read();

    std::cout << appendreadAttr->getValuesString() << std::endl;
    std::cout << appendreadAttr2->getValuesString() << std::endl;
    std::cout << appendreadAttr3->getValuesString() << std::endl;

    for (unsigned int i = 0; i < arraySize; ++i)
    {
      assert(appendreadAttr->getValue<double>(i) ==  1.0 * (1 + iteration));
      assert(appendreadAttr2->getValue<double>(i) == 2.0 * (1 + iteration));
      assert(appendreadAttr3->getValue<double>(i) == 3.0 * (1 + iteration));
    }

    appendreadTemp->clearStep();
  }

  // Overwrite

  shared_ptr<XdmfTemplate> overwritetemp = XdmfTemplate::New();

  shared_ptr<XdmfUnstructuredGrid> overwriteungrid = XdmfUnstructuredGrid::New();

  shared_ptr<XdmfTopology> overwritetopo = XdmfTopology::New();

  overwritetopo->initialize(XdmfArrayType::Float64(), 12);

  shared_ptr<XdmfGeometry> overwritegeo = XdmfGeometry::New();

  overwritegeo->initialize(XdmfArrayType::Float64(), 36);

  overwriteungrid->setTopology(overwritetopo);
  overwriteungrid->setGeometry(overwritegeo);

  shared_ptr<XdmfAttribute> overwriteattr = XdmfAttribute::New();

  overwriteattr->initialize(XdmfArrayType::Float64(), 12);

  overwriteattr->release();

  shared_ptr<XdmfAttribute> overwriteattr2 = XdmfAttribute::New();

  overwriteattr2->initialize(XdmfArrayType::Float64(), 12);

  overwriteattr2->release();

  shared_ptr<XdmfAttribute> overwriteattr3 = XdmfAttribute::New();

  overwriteattr3->initialize(XdmfArrayType::Float64(), 12);

  overwriteattr3->release();

  overwriteungrid->insert(overwriteattr);
  overwriteungrid->insert(overwriteattr2);
  overwriteungrid->insert(overwriteattr3);

  shared_ptr<XdmfWriter> writer4 = XdmfWriter::New("template4.xmf");
  writer4->setLightDataLimit(2);

  shared_ptr<XdmfHeavyDataWriter> heavyWriter4 = writer4->getHeavyDataWriter();

  overwritetemp->setHeavyDataWriter(heavyWriter4);

  overwritetemp->setBase(overwriteungrid);

  for (unsigned int i = 0; i < arraySize; ++i)
  {
    overwriteattr->insert<double>(i, 0.0);
    overwriteattr2->insert<double>(i, 0.0);
    overwriteattr3->insert<double>(i, 0.0);
  }

  overwritetemp->preallocateSteps(numSteps);

  for (unsigned int iteration = 1; iteration <= numSteps; ++iteration)
  {
    for (unsigned int i = 0; i < arraySize; ++i)
    {
      overwriteattr->insert<double>(i, 1.0 * iteration);
      overwriteattr2->insert<double>(i, 2.0 * iteration);
      overwriteattr3->insert<double>(i, 3.0 * iteration);
    }

    overwritetemp->addStep();

    overwritetemp->clearStep();
  }

  std::cout << "writing" << std::endl;

  heavyWriter3->setMode(XdmfHeavyDataWriter::Default);

  overwritetemp->accept(writer4);

  std::cout << "reading" << std::endl;

  shared_ptr<XdmfTemplate> overwritereadTemp = shared_dynamic_cast<XdmfTemplate>(reader->read("template4.xmf"));

  shared_ptr<XdmfUnstructuredGrid> overwritereadungrid = shared_dynamic_cast<XdmfUnstructuredGrid>(overwritereadTemp->getBase());

  std::cout << "?" << overwritereadungrid << std::endl;

  assert(overwritereadungrid);

  shared_ptr<XdmfAttribute> overwritereadAttr = overwritereadungrid->getAttribute(0);
  shared_ptr<XdmfAttribute> overwritereadAttr2 = overwritereadungrid->getAttribute(1);
  shared_ptr<XdmfAttribute> overwritereadAttr3 = overwritereadungrid->getAttribute(2);

  std::cout << overwritereadTemp->getNumberSteps() << " ?= " << numSteps << std::endl;

  assert(overwritereadTemp->getNumberSteps() == numSteps);

  for (unsigned int iteration = 0; iteration < numSteps; ++iteration)
  {
    overwritereadTemp->setStep(iteration);

    overwritereadAttr->read();
    overwritereadAttr2->read();
    overwritereadAttr3->read();

    std::cout << overwritereadAttr->getValuesString() << std::endl;
    std::cout << overwritereadAttr2->getValuesString() << std::endl;
    std::cout << overwritereadAttr3->getValuesString() << std::endl;

    for (unsigned int i = 0; i < arraySize; ++i)
    {
      assert(overwritereadAttr->getValue<double>(i) ==  1.0 * (1 + iteration));
      assert(overwritereadAttr2->getValue<double>(i) == 2.0 * (1 + iteration));
      assert(overwritereadAttr3->getValue<double>(i) == 3.0 * (1 + iteration));
    }

    overwritereadTemp->clearStep();
  }

  std::cout << "Removing step 2" << std::endl;

  offset = 0;

  overwritereadTemp->removeStep(2);

  for (unsigned int iteration = 0; iteration < numSteps - 1; ++iteration)
  {
    if (iteration == 2)
    {
      ++offset;
    }
    overwritereadTemp->setStep(iteration);

    overwritereadAttr->read();
    overwritereadAttr2->read();
    overwritereadAttr3->read();

    std::cout << overwritereadAttr->getValuesString() << std::endl;
    std::cout << overwritereadAttr2->getValuesString() << std::endl;
    std::cout << overwritereadAttr3->getValuesString() << std::endl;

    for (unsigned int i = 0; i < arraySize; ++i)
    {
      assert(overwritereadAttr->getValue<double>(i) ==  1.0 * (1 + iteration+offset));
      assert(overwritereadAttr2->getValue<double>(i) == 2.0 * (1 + iteration+offset));
      assert(overwritereadAttr3->getValue<double>(i) == 3.0 * (1 + iteration+offset));
    }

    overwritereadTemp->clearStep();
  }
    

  return 0;
}
