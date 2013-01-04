#include "XdmfTime.hpp"

int main(int, char **)
{
	//Assumming that exampleTime is a shared pointer to an XdmfTime object

	shared_ptr<XdmfTime> exampleTime = XdmfTime::New();
	//The Default case sets the time to 0.0
	//You can also specify a time when creating the XdmfTime object

        double newTime = 5.0;
        shared_ptr<XdmfTime> exampleTime2 = XdmfTime::New(newTime);

	double readTime = exampleTime->getTime();

	exampleTime->setValue(newTime);

	return 0;
}
