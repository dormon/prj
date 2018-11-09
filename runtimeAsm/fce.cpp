#include<cstdint>

uint64_t internalFunction(uint64_t a){
  return a;
}

uint64_t internalFunction2(uint64_t a){
  return a+1;
}

void sink(uint64_t){
}

using FCE = uint64_t(*)(uint64_t);

FCE fillPointerCall(){
  return internalFunction;
}

FCE fillPointerCall2(){
  return internalFunction2;
}

FCE doublePointer = nullptr;

FCE fillDP();
FCE fillDP2();

FCE* fillDoublePointerCall(int a){
  if(a<2)
    doublePointer = fillDP();
   else
    doublePointer = fillDP2();
  return &doublePointer;
}
