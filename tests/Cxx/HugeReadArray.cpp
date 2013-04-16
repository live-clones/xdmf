#include <iostream>
#include <stdlib.h>
#include <XdmfArray.hpp>
#include <XdmfReader.hpp>


int main(int, char **)
{
	shared_ptr<XdmfReader> testReader = XdmfReader::New();
	shared_ptr<XdmfArray> readArray = shared_dynamic_cast<XdmfArray>(testReader->read("arraydata.xmf"));
	readArray->read();
	printf("Array ocntains %s\n", readArray->getValuesString());
	return 0;
}
