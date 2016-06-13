#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfSubset.hpp"
#include <vector>
#include <map>

int main(int, char **)
{
        //#initialization begin

        shared_ptr<XdmfAggregate> exampleAggregate = XdmfAggregate::New();

        shared_ptr<XdmfArray> partArray = XdmfArray::New();

        for (unsigned int i = 0; i < 10; ++i)
        {
                partArray->pushBack(i);
        }

        exampleAggregate->insert(partArray);

        shared_ptr<XdmfArray> partArray2 = XdmfArray::New();

        for (unsigned int i = 0; i < 10; ++i)
        {
                partArray2->pushBack(i);
        }

        exampleAggregate->insert(partArray2);

        //#initialization end

        //#getDimensions begin

        std::vector<unsigned int> exampleDimensions = exampleAggregate->getDimensions();

        //#getDimensions end

        //#getSize begin

        int exampleSize = exampleAggregate->getSize();

        //#getSize end

        //#read begin

        shared_ptr<XdmfArray> aggregateResult = exampleAggregate->read();

        //#read end

        return 0;
}
