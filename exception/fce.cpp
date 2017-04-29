#include"fce.h"

size_t add(size_t a){
  if(a%2)return 0;
  return a*2+2;
}

size_t addX(size_t a){
  if(a%2)throw("ahoj");
  return a*2+2;
}
