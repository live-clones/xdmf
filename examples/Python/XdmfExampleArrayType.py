from Xdmf import *

if __name__ == "__main__":
        #//getElementSize begin

        dataSize = XdmfArrayType.Int8().getElementSize()

        #//getElementSize end

        #//getName begin

        dataName = XdmfArrayType.Int8().getName()

        #//getName end

        #//getIsSigned begin

        isSigned = XdmfArrayType.UInt8().getIsSigned()

        #//getIsSigned end

        #//getIsFloat begin

        isFloat = XdmfArrayType.UInt8().getIsFloat()

        #//getIsFloat end

        #//getType begin

        exampleArray = XdmfArray.New()

        exampleType = exampleArray.getType()

        if exampleType == XdmfArrayType.Int8():
                #do whatever is to be done if the type is Int8

        #//getType end

        #//comparePrecision begin

        resultType = XdmfArrayType.comparePrecision(XdmfArrayType.Int16(), XdmfArrayType.UInt8())

        #//comparePrecision end
