#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include <vector>
#include <map>

//#declarefunction begin

shared_ptr<XdmfArray> maximum(std::vector<shared_ptr<XdmfArray> > values);

//#declarefunction end

//#declareoperation begin

shared_ptr<XdmfArray> prepend(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2);

//#declareoperation end

//#programstart begin

int main(int, char **)
{

	//#programstart end

	//#initialization begin

	shared_ptr<XdmfArray> exampleArray = XdmfArray::New();

	//#initialization end

	//#setName begin

	std::string newName = "New Name";
	exampleArray->setName(newName);

	//#setName end

	//#sizedeclaration begin

	int newSize = 10;
	
	//#sizedeclaration end

	//#reserve begin

	exampleArray->reserve(newSize);

	//#reserve end

	//#initializesingletemplate begin

	shared_ptr<std::vector<int> > exampleVector = exampleArray->initialize<int>(newSize);

	//#initializesingletemplate end

	//#initializesingletype begin

	exampleArray->initialize(XdmfArrayType::Int32(), newSize);

	//#initializesingletype end

	//#sizevectordeclaration begin

	std::vector<unsigned int> newSizeVector;
	newSizeVector.push_back(4);
	newSizeVector.push_back(5);

	//#sizevectordeclaration end

	//#initializevectortemplate begin

	shared_ptr<std::vector<int> > exampleVectorFromVector = exampleArray->initialize<int>(newSizeVector);

	//#initializevectortemplate end

	//#initializevectortype begin

	exampleArray->initialize(XdmfArrayType::Int32(), newSize);

	//#initializevectortype end

	//#isInitialized begin

	if (exampleArray->isInitialized())
	{
		//do whatever is to be done if the array is initialized
	}

	//#isInitialized end

	//#read begin

	if (!exampleArray->isInitialized())
	{
		exampleArray->read();
	}

	//#read end

	//#datapointersetup begin

	int initArray [10] = {0,1,2,3,4,5,6,7,8,9};

	//#datapointersetup end

	//#pointerinsert begin

	exampleArray->insert(0, initArray, 10, 1, 1);
	//exampleArray now contains {0,1,2,3,4,5,6,7,8,9}
	exampleArray->insert(0, initArray, 5, 2, 1);
	//exampleArray now contains {0,1,1,3,2,5,3,7,4,9}
	exampleArray->insert(0, initArray, 5, 1, 2);
	//exampleArray now contains {0,2,4,6,8,5,3,7,4,9}

	//#pointerinsert end

	//#arrayinsert begin

	shared_ptr<XdmfArray> tempArray = XdmfArray::New();
	tempArray->insert(0, exampleArray, 0, 10, 1, 1);
	//tempArray now contains {0,1,2,3,4,5,6,7,8,9}
	tempArray->insert(0, exampleArray, 0, 5, 2, 1);
	//tempArray now contains {0,1,1,3,2,5,3,7,4,9}
	tempArray->insert(0, exampleArray, 0, 5, 1, 2);
	//tempArray now contains {0,2,4,6,8,5,3,7,4,9}

	//#arrayinsert end

	//#pointinsertvalues begin

	double newValue = 3.5;

	//#pointinsertvalues end

	//#pushBack begin

	exampleArray->pushBack(newValue);

	//#pushBack end

	//#pointinsert begin

	int newIndex = 0;
	exampleArray->insert(newIndex, newValue);
	//the value of 3.5 is inserted at index 0

	//#pointinsert end

	//#setValuesInternalpointer begin

	exampleArray->setValuesInternal(initArray, 10, 1);

	//#setValuesInternalpointer end

	//#initinternalvector begin

	std::vector<int> initVector;
	initVector.push_back(1);
	initVector.push_back(2);
	initVector.push_back(3);
	initVector.push_back(4);
	initVector.push_back(5);

	//#initinternalvector end

	//#setValuesInternalvector begin

	exampleArray->setValuesInternal(initVector, 1);

	//#setValuesInternalvector end

	//#initsharedvector begin

	shared_ptr<std::vector<int> > storeVector(&initVector);

	//#initsharedvector end

	//#setValuesInternalsharedvector begin

	exampleArray->setValuesInternal(storeVector);

	//#setValuesInternalsharedvector end

	//#setarraybase begin

        exampleArray->insert(0, initArray, 10, 1, 1);
        //exampleArray now contains {0,1,2,3,4,5,6,7,8,9}

	//#setarraybase end

	//#swapvector begin

	//The vector contains {1,2,3,4,5} and the XdmfArray contains {0,1,2,3,4,5,6,7,8,9}
	bool swapSucceded = exampleArray->swap(initVector);
	//The vector contains {0,1,2,3,4,5,6,7,8,9} and the XdmfArray contains {1,2,3,4,5}

	//#swapvector end

	//#swapsharedvector begin

	//The vector contains {1,2,3,4,5} and the XdmfArray contains {0,1,2,3,4,5,6,7,8,9}
	bool storeSwapSucceded = exampleArray->swap(storeVector);
	//storeVector contains {0,1,2,3,4,5,6,7,8,9} and the XdmfArray contains {1,2,3,4,5}

	//#swapsharedvector end

	//#swaparray begin

	shared_ptr<XdmfArray> swapArray = XdmfArray::New();
	int initArray2 [5] = {1,2,3,4,5};
	swapArray->insert(0, initArray2, 5, 1, 1);
	//exampleArray contains {0,1,2,3,4,5,6,7,8,9} and swapArray contains {1,2,3,4,5}
	exampleArray->swap(swapArray);
	//Now exampleArray contains {1,2,3,4,5} and swapArray contains {0,1,2,3,4,5,6,7,8,9}

	//#swaparray end

	//#clear begin

	exampleArray->clear();

	//#clear end

	//#getValues begin

	int storeArray [10] = {0,1,2,3,4,5,6,7,8,9};
	exampleArray->insert(0, storeArray, 10, 1, 1);
	int readArray [10] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
	exampleArray->getValues(0, readArray, 5, 1, 2);
	//readArray now contains {0, 11, 1, 13, 2, 15, 3, 17, 4, 19}
	exampleArray->getValues(0, readArray, 5, 2, 1);
	//readArray now contains {0, 2, 4, 6, 8, 15, 3, 17, 4, 19}

	//#getValues end

	//#resizesingle begin

	newSize = 20;
	int baseValue = 1;
	exampleArray->resize(newSize, baseValue);
	//exampleArray now contains {0,1,2,3,4,5,6,7,8,9,1,1,1,1,1,1,1,1,1,1}
	newSize = 5;
	exampleArray->resize(newSize, baseValue);
	//exampleArray now contains {0,1,2,3,4}

	//#resizesingle end

	//#resizevector begin

	std::vector<unsigned int> newresizeVector;
	newResizeVector.push_back(4);
	newResizeVector.push_back(5);	

	exampleArray->resize(newResizeVector, baseValue);
	//exampleArray now contains {0,1,2,3,4,5,6,7,8,9,1,1,1,1,1,1,1,1,1,1}
	newResizeVector[0] = 1;
	exampleArray->resize(newResizeVector, baseValue);
	//exampleArray now contains {0,1,2,3,4}

	//#resizevector end

	//#getCapacity begin

	unsigned int exampleCapacity = exampleArray->getCapacity();

	//#getCapacity end

	//#getDimensions begin

	std::vector<unsigned int> exampleDimensions = exampleArray->getDimensions();

	//#getDimensions end

	//#getDimensionsString begin

	std::string exampleDimensionString = exampleArray->getDimensionsString();

	//#getDimensionsString end

	//#getArrayType begin

	shared_ptr<const XdmfArrayType> exampleType = exampleArray->getArrayType();

	//#getArrayType end

	//#getName begin

	std::string exampleName = exampleArray->getName();

	//#getName end

	//#getSize begin

	unsigned int exampleSize = exampleArray->getSize();

	//#getSize end

	//#getHeavyDataController begin

	shared_ptr<XdmfHeavyDataController> exampleController = exampleArray->getHeavyDataController();

	//#getHeavyDataController end

	//#setHeavyDataController begin

	shared_ptr<XdmfArray> newArray = XdmfArray::New();
	newArray->setHeavyDataController(exampleController);

	//#setHeavyDataController end

	//#getHeavyDataControllerconst begin

	shared_ptr<const XdmfHeavyDataController> exampleControllerConst = exampleArray->getHeavyDataController();

	//#getHeavyDataControllerconst end

	//#getValueindex begin

	//if exampleArray contains {0,1,2,3,4,5,6,7,8,9}
	int exampleValue = exampleArray->getValue<int>(4);
	//exampleValue now has the value of what was stored at index 4, which in this case is 4

	//#getValueindex end

	//#getValuesString begin

	std::string exampleValueString = exampleArray->getValuesString();

	//#getValuesString end

	//#getValuesInternalvector begin

	shared_ptr<std::vector<int> > exampleInternalVector = exampleArray->getValuesInternal<int>();

	//#getValuesInternalvector end

	//#getValuesInternalvoid begin

	void * exampleInternalPointer = exampleArray->getValuesInternal();

	//#getValuesInternalvoid end

	//#getValuesInternalvoidconst begin

	const void * exampleInternalPointerConst = exampleArray->getValuesInternal();

	//#getValuesInternalvoidconst end

	//#erase begin

	//if exampleArray contains {0,1,2,3,4,5,6,7,8,9}
	unsigned int erasedIndex = 4;
	exampleArray->erase(erasedIndex);
	//exampleArray now contains the following
	// {0,1,2,3,5,6,7,8,9}

	//#erase end

	//#release begin

	exampleArray->release();

	//#release end

	//#getSupportedOperations begin

	std::string exampleOperations = XdmfFunction::getSupportedOperations();

	//#getSupportedOperations end

	//#getSupportedFunctions begin

	std::vector<std::string> exampleFunctions = XdmfFunction::getSupportedFunctions();

	//#getSupportedFunctions end

	//#getValidVariableChars begin

	std::string exampleVariableChars = XdmfFunction::getValidVariableChars();

	//#getValidVariableChars end

	//#getValidDigitChars begin

	std::string exampleDigitChars = XdmfFunction::getValidDigitChars();

	//#getValidDigitChars end

	//#getOperationPriority begin

	int examplePriority = XdmfFunction::getOperationPriority('|');

	//#getOperationPriority end

	//#valueinit begin

	shared_ptr<XdmfArray> valueArray1 = XdmfArray::New();
	valueArray1->pushBack(1);
	valueArray1->pushBack(2);
	valueArray1->pushBack(3);
	valueArray1->pushBack(4);
	shared_ptr<XdmfArray> valueArray2 = XdmfArray::New();
	valueArray2->pushBack(9);
	valueArray2->pushBack(8);
	valueArray2->pushBack(7);
	valueArray2->pushBack(6);

	std::vector<shared_ptr<XdmfArray> > valueVector;
	valueVector.push_back(valueArray1);
	valueVector.push_back(valueArray2);

	shared_ptr<XdmfArray> answerArray;

	//#valueinit end

	//#sum begin

	answerArray = XdmfFunction::sum(valueVector);

	//#sum end

	//#average begin

	answerArray = XdmfFunction::average(valueVector);

	//#average end

	//#chunk begin

	answerArray = XdmfFunction::chunk(valueArray1, valueArray2);

	//#chunk end

	//#interlace begin

	answerArray = XdmfFunction::interlace(valueArray1, valueArray2);

	//#interlace end

	//#addOperation begin

	int exampleNumOperations = XdmfFunction::addOperation('@', (shared_ptr<XdmfArray>(*)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>))prepend, 2);

	//#addOperation end

	//#evaluateOperation begin

	answerArray = XdmfFunction::evaluateOperation(valueArray1, valueArray2, '@');

	//#evaluateOperation end

	//#addFunction begin

	int exampleNumFunctions = XdmfFunction::addFunction("MAX", (shared_ptr<XdmfArray>(*)(std::vector<shared_ptr<XdmfArray> >))maximum);

	//#addFunction end

	//#evaluateFunction begin

	answerArray = XdmfFunction::evaluateFunction(valueVector, "MAX");

	//#evaluateFunction end

	//#evaluateExpression begin

	std::map<std::string, shared_ptr<XdmfArray> > valueMap;
	valueMap["A"] = valueArray1;
	valueMap["B"] = valueArray2;

	std::string parsedExpression = "MAX(A,B)@(A#B)";
	answerArray = XdmfFunction::evaluateExpression(parsedExpression, valueMap);

	//#evaluateExpression end

	//#insertmultidim begin

        shared_ptr<XdmfArray> writtenArray = XdmfArray::New();
        std::vector<unsigned int> dimensionVector;
        dimensionVector.push_back(5);
        dimensionVector.push_back(4);
        writtenArray->initialize<int>(dimensionVector);
        for (int i = 0; i < 20; ++i)
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

	//#insertmultidim end

	//#programend start

	return 0;
}

//#programend end

//#definefunction begin

shared_ptr<XdmfArray> maximum(std::vector<shared_ptr<XdmfArray> > values)
{
	if (values[0]->getArrayType() == XdmfArrayType::String())
	{
		shared_ptr<XdmfArray> returnArray = XdmfArray::New();
		returnArray->pushBack(values[0]->getValue<std::string>(0));
		return returnArray;
	}
	else
	{
		double maxVal = values[0]->getValue<double>(0);
		for (int i = 0; i < values.size(); ++i)
		{
			for (int j = 0; j < values[i]->getSize(); ++j)
			{
				if (maxVal < values[i]->getValue<double>(j))
				{
					maxVal = values[i]->getValue<double>(j);
				}
			}
		}
		shared_ptr<XdmfArray> returnArray = XdmfArray::New();
		returnArray->pushBack(maxVal);
		return returnArray;
	}
}

//#definefunction end

//#defineoperation begin

shared_ptr<XdmfArray> prepend(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2)
{
	//joins into new array and returns it
	shared_ptr<XdmfArray> returnArray = XdmfArray::New();
	returnArray->insert(0, val2, 0, val2->getSize(),  1, 1);
	returnArray->insert(val2->getSize(), val1, 0, val1->getSize(), 1, 1);
	return returnArray;
}

//#defineoperation end
