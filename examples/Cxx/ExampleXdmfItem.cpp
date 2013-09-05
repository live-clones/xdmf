#include "XdmfInformation.hpp"
#include "XdmfDomain.hpp"
#include "XdmfWriter.hpp"

int main(int, char **)
{
        //Assume that exampleItem is a shared pointer to the ParentClass object
        //Using an XdmfInformation as an example because all XdmfItems have XdmfInformation as a child class

        shared_ptr<XdmfInformation> exampleItem = XdmfInformation::New("Parent", "This is a parent information");
        shared_ptr<XdmfInformation> addChild = XdmfInformation::New("Child", "This is a child information");

        exampleItem->insert(addChild);

        unsigned int getIndex = 0;
        shared_ptr<XdmfInformation> exampleChild = exampleItem->getInformation(getIndex);
        shared_ptr<const XdmfInformation> exampleChildConst = exampleItem->getInformation(getIndex);

        std::string findingInfo = "Find this";
        shared_ptr<XdmfInformation> exampleStringChild = exampleItem->getInformation(findingInfo);
        shared_ptr<const XdmfInformation> exampleStringChildConst = exampleItem->getInformation(findingInfo);

        unsigned int exampleSize = exampleItem->getNumberInformations();

        unsigned int removeIndex = 0;
        exampleItem->removeInformation(removeIndex);

        std::string removeInfo = "Remove this";
        exampleItem->removeInformation(removeInfo);

        //#initialization begin

        //Using a shared pointer to an XdmfDomain object as an example

        shared_ptr<XdmfDomain> exampleDomain = XdmfDomain::New();

        //#initialization end

        //#getItemTag begin

        std::string exampleTag = exampleDomain->getItemTag();

        //#getItemTag end

        //#getItemProperties begin

        std::map<std::string, std::string> propertyMap = exampleDomain->getItemProperties();

        //#getItemProperties end

        //#traverse begin

        std::string writePath = "file path here";
        shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New(writepath);
        exampleDomain->traverse(exampleWriter);

        //#traverse end

        return 0;
}
