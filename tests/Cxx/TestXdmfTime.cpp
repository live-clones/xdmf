#include "XdmfTime.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfTime> time = XdmfTime::New();
	assert(time->getValue() == 0);
	time->setValue(50);

	boost::shared_ptr<XdmfTime> time2 = XdmfTime::New(50);
	assert(time->getValue() == time2->getValue());

	return 0;
}
