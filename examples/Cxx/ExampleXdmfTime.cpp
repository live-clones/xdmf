#include "XdmfTime.hpp"

int main(int, char **)
{
	shared_ptr<XdmfTime> exampleTime = XdmfTime::New();

        double newTime = 5.0;
        shared_ptr<XdmfTime> exampleTime2 = XdmfTime::New(newTime);

	double readTime = exampleTime->getTime();

	exampleTime->setValue(newTime);

	return 0;
}
