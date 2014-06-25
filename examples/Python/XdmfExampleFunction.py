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

        #//abs begin

        toBeAbs = XdmfArray.New()
        toBeAbs.pushBackAsFloat64(-5.5)
        absVector = ArrayVector()
        absVector.push_back(toBeAbs)
        absResult = XdmfFunction.abs(absVector)
        assert absResult.getValuesString() == "5.5"

        #//abs end

        #//arcsin begin

        toBeArcsin = XdmfArray.New()
        toBeArcsin.pushBackAsFloat64(-0.5)
        arcsinVector = ArrayVector()
        arcsinVector.push_back(toBeArcsin)
        arcsinResult = XdmfFunction.arcsin(arcsinVector)
        assert arcsinResult.getValuesString() == "-0.52359877559829893"

        #//arcsin end

        #//arccos begin

        toBeArccos = XdmfArray.New()
        toBeArccos.pushBackAsFloat64(-0.5)
        arccosVector = ArrayVector()
        arccosVector.push_back(toBeArccos)
        arccosResult = XdmfFunction.arccos(arccosVector)
        assert arccosResult.getValuesString() == "2.0943951023931957"

        #//arccos end

        #//cos begin

        toBeCos = XdmfArray.New()
        toBeCos.pushBackAsFloat64(-0.5)
        cosVector = ArrayVector()
        cosVector.push_back(toBeCos)
        cosResult = XdmfFunction.cos(cosVector)
        assert cosResult.getValuesString() == "0.87758256189037276"

        #//cos end

        #//sin begin

        toBeSin = XdmfArray.New()
        toBeSin.pushBackAsFloat64(-0.5)
        sinVector = ArrayVector()
        sinVector.push_back(toBeSin)
        sinResult = XdmfFunction.sin(sinVector)
        assert sinResult.getValuesString() == "-0.47942553860420301"

        #//sin end

        #//tan begin

        toBeTan = XdmfArray.New()
        toBeTan.pushBackAsFloat64(-0.5)
        tanVector = ArrayVector()
        tanVector.push_back(toBeTan)
        tanResult = XdmfFunction.tan(tanVector)
        assert tanResult.getValuesString() == "-0.54630248984379048"

        #//tan end

        #//sqrt begin

        toBeSqrt = XdmfArray.New()
        toBeSqrt.pushBackAsFloat64(2)
        sqrtVector = ArrayVector()
        sqrtVector.push_back(toBeSqrt)
        sqrtResult = XdmfFunction.sqrt(sqrtVector)
        assert sqrtResult.getValuesString() == "1.4142135623730951"

        #//sqrt end

        #//log begin

        toBeLog = XdmfArray.New()
        toBeLog.pushBackAsFloat64(2)
        logBase = XdmfArray.New()
        logBase.pushBackAsFloat64(4)
        logVector = ArrayVector()
        logVector.push_back(toBeLog)
        logVector.push_back(logBase)
        logResult = XdmfFunction.log(logVector)
        assert logResult.getValuesString() == "0.5"

        #//log end

        #//exp begin

        powerBase = XdmfArray.New()
        powerBase.pushBackAsFloat64(-0.5)
        powerFactor = XdmfArray.New()
        powerFactor.pushBackAsFloat64(2)
        expVector = ArrayVector()
        expVector.push_back(powerBase)
        expVector.push_back(powerFactor)
        expResult = XdmfFunction.exponent(expVector)
        assert expResult.getValuesString() == "0.25"

        #//exp end

        #//join begin

        array1 = XdmfArray.New()
        array1.pushBackAsFloat64(-0.5)
        array2 = XdmfArray.New()
        array2.pushBackAsFloat64(2)
        joinVector = ArrayVector()
        joinVector.push_back(array1)
        joinVector.push_back(array2)
        joinResult = XdmfFunction.join(joinVector)
        assert joinResult.getValuesString() == "-0.5 2"

        #//join end

        #//addition begin

        addArray1 = XdmfArray.New()
        addArray1.pushBackAsFloat64(-0.5)
        addArray2 = XdmfArray.New()
        addArray2.pushBackAsFloat64(2)
        addResult = XdmfFunction.addition(addArray1, addArray2)
        assert addResult.getValuesString() == "1.5"

        #//addition end

        #//division begin

        divArray1 = XdmfArray.New()
        divArray1.pushBackAsFloat64(-0.5)
        divArray2 = XdmfArray.New()
        divArray2.pushBackAsFloat64(2)
        divResult = XdmfFunction.division(divArray1, divArray2)
        assert divResult.getValuesString() == "-0.25"

        #//division end

        #//multiplication begin

        mulArray1 = XdmfArray.New()
        mulArray1.pushBackAsFloat64(-0.5)
        mulArray2 = XdmfArray.New()
        mulArray2.pushBackAsFloat64(2)
        mulResult = XdmfFunction.multiplication(mulArray1, mulArray2)
        assert mulResult.getValuesString() == "-1"

        #//multiplication end

        #//subtraction begin

        subArray1 = XdmfArray.New()
        subArray1.pushBackAsFloat64(-0.5)
        subArray2 = XdmfArray.New()
        subArray2.pushBackAsFloat64(2)
        subResult = XdmfFunction.subtraction(subArray1, subArray2)
        assert subResult.getValuesString() == "-2.5"

        #//subtraction end
