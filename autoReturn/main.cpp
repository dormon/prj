#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <array>

class Base{
  public:
    float a;
};

auto factory(){
  return std::make_shared<Base>();
}

std::shared_ptr<Base>factory(){
}

int main(int, char*[]) {
  auto a = factory();
  return EXIT_SUCCESS;
}
