from Xdmf import *


def maximum(values):
    # Need to cast to the right data type
    values = ArrayVector(values)
    if values[0].getArrayType() == XdmfArrayType.String():
        returnArray = XdmfArray.New()
        returnArray.pushBackAsString(values[0].getValueAsString(0))
        return returnArray
    else:
        maxVal = values[0].getValueAsFloat64(0)
        for i in range(0, values.size()):
            for j in range(0, values[i].getSize()):
                if maxVal < values[i].getValueAsFloat64(j):
                    maxVal = values[i].getValueAsFloat64(j)
        returnArray = XdmfArray.New()
        returnArray.pushBackAsFloat64(maxVal)
        return returnArray


def prepend(val1, val2):
    # Need to cast to the right data type
    # Had to write a custom casting method to integrate it properly
    val1 = XdmfArray.XdmfArrayPtr(val1)
    val2 = XdmfArray.XdmfArrayPtr(val2)
    returnArray = XdmfArray.New()
    returnArray.insert(0, val2, 0, val2.getSize())
    returnArray.insert(val2.getSize(), val1, 0, val1.getSize())
    return returnArray


if __name__ == "__main__":
    functionExpression = "MAX(A,B)@(A#B)"

    numOperations = XdmfFunction.addOperation("@", prepend, 2)
    numFunctions = XdmfFunction.addFunction("MAX", maximum)

    functionVariables = ArrayMap()

    variable1 = XdmfArray.New()

    for i in range(0, 15):
        variable1.pushBackAsInt32(i * 3)

    functionVariables["A"] = variable1

    variable2 = XdmfArray.New()

    for i in range(0, 15):
        variable2.pushBackAsInt32(i * 5)

    functionVariables["B"] = variable2

    testFunction = XdmfFunction.New(functionExpression, functionVariables)

    functionHolder = XdmfAttribute.New()

    functionHolder.setReference(testFunction)

    functionHolder.setReadMode(XdmfArray.Reference)

    functionWriter = XdmfWriter.New("function.xmf")

    functionHolder.accept(functionWriter)

    functionHolder.readReference()

    print(functionHolder.getValuesString())

    functionReader = XdmfReader.New()

    readItem = functionReader.read("function.xmf")

    print(readItem.getItemTag() + " ?= Attribute")

    assert readItem.getItemTag() == "Attribute"

    readFunctionHolder = readItem

    readFunctionHolder.readReference()

    print(
        readFunctionHolder.getValuesString()
        + " ?= "
        + functionHolder.getValuesString()
    )

    assert (
        readFunctionHolder.getValuesString()
        == functionHolder.getValuesString()
    )
