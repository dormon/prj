#include<cstdint>

using FCE = uint64_t(*)(uint64_t);

uint64_t internalFunction(uint64_t a);
uint64_t internalFunction2(uint64_t a);

FCE fillDP(){
  return internalFunction; 
}

FCE fillDP2(){
  return internalFunction2; 
}
