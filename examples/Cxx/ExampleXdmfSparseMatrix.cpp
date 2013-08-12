#include "XdmfSparseMatrix.hpp"
#include "XdmfArray.hpp"

int main(int, char **)
{
	//#initialization begin

	shared_ptr<XdmfSparseMatrix> exampleMatrix = XdmfSparseMatrix::New(3, 3);

	//#initialization end

	//#setName begin

	exampleMatrix->setName("TestMatrix");

	//#setName end

	//#getName begin

	std::string exampleName = exampleMatrix->getName();

	//#getName end

	//#setRowPointer begin

	shared_ptr<XdmfArray> newRowPointer = XdmfArray::New();

	newRowPointer->insert<unsigned int>(0, 1);
	newRowPointer->insert<unsigned int>(1, 1);
	newRowPointer->insert<unsigned int>(2, 2);
	newRowPointer->insert<unsigned int>(3, 3);

	exampleMatrix->setRowPointer(newRowPointer);

	//#setRowPointer end

	//#getRowPointer begin

	shared_ptr<XdmfArray> exampleRowPointer = exampleMatrix->getRowPointer();

	//#getRowPointer end

	//#setColumnIndex begin

	shared_ptr<XdmfArray> newColumnIndex = XdmfArray::New();

	newColumnIndex->pushBack<unsigned int>(1);
	newColumnIndex->pushBack<unsigned int>(2);
	newColumnIndex->pushBack<unsigned int>(0);

	exampleMatrix->setColumnIndex(newColumnIndex);

	//#setColumnIndex end

	//#getColumnIndex begin

	shared_ptr<XdmfArray> exampleColumnIndex = exampleMatrix->getColumnIndex();

	//#getColumnIndex end

	//#getNumberRows begin

	unsigned int exampleNumRows = exampleMatrix->getNumberRows();

	//#getNumberRows end

	//#getNumberColumns begin

	unsigned int exampleNumCols = exampleMatrix->getNumberColumns();

	//#getNumberColumns end

	//#setValues begin

	shared_ptr<XdmfArray> newValues = XdmfArray::New();

	newValues->pushBack<double>(5.0);
	newValues->pushBack<double>(6.0);
	newValues->pushBack<double>(-1.0);

	exampleMatrix->setValues(newValues);

	//#setValues end

	//#getValues begin

	shared_ptr<XdmfArray> exampleValues = exampleMatrix->getValues();

	//#getValues end

	//#getValuesString begin

	std::string exampleValueString = exampleMatrix->getValuesString();

	//#getValuesString end

	return 0;
}
