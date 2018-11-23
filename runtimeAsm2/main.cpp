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
#include <cstdlib>

uint32_t fun(uint32_t a) { return a + 13; }
uint32_t fun2() { return 13; }

void measure(std::string const&name,std::function<void()>const&f){
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end   = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = end - start;
  std::cout << name << ": " << elapsed.count() << std::endl;
}

uint8_t* testfun;

class RuntimeAsm{
  public:
    RuntimeAsm(size_t nofPages){
      pagesize = getpagesize();
      mem      = reinterpret_cast<uint8_t*>(aligned_alloc(pagesize*nofPages,pagesize));
      if(mem == nullptr)
        throw std::runtime_error("cannot allocate memory");
      if (auto err = mprotect(mem, pagesize, PROT_READ | PROT_EXEC | PROT_WRITE))
        throw std::runtime_error(std::string("cannot mprotect - ")+strerror(err));
    }
    ~RuntimeAsm(){
      free(mem);
    }
    void movEAX(uint32_t v){
      mem[offset++] = 0xb8;
      write32u(v);
    }
    void movRAX(uint64_t v){
      mem[offset++] = 0x48;
      mem[offset++] = 0xb8;
      write64u(v);
    }
    void movEDI(uint32_t v){
      mem[offset++] = 0xbf;
      write32u(v);
    }
    void movESI(uint32_t v){
      mem[offset++] = 0xbe;
      write32u(v);
    }
    void movEDX(uint32_t v){
      mem[offset++] = 0xba;
      write32u(v);
    }
    void movECX(uint32_t v){
      mem[offset++] = 0xb9;
      write32u(v);
    }
    void push(uint32_t v){
      mem[offset++] = 0x68;
      write32u(v);
    }
    void callRAX(){
      mem[offset++] = 0xff;
      mem[offset++] = 0xd0;
    }
    void callRAXindirect(){
      mem[offset++] = 0xff;
      mem[offset++] = 0x10;
    }
    void retq(){
      mem[offset++] = 0xc3;
    }
    using FCE = void(*)();
    void writeCall(FCE const&f){
      std::cerr << (void*)f << std::endl;
      movRAX(*((uint64_t*)f));
      callRAX();
    }
    uint32_t call(){
      using F = uint32_t(*)();
      return reinterpret_cast<F>(mem)();
    }
    size_t getOffset()const{return offset;}
    void   setOffset(size_t o){offset = o;}
  protected:
    void write32u(uint32_t v){
      for(size_t i=0;i<4;++i)
        mem[offset++] = (v>>(8*i))&0xff;
    }
    void write64u(uint64_t v){
      for(size_t i=0;i<8;++i)
        mem[offset++] = (v>>(8*i))&0xff;
    }
    size_t  offset = 0;
    int     pagesize;
    uint8_t*mem;
};

void hi(){
  std::cerr << "hi function was called" << std::endl;
}

void world(){
  std::cerr << "world function was called" << std::endl;
}

int main(int argc,char*[]) {

  /*
  std::ofstream f("code.cpp");
  f << R".(
  using FCE = void(*)();
  FCE fafa;
  void lala(){
    fafa();
  }
  ).";
  f.close();
  int as;
  as = system("g++ -S code.cpp");
  as = system("cat code.s");
  as = system("rm code.cpp");
  as = system("rm code.s");
  return 0;
  // */

  using FCE = void(*)();
  auto rasm = RuntimeAsm(2);
  //rasm.movEAX(101);
  //rasm.retq();
  //std::cerr << rasm.call() << std::endl;
  rasm.writeCall((FCE)0x10a2f6);
  rasm.writeCall((FCE)0x10a325);
  rasm.retq();
  rasm.call();
  return 0;

  // 400687: b8 0d 00 00 00          mov    $0xd,%eax
  // 40068d: c3                      retq

}
