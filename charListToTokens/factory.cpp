#include<factory.h>

#include<memory>

auto factory(){
  return std::make_shared<size_t>(42);
}
