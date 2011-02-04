#include <limits.h>
#include <stdlib.h>
#include "XdmfSystemUtils.hpp"

XdmfSystemUtils::XdmfSystemUtils()
{
}

XdmfSystemUtils::~XdmfSystemUtils()
{
}

std::string XdmfSystemUtils::getRealPath(const std::string & path)
{
	char realPath[PATH_MAX];
	realpath(path.c_str(), realPath);
	return realPath;
}
