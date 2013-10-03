from XdmfCore import *

if __name__ == "__main__":

    #
    # Create 1D arrays
    #
    array1 = XdmfArray.New()
    array1.initialize(XdmfArrayType.UInt32(), 2)
    assert array1.getArrayType() == XdmfArrayType.UInt32()
    assert array1.getSize() == 2
    dimensions = array1.getDimensions()
    assert dimensions.size() == 1
    assert dimensions[0] == 2
    dimensionsString = array1.getDimensionsString()
    assert dimensionsString == "2"
    array1.resizeAsInt32(3)
    assert array1.getSize() == 3
    dimensions = array1.getDimensions()
    assert dimensions.size() == 1
    assert dimensions[0] == 3

    #
    # Create 2D arrays
    #
    array2 = XdmfArray.New()
    newDimensions = UInt32Vector(2, 2)
    array2.initialize(XdmfArrayType.UInt16(), newDimensions)
    assert array2.getArrayType() == XdmfArrayType.UInt16()
    assert array2.getSize() == 4
    dimensions = array2.getDimensions()
    assert dimensions.size() == 2
    assert dimensions[0] == 2 and dimensions[1] == 2
    dimensionsString = array2.getDimensionsString()
    assert dimensionsString == "2 2"

    newDimensions = UInt32Vector(3, 3)
    array2.resizeAsUInt16(newDimensions)
    assert array2.getSize() == 27
    dimensions = array2.getDimensions()
    assert dimensions.size() == 3
    assert dimensions[0] == 3 and dimensions[1] == 3 and dimensions[2] == 3
    dimensionsString = array2.getDimensionsString()
    assert dimensionsString == "3 3 3"

    values = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    array2.insertAsUInt16(0, values)
    assert array2.getSize() == 27
    dimensions = array2.getDimensions()
    assert dimensions.size() == 3
    assert dimensions[0] == 3 and dimensions[1] == 3 and dimensions[2] == 3

    array2.pushBackAsInt16(10)
    assert array2.getSize() == 28
    dimensions = array2.getDimensions()
    assert dimensions.size() == 1
    assert dimensions[0] == 28

