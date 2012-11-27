#include "XdmfInformation.hpp"

int main(int, char **)
{
	shared_ptr<XdmfInformation> exampleInformation = XdmfInformation::New();
	//Then the key and value must be set seperately
	exampleInformation->setKey("Your Key String");
	exampleInformation->setValue("Your Value String");

	shared_ptr<XdmfInformation> infoExample = XdmfInformation::New("Your Key String", "Your Value String");
	//This code creates an information with the key "Your Key String" and the value "Your Value String"

	std::string storedKey = infoExample->getKey();
	//"Your Key String" is now stored in the variable storedKey

	std::string storedValue = infoExample->getValue();
	//"Your Value String" is now stored in the variable storedValue

	infoExample->setKey("Your New Key");
	//"Your New Key" is now the key for infoExample

	infoExample->setValue("Your New Value");
	//"Your New Value" is now the value for infoExample

	return 0;
}
