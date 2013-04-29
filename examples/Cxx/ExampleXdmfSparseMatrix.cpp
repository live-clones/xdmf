#include "XdmfSparseMatrix.hpp"
#include "XdmfArray.hpp"

int main(int, char **)
{
	shared_ptr<XdmfSparseMatrix> exampleMatrix = XdmfSparseMatrix::New(3, 3);

	exampleMatrix->setName("TestMatrix");

	std::string exampleName = exampleMatrix->getName();

	shared_ptr<XdmfArray> newRowPointer = XdmfArray::New();

	newRowPointer->insert<unsigned int>(0, 1);
	newRowPointer->insert<unsigned int>(1, 1);
	newRowPointer->insert<unsigned int>(2, 2);
	newRowPointer->insert<unsigned int>(3, 3);

	exampleMatrix->setRowPointer(newRowPointer);

	shared_ptr<XdmfArray> exampleRowPointer = exampleMatrix->getRowPointer();

	shared_ptr<XdmfArray> newColumnIndex = XdmfArray::New();

	newColumnIndex->pushBack<unsigned int>(1);
	newColumnIndex->pushBack<unsigned int>(2);
	newColumnIndex->pushBack<unsigned int>(0);

	exampleMatrix->setColumnIndex(newColumnIndex);

	shared_ptr<XdmfArray> exampleColumnIndex = exampleMatrix->getColumnIndex();

	unsigned int exampleNumRows = exampleMatrix->getNumberRows();

	unsigned int exampleNumCols = exampleMatrix->getNumberColumns();

	shared_ptr<XdmfArray> newValues = XdmfArray::New();

	newValues->pushBack<double>(5.0);
	newValues->pushBack<double>(6.0);
	newValues->pushBack<double>(-1.0);

	exampleMatrix->setValues(newValues);

	shared_ptr<XdmfArray> exampleValues = exampleMatrix->getValues();

	std::string exampleValueString = exampleMatrix->getValuesString();

	return 0;
}
