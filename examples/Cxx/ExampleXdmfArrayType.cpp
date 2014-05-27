#include "XdmfDomain.hpp"

int main(int, char **)
{
        //#getType begin

        shared_ptr<XdmfArray> exampleArray = XdmfArray::New();

        if (XdmfArrayType::Int8() == exampleArray->getArrayType())
        {
                //do whatever is to be done with in the case that the array type is Int8
        }

        //#getType end

        //#getElementSize begin

        unsigned int dataSize = XdmfArrayType::Int8()->getElementSize();

        //#getElementSize end

        //#getIsSigned begin

        bool isSigned = XdmfArrayType::UInt8()->getIsSigned();

        //#getIsSigned end

        //#getIsFloat begin

        bool isFloat = XdmfArrayType::UInt8()->getIsFloat();

        //#getIsFloat end

        //#getName begin

        std::string dataName = XdmfArrayType::Int8()->getName();

        //#getName end

        //#comparePrecision begin

        shared_ptr<const XdmfArrayType> resultType = XdmfArrayType::comparePrecision(XdmfArrayType::Int16(), XdmfArrayType::UInt8());

        //#comparePrecision end

        return 0;
}
