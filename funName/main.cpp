#include <iostream>

namespace cssv::sides{
  void asd(){
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
  }
}

int main()
{
  cssv::sides::asd();
  return 0;
}
