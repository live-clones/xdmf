#include "XdmfArray.hpp"

int main(int argc, char* argv[])
{

	boost::shared_ptr<XdmfArray> array = XdmfArray::New();
	std::vector<char> values;
	values.push_back(1);
	values.push_back(2);
	values.push_back(3);
	values.push_back(4);
	array->setValues(0, &values[0], 4, 1, 1);
	std::cout << array->getValues() << std::endl;
	std::cout << array->getType() << std::endl;


	boost::shared_ptr<XdmfArray> array2 = XdmfArray::New();
	std::vector<int> values2;
	values2.push_back(100);
	values2.push_back(200);
	array2->setValues(0, &values2[0], 2, 1, 1);
	std::cout << array2->getValues() << std::endl;
	std::cout << array2->getType() << std::endl;
	array2->setValues(2, &values[0], 2, 1, 1);
	std::cout << array2->getValues() << std::endl;
	std::cout << array2->getType() << std::endl;

	boost::shared_ptr<XdmfArray> array3 = XdmfArray::New();
	double values3[] = {0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1, 1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1,
			0.1, 0.1, -1.1, 1.1, 0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1, -1.1, 3.1, 2.1, -2.1};
	array3->setValues(0, &values3[0], 36);
	std::cout << array3->getValues() << std::endl;
	std::cout << array3->getType() << std::endl;
	const double* const array3Pointer = (const double* const)array3->getValuesPointer();
	for(int i=0; i<array3->getSize(); ++i)
	{
		assert(array3Pointer[i] == values3[i]);
	}

	boost::shared_ptr<XdmfArray> array4 = XdmfArray::New();
	std::vector<long> values4;
	for(int i=1000; i<1100; ++i)
	{
		values4.push_back(i);
	}
	array4->setValues(values4);

	const long* const array4Pointer = (const long* const)array4->getValuesPointer();
	for(int i=0; i<array4->getSize(); ++i)
	{
		assert(array4Pointer[i] == values4[i]);
	}

	return 0;
}
