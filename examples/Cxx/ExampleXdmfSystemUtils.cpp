#include "XdmfSystemUtils.hpp"

int main(int, char **)
{
	//#getRealPath begin

	std::string priorPath = "Path you want to convert";
	std::string convertedPath = XdmfSystemUtils::getRealPath(priorPath);

	//#getRealPath end

	return 0;
}
