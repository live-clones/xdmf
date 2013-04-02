#include "XdmfSparseMatrix.hpp"
#include "XdmfWriter.hpp"

int main(int, char **)
{

  shared_ptr<XdmfSparseMatrix> matrix = XdmfSparseMatrix::New(3, 3);
  matrix->setName("foo");
  shared_ptr<XdmfArray> rowPointer = matrix->getRowPointer();
  shared_ptr<XdmfArray> columnIndex = matrix->getColumnIndex();
  shared_ptr<XdmfArray> values = matrix->getValues();
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

  shared_ptr<XdmfWriter> writer = XdmfWriter::New("TestXdmfSparseMatrix.xmf");
  matrix->accept(writer);

  return 0;
}
