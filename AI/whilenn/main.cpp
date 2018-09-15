#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>

class Memory{
  public:
    virtual float&at(size_t i) = 0;
    virtual ~Memory(){}
};

class TableMemory: public Memory{
  public:
    TableMemory(std::shared_ptr<Memory>const&m,std::vector<size_t>const&t):mem(m),table(t){}
    virtual float&at(size_t i)override{
      return mem->at(table.at(i));
    }
  protected:
    std::shared_ptr<Memory>mem;
    std::vector<size_t>table;
};

class OffsetMemory: public Memory{
  public:
    OffsetMemory(std::shared_ptr<Memory>const&m,size_t o):mem(m),offset(o){}
    virtual float&at(size_t i)override{
      return mem->at(i+offset);
    }
  protected:
    std::shared_ptr<Memory>mem;
    size_t offset;
};

class DirectMemory: public Memory{
  public:
    DirectMemory(float*m):mem(m){}
    virtual float&at(size_t i)override{
      return mem[i];
    }
  protected:
    float*mem;
};

class Function{
  public:
    virtual ~Function(){}
    virtual size_t getNofLocals()const = 0;
    virtual size_t getNofArguments()const = 0;
};

class NonListFunction: public Function{
};

class BindFunction: public NonListFunction{
  public:
    BindFunction(std::shared_ptr<Function>f,size_t p):fce(f),bindParameter(p){}
    virtual size_t getNofArguments()const override{return fce->getNofArguments()-1;}
    virtual size_t getNofLocals()const override{return std::max(static_cast<size_t>(1),fce->getNofLocals());}
    size_t getBindParameter()const{return bindParameter;}
  protected:
    std::shared_ptr<Function>fce;
    size_t bindParameter;
};

class AtomicFunction: public NonListFunction{
  public:
    AtomicFunction(size_t n):nofArguments(n){}
    virtual size_t getNofArguments()const override{return nofArguments;}
    virtual size_t getNofLocals()const override{return 0;}
  protected:
    size_t nofArguments;
};

class PairFunctionElement{
  public:
    std::shared_ptr<Function>fce;
    std::vector<size_t>mapping; // |mapping| == |fce|
};

class ListFunction: public Function{
  public:
    ListFunction(std::array<PairFunctionElement,2>const&a):fces(a){}
    virtual size_t getNofArguments()const override{
      size_t n=0;
      for(auto const&f:fces)
        for(auto const&m:f.mapping)
          n = std::max(n,m);
      return n;
    }
    virtual size_t getNofLocals()const override{
      size_t n=0;
      for(auto const&f:fces)
        n = std::max(n,f.fce->getNofLocals());
      return n;
    }
  protected:
    std::shared_ptr<NonListFunction>head;
    std::vector<size_t>headMapping;
    std::shared_ptr<ListFunction>tail;
};

class WhileFunction: public NonListFunction{
  public:
    WhileFunction(std::shared_ptr<Function>const&f):fce(f){}
    virtual size_t getNofArguments()const override{return fce->getNofArguments()+1;}
    virtual size_t getNofLocals()const override{return fce->getNofLocals();}
  protected:
    std::shared_ptr<Function>fce;
};

class IfFunction: public WhileFunction{};

// BIND(fce(a,b,c,d,e),c) -> fce<c>(a,b,d,e)
//
//
//


// Program transformations:
// Atom0     -> Atom1
// Fce       -> Bind
// If(fce)   -> While(fce)
// If(fce0)  -> If(fce1)
// 
//
//
//
// bindParameter
//
//
//
//
//


int main(){
  return 0;
}





