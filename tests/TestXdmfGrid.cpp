#include "XdmfGrid.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfGrid> grid = XdmfGrid::New();
	std::string gridName = "Grid1";

	grid->setName(gridName);
	assert(grid->getName().compare(gridName) == 0);
	std::cout << grid->getName() << std::endl;

	return 0;
}
