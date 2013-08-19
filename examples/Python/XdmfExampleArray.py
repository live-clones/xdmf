from Xdmf import *
from numpy import *

if __name__ == "__main__":

	#//initialization begin

	exampleArray = XdmfArray.New()

	#//initialization end

	#//setName begin

	newName = "New Name"
	exampleArray.setName(newName)

	#//setName end

	#//reserve begin

	newSize = 10
	exampleArray.reserve(newSize)

	#//reserve end

	#//initializesingle begin

	newSize = 10
	exampleArray.initialize(XdmfArrayType.Int32(), newSize)

	#//initializesingle end

	#//initializevector begin

	newSizeVector = UInt32Vector()
	newSizeVector.push_back(5)
	newSizeVector.push_back(5)
	newSizeVector.push_back(5)
	exampleArray.initialize(XdmfArrayType.Int32(), newSizeVector)

	#//initializevector end

	#//getArrayType begin

	exampleType = exampleArray.getArrayType()

	#//getArrayType end

	#//getCapacity begin

	exampleCapacity = exampleArray.getCapacity()

	#//getCapacity end

	#//getDimensions begin

	exampleDimensions = exampleArray.getDimensions()

	#//getDimensions end

	#//getDimensionsString begin

	exampleDimensionString = exampleArray.getDimensionsString()

	#//getDimensionsString end

	#//getName begin

	exampleName = exampleArray.getName()

	#//getName end

	#//getSize begin

	exampleSize = exampleArray.getSize()

	#//getSize end

	#//insertarray begin

	initArray = [0,1,2,3,4,5,6,7,8,9]
	storeArray = XdmfArray.New()
	exampleArray.insertAsInt32(0, initArray)
	storeArray.insert(0, exampleArray, 0, 10, 1, 1)
	#storeArray now contains {0,1,2,3,4,5,6,7,8,9}
	storeArray.insert(0, exampleArray, 0, 5, 2, 1)
	#storeArray now contains {0,1,1,3,2,5,3,7,4,9}
	storeArray.insert(0, exampleArray, 0, 5, 1, 2)
	#storeArray now contains {0,2,4,6,8,5,3,7,4,9}

	#//insertarray end

	#//insertlist begin

	initArray = [0,1,2,3,4,5,6,7,8,9]
	exampleArray.insertAsInt32(0, initArray)
	#exampleArray now contains {0,1,2,3,4,5,6,7,8,9}
	exampleArray.insertAsInt32(0, initArray[0:5:2])
	#exampleArray now contains {0,2,4,6,8,5,3,7,4,9}
	exampleArray.insertAsInt32(0, initArray[::-1])
	#exampleArray now contains {9,8,7,6,5,4,3,2,1,0}

	#Python uses a different function for each data type
	#This example uses insertAsInt32 to insert ints
	#insertAsFloat64 can also be used to insert doubles
	#This function takes a start index and a list
	#Sublists are inserted using Python's sublist notation

	#//insertlist end

	#//getNumpyArray begin

	outputArray = exampleArray.getNumpyArray()

	#//getNumpyArray end

	#//arraydefaultvalues begin

	initArray = [0,1,2,3,4,5,6,7,8,9]
        exampleArray.insertAsInt32(0, initArray)

	#//arraydefaultvalues end

	#//resizesingle begin

	newSize = 20
	baseValue = 1
	exampleArray.resizeAsInt32(newSize, baseValue)
	#exampleArray now contains {0,1,2,3,4,5,6,7,8,9,1,1,1,1,1,1,1,1,1,1}
	newSize = 5
	exampleArray.resizeAsInt32(newSize, baseValue)
	#exampleArray now contains {0,1,2,3,4}
	#This example uses resizeAsInt32 because the baseValue inserted is to be an integer
	#All other supported data types have similarly named function calls
	#For example to insert a double resizeAsFloat64 is called

	#//resizesingle end

	#//resizevector begin

        newSizeArray = UInt32Vector()
	newSizeArray.push_back(4)
	newSizeArray.push_back(5)
        baseValue = 1
        exampleArray.resizeAsInt32(newSizeArray, baseValue)
        #exampleArray now contains {0,1,2,3,4,5,6,7,8,9,1,1,1,1,1,1,1,1,1,1}
	newSizeArray[0] = 1
        exampleArray.resizeAsInt32(newSizeArray, baseValue)
	#exampleArray now contains {0,1,2,3,4}
	#This example uses resizeAsInt32 because the baseValue inserted is to be an integer
	#All other supported data types have similarly named function calls
	#For example to insert a double resizeAsFloat64 is called

	#//resizevector end

	#//getValueindex begin

	#If exampleArray contains  [0, 1, 2, 3, 4, 5, 6, 7]
	exampleValue = exampleArray.getValueAsInt32(4)
	#exampleValue now equals 4
	#The data type of the returned value can be changed by changing the function name
	#getValueAsInt32 returns an int value while getValueAsFloat64 returns a double value
	#Variations of this function exist for all supported data types

	#//getValueindex end

	#//getValuesparse begin

	exampleValueString = exampleArray.getValuesString()
	valueArray = [float(piece) for piece in exampleArray.getValuesString().split()]
	#This is one method of getting the contained values of the array

	#//getValuesparse end

	#//isInitialized begin

	if not(exampleArray.isInitialized()):
		exampleArray.read()

	#//isInitialized end

	#//getValuesInternal begin

	exampleValues = exampleArray.getValuesInternal()
	#alternatively getBuffer gives a buffer object
	exampleValues = exampleArray.getBuffer()
	#due to the way python handles void pointers, this function is only useful for getting a pointer to pass
	#if the retrieval of the internal values of the array is required, another function should be used

	#//getValuesInternal end

	#//swap begin

	swapArray = XdmfArray.New()
	initArray2 = [1,2,3,4,5]
	swapArray.insertAsInt32(0, initArray2)
	#exampleArray contains {0,1,2,3,4,5,6,7,8,9} and swapArray contains {1,2,3,4,5}
	exampleArray.swap(swapArray)
	#Now exampleArray contains {1,2,3,4,5} and swapArray contains {0,1,2,3,4,5,6,7,8,9}

	#//swap end

	#//pushBack begin

	newValue = 5
	exampleArray.pushBackAsInt32(newValue)
	#For Python pushBack has multiple functions to cover different data types
	#This case used an int so the function was pushBackAsInt32
	#Another example would be to use pushBackAsFloat64 for double values

	#//pushBack end

	#//pointinsert begin

	newIndex = 0
	newValue = 3.5
	exampleArray.insertAsFloat64(newIndex, [newValue])
	#this example uses insertAsFloat64 to insert a double value
	#versions for all other data types exist
	#for example insertAsInt32 inserts as an int

	#//pointinsert end

	#//erase begin

	#If exampleArray contains  [0, 1, 2, 3, 4, 5, 6, 7]
	erasedIndex = 4
	exampleArray.erase(erasedIndex)
	#exampleArray now contains the following
	#[0, 1, 2, 3, 5, 6, 7]

	#//erase end

	#//clear begin

	exampleArray.clear()

	#//clear end

	#//getHeavyDataController begin

	exampleController = exampleArray.getHeavyDataController()

	#//getHeavyDataController end

	#//setHeavyDataController begin

	newArray = XdmfArray.New()
	newArray.setHeavyDataController(exampleController)

	#//setHeavyDataController end

	#//release begin

	exampleArray.release()

	#//release end

	#//insertmultidim begin

	writtenArray = XdmfArray.New()
	dimensionVector = UInt32Vector()
	dimensionVector.push_back(5)
	dimensionVector.push_back(4)
	writtenArray.initializeAsInt32(dimensionVector)
	for i in range (0, 20):
		writtenArray.insert(i, i + 1)
	readArray = XdmfArray.New()
	readDimensionVector = UInt32Vector()
	readDimensionVector.push_back(6)
	readDimensionVector.push_back(4)
	readArray.initializeAsInt32(readDimensionVector)

	writeStarts = UInt32Vector()
	writeStarts.push_back(0)
	writeStarts.push_back(0)
	writeStrides = UInt32Vector()
	writeStrides.push_back(2)
	writeStrides.push_back(2)
	writeDim = UInt32Vector()
	writeDim.push_back(3)
	writeDim.push_back(2)
	readStarts = UInt32Vector()
	readStarts.push_back(0)
	readStarts.push_back(0)
	readStrides = UInt32Vector()
	readStrides.push_back(2)
	readStrides.push_back(2)
	readDim = UInt32Vector()
	readDim.push_back(3)
	readDim.push_back(2)

	readArray.insert(readStarts, writtenArray, writeStarts, writeDim, readDim, readStrides, writeStrides)

	#//insertmultidim end

        #//setFunction begin

	variableArray = XdmfArray.New()

	for i in range(0, 10):
		variableArray.pushBack(i)

	variableMap = ArrayMap()

	variableMap["A"] = variableArray

	arrayFunction = XdmfFunction.New("AVE(A)", variableMap)

	exampleArray.setFunction(exampleFunction)

        #//setFunction end

        #//getFunction begin

	exampleFunction = exampleArray.getFunction()

        #//getFunction end

        #//readFunction begin

        exampleArray.readFunction()

        #//readFunction end

        #//setWriteAsFunction begin

        exampleArray.setWriteAsFunction(True)

        #//setWriteAsFunction end

        #//getWriteAsFunction begin

        isFunction = exampleArray.getWriteAsFunction()

        #//getWriteAsFunction end

