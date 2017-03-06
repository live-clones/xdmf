#include "XdmfInformation.hpp"
#include <iostream>
#include <assert.h>

int main(int, char **)
{
  shared_ptr<XdmfInformation> information = XdmfInformation::New();

  std::cout << information->getKey() << " ?= " << "" << std::endl;
  std::cout << information->getValue() << " ?= " << "" << std::endl;

  assert(information->getKey().compare("") == 0);
  assert(information->getValue().compare("") == 0);
  information->setKey("Key");
  information->setValue("Value");

  std::cout << information->getKey() << " ?= " << "Key" << std::endl;
  std::cout << information->getValue() << " ?= " << "Value" << std::endl;

  assert(information->getKey().compare("Key") == 0);
  assert(information->getValue().compare("Value") == 0);

  shared_ptr<XdmfInformation> information1 = XdmfInformation::New("Key1", 
                                                                  "Value1");

  std::cout << information1->getKey() << " ?= " << "Key1" << std::endl;
  std::cout << information1->getValue() << " ?= " << "Value1" << std::endl;

  assert(information1->getKey().compare("Key1") == 0);
  assert(information1->getValue().compare("Value1") == 0);

  std::cout << information->getNumberInformations() << " ?= " << 0 << std::endl;
  std::cout << information->getInformation(0) << " ?= " << "NULL" 
            << std::endl;
  std::cout << information->getInformation("foo") << " ?= " << "NULL"
            << std::endl;

  assert(information->getNumberInformations() == 0);
  assert(information->getInformation(0) == NULL);
  assert(information->getInformation("foo") == NULL);
  information->removeInformation(0);

  std::cout << information->getNumberInformations() << " ?= " << 0 << std::endl;

  assert(information->getNumberInformations() == 0);
  information->insert(information1);

  std::cout << information->getNumberInformations() << " ?= " << 1 << std::endl;

  assert(information->getNumberInformations() == 1);

  std::cout << information->getInformation(0) << " ?= " << information1 
            << std::endl;
  std::cout << information->getInformation(1) << " ?= " << "NULL" 
            << std::endl;
  std::cout << information->getInformation("Key1") << " ?= " << information1 
            << std::endl;
  std::cout << information->getInformation("foo") << " ?= " << "NULL"
            << std::endl;

  assert(information->getInformation(0) == information1);
  assert(information->getInformation(1) == NULL);
  assert(information->getInformation("Key1") == information1);
  assert(information->getInformation("foo") == NULL);
  information->removeInformation("Key1");

  std::cout << information->getNumberInformations() << " ?= " << 0 << std::endl;

  assert(information->getNumberInformations() == 0);

  return 0;
}
