#include "XdmfInformation.hpp"

int main(int, char **)
{
        //#initialization begin

        shared_ptr<XdmfInformation> exampleInformation = XdmfInformation::New();
        //Then the key and value must be set seperately

        exampleInformation->setKey("Your Key String");
        exampleInformation->setValue("Your Value String");

        //#initialization end

        //#initializationfull begin

        shared_ptr<XdmfInformation> infoExample = XdmfInformation::New("Your Key String", "Your Value String");
        //This code creates an information with the key "Your Key String" and the value "Your Value String"

        //#initializationfull end

        //#getKey begin

        std::string storedKey = infoExample->getKey();
        //"Your Key String" is now stored in the variable storedKey

        //#getKey end

        //#getValue begin

        std::string storedValue = infoExample->getValue();
        //"Your Value String" is now stored in the variable storedValue

        //#getValue end

        //#setKey begin

        infoExample->setKey("Your New Key");
        //"Your New Key" is now the key for infoExample

        //#setKey end

        //#setValue begin

        infoExample->setValue("Your New Value");
        //"Your New Value" is now the value for infoExample

        //#setValue end

        return 0;
}
