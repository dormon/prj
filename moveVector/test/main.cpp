#include<iostream>

void asd();

using FCE = void(*)();
FCE f;

int main(){
  std::cerr << std::hex << (uint64_t)(void*)asd << std::endl;
  f = asd;
  f();
  return 0;
}
