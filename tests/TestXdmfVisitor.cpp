#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"

int main(int argc, char* argv[])
{
	// Test != and == operators
	boost::shared_ptr<XdmfVisitor> visitor = XdmfVisitor::New();
	boost::shared_ptr<XdmfTopology> top = XdmfTopology::New();

	top->write(visitor);
	std::cout << visitor->printSelf() << std::endl;

	return 0;
}
