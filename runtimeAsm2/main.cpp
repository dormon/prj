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
#include <memory>

void measure(std::string const&name,std::function<void()>const&f){
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end   = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = end - start;
  std::cout << name << ": " << elapsed.count() << std::endl;
}

namespace assembler{

enum class Type{
  MOV_EAX          ,
  MOV_EDI          ,
  MOV_ESI          ,
  MOV_EDX          ,
  MOV_ECX          ,
  MOV_R8D          ,
  MOV_R9D          ,
  PUSHQ            ,
  PUSH_RBP         ,
  POP_RBP          ,
  POP_RAX          ,
  CALL_RAX         ,
  CALL_RAX_INDIRECT,
  RETQ             ,
  MOV_RAX          ,
  MOVABS_RDI       ,
  MOVABS_RSI       ,
  MOVABS_RDX       ,
  MOVABS_RCX       ,
  MOVABS_R8        ,
  MOVABS_R9        ,


};

class Memory{
  public:
    Memory(size_t nofPages){
      pagesize = getpagesize();
      memory   = reinterpret_cast<uint8_t*>(aligned_alloc(pagesize*nofPages,pagesize));
      if(memory == nullptr)
        throw std::runtime_error("cannot allocate memory");
      if (auto err = mprotect(memory, pagesize, PROT_READ | PROT_EXEC | PROT_WRITE))
        throw std::runtime_error(std::string("cannot mprotect - ")+strerror(err));
    }
    ~Memory(){
      free(memory);
    }
    template<size_t N>
    void writeBytes(uint64_t v){
      for(size_t i=0;i<N;++i)
        memory[offset++] = (v>>(8*i))&0xff;
    }
    void write8u(uint8_t v){
      writeBytes<1>(v);
    }
    void write16u(uint16_t v){
      writeBytes<2>(v);
    }
    void write32u(uint32_t v){
      writeBytes<4>(v);
    }
    void write64u(uint64_t v){
      writeBytes<8>(v);
    }
    void call(){
      using F = void(*)();
      reinterpret_cast<F>(memory)();
    }
  protected:
    size_t  offset;
    uint8_t*memory;
    size_t  pagesize;
};

class Instruction{
  public:
    Instruction(Type t):type(t){}
    virtual ~Instruction(){}
    virtual void write(Memory&mem)const = 0;
  protected:
    Type type;
};

constexpr size_t usedBytes(size_t value){
  size_t counter = 0;
  size_t const maxNofBytes = sizeof(decltype(value));
  size_t const byteBits = 8;
  for(size_t i = 0; i < maxNofBytes; ++i)
    if(value >= (1llu << (byteBits*i)))counter++;
  return counter;
}

template<Type type,size_t OP_CODE,typename VALUE>
class Basic: public Instruction{
  public:
    Basic(VALUE v):Instruction(type),value(v){}
    virtual ~Basic(){}
    virtual void write(Memory&mem)const override final{
      mem.writeBytes<usedBytes(OP_CODE)>(OP_CODE);
      mem.writeBytes<sizeof(VALUE)     >(value);
    }
  protected:
    VALUE value;
};

template<Type type,size_t OP_CODE>
class NoValue: public Instruction{
  public:
    NoValue():Instruction(type){}
    ~NoValue(){}
    virtual void write(Memory&mem)const override final{
      mem.writeBytes<usedBytes(OP_CODE)>(OP_CODE);
    }
};






class MovEAX         :public Basic  <Type::MOV_EAX          ,0xb8  ,uint32_t>{public:MovEAX         (uint32_t v):Basic  (v){}};
class MovEDI         :public Basic  <Type::MOV_EDI          ,0xbf  ,uint32_t>{public:MovEDI         (uint32_t v):Basic  (v){}};
class MovESI         :public Basic  <Type::MOV_ESI          ,0xbe  ,uint32_t>{public:MovESI         (uint32_t v):Basic  (v){}};
class MovEDX         :public Basic  <Type::MOV_EDX          ,0xba  ,uint32_t>{public:MovEDX         (uint32_t v):Basic  (v){}};
class MovECX         :public Basic  <Type::MOV_ECX          ,0xb9  ,uint32_t>{public:MovECX         (uint32_t v):Basic  (v){}};
class MovR8D         :public Basic  <Type::MOV_R8D          ,0xb841,uint32_t>{public:MovR8D         (uint32_t v):Basic  (v){}};
class MovR9D         :public Basic  <Type::MOV_R9D          ,0xb941,uint32_t>{public:MovR9D         (uint32_t v):Basic  (v){}};
class Pushq          :public Basic  <Type::PUSHQ            ,0x68  ,uint32_t>{public:Pushq          (uint32_t v):Basic  (v){}};
class PushRBP        :public NoValue<Type::PUSH_RBP         ,0x55           >{public:PushRBP        (          ):NoValue( ){}};
class PopRBP         :public NoValue<Type::POP_RBP          ,0x5d           >{public:PopRBP         (          ):NoValue( ){}};
class PopRAX         :public NoValue<Type::POP_RAX          ,0x58           >{public:PopRAX         (          ):NoValue( ){}};
class CallRAX        :public NoValue<Type::CALL_RAX         ,0xd0ff         >{public:CallRAX        (          ):NoValue( ){}};
class CallRAXIndirect:public NoValue<Type::CALL_RAX_INDIRECT,0x10ff         >{public:CallRAXIndirect(          ):NoValue( ){}};
class Retq           :public NoValue<Type::RETQ             ,0xc3           >{public:Retq           (          ):NoValue( ){}};
class MovRAX         :public Basic  <Type::MOV_RAX          ,0xb848,uint64_t>{public:MovRAX         (uint64_t v):Basic  (v){}};
class MovabsRDI      :public Basic  <Type::MOVABS_RDI       ,0xbf48,uint64_t>{public:MovabsRDI      (uint64_t v):Basic  (v){}};
class MovabsRSI      :public Basic  <Type::MOVABS_RSI       ,0xbe48,uint64_t>{public:MovabsRSI      (uint64_t v):Basic  (v){}};
class MovabsRDX      :public Basic  <Type::MOVABS_RDX       ,0xba48,uint64_t>{public:MovabsRDX      (uint64_t v):Basic  (v){}};
class MovabsRCX      :public Basic  <Type::MOVABS_RCX       ,0xb948,uint64_t>{public:MovabsRCX      (uint64_t v):Basic  (v){}};
class MovabsR8       :public Basic  <Type::MOVABS_R8        ,0xb849,uint64_t>{public:MovabsR8       (uint64_t v):Basic  (v){}};
class MovabsR9       :public Basic  <Type::MOVABS_R9        ,0xb949,uint64_t>{public:MovabsR9       (uint64_t v):Basic  (v){}};

class Program{
  public:
    void add(std::shared_ptr<Instruction>const&inst){
      instructions.push_back(inst);
    }
    void writeProgram(Memory&mem){
      for(auto const&i:instructions)
        i->write(mem);
    }
  protected:
    std::vector<std::shared_ptr<Instruction>>instructions;
};

}

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
    void movR8D(uint32_t v){
      mem[offset++] = 0x41;
      mem[offset++] = 0xb8;
      write32u(v);
    }
    void movR9D(uint32_t v){
      mem[offset++] = 0x41;
      mem[offset++] = 0xb9;
      write32u(v);
    }

    void movRAX(uint64_t v){
      mem[offset++] = 0x48;
      mem[offset++] = 0xb8;
      write64u(v);
    }
    void pushq(uint32_t a){
      mem[offset++] = 0x68;
      write32u(a);
    }
    void pushRBP(){
      mem[offset++] = 0x55;
    }
    void popRBP(){
      mem[offset++] = 0x5d;
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
    void popRAX(){
      mem[offset++] = 0x58;
    }
    void movabsRDI(uint64_t a){
      mem[offset++] = 0x48;
      mem[offset++] = 0xbf;
      write64u(a);
    }
    void movabsRSI(uint64_t a){
      mem[offset++] = 0x48;
      mem[offset++] = 0xbe;
      write64u(a);
    }
    void movabsRDX(uint64_t a){
      mem[offset++] = 0x48;
      mem[offset++] = 0xba;
      write64u(a);
    }
    void movabsRCX(uint64_t a){
      mem[offset++] = 0x48;
      mem[offset++] = 0xb9;
      write64u(a);
    }
    void movabsR8(uint64_t a){
      mem[offset++] = 0x49;
      mem[offset++] = 0xb8;
      write64u(a);
    }
    void movabsR9(uint64_t a){
      mem[offset++] = 0x49;
      mem[offset++] = 0xb9;
      write64u(a);
    }
    using FCE = void(*)();
    void writeCall(FCE const&f){
      movRAX((uint64_t)((uint64_t*)f));
      callRAX();
    }
    void movlRSP4(uint32_t d){
      mem[offset++]=0xc7;
      mem[offset++]=0x44;
      mem[offset++]=0x24;
      mem[offset++]=0x04;
      write32u(d);
    }
    void pushParam(size_t n,size_t d){
      if     (n==0)movabsRDI(d);
      else if(n==1)movabsRSI(d);
      else if(n==2)movabsRDX(d);
      else if(n==3)movabsRCX(d);
      else if(n==4)movabsR8 (d);
      else if(n==5)movabsR9 (d);
      else{
        pushq(d&0xffffffff);
        movlRSP4(d>>32);
      }
    }
    void popParams(size_t n){
      for(size_t i=6;i<n;++i)
        popRAX();
    }
    void voidCall(){
      using F = void(*)();
      reinterpret_cast<F>(mem)();
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

size_t nofTimesHiWasCalled = 0;

void uint64ParamCall(
    uint64_t a,
    uint64_t b,
    uint64_t c,
    uint64_t d,
    uint64_t e,
    uint64_t f,
    uint64_t g,
    uint64_t h,
    uint64_t i
    
    ){
  std::cerr << "uint64ParamCall: " << a << std::endl;
  std::cerr << "uint64ParamCall: " << b << std::endl;
  std::cerr << "uint64ParamCall: " << c << std::endl;
  std::cerr << "uint64ParamCall: " << d << std::endl;
  std::cerr << "uint64ParamCall: " << e << std::endl;
  std::cerr << "uint64ParamCall: " << f << std::endl;
  std::cerr << "uint64ParamCall: " << g << std::endl;
  std::cerr << "uint64ParamCall: " << h << std::endl;
  std::cerr << "uint64ParamCall: " << i << std::endl;
}

void uint32ParamCall(
    uint32_t a,
    uint32_t b,
    uint32_t c,
    uint32_t d,
    uint32_t e,
    uint32_t f,
    uint32_t g,
    uint32_t h,
    uint32_t i
    ){
  std::cerr << "uint32ParamCall: " << a << std::endl;
  std::cerr << "uint32ParamCall: " << b << std::endl;
  std::cerr << "uint32ParamCall: " << c << std::endl;
  std::cerr << "uint32ParamCall: " << d << std::endl;
  std::cerr << "uint32ParamCall: " << e << std::endl;
  std::cerr << "uint32ParamCall: " << f << std::endl;
  std::cerr << "uint32ParamCall: " << g << std::endl;
  std::cerr << "uint32ParamCall: " << h << std::endl;
  std::cerr << "uint32ParamCall: " << i << std::endl;
}

void uint16ParamCall(
    uint16_t a,
    uint16_t b,
    uint16_t c,
    uint16_t d,
    uint16_t e,
    uint16_t f,
    uint16_t g,
    uint16_t h,
    uint16_t i
    ){
  std::cerr << "uint16ParamCall: " << a << std::endl;
  std::cerr << "uint16ParamCall: " << b << std::endl;
  std::cerr << "uint16ParamCall: " << c << std::endl;
  std::cerr << "uint16ParamCall: " << d << std::endl;
  std::cerr << "uint16ParamCall: " << e << std::endl;
  std::cerr << "uint16ParamCall: " << f << std::endl;
  std::cerr << "uint16ParamCall: " << g << std::endl;
  std::cerr << "uint16ParamCall: " << h << std::endl;
  std::cerr << "uint16ParamCall: " << i << std::endl;
}

void uint8ParamCall(
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint8_t d,
    uint8_t e,
    uint8_t f,
    uint8_t g,
    uint8_t h,
    uint8_t i
    ){
  std::cerr << "uint8ParamCall: " << a << std::endl;
  std::cerr << "uint8ParamCall: " << b << std::endl;
  std::cerr << "uint8ParamCall: " << c << std::endl;
  std::cerr << "uint8ParamCall: " << d << std::endl;
  std::cerr << "uint8ParamCall: " << e << std::endl;
  std::cerr << "uint8ParamCall: " << f << std::endl;
  std::cerr << "uint8ParamCall: " << g << std::endl;
  std::cerr << "uint8ParamCall: " << h << std::endl;
  std::cerr << "uint8ParamCall: " << i << std::endl;
}

void uint64_8ParamCall(
    uint64_t a,
    uint8_t  b,
    uint64_t c,
    uint8_t  d,
    uint64_t e,
    uint8_t  f,
    uint64_t g,
    uint8_t  h,
    uint64_t i
    ){
  std::cerr << "uint8ParamCall: " << a << std::endl;
  std::cerr << "uint8ParamCall: " << b << std::endl;
  std::cerr << "uint8ParamCall: " << c << std::endl;
  std::cerr << "uint8ParamCall: " << d << std::endl;
  std::cerr << "uint8ParamCall: " << e << std::endl;
  std::cerr << "uint8ParamCall: " << f << std::endl;
  std::cerr << "uint8ParamCall: " << g << std::endl;
  std::cerr << "uint8ParamCall: " << h << std::endl;
  std::cerr << "uint8ParamCall: " << i << std::endl;
}


    
    

//64bit arguments
//last argument is first pushed to the stack
//...
//push + movl - last argument
//push + movl
//pushq $0xffffffffdadadada movl $0xdadadada,0x4(%rsp)
//movabs r9
//movabs r8
//movabs rcx
//movabs rdx
//movabs rsi
//movabs rdi  - first argument
//
//
//32bit arguments
//last argument is first pushed to the stack
//mov r9d
//mov r8d
//mov ecx
//mov edx
//mov esi
//mov edi - first argument

void hi(){
  std::cerr << "hi was called" << std::endl;
  nofTimesHiWasCalled++;
}

size_t nofTimesWorldWasCalled = 0;

void world(){
  std::cerr << "world was called" << std::endl;
  nofTimesWorldWasCalled++;
}

template<typename...A>
void ncall(A...a){
  uint64_t ar[] = {a...};
  std::cerr << "ncall(";
  for(size_t i=0;i<sizeof...(a);++i)
    std::cerr << ar[i] << ", ";
  std::cerr << ")" << std::endl;
}

int main(int,char*[]) {
  auto mem  = assembler::Memory(2);
  auto prog = assembler::Program();
  prog.add(std::make_shared<assembler::MovabsRSI>(1));
  prog.add(std::make_shared<assembler::MovabsRDI>(0));
  prog.add(std::make_shared<assembler::MovRAX   >((uint64_t)((uint64_t*)ncall<uint64_t,uint64_t>)));
  prog.add(std::make_shared<assembler::CallRAX  >());
  prog.writeProgram(mem);
  mem.call();



#if 0
  /*
decltype(uint64ParamCall  )* g    ;
decltype(uint32ParamCall  )* g32  ;
decltype(uint16ParamCall  )* g16  ;
decltype(uint8ParamCall   )* g8   ;
decltype(uint64_8ParamCall)* g64_8;

  g = uint64ParamCall;
  g(
       0xAAAAAAAAAAAAAAAA
      ,0xBBBBBBBBBBBBBBBB
      ,0xCCCCCCCCCCCCCCCC
      ,0xDDDDDDDDDDDDDDDD
      ,0xEEEEEEEEEEEEEEEE
      ,0xFAFAFAFAFAFFAFAF
      ,0xAAAAAAAAAAAAAAAA
      ,0xBBBBBBBBBBBBBBBB
      ,0xDADADADADADADADA
      );

  g32 = uint32ParamCall;
  g32(
       0xAAAAAAAA
      ,0xBBBBBBBB
      ,0xCCCCCCCC
      ,0xDDDDDDDD
      ,0xEEEEEEEE
      ,0xFAFAFAFA
      ,0xAAAAAAAA
      ,0xBBBBBBBB
      ,0xDADADADA
      );
  g16 = uint16ParamCall;
  g16(
       0xAAAA
      ,0xBBBB
      ,0xCCCC
      ,0xDDDD
      ,0xEEEE
      ,0xFAFA
      ,0xAAAA
      ,0xBBBB
      ,0xDADA
     );
  g8 = uint8ParamCall;
  g8(
       0xAA
      ,0xBB
      ,0xCC
      ,0xDD
      ,0xEE
      ,0xFF
      ,0xAA
      ,0xBB
      ,0xDA
     );
  g64_8 = uint64_8ParamCall;
  g64_8(
       0xAAAAAAAAAAAAAAAA
      ,0xBB
      ,0xCCCCCCCCCCCCCCCC
      ,0xDD
      ,0xEEEEEEEEEEEEEEEE
      ,0xFF
      ,0xAAAAAAAAAAAAAAAA
      ,0xBB
      ,0xDADADADADADADADA
      );
  // */

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

  hi();
  hi();
  world();
  world();
  using FCE = void(*)();
  FCE f = hi;
  auto rasm = RuntimeAsm(2);
  //rasm.movEAX(101);
  //rasm.retq();
  //std::cerr << rasm.call() << std::endl;
  
  rasm.writeCall((FCE)hi);
  rasm.writeCall((FCE)world);

  rasm.pushParam(0,111);
  rasm.writeCall((FCE)ncall<uint64_t>);
  rasm.popParams(1);

  rasm.pushParam(1,51);
  rasm.pushParam(0,50);
  rasm.writeCall((FCE)ncall<uint64_t,uint64_t>);
  rasm.popParams(2);

  rasm.pushParam(2,52);
  rasm.pushParam(1,51);
  rasm.pushParam(0,50);
  rasm.writeCall((FCE)ncall<uint64_t,uint64_t,uint64_t>);
  rasm.popParams(3);

  rasm.pushParam(3,53);
  rasm.pushParam(2,52);
  rasm.pushParam(1,51);
  rasm.pushParam(0,50);
  rasm.writeCall((FCE)ncall<uint64_t,uint64_t,uint64_t,uint64_t>);
  rasm.popParams(4);

  rasm.pushParam(4,54);
  rasm.pushParam(3,53);
  rasm.pushParam(2,52);
  rasm.pushParam(1,51);
  rasm.pushParam(0,50);
  rasm.writeCall((FCE)ncall<uint64_t,uint64_t,uint64_t,uint64_t,uint64_t>);
  rasm.popParams(5);

  rasm.pushParam(5,55);
  rasm.pushParam(4,54);
  rasm.pushParam(3,53);
  rasm.pushParam(2,52);
  rasm.pushParam(1,51);
  rasm.pushParam(0,50);
  rasm.writeCall((FCE)ncall<uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t>);
  rasm.popParams(6);
// */

  rasm.pushParam(6,56);
  rasm.pushParam(5,55);
  rasm.pushParam(4,54);
  rasm.pushParam(3,53);
  rasm.pushParam(2,52);
  rasm.pushParam(1,51);
  rasm.pushParam(0,50);
  rasm.writeCall((FCE)ncall<uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t>);
  rasm.popParams(7);

  rasm.pushParam(7,57);
  rasm.pushParam(6,56);
  rasm.pushParam(5,55);
  rasm.pushParam(4,54);
  rasm.pushParam(3,53);
  rasm.pushParam(2,52);
  rasm.pushParam(1,51);
  rasm.pushParam(0,50);
  rasm.writeCall((FCE)ncall<uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t>);
  rasm.popParams(8);


  //rasm.writeCall((FCE)world);
  rasm.retq();
  //hi();
  //hi();
  //hi();
  //hi();
  //f();
  //f();
  rasm.voidCall();

  std::cerr << "nofTimesHiWasCalled   : " << nofTimesHiWasCalled    << std::endl;
  std::cerr << "nofTimesWorldWasCalled: " << nofTimesWorldWasCalled << std::endl;
  ////f();
  ////f();
  ////hi();
  return 0;
#endif

}
