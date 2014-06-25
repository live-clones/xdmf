#include "XdmfArray.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHeavyDataWriter.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

int main(int, char **)
{

  //
  // Create 2x2 array.
  //
  shared_ptr<XdmfArray> array = XdmfArray::New();
  std::vector<unsigned int> dimensions(2, 2);
  array->initialize<int>(dimensions);
  int values[] = {0, 1, 2, 3};
  array->insert(0, &values[0], 4);
  std::cout << array->getSize() << " ?= " << 4 << std::endl;
  assert(array->getSize() == 4);
  dimensions = array->getDimensions();
  std::cout << dimensions.size() << " ?= " << 2 << std::endl;
  std::cout << dimensions[0] << " ?= " << 2 << std::endl;
  std::cout << dimensions[1] << " ?= " << 2 << std::endl;
  assert(dimensions.size() == 2);
  assert(dimensions[0] == 2 && dimensions[1] == 2);

  //
  // Write array to disk.
  //
  shared_ptr<XdmfWriter> writer =
    XdmfWriter::New("TestXdmfHDF5Hyperslab.xmf");
  writer->setLightDataLimit(0);
  array->accept(writer);

  //
  // Read array from disk.
  //
  shared_ptr<XdmfReader> reader = XdmfReader::New();
  shared_ptr<XdmfItem> item = reader->read("TestXdmfHDF5Hyperslab.xmf");
  shared_ptr<XdmfArray> readArray = shared_dynamic_cast<XdmfArray>(item);
  if (readArray)
  {
    std::cout << "array exists" << std::endl;
  }
  else
  {
    std::cout << "array does not exist" << std::endl;
  }
  std::cout << readArray->getSize() << " ?= " << 4 << std::endl;
  assert(readArray);
  assert(readArray->getSize() == 4);
  std::vector<unsigned int> readDimensions = readArray->getDimensions();
  std::cout << readDimensions.size() << " ?= " << 2 << std::endl;
  std::cout << readDimensions[0] << " ?= " << 2 << std::endl;
  std::cout << readDimensions[1] << " ?= " << 2 << std::endl;
  assert(readDimensions.size() == 2);
  assert(readDimensions[0] == 2 && readDimensions[1] == 2);

  //
  // Read heavy values from disk.
  //
  readArray->read();
  if (readArray->isInitialized())
  {
    std::cout << "array is initialized" << std::endl;
  }
  else
  {
    std::cout << "array is not initialized" << std::endl;
  }
  std::cout << readArray->getSize() << " ?= " << 4 << std::endl;
  assert(readArray->isInitialized());
  assert(readArray->getSize() == 4);
  readDimensions = readArray->getDimensions();
  std::cout << readDimensions.size() << " ?= " << 2 << std::endl;
  std::cout << readDimensions[0] << " ?= " << 2 << std::endl;
  std::cout << readDimensions[1] << " ?= " << 2 << std::endl;
  assert(readDimensions.size() == 2);
  assert(readDimensions[0] == 2 && readDimensions[1] == 2);

  for(int i=0; i<4; ++i) {
    std::cout << readArray->getValue<int>(i) << " ?= " << i << std::endl;
    assert(readArray->getValue<int>(i) == i);
  }

  //
  // Modify read array to do an overwrite of a different sized dataset.
  //
  dimensions = std::vector<unsigned int>(2,3);
  readArray->resize(dimensions, 1000);
  for(int i=0; i<9; ++i) {
    readArray->insert(i, i);
  }
  std::cout << readArray->getSize() << " ?= " << 9 << std::endl;
  assert(readArray->getSize() == 9);
  dimensions = readArray->getDimensions();
  std::cout << dimensions.size() << " ?= " << 2 << std::endl;
  std::cout << dimensions[0] << " ?= " << 3 << std::endl;
  std::cout << dimensions[1] << " ?= " << 3 << std::endl;
  assert(dimensions.size() == 2);
  assert(dimensions[0] == 3 && dimensions[1] == 3);

  shared_ptr<XdmfWriter> writer2 =
    XdmfWriter::New("TestXdmfHDF5Hyperslab2.xmf");
  writer2->setLightDataLimit(0);
  writer2->getHeavyDataWriter()->setMode(XdmfHeavyDataWriter::Overwrite);

  readArray->accept(writer2);

  //
  // Read array from disk.
  //

  shared_ptr<XdmfItem> item2 = reader->read("TestXdmfHDF5Hyperslab2.xmf");
  shared_ptr<XdmfArray> readArray2 = shared_dynamic_cast<XdmfArray>(item2);
  if (readArray2)
  {
    std::cout << "array exists" << std::endl;
  }
  else
  {
    std::cout << "array does not exist" << std::endl;
  }
  std::cout << readArray2->getSize() << " ?= " << 9 << std::endl;
  assert(readArray2);
  assert(readArray2->getSize() == 9);
  readDimensions = readArray2->getDimensions();
  std::cout << readDimensions.size() << " ?= " << 2 << std::endl;
  std::cout << readDimensions[0] << " ?= " << 3 << std::endl;
  std::cout << readDimensions[1] << " ?= " << 3 << std::endl;
  assert(readDimensions.size() == 2);
  assert(readDimensions[0] == 3 && readDimensions[1] == 3);

  //
  // Read heavy values from disk.
  //
  readArray2->read();
  if (readArray2->isInitialized())
  {
    std::cout << "array is initialized" << std::endl;
  }
  else
  {
    std::cout << "array is not initialized" << std::endl;
  }
  std::cout << readArray2->getSize() << " ?= " << 9 << std::endl;
  assert(readArray2->isInitialized());
  assert(readArray2->getSize() == 9);
  readDimensions = readArray2->getDimensions();
  std::cout << readDimensions.size() << " ?= " << 2 << std::endl;
  std::cout << readDimensions[0] << " ?= " << 3 << std::endl;
  std::cout << readDimensions[1] << " ?= " << 3 << std::endl;
  assert(readDimensions.size() == 2);
  assert(readDimensions[0] == 3 && readDimensions[1] == 3);

  for(int i=0; i<9; ++i) {
    std::cout << readArray2->getValue<int>(i) << " ?= " << i << std::endl;
    assert(readArray2->getValue<int>(i) == i);
  }

  //
  // Read hyperslab of data
  //
  // 0 1 2
  // 3 4 5
  // 6 7 8
  //
  // becomes...
  //
  // 4 5
  // 7 8
  //
  const std::vector<unsigned int> start(2, 1);
  const std::vector<unsigned int> stride(2, 1);
  const std::vector<unsigned int> count(2, 2);
  const std::vector<unsigned int> dataSpaceSize(2, 3);

  shared_ptr<XdmfHDF5Controller> controller =
    shared_dynamic_cast<XdmfHDF5Controller>(readArray2->getHeavyDataController());

  if (controller)
  {
    std::cout << "HDF5 controller exists" << std::endl;
  }
  else
  {
    std::cout << "HDF5 controller does not exists" << std::endl;
  }

  assert(controller);

  shared_ptr<XdmfHDF5Controller> hyperslabController =
    XdmfHDF5Controller::New(controller->getFilePath(),
                            controller->getDataSetPath(),
                            controller->getType(),
                            start,
                            stride,
                            count,
                            dataSpaceSize);

  shared_ptr<XdmfArray> hyperslabArray = XdmfArray::New();
  hyperslabArray->setHeavyDataController(hyperslabController);
  std::cout << hyperslabArray->getSize() << " ?= " << 4 << std::endl;
  assert(hyperslabArray->getSize() == 4);
  std::vector<unsigned int> hyperslabDimensions =
    hyperslabArray->getDimensions();
  std::cout << hyperslabDimensions.size() << " ?= " << 2 << std::endl;
  std::cout << hyperslabDimensions[0] << " ?= " << 2 << std::endl;
  std::cout << hyperslabDimensions[1] << " ?= " << 2 << std::endl;
  assert(hyperslabDimensions.size() == 2);
  assert(hyperslabDimensions[0] == 2 && hyperslabDimensions[1] == 2);
  hyperslabArray->read();

  hyperslabDimensions = hyperslabArray->getDimensions();
  std::cout << hyperslabDimensions.size() << " ?= " << 2 << std::endl;
  std::cout << hyperslabDimensions[0] << " ?= " << 2 << std::endl;
  std::cout << hyperslabDimensions[1] << " ?= " << 2 << std::endl;
  std::cout << hyperslabArray->getValue<int>(0) << " ?= " << 4 << std::endl;
  std::cout << hyperslabArray->getValue<int>(1) << " ?= " << 5 << std::endl;
  std::cout << hyperslabArray->getValue<int>(2) << " ?= " << 7 << std::endl;
  std::cout << hyperslabArray->getValue<int>(3) << " ?= " << 8 << std::endl;
  assert(hyperslabDimensions.size() == 2);
  assert(hyperslabDimensions[0] == 2 && hyperslabDimensions[1] == 2);
  assert(hyperslabArray->getValue<int>(0) == 4);
  assert(hyperslabArray->getValue<int>(1) == 5);
  assert(hyperslabArray->getValue<int>(2) == 7);
  assert(hyperslabArray->getValue<int>(3) == 8);

  return 0;

}
