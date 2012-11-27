#include "XdmfArray.hpp"

int main(int, char **)
{
	shared_ptr<XdmfArray> exampleArray = XdmfArray::New();
	//Assuming that exampleArray is a shared pointer to an XdmfArray object

	std::string newName = "New Name";
	exampleArray->setName(newName);

	int newSize = 10;
	exampleArray->reserve(newSize);
	shared_ptr<std::vector<int> > exampleVector = exampleArray->initialize(newSize);
	exampleArray->initialize(XdmfArrayType::Int32(), newSize);

	std::vector<unsigned int> newSizeVector;
	newSizeVector.push_back(4);
	newSizeVector.push_back(5);
	shared_ptr<std::vector<int> > exampleVector = exampleArray->initialize(newSizeVector);
	exampleArray->initialize(XdmfArrayType::Int32(), newSize);

	if (exampleArray->isInitialized())
	{
		//do whatever is to be done if the array is initialized
	}

	if (!exampleArray->isInitialized())
	{
		exampleArray->read();
	}

	int initArray [10] = {0,1,2,3,4,5,6,7,8,9};
	shared_ptr<XdmfArray> tempArray = XdmfArray::New();
	exampleArray->insert(0, initArray, 10, 1, 1);
	//exampleArray now contains {0,1,2,3,4,5,6,7,8,9}
	exampleArray->insert(0, initArray, 0, 5, 2, 1);
	//exampleArray now contains {0,1,1,3,2,5,3,7,4,9}
	examleArray->insert(0, initArray, 0, 5, 1, 2);
	//exampleArray now contains {0,2,4,6,8,5,3,7,4,9}
	tempArray->insert(0, exampleArray, 0, 10, 1, 1);
	//tempArray now contains {0,1,2,3,4,5,6,7,8,9}
	tempArray->insert(0, exampleArray, 0, 5, 2, 1);
	//tempArray now contains {0,1,1,3,2,5,3,7,4,9}
	tempArray->insert(0, exampleArray, 0, 5, 1, 2);
	//tempArray now contains {0,2,4,6,8,5,3,7,4,9}

	int newIndex = 0;
	double newValue = 3.5;
	exampleArray->pushBack(newValue);
	exampleArray->insert(newIndex, newValue);//the value of 3.5 is inserted at index 0

	exampleArray->setValuesInternal(&initArray, 10, 1);

	std::vector<int> initVector;
	initVector.push_back(1);
	initVector.push_back(2);
	initVector.push_back(3);
	initVector.push_back(4);
	initVector.push_back(5);
	exampleArray->setValuesInternal(initVector, 1);

	shared_ptr<std::vector> storeVector(&initVector);
	exampleArray->setValuesInternal(storeVector);

	//The vector contains {1,2,3,4,5} and the XdmfArray contains {0,1,2,3,4,5,6,7,8,9}
	bool swapSucceded = exampleArray->swap(initVector);
	//The vector contains {0,1,2,3,4,5,6,7,8,9} and the XdmfArray contains {1,2,3,4,5}
	bool storeSwapSucceded = exampleArray->swap(storeVector);
	//storeVector contains {0,1,2,3,4,5,6,7,8,9} and the XdmfArray contains {1,2,3,4,5}
	shared_ptr<XdmfArray> swapArray = XdmfArray::New();
	int initArray2 [5] = {1,2,3,4,5};
	swapArray->insert(0, initArray2, 5, 1, 1);
	//exampleArray contains {0,1,2,3,4,5,6,7,8,9} and swapArray contains {1,2,3,4,5}
	exampleArray->swap(swapArray);
	//Now exampleArray contains {1,2,3,4,5} and swapArray contains {0,1,2,3,4,5,6,7,8,9}

	exampleArray->clear();

	int storeArray [10] = {0,1,2,3,4,5,6,7,8,9};
	exampleArray->insert(0, storeArray, 10, 1, 1);
	int readArray [10] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
	exampleArray->getValues(0, readArray, 5, 1, 2);
	//readArray now contains {0, 11, 1, 13, 2, 15, 3, 17, 4, 19}
	exampleArray->getValues(0, readArray, 5, 2, 1);
	//readArray now contains {0, 2, 4, 6, 8, 15, 3, 17, 4, 19}

	newSize = 20
	int baseValue = 1
	exampleArray->resize(newSize, baseValue)
	//exampleArray now contains {0,1,2,3,4,5,6,7,8,9,1,1,1,1,1,1,1,1,1,1}
	newSize = 5;
	exampleArray->resize(newSize, baseValue)
	//exampleArray now contains {0,1,2,3,4}

	exampleArray->resize(newSizeVector, baseValue)
	//exampleArray now contains {0,1,2,3,4,5,6,7,8,9,1,1,1,1,1,1,1,1,1,1}
	newSizeVector[0] = 1;
	exampleArray->resize(newSizeVector, baseValue)
	//exampleArray now contains {0,1,2,3,4}



	unsigned int exampleCapacity = exampleArray->getCapacity();

	std::vector<unsigned int> exampleDimensions = exampleArray->getDimensions();

	std::string exampleDimensionString = exampleArray->getDimensionsString();

	shared_ptr<const XdmfArrayType> exampleType = exampleArray->getArrayType();

	std::string exampleName = exampleArray->getName();

	unsigned int exampleSize = exampleArray->getSize();

	shared_ptr<XdmfHeavyDataController> exampleController = exampleArray->getHeavyDataController();
	newArray = XdmfArray.New()
	newArray.setHeaveyDataController(exampleController)
	shared_ptr<const XdmfHeavyDataController> exampleControllerConst = exampleArray->getHeavyDataController();

	//if exampleArray contains [0, 1, 2, 3, 4, 5, 6, 7]
	int exampleValue = exampleArray->getValue(4);
	//exampleValue now has the value of what was stored at index 4, which in this case is 4

	std::string exampleValueString = exampleArray->getValuesString();

	//assuming that exampleArray is filled with ints
	shared_ptr<std::vector<int> > exampleInternalVector = exampleArray->getValuesInternal();
	void * exampleInternalPointer = exampleArray->getValuesInternal();
	const void * exampleInternalPointerConst = exampleArray->getValuesInternal();

	//if exampleArray contains [0, 1, 2, 3, 4, 5, 6, 7]
	unsigned int erasedIndex = 4;
	exampleArray->erase(erasedIndex);
	//exampleArray now contains the following
	// [0, 1, 2, 3, 5, 6, 7]

	exampleArray->release();

	return 0;
}
