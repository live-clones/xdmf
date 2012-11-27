#include "XdmfArrayType.hpp"

int main(int, char **)
{
	//Using XdmfArrayType::Int32() as an example
	std::map<std::string, std::string> propertyMap;
	XdmfArrayType::Int32()->getProperties(propertyMap);

	return 0;
}
