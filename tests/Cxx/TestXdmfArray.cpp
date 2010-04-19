#include "XdmfArray.hpp"

int main(int argc, char* argv[])
{

	/**
	 * Copy from array
	 */
	double values[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
	boost::shared_ptr<XdmfArray> array = XdmfArray::New();
	array->copyValues(0, &values[0], 9);
	assert(array->getSize() == 9);
	assert(array->getType().compare("Float") == 0);
	assert(array->getPrecision() == 8);
	assert(array->getValuesString().compare("1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 ") == 0);
	// Assert we copied values correctly
	boost::shared_ptr<std::vector<double> > storedValues = array->getValues<double>();
	int i = 0;
	for(std::vector<double>::const_iterator iter = storedValues->begin(); iter!= storedValues->end(); ++iter, ++i)
	{
		assert(*iter == values[i]);
	}
	// Now modify original array
	values[0] = 0;
	assert(array->getValuesString().compare("1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 ") == 0);

	/**
	 * Copy from vector
	 */
	std::vector<int> values2;
	values2.push_back(100);
	values2.push_back(200);
	boost::shared_ptr<XdmfArray> array2 = XdmfArray::New();
	array2->copyValues(0, &values2[0], 2);
	assert(array2->getSize() == 2);
	assert(array2->getType().compare("Int") == 0);
	assert(array2->getPrecision() == 4);
	assert(array2->getValuesString().compare("100 200 ") == 0);
	// Assert we copied values correctly
	boost::shared_ptr<std::vector<int> > storedValues2 = array2->getValues<int>();
	i = 0;
	for(std::vector<int>::const_iterator iter = storedValues2->begin(); iter!= storedValues2->end(); ++iter, ++i)
	{
		assert(*iter == values2[i]);
	}
	// Now modify original array
	values2.push_back(300);
	assert(array2->getSize() == 2);
	assert(array2->getType().compare("Int") == 0);
	assert(array2->getPrecision() == 4);
	assert(array2->getValuesString().compare("100 200 ") == 0);

	/**
	 * Shared vector assignment
	 */
	boost::shared_ptr<std::vector<char> > values3(new std::vector<char>());
	values3->push_back(-2);
	values3->push_back(-1);
	values3->push_back(0);
	values3->push_back(1);
	values3->push_back(2);
	boost::shared_ptr<XdmfArray> array3 = XdmfArray::New();
	array3->setValues(values3);
	assert(array3->getSize() == 5);
	assert(array3->getType().compare("Char") == 0);
	assert(array3->getPrecision() == 1);
	assert(array3->getValuesString().compare("-2 -1 0 1 2 ") == 0);
	// Now modify original array
	values3->push_back(8);
	assert(array3->getSize() == 6);
	assert(array3->getValuesString().compare("-2 -1 0 1 2 8 ") == 0);
	// Assert we have the same values!
	boost::shared_ptr<std::vector<char> > storedValues3 = array3->getValues<char>();
	assert(storedValues3 == values3);

	// Assert that we can't get a smart pointer to an int vector
	boost::shared_ptr<std::vector<int> > storedValues3Int = array3->getValues<int>();
	assert(storedValues3Int == NULL);

	/**
	 * Copy values from another XdmfArray
	 */
	boost::shared_ptr<XdmfArray> array4 = XdmfArray::New();
	array4->copyValues(0, array, 1, 4);
	assert(array4->getSize() == 4);
	assert(array4->getType().compare("Float") == 0);
	assert(array4->getPrecision() == 8);
	boost::shared_ptr<std::vector<double> > storedValues4 = array4->getValues<double>();
	i = 1;
	for(std::vector<double>::const_iterator iter = storedValues4->begin(); iter!= storedValues4->end(); ++iter, ++i)
	{
		assert((*iter) == storedValues->operator[](i));
	}

	/**
	 * Swap values from a vector
	 */
	std::vector<short> values4;
	values4.push_back(-1);
	values4.push_back(0);
	values4.push_back(1);
	boost::shared_ptr<XdmfArray> array5 = XdmfArray::New();
	array5->swap(values4);
	assert(values4.size() == 0);
	assert(array5->getSize() == 3);
	assert(array5->getType().compare("Short") == 0);
	assert(array5->getPrecision() == 2);
	boost::shared_ptr<std::vector<short> > storedValues5 = array5->getValues<short>();
	assert((*storedValues5)[0] == -1);
	assert((*storedValues5)[1] == 0);
	assert((*storedValues5)[2] == 1);

	/**
	 * Swap values from a shared vector
	 */
	array5->clear();
	array5->swap(storedValues3);
	assert(storedValues3->size() == 0);
	assert(array5->getSize() == 6);

	/**
	 * Swap values from an XdmfArray
	 */
	array5->clear();
	array5->swap(array4);
	assert(array4->getSize() == 0);
	assert(array5->getSize() == 4);

	/**
	 * Get values via swap
	 */
	std::vector<int> valsInt;
	bool success = array5->swap(valsInt);
	assert(success == false);
	std::vector<double> valsDouble;
	success = array5->swap(valsDouble);
	assert(success == true);
	assert(valsDouble.size() == 4);
	assert(array5->getSize() == 0);

	return 0;
}
