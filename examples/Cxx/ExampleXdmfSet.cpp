#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"

int main(int, char **)
{
        //#initialize begin

        shared_ptr<XdmfSet> exampleSet = XdmfSet::New();

        //#initialize end

        //#setName begin

        std::string newName = "New Name";
        exampleSet->setName(newName);

        //#setName end

        //#setType begin

        exampleSet->setType(XdmfSetType::Node());

        //#setType end

        //#getName begin

        std::string exampleName = exampleSet->getName();

        //#getName end

        //#getType begin

        shared_ptr<const XdmfSetType> exampleType = exampleSet->getType();

        if (exampleType == XdmfSetType::Node())
        {
                //Do whatever is to be done if the set is a node
        }

        //#getType end

        return 0;
}
