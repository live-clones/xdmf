from Xdmf import *
from numpy import *

#//definefunction begin

def maximum(values):
        # Need to cast to the right data type
        values = ArrayVector(values)
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

#//definefunction end

#//defineoperation begin

def prepend(val1, val2):
        # Need to cast to the right data type
        # Had to write a custom casting method to integrate it properly
        val1 = XdmfArray.XdmfArrayPtr(val1)
        val2 = XdmfArray.XdmfArrayPtr(val2)
        returnArray = XdmfArray.New()
        returnArray.insert(0, val2, 0, val2.getSize())
        returnArray.insert(val2.getSize(), val1, 0, val1.getSize())
        return returnArray

#//defineoperation end

#//programstart begin

if __name__ == "__main__":

        #//programstart end

        #//getSupportedOperations begin

        exampleOperations = XdmfFunction.getSupportedOperations()

        #//getSupportedOperations end

        #//getSupportedFunctions begin

        exampleFunctions = XdmfFunction.getSupportedFunctions()

        #//getSupportedFunctions end

        #//getValidVariableChars begin

        exampleVariableChars = XdmfFunction.getValidVariableChars()

        #//getValidVariableChars end

        #//getValidDigitChars begin

        exampleDigitChars = XdmfFunction.getValidDigitChars()

        #//getValidDigitChars end

        #//getOperationPriority begin

        examplePriority = XdmfFunction.getOperationPriority('|')

        #//getOperationPriority end

        #//valueinit begin

        valueArray1 = XdmfArray.New()
        valueArray1.pushBackAsInt32(1)
        valueArray1.pushBackAsInt32(2)
        valueArray1.pushBackAsInt32(3)
        valueArray1.pushBackAsInt32(4)
        valueArray2 = XdmfArray.New()
        valueArray2.pushBackAsInt32(9)
        valueArray2.pushBackAsInt32(8)
        valueArray2.pushBackAsInt32(7)
        valueArray2.pushBackAsInt32(6)

        valueVector = ArrayVector()
        valueVector.push_back(valueArray1)
        valueVector.push_back(valueArray2)

        #//valueinit end

        #//sum begin

        answerArray = XdmfFunction.sum(valueVector)

        #//sum end

        #//average begin

        answerArray = XdmfFunction.average(valueVector)

        #//average end

        #//chunk begin

        answerArray = XdmfFunction.chunk(valueArray1, valueArray2)

        #//chunk end

        #//interlace begin

        answerArray = XdmfFunction.interlace(valueArray1, valueArray2)

        #//interlace end

        #//addOperation begin

        exampleNumOperations = XdmfFunction.addOperation('@', prepend, 2)

        #//addOperation end

        #//evaluateOperation begin

        answerArray = XdmfFunction.evaluateOperation(valueArray1, valueArray2, '@')

        #//evaluateOperation end

        #//addFunction begin

        exampleNumFunctions = XdmfFunction.addFunction("MAX", maximum)

        #//addFunction end

        #//evaluateFunction begin

        answerArray = XdmfFunction.evaluateFunction(valueVector, "MAX")

        #//evaluateFunction end

        #//evaluateExpression begin

        valueMap = ArrayMap()
        valueMap["A"] = valueArray1
        valueMap["B"] = valueArray2

        parsedExpression = "MAX(A,B)@(A#B)"
        answerArray = XdmfFunction.evaluateExpression(parsedExpression, valueMap)

        #//evaluateExpression end

        #//initialization begin

        testFunction = XdmfFunction.New()

        #//initalization end

        #//initexpression begin

        functionExpression = "AVE(A)"
        variableArray = XdmfArray.New()

        for i in range(0, 10):
                variableArray.pushBackAsInt32(i)

        variableMap = ArrayMap()
        variableMap["A"] = variableArray;

        exampleFunction = XdmfFunction.New(functionExpression, variableMap)

        #//initexpression end

        #//setExpression begin

        newExpression = "SUM(A)"
        exampleFunction.setExpression(newExpression)

        #//setExpression end

        #//getExpression begin

        exampleExpression = exampleFunction.getExpression()

        #//getExpression end

        #//insertVariable begin

        secondVariableArray = XdmfArray.New()

        for i in range(0, 10):
                secondVariableArray.pushBack(i)

        exampleFunction.insertVariable("B", secondVariableArray)

        #//insertVariable end

        #//getVariable begin

        variableValue = exampleFunction->getVariable("B")

        #//getVariable end

        #//getVariableList begin

        exampleVarList = exampleFunction.getVariableList()

        #//getVariableList end

        #//removeVariable begin

        exampleFunction.removeVariable("B")

        #//removeVariable end

        #//setConstructedType begin

        typeAttribute = XdmfAttribute.New()

        exampleFunction.setConstructedType(typeAttribute.getItemTag())

        #//setConstructedType end

        #//getConstructedType begin

        std::string exampleType = exampleFunction->getConstructedType();

        #//getConstructedType end

        #//setConstructedProperties begin

        propertyAttribute = XdmfAttributeNew()
        exampleFunction.setConstructedProperties(propertyAttribute.getItemProperties())

        #//setConstructedProperties end

        #//getConstructedProperties begin

        exampleProperties = exampleFunction.getConstructedProperties()

        #//getConstructedProperties end

        #//read begin

        functionResult = exampleFunction.read()

        #//read end
