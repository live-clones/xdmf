#include "XdmfInformation.hpp"
#include "XdmfArray.hpp"

#include "stdio.h"
#include "stdlib.h"

int main()
{
  int status = 0;

  XDMFINFORMATION * information = XdmfInformationNew("keyvalue", "valuevalue");

  char * infoKey = XdmfInformationGetKey(information);

  printf("The key equals: %s\n", infoKey);

  free(infoKey);

  char * infoValue = XdmfInformationGetValue(information);

  printf("The value equals: %s\n", infoValue);

  free(infoValue);

  XdmfInformationSetKey(information, "newKey", &status);

  XdmfInformationSetValue(information, "newValue", &status);

  infoKey = XdmfInformationGetKey(information);

  infoValue = XdmfInformationGetValue(information);

  printf("The key equals: %s\n", infoKey);

  printf("The value equals: %s\n", infoValue);

  free(infoKey);
  
  free(infoValue);

  XDMFINFORMATION * childInfo = XdmfInformationNew("childKey", "childValue");

  XdmfInformationInsertInformation(information, childInfo, 0);

  XdmfInformationSetKey(childInfo, "newChildKey", &status);
  XdmfInformationSetValue(childInfo, "newChildValue", &status);

  XDMFINFORMATION * internalInfo = XdmfInformationGetInformation(information, 0);

  char * childKey = XdmfInformationGetKey(internalInfo);

  char * childValue = XdmfInformationGetValue(internalInfo);

  printf("The Child's key equals: %s\nThe Child's Value equals: %s\n", childKey, childValue);

  free(childKey);

  free(childValue);

  unsigned int numArrays = XdmfInformationGetNumberArrays(information);
  unsigned int numInfo = XdmfInformationGetNumberInformations(information);

  printf("The main information contains %d arrays and %d information\n", numArrays, numInfo);

  // Doing this used to deallocate the Information
  // After changing the way that shared pointers interact with objects
  // there is no longer any deallocation
  XdmfInformationRemoveInformation(information, 0);

  childKey = XdmfInformationGetKey(internalInfo);

  childValue = XdmfInformationGetValue(internalInfo);

  printf("The Child's key equals: %s\nThe Child's Value equals: %s\n", childKey, childValue);

  free(childKey);
  
  free(childValue);

  childKey = XdmfInformationGetKey(childInfo);

  childValue = XdmfInformationGetValue(childInfo);

  printf("The Child's key equals: %s\nThe Child's Value equals: %s\n", childKey, childValue);

  free(childKey);

  free(childValue);

  XdmfInformationFree(information);

  XdmfInformationFree(childInfo);

  XdmfInformationFree(internalInfo);

  return 0;
}
