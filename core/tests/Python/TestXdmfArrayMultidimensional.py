from XdmfCore import *

if __name__ == "__main__":

    #
    # Create 1D arrays
    #
    array1 = XdmfArray.New()
    array1.resizeAsUInt32(2)
    print str(array1.getArrayType()) + " ?= " + str(XdmfArrayType.UInt32())
    print str(array1.getSize()) + " ?= " + str(2)
    assert array1.getArrayType() == XdmfArrayType.UInt32()
    assert array1.getSize() == 2
    dimensions = array1.getDimensions()
    print str(dimensions.size()) + " ?= " + str(1)
    print str(dimensions[0]) + " ?= " + str(2)
    assert dimensions.size() == 1
    assert dimensions[0] == 2
    dimensionsString = array1.getDimensionsString()
    print dimensionsString + " ?= 2"
    assert dimensionsString == "2"
    array1.resizeAsInt32(3)
    print str(array1.getSize()) + " ?= " + str(3)
    assert array1.getSize() == 3
    dimensions = array1.getDimensions()
    print str(dimensions.size()) + " ?= " + str(1)
    print str(dimensions[0]) + " ?= " + str(3)
    assert dimensions.size() == 1
    assert dimensions[0] == 3

    #
    # Create 2D arrays
    #
    array2 = XdmfArray.New()
    newDimensions = UInt32Vector(2, 2)
    array2.initialize(XdmfArrayType.UInt16(), newDimensions)
    print str(array2.getArrayType()) + " ?= " + str(XdmfArrayType.UInt16())
    print str(array2.getSize()) + " ?= " + str(4)
    assert array2.getArrayType() == XdmfArrayType.UInt16()
    assert array2.getSize() == 4
    dimensions = array2.getDimensions()
    print str(dimensions.size()) + " ?= " + str(2)
    print str(dimensions[0]) + " ?= " + str(2)
    print str(dimensions[1]) + " ?= " + str(2)
    assert dimensions.size() == 2
    assert dimensions[0] == 2 and dimensions[1] == 2
    dimensionsString = array2.getDimensionsString()
    print dimensionsString + " ?= 2 2" 
    assert dimensionsString == "2 2"

    newDimensions = UInt32Vector(3, 3)
    array2.resizeAsUInt16(newDimensions)
    print str(array2.getSize()) + " ?= " + str(27)
    assert array2.getSize() == 27
    dimensions = array2.getDimensions()
    print str(dimensions.size()) + " ?= " + str(3)
    print str(dimensions[0]) + " ?= " + str(3)
    print str(dimensions[1]) + " ?= " + str(3)
    print str(dimensions[2]) + " ?= " + str(3)
    assert dimensions.size() == 3
    assert dimensions[0] == 3 and dimensions[1] == 3 and dimensions[2] == 3
    dimensionsString = array2.getDimensionsString()
    print dimensionsString + " ?= 3 3 3"
    assert dimensionsString == "3 3 3"

    values = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    array2.insertAsUInt16(0, values)
    print str(array2.getSize()) + " ?= " + str(27)
    assert array2.getSize() == 27
    dimensions = array2.getDimensions()
    print str(dimensions.size()) + " ?= " + str(3)
    print str(dimensions[0]) + " ?= " + str(3)
    print str(dimensions[1]) + " ?= " + str(3)
    print str(dimensions[2]) + " ?= " + str(3)
    assert dimensions.size() == 3
    assert dimensions[0] == 3 and dimensions[1] == 3 and dimensions[2] == 3

    array2.pushBackAsInt16(10)
    print str(array2.getSize()) + " ?= " + str(28)
    assert array2.getSize() == 28
    dimensions = array2.getDimensions()
    print str(dimensions.size()) + " ?= " + str(1)
    print str(dimensions[0]) + " ?= " + str(28)
    assert dimensions.size() == 1
    assert dimensions[0] == 28

