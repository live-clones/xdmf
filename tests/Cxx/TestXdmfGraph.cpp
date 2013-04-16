#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfGraph.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

int main(int, char **)
{

  shared_ptr<XdmfGraph> graph = XdmfGraph::New(3);
  graph->setName("foo");
  shared_ptr<XdmfArray> rowPointer = graph->getRowPointer();
  shared_ptr<XdmfArray> columnIndex = graph->getColumnIndex();
  shared_ptr<XdmfArray> values = graph->getValues();
  rowPointer->insert<unsigned int>(0, 0);
  rowPointer->insert<unsigned int>(1, 0);
  rowPointer->insert<unsigned int>(2, 2);
  rowPointer->insert<unsigned int>(3, 3);
  columnIndex->pushBack<unsigned int>(1);
  columnIndex->pushBack<unsigned int>(2);
  columnIndex->pushBack<unsigned int>(0);
  values->pushBack<double>(5.0);
  values->pushBack<double>(6.0);
  values->pushBack<double>(-1.0);

  shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
  attribute->setName("Node Weight");
  attribute->setCenter(XdmfAttributeCenter::Node());
  attribute->setType(XdmfAttributeType::Scalar());
  attribute->pushBack<double>(0.0);
  attribute->pushBack<double>(1.0);
  attribute->pushBack<double>(2.0);
  graph->insert(attribute);

  shared_ptr<XdmfWriter> writer = XdmfWriter::New("TestXdmfGraph.xmf");
  graph->accept(writer);
	printf("after writing\n");
  shared_ptr<XdmfReader> reader = XdmfReader::New();
  shared_ptr<XdmfGraph> readGraph = 
    shared_dynamic_cast<XdmfGraph>(reader->read("TestXdmfGraph.xmf"));
	printf("after reading\n");
  assert(readGraph);
  assert(readGraph->getNumberNodes() == graph->getNumberNodes());
  assert(readGraph->getNumberAttributes() == graph->getNumberAttributes());
  
  shared_ptr<XdmfAttribute> readAttribute = readGraph->getAttribute(0);
  
  assert(readAttribute->getName().compare(attribute->getName()) == 0);
  assert(readAttribute->getSize() == attribute->getSize());

  return 0;
}
