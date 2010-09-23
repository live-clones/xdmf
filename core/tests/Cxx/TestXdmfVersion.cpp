#include <iostream>
#include "XdmfVersion.hpp"

int main() {
	Version *v = new Version("Xdmf");
        if(v->getVersion() == "")
                exit(-1);
        if(v->getVersionShort() == "")
                exit(-1);
        std::cout << v->getVersion() << std::endl;
        std::cout << v->getVersionShort();
	return 0;
}
