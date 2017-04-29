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
  int target = 0;
  asm("nop");
  asm("call _Z1rv\n"
      "movl %%eax,%0\n"
      :"=r"(target)
      :
      );
  asm("nop");
  std::cout<<target<<std::endl;
  return 0;
}


