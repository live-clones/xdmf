#include "XdmfTime.hpp"
#include <iostream>
#include <assert.h>

int main(int, char **)
{
  shared_ptr<XdmfTime> time = XdmfTime::New();
  std::cout << time->getValue() << " ?= " << 0 << std::endl;
  assert(time->getValue() == 0);
  time->setValue(50);

  shared_ptr<XdmfTime> time2 = XdmfTime::New(50);
  std::cout << time->getValue() << " ?= " << time2->getValue() << std::endl;
  assert(time->getValue() == time2->getValue());

  return 0;
}
