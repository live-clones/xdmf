#include "XdmfSystemUtils.hpp"

int main(int, char **)
{
	std::string priorPath = "Path you want to convert";
	std::string convertedPath = XdmfSystemUtils::getRealPath(priorPath);

	return 0;
}
