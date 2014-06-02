#include <XdmfDomain.hpp>
#include <XdmfInformation.hpp>
#include <XdmfReader.hpp>

#include <iostream>
#include <fstream>

int main(int ac, char * av[]) 
{
  
  //
  // write outer file
  //
  std::ofstream outerFile("nestedOuter.xmf");
  outerFile << "<?xml version=\"1.0\" ?><Xdmf Version=\"2.1\" xmlns:xi=\"http://www.w3.org/2001/XInclude\"><Domain><xi:include href=\"nestedInner.xmf\" xpointer=\"element(/1/1/2)\"/></Domain></Xdmf>";
  outerFile.close();

  //
  // write inner file
  //
  std::ofstream innerFile("nestedInner.xmf");
  innerFile << "<?xml version=\"1.0\" ?><!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" []><Xdmf xmlns:xi=\"http://www.w3.org/2003/XInclude\" Version=\"2.1\"><Domain><Information Name=\"foo\" Value=\"bar\"/><xi:include xpointer=\"element(/1/1/1)\"/></Domain></Xdmf>";
  innerFile.close();

  shared_ptr<XdmfReader> reader = XdmfReader::New();
 
  shared_ptr<XdmfDomain> innerDomain = 
    shared_dynamic_cast<XdmfDomain>(reader->read("nestedInner.xmf"));
  assert(innerDomain->getNumberInformations() == 2);
  shared_ptr<XdmfInformation> information0 = innerDomain->getInformation(0);
  shared_ptr<XdmfInformation> information1 = innerDomain->getInformation(1);
  
  // should be the same since 1 is xpoint to 1
  assert(information0 == information1);
  
  shared_ptr<XdmfDomain> outerDomain = 
    shared_dynamic_cast<XdmfDomain>(reader->read("nestedOuter.xmf"));
  
  assert(outerDomain->getNumberInformations() == 1);
  shared_ptr<XdmfInformation> information = outerDomain->getInformation(0);
  assert(information->getKey().compare("foo") == 0);
  assert(information->getValue().compare("bar") == 0);

}
