#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfFunction.hpp"
#include <vector>
#include <map>

int main(int, char **)
{
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

        //#readController begin

        newArray->readController();

        //#readController end

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

        //#setReference begin

        shared_ptr<XdmfArray> variableArray = XdmfArray::New();

        for (unsigned int i = 0; i < 10; ++i)
        {
                variableArray->pushBack(i);
        }

        std::map<std::string, shared_ptr<XdmfArray> > variableMap;

        variableMap["A"] = variableArray;

        shared_ptr<XdmfFunction> arrayFunction = XdmfFunction::New("AVE(A)", variableMap);

	// Using an XdmfFunction as an example but any array reference will work.

        exampleArray->setReference(arrayFunction);

        //#setReference end

        //#getReference begin

        shared_ptr<XdmfFunction> exampleFunction = shared_dynamic_cast<XdmfFunction>(exampleArray->getReference());

        //#getReference end

        //#readReference begin

        exampleArray->readReference();

        //#readReference end

        //#setReadMode begin

        exampleArray->setReadMode(XdmfArray::Reference);

        //#setReadMode end

        //#getReadMode begin

        ReadMode isReference = exampleArray->getReadMode();

        if (isReference == XdmfArray::Reference)
        {
                exampleArray->readReference();
        }

        //#getReadMode end

        return 0;
}
