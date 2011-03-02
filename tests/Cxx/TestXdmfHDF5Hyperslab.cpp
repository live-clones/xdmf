#include "XdmfArray.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHeavyDataWriter.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

int main(int, char *)
{

  //
  // Create 2x2 array.
  //
  boost::shared_ptr<XdmfArray> array = XdmfArray::New();
  std::vector<unsigned int> dimensions(2, 2);
  array->initialize<int>(dimensions);
  int values[] = {0, 1, 2, 3};
  array->insert(0, &values[0], 4);
  assert(array->getSize() == 4);
  dimensions = array->getDimensions();
  assert(dimensions.size() == 2);
  assert(dimensions[0] == 2 && dimensions[1] == 2);

  //
  // Write array to disk.
  //
  boost::shared_ptr<XdmfWriter> writer = 
    XdmfWriter::New("TestXdmfHDF5Hyperslab.xmf");
  writer->setLightDataLimit(0);
  array->accept(writer);

  //
  // Read array from disk.
  //
  boost::shared_ptr<XdmfReader> reader = 
    XdmfReader::New();
  boost::shared_ptr<XdmfItem> item = reader->read("TestXdmfHDF5Hyperslab.xmf");
  boost::shared_ptr<XdmfArray> readArray = 
    boost::shared_dynamic_cast<XdmfArray>(item);
  assert(readArray);
  assert(readArray->getSize() == 4);
  std::vector<unsigned int> readDimensions = readArray->getDimensions();
  assert(readDimensions.size() == 2);
  assert(readDimensions[0] == 2 && readDimensions[1] == 2);
 
  //
  // Read heavy values from disk.
  //
  readArray->read();
  assert(readArray->isInitialized());
  assert(readArray->getSize() == 4);
  readDimensions = readArray->getDimensions();
  assert(readDimensions.size() == 2);
  assert(readDimensions[0] == 2 && readDimensions[1] == 2);

  for(int i=0; i<4; ++i) {
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
  assert(readArray->getSize() == 9);
  dimensions = readArray->getDimensions();
  assert(dimensions.size() == 2);
  assert(dimensions[0] == 3 && dimensions[1] == 3);
 
  boost::shared_ptr<XdmfWriter> writer2 = 
    XdmfWriter::New("TestXdmfHDF5Hyperslab2.xmf");
  writer2->setLightDataLimit(0);
  writer2->getHeavyDataWriter()->setMode(XdmfHeavyDataWriter::Overwrite);
  readArray->accept(writer2);

  //
  // Read array from disk.
  //
  
  boost::shared_ptr<XdmfItem> item2 = 
    reader->read("TestXdmfHDF5Hyperslab2.xmf");
  boost::shared_ptr<XdmfArray> readArray2 = 
    boost::shared_dynamic_cast<XdmfArray>(item2);
  assert(readArray2);
  assert(readArray2->getSize() == 9);
  readDimensions = readArray2->getDimensions();
  assert(readDimensions.size() == 2);
  assert(readDimensions[0] == 3 && readDimensions[1] == 3);

  //
  // Read heavy values from disk.
  //
  readArray2->read();
  assert(readArray2->isInitialized());
  assert(readArray2->getSize() == 9);
  readDimensions = readArray2->getDimensions();
  assert(readDimensions.size() == 2);
  assert(readDimensions[0] == 3 && readDimensions[1] == 3);

  for(int i=0; i<9; ++i) {
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
  const std::vector<unsigned int> stride(2,1);
  const std::vector<unsigned int> count(2, 2);

  boost::shared_ptr<XdmfHeavyDataController> controller =
    readArray2->getHeavyDataController();
  assert(controller);

  boost::shared_ptr<XdmfHDF5Controller> hyperslabController = 
    XdmfHDF5Controller::New(controller->getFilePath(),
                            controller->getDataSetPath(),
                            controller->getType(),
                            start,
                            stride,
                            count);

  boost::shared_ptr<XdmfArray> hyperslabArray = XdmfArray::New();
  hyperslabArray->setHeavyDataController(hyperslabController);
  assert(hyperslabArray->getSize() == 4);
  std::vector<unsigned int> hyperslabDimensions = 
    hyperslabArray->getDimensions();
  assert(hyperslabDimensions.size() == 2);
  assert(hyperslabDimensions[0] == 2 && hyperslabDimensions[1] == 2);
  hyperslabArray->read();

  hyperslabDimensions = hyperslabArray->getDimensions();
  assert(hyperslabDimensions.size() == 2);
  assert(hyperslabDimensions[0] == 2 && hyperslabDimensions[1] == 2);
  assert(hyperslabArray->getValue<int>(0) == 4);
  assert(hyperslabArray->getValue<int>(1) == 5);
  assert(hyperslabArray->getValue<int>(2) == 7);
  assert(hyperslabArray->getValue<int>(3) == 8);
 
  return 0;

}
