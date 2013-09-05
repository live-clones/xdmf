from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        baseArray = XdmfArray.New()

        for i in range(0, 10):
                baseArray.pushBack(i)

        initStart = UInt32Vector()
        initStart.push_back(0)
        initStride = UInt32Vector()
        initStride.push_back(1)
        initDimension = UInt32Vector()
        initDimension.push_back(10)

        exampleSubset = XdmfSubset.New(baseArray,
                                       initStart,
                                       initStride,
                                       initDimension)

        #//initialization end

        #//getStart begin

        exampleStart = exampleSubset.getStart()

        #//getStart end

        #//setStart begin

        exampleSubset.setStart(exampleStart)

        #//setStart end

        #//getStride begin

        exampleStride = exampleSubset.getStride()

        #getStride end

        #//setStride begin

        exampleSubset.setStride(exampleStride)

        #//setStride end

        #//getDimensions begin

        exampleDimensions = exampleSubset.getDimensions()

        #//getDimensions end

        #//setDimensions begin

        exampleSubset.setDimensions(exampleDimensions)

        #//setDimensions end

        #//getReferenceArray begin

        exampleInternalArray = exampleSubset.getReferenceArray()

        #//getReferenceArray end

        #//setReferenceArray begin

        exampleSubset.setReferenceArray(exampleInternalArray)

        #//setReferenceArray end

        #//getSize begin

        exampleSize = exampleSubset.getSize()

        #//getSize end

        #//setConstructedType begin

        typeAttribute = XdmfAttribute.New()
        exampleSubset.setConstructedType(typeAttribute.getItemTag())

        #//setConstructedType end

        #//getConstructedType begin

        exampleType = exampleSubset.getConstructedType()

        #//getConstructedType end

        #//setConstructedProperties begin

        propertyAttribute = XdmfAttribute.New()
        exampleSubset.setConstructedProperties(propertyAttribute.getItemProperties())

        #//setConstructedProperties end

        #//getConstructedProperties begin

        exampleProperties = exampleSubset.getConstructedProperties()

        #//getConstructedProperties end

        #//read begin

        subsetResult = exampleSubset.read()

        #//read end
