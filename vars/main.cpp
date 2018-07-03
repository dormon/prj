#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <tuple>
#include <typeinfo>
#include <functional>

#include <Vars.h>

int main() {
  vars::Vars vars;
  vars.add<float>("mass",1.f);
  vars.add<std::string>("name","two.obj");
  std::cout << *vars.get<float>("mass") << std::endl;
  return 0; 
}
