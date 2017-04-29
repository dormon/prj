#include<cstdlib>
#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include <iostream>
 
int r(){
  return 17;
}

int main(){
  int(*fce)()=nullptr;
  int target = 0;
  asm("nop");
  //*
  asm(
  "movq	%1, %%rax\n"
	"call	*%%rax\n"
	"movl	%%eax, %0\n"
  :"=r"(target)
  :""(&r)
  );
  // */
  /*
  fce = r;
  target = fce();
  // */
  asm("nop");
  std::cout<<target<<std::endl;
  return 0;
}


