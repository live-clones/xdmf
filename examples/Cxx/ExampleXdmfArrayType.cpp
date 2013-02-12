#include "XdmfDomain.hpp"

int main(int, char **)
{
	shared_ptr<XdmfArray> exampleArray = XdmfArray::New();

	if (XdmfArrayType::Int8() == exampleArray->getArrayType())
	{
		//do whatever is to be done with in the case that the array type is Int8
	}

	unsigned int dataSize = XdmfArrayType::Int8()->getElementSize();

	std::string dataName = XdmfArrayType::Int8()->getName();

	return 0;
}
