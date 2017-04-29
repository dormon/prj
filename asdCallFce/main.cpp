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
  int a=123;
  int(*fce)();
  int randomness = 4;
  int target = 0;
  asm("nop");
  /*
	movq	$_Z1rv, -8(%rbp)
	movq	-8(%rbp), %rax
	call	*%rax
	movl	%eax, -12(%rbp)
  */
  /*
  fce=&r;
  a=fce();
  // */
  //*
  /*
  asm( "movl %0,%%eax"
      :
      : "" (randomness)
      : "%eax"
     );
  asm( "movl %%eax,%0"
      : "=r" (target)
      :
      : "%eax");
  */
  asm("call _Z1rv\n"
      "movl %%eax,%0\n"
      :"=r"(target)
      :
      );
  /*
  asm( "movl %1,%0\n"
      : "=r" (target)
      : "" (randomness)
     );
     */

  // */
  //asm("movl %eax, -12(%rbp)");
  asm("nop");
  std::cout<<a<<std::endl;

  std::cout<<target<<std::endl;
  return 0;
}


