#include "XdmfInformation.hpp"

int main(int, char **)
{
  boost::shared_ptr<XdmfInformation> information = XdmfInformation::New();
  assert(information->getKey().compare("") == 0);
  assert(information->getValue().compare("") == 0);
  information->setKey("Key");
  information->setValue("Value");
  assert(information->getKey().compare("Key") == 0);
  assert(information->getValue().compare("Value") == 0);

  boost::shared_ptr<XdmfInformation> information1 =
    XdmfInformation::New("Key1", "Value1");
  assert(information1->getKey().compare("Key1") == 0);
  assert(information1->getValue().compare("Value1") == 0);

  assert(information->getNumberInformations() == 0);
  assert(information->getInformation(0) == NULL);
  assert(information->getInformation("foo") == NULL);
  information->removeInformation(0);
  assert(information->getNumberInformations() == 0);
  information->insert(information1);
  assert(information->getNumberInformations() == 1);
  assert(information->getInformation(0) == information1);
  assert(information->getInformation(1) == NULL);
  assert(information->getInformation("Key1") == information1);
  assert(information->getInformation("foo") == NULL);
  information->removeInformation("Key1");
  assert(information->getNumberInformations() == 0);

  return 0;
}
