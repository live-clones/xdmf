#include <iostream>
#include "XdmfVersion.hpp"

int main() {
	std::cout << XdmfVersion.getFull() << std::endl;
    std::cout << XdmfVersion.getShort() << std::endl;
	return 0;
}
