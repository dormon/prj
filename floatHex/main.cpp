#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstring>
#include <functional>
#include <typeinfo>
#include <set>
#include <chrono>
#include <stack>
#include <thread>
#include <bitset>

uint32_t floatBitsToUint(float a){
  float*ptr = &a;
  uint32_t*iptr = (uint32_t*)ptr;
  return *iptr;
}

#define PRINTB(a)\
  std::cerr << #a << " " << std::bitset<32>(floatBitsToUint(a)) << " " << a << std::endl

int main(){
  for(uint32_t a=0;a<30;++a){

    float v = (float)(1<<a);
    
    PRINTB(v);
  }
  return 0;
}
