#include "XdmfArrayType.hpp"

int main(int, char **)
{
        //#getProperties begin

        //Using XdmfArrayType::Int32() as an example
        std::map<std::string, std::string> propertyMap;
        XdmfArrayType::Int32()->getProperties(propertyMap);

        //#getProperties end

        return 0;
}
