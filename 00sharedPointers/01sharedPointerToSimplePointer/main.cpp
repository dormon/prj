#include<iostream>
#include<memory>
#include<glm/glm.hpp>

int main(){
  std::shared_ptr<int>sp = std::make_shared<int>(32);
  int* ss = &(*sp);
  std::cout << *ss <<std::endl;
  return 0;
}
