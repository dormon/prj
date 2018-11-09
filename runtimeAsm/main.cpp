#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>

//uint8_t *testfun;

uint64_t internalFunction(uint64_t a);
void sink(uint64_t v);

//uint32_t fun(uint32_t a) { return a + 13; }
//
//uint32_t fun2() { return 13; }
//
void measure(std::string const&name,std::function<void()>const&f){
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end   = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = end - start;
  std::cout << name << ": " << elapsed.count() << std::endl;
}

using FCE = uint64_t(*)(uint64_t);

FCE fillPointerCall();
FCE fillPointerCall2();
FCE* fillDoublePointerCall(int a);
FCE* fillDoublePointerCall2(int a);

FCE pointerCall = nullptr;
FCE* doublePointerCall = nullptr;

int main(int argc,char*argv[]) {

  if(argc<2){
    pointerCall = fillPointerCall();
    doublePointerCall = fillDoublePointerCall(argc);
  }else{
    pointerCall = fillPointerCall2();
    doublePointerCall = fillDoublePointerCall(argc+1);
  }


  uint64_t cc = 0;
  for(uint64_t i=0;i<1000000000;++i)
    cc += internalFunction(i);
  sink(cc);

  measure("pointerCall",[](){
    uint64_t c = 0;
    for(uint64_t i=0;i<1000000000;++i)
      c += pointerCall(i);
    sink(c);
  });

  measure("doublePointerCall",[](){
    uint64_t c = 0;
    for(uint64_t i=0;i<1000000000;++i)
      c += (*doublePointerCall)(i);
    sink(c);
  });
  
  measure("internalFunction",[](){
    uint64_t c = 0;
    for(uint64_t i=0;i<1000000000;++i)
      c += internalFunction(i);
    sink(c);
  });

/*
  std::ofstream f("code.cpp");
  f << "#include<cstdint>\n uint64_t ccc(uint64_t); int main(int argc,char*argv[]){return ccc(argc);}";
  f.close();
  int as;
  as = system("g++ -S code.cpp");
  as = system("cat code.s");
  as = system("rm code.cpp");
  as = system("rm code.s");


  return 0;

  uint32_t ra;
  uint32_t pagesize;
  uint8_t* ptr;
  uint32_t offset;

  pagesize = getpagesize();
  testfun  = (uint8_t*)malloc(1023 + pagesize + 1);
  if (testfun == NULL) return 1;
  // need to align the address on a page boundary
  printf("%p\n", testfun);
  testfun = (uint8_t*)(((long)testfun + pagesize - 1) & ~(pagesize - 1));
  printf("%p\n", testfun);

  if (mprotect(testfun, 1024, PROT_READ | PROT_EXEC | PROT_WRITE)) {
    printf("mprotect failed\n");
    return 1;
  }

  // 400687: b8 0d 00 00 00          mov    $0xd,%eax
  // 40068d: c3                      retq

  testfun[0] = 0xb8;
  testfun[1] = 0x0d;
  testfun[2] = 0x00;
  testfun[3] = 0x00;
  testfun[4] = 0x00;
  testfun[5] = 0xc3;

  ra = ((uint32_t (*)())testfun)();
  printf("0x%02X\n", ra);

  testfun[0] = 0xb8;
  testfun[1] = 0x20;
  testfun[2] = 0x00;
  testfun[3] = 0x00;
  testfun[4] = 0x00;
  testfun[5] = 0xc3;

  ra = ((uint32_t (*)())testfun)();
  printf("0x%02X\n", ra);

  printf("%p\n", fun);
  offset = (uint32_t)(((long)fun) & (pagesize - 1));
  ptr    = (uint8_t*)((long)fun & (~(pagesize - 1)));

  printf("%p 0x%X\n", ptr, offset);

  if (mprotect(ptr, pagesize, PROT_READ | PROT_EXEC | PROT_WRITE)) {
    printf("mprotect failed\n");
    return (1);
  }

  // for(ra=0;ra&lt;20;ra++) printf("0x%02X,",ptr[offset+ra]); printf("\n");

  ra = 4;
  ra = fun(ra);
  printf("0x%02X\n", ra);

  ptr[offset + 0] = 0xb8;
  ptr[offset + 1] = 0x22;
  ptr[offset + 2] = 0x00;
  ptr[offset + 3] = 0x00;
  ptr[offset + 4] = 0x00;
  ptr[offset + 5] = 0xc3;

  ra = 4;
  ra = fun(ra);
  printf("0x%02X\n", ra);
*/
  return (0);
}
