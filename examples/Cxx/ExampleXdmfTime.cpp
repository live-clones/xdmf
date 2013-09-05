#include "XdmfTime.hpp"

int main(int, char **)
{
        //#initialization begin

        shared_ptr<XdmfTime> exampleTime = XdmfTime::New();
        //The Default case sets the time to 0.0
        //You can also specify a time when creating the XdmfTime object

        double newTime = 5.0;
        shared_ptr<XdmfTime> exampleTime2 = XdmfTime::New(newTime);

        //#initialization end

        //#getValue begin

        double readTime = exampleTime->getValue();

        //#getValue end

        //#setValue begin

        exampleTime->setValue(newTime);

        //#setValue end

        return 0;
}
