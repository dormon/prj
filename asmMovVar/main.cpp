#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include <iostream>
 
int main(){
  int randomness = 4;
  int target = 0;
  asm("nop");
  asm( "movl %1,%0\n"
      : "=r" (target)
      : "" (randomness)
     );
  asm("nop");
  std::cout<<target<<std::endl;
  return 0;
}


