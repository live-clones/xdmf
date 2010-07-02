#include "XdmfInformation.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfInformation> information = XdmfInformation::New();
	assert(information->getKey().compare("") == 0);
	assert(information->getValue().compare("") == 0);
	information->setKey("Key");
	information->setValue("Value");
	assert(information->getKey().compare("Key") == 0);
	assert(information->getValue().compare("Value") == 0);
	return 0;
}
