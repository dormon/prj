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

class Add;
class Mul;

class Instruction{
  public:
    size_t inputA;
    size_t inputB;
    size_t output;
    enum Type{
      ADD,
      MUL,
    }type;
    Instruction(Type t,size_t a,size_t b,size_t o):type(t),inputA(a),inputB(b),output(o){}
};

class Program{
  public:
    std::vector<Instruction>instructions;
    size_t nofLocals;
    size_t nofOutputs;
    size_t nofInputs;
};

class Operation{
  public:
    Operation(size_t i,size_t o):nofInputs(i),nofOutputs(o){}
    void setInput(size_t i,std::shared_ptr<Operation>const&o){
      if(i>=inputs.size())inputs.resize(i+1);
      inputs.at(i) = o;
    }
    void setInputMeta(Operation const*const&o){
      nofInputs = o->nofInputs;
      nofOutputs = o->nofOutputs;
      inputs = o->inputs;
    }
    virtual std::shared_ptr<Operation>getCopy()const = 0;
    Add operator+(Operation const&a);
    Mul operator*(Operation const&a);
    Add operator+(float a);
    Mul operator*(float a);
    std::vector<std::shared_ptr<Operation>>inputs;
    virtual void operator()() = 0;
    virtual float getOutput(size_t o = 0) = 0;
    virtual Program getProgram()const = 0;
    size_t nofInputs;
    size_t nofOutputs;
};

Add operator+(float a,Operation const&b);
Mul operator*(float a,Operation const&b);

class Var: public Operation{
  public:
    virtual void setOutput(size_t o,float d) = 0;
    virtual void operator()()override final{}
    virtual Program getProgram()const override{
      return {};
    }
    Var():Operation(0,0){}
};

class Scalar: public Var{
  public:
    Scalar(float d):output(d){}
    virtual void setOutput(size_t,float d)override{
      output = d;
    }
    virtual float getOutput(size_t = 0)override{
      return output;
    }
    virtual std::shared_ptr<Operation>getCopy()const override{
      return std::make_shared<Scalar>(output);
    }
    float output;
};

class Binary: public Operation{
  public:
    Binary():Operation(2,1){}
    virtual float getOutput(size_t = 0)override{
      (*this)();
      return output;
    }
    float output;
};

class Add: public Binary{
  public:
    Add():Binary(){}
    virtual void operator()() override{
      output = inputs.at(0)->getOutput(0) + inputs.at(1)->getOutput(0);
    }
    virtual std::shared_ptr<Operation>getCopy()const override{
      auto r = std::make_shared<Add>();
      r->setInputMeta(this);
      return r;
    }
    virtual Program getProgram()const override{
      auto aProgram = inputs.at(0)->getProgram();
      auto bProgram = inputs.at(1)->getProgram();
      auto r = Program{};
      r.instructions = aProgram.instructions;
      for(auto const&i:bProgram.instructions)
        r.instructions.push_back(i);
      r.instructions.push_back(Instruction(ADD));
      return r;
    }
};

class Mul: public Binary{
  public:
    Mul():Binary(){}
    virtual void operator()() override{
      output = inputs.at(0)->getOutput(0) * inputs.at(1)->getOutput(0);
    }
    virtual std::shared_ptr<Operation>getCopy()const override{
      auto r = std::make_shared<Mul>();
      r->setInputMeta(this);
      return r;
    }
};

Add operator+(float a,Operation const&b){
  Add res;
  res.setInput(0,std::make_shared<Scalar>(a));
  res.setInput(1,b.getCopy());
  return res;

}

Mul operator*(float a,Operation const&b){
  Mul res;
  res.setInput(0,std::make_shared<Scalar>(a));
  res.setInput(1,b.getCopy());
  return res;
}

Add Operation::operator+(Operation const&a){
  Add res;
  res.setInput(0,getCopy());
  res.setInput(1,a.getCopy());
  return res;
}

Mul Operation::operator*(Operation const&a){
  Mul res;
  res.setInput(0,getCopy());
  res.setInput(1,a.getCopy());
  return res;
}

Add Operation::operator+(float a){
  Add res;
  res.setInput(0,getCopy());
  res.setInput(1,std::make_shared<Scalar>(a));
  return res;
}

Mul Operation::operator*(float a){
  Mul res;
  res.setInput(0,getCopy());
  res.setInput(1,std::make_shared<Scalar>(a));
  return res;
}

int main(){
  auto a = Scalar(10);
  auto b = Scalar(20);
  auto c = 10*(a+b);

  c();
  std::cerr << c.getOutput() << std::endl;
  return 0;
}
