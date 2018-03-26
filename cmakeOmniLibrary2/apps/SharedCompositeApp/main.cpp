#include<iostream>

#include<SharedCompositeA.h>
#include<SharedCompositeB.h>

int main(){
  std::cout << sharedCompositeA(sharedCompositeB(10)) << std::endl;
  return 0;
}
