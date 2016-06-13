from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        exampleAggregate = XdmfAggregate.New()

        partArray = XdmfArray.New()

        for i in range(0, 10):
                partArray.pushBack(i)

        exampleAggregate.insert(partArray)

        partArray2 = XdmfArray.New()

        for i in range(0, 10):
                partArray2.pushBack(i)

        exampleAggregate.insert(partArray2)

        #//initialization end

        #//getDimensions begin

        exampleDimensions = exampleAggregate.getDimensions()

        #//getDimensions end

        #//getSize begin

        exampleSize = exampleAggregate.getSize()

        #//getSize end

        #//read begin

        aggregateResult = exampleAggregate.read()

        #//read end
