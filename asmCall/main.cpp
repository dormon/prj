#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include <iostream>
 

extern "C" int func();
// the definition of func is written in assembly language
asm(".globl func\n\t"
    ".type func, @function\n\t"
    "func:\n\t"
    ".cfi_startproc\n\t"
    "movl $7, %eax\n\t"
    "ret\n\t"
    ".cfi_endproc");

extern "C" void hoj(int*);
asm(".globl hoj\n"
    ".type hoj, @function\n"
    "hoj:\n"
    ".cfi_startproc\n"
	  "pushq	%rbp\n"
  	".cfi_def_cfa_offset 16\n"
	  ".cfi_offset 6, -16\n"
  	"movq	%rsp, %rbp\n"
	  ".cfi_def_cfa_register 6\n"
	  "movq	%rdi, -8(%rbp)\n"
	  "movq	-8(%rbp), %rax\n"
	  "movl	$110, (%rax)\n"
	  "popq	%rbp\n"
	  ".cfi_def_cfa 7, 8\n"
	  "ret\n"
	  ".cfi_endproc");

int r(){
  return 17;
}

int main()
{
  int n = func();
  // extended inline assembly
  asm ("leal (%0,%0,4),%0"
      : "=r" (n)
      : "0" (n));
  std::cout << "7*5 = " << n << std::endl; // flush is intentional

  int c;
  hoj(&c);

  std::cout<<c<<std::endl;
  int a=11, b;
  asm ("movl %1, %%eax;"
      "movl %%eax, %0;"
      :"=r"(b)        /* output */
      :"r"(a)         /* input */
      :"%eax"         /* clobbered register */
      );
  a=123123;
  asm ("movl %1, %0"
      :"=r"(b)
      :"r"(a));
  std::cout<<b<<std::endl;

  int(*fce)()=r;
  a=fce();
  std::cout<<a<<std::endl;


  // standard inline assembly
  asm ("movq $60, %rax\n\t" // the exit syscall number on Linux
      "movq $0,  %rdi\n\t" // this program returns 0
      "syscall");
}


