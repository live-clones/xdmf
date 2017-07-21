#include "XdmfStringUtils.hpp"

#include <cassert>
#include <vector>

int main() {

  std::vector<int> split;
  XdmfStringUtils::split("1 2 3", split);
  
  assert(split.size() == 3);
  assert(split[0] == 1);
  assert(split[1] == 2);
  assert(split[2] == 3);
  
  split.clear();
  XdmfStringUtils::split(" 1 2 3 ", split);
  
  assert(split.size() == 3);
  assert(split[0] == 1);
  assert(split[1] == 2);
  assert(split[2] == 3);

  split.clear();
  XdmfStringUtils::split("1\t2\t3", split);
  
  assert(split.size() == 3);
  assert(split[0] == 1);
  assert(split[1] == 2);
  assert(split[2] == 3);

  split.clear();
  XdmfStringUtils::split("1\n2\n3", split);
  
  assert(split.size() == 3);
  assert(split[0] == 1);
  assert(split[1] == 2);
  assert(split[2] == 3);

  split.clear();
  XdmfStringUtils::split("1\n\t 2\n\t 3", split);
  
  assert(split.size() == 3);
  assert(split[0] == 1);
  assert(split[1] == 2);
  assert(split[2] == 3);

  const std::string abc("abc");
  const std::string capABC = XdmfStringUtils::toUpper(abc);

  assert(abc.compare("abc") == 0);
  assert(capABC.compare("ABC") == 0);

  return 0;
}
