from Xdmf import *


def maximum(values):
	values = ArrayVector(values)#need to cast to the right data type
        if values[0].getArrayType() == XdmfArrayType.String():
                returnArray = XdmfArray.New()
                returnArray.pushBackAsString(values[0].getValueAsString(0))
                return returnArray;
        else:
                maxVal = values[0].getValueAsFloat64(0)
                for i in range (0, values.size()):
                        for j in range (0, values[i].getSize()):
                                if maxVal < values[i].getValueAsFloat64(j):
                                        maxVal = values[i].getValueAsFloat64(j)
                returnArray = XdmfArray.New()
                returnArray.pushBackAsFloat64(maxVal)
                return returnArray

def prepend(val1, val2):
	val1 = XdmfArray.XdmfArrayPtr(val1)#need to cast to the right data type
	val2 = XdmfArray.XdmfArrayPtr(val2)#had to write a custom casting method to integrate it properly
	returnArray = XdmfArray.New()
	returnArray.insert(0, val2, 0, val2.getSize())
	returnArray.insert(val2.getSize(), val1, 0, val1.getSize())
	return returnArray

if __name__ == "__main__":
	#functions are passed as callable pyobjects
	XdmfFunction.addFunction("MAX", maximum)
	functionVector = XdmfFunction.getSupportedFunctions()
	for i in range (0, functionVector.size()):
		print(functionVector[i])
	XdmfFunction.addOperation("@", prepend, 2)
	print(XdmfFunction.getSupportedOperations())
        testVector = ArrayVector()
	testArray = XdmfArray.New()
	testArray.pushBackAsInt32(10)
	testArray.pushBackAsInt32(9)
	testVector.push_back(testArray)
	print("before evaluating function")
	resultArray = XdmfFunction.evaluateFunction(testVector, "MAX")
	print(type(resultArray))
	print("after function is evaulated")
	print(resultArray.getValuesString())
	print("before evaluating function")
	resultArray = XdmfFunction.evaluateFunction(testVector, "AVE")
	print(type(resultArray))
	print("after function is evaulated")
	print(resultArray.getValuesString())
	testArray2 = XdmfArray.New()
	testArray2.pushBackAsInt32(1)
	testArray2.pushBackAsInt32(2)
	print("before evaluating Operation")
	resultArray = XdmfFunction.evaluateOperation(testArray, testArray2, "|")
	print(type(resultArray))
	print("after evaluationg Operation")
	print(resultArray.getValuesString())
	resultArray = XdmfFunction.evaluateOperation(testArray, testArray2, "@")
	print(type(resultArray))
	print("after evaluationg Operation")
	print(resultArray.getValuesString())
	print("before evaluating expression")
	testMap = ArrayMap()
	testMap["A"] = testArray
	testMap["B"] = testArray2
	testArray3 = XdmfArray.New()
	testArray3.pushBackAsInt32(5)
	testArray3.pushBackAsInt32(5)
	testArray3.pushBackAsInt32(5)
	testArray3.pushBackAsInt32(5)
	testMap["C"] = testArray3
	resultArray = XdmfFunction.evaluateExpression("A|B#C", testMap)
	print(type(resultArray))
	print(resultArray.getValuesString())
	print("after evaluating expression")
