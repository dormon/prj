#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<cassert>
#include<vector>
#include<map>
#include<Domain.h>

int main(){
  Domain dom;
  dom.addAtomicModel("Value");
  dom.addAtomicModel("Real"   ,"Value"  );
  dom.addAtomicModel("Integer","Real"   );
  dom.addAtomicModel("Natural","Integer");
  dom.addAtomicModel("Bool"   ,"Natural");
  return 0;
}
