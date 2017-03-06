#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <XdmfArray.hpp>
#include <XdmfArrayType.hpp>
#include <XdmfWriter.hpp>

int main(int, char **)
{
	
	shared_ptr<XdmfArray> writtenArray = XdmfArray::New();
	std::vector<unsigned int> dimensionVector;
	dimensionVector.push_back(5);
	dimensionVector.push_back(4);
	writtenArray->initialize<int>(dimensionVector);
	for (int i = 0; i < 20; i++)
	{
		writtenArray->insert(i, i + 1);
	}

	shared_ptr<XdmfArray> readArray = XdmfArray::New();
	std::vector<unsigned int> readDimensionVector;
	readDimensionVector.push_back(6);
	readDimensionVector.push_back(4);
	readArray->initialize<int>(readDimensionVector);

	std::vector<unsigned int> writeStarts;
	writeStarts.push_back(0);
	writeStarts.push_back(0);
	std::vector<unsigned int> writeStrides;
	writeStrides.push_back(2);
	writeStrides.push_back(2);
	std::vector<unsigned int> writeDim;
	writeDim.push_back(3);
	writeDim.push_back(2);
	std::vector<unsigned int> readStarts;
	readStarts.push_back(0);
	readStarts.push_back(0);
	std::vector<unsigned int> readStrides;
	readStrides.push_back(2);
	readStrides.push_back(2);
	std::vector<unsigned int> readDim;
	readDim.push_back(3);
	readDim.push_back(2);
	
	readArray->insert(readStarts, writtenArray, writeStarts, writeDim, readDim, readStrides, writeStrides);

        std::cout << readArray->getValuesString() << " ?= " << "1 0 3 0 5 0 0 0 0 0 0 0 11 0 13 0 15 0 0 0 0 0 0 0" << std::endl;

	assert(readArray->getValuesString().compare("1 0 3 0 5 0 0 0 0 0 0 0 11 0 13 0 15 0 0 0 0 0 0 0") == 0);

	return 0;
}
