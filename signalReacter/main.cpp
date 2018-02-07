#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<sstream>

#include<SignalReacter.h>

class Signal{
  public:
};

class Resource: public SignalReacter<Signal>{
  public:
    float data = 0.f;
    size_t updateTicks = 1;
};


enum class InputKind{
  FUNCTION,
  RESOURCE,
};

class Input{
  public:
    InputKind inputKind;
    Input(InputKind const&k):inputKind(k){}
};

class Function;
class Resource;

class FunctionInput: public Input{
  public:
    std::shared_ptr<Function>fce;
    size_t outputId;
    FunctionInput(
        std::shared_ptr<Function>const&f,
        size_t                         i):
      Input(InputKind::FUNCTION),
      fce(f),
      outputId(i){}
};

class ResourceInput: public Input{
  public:
    std::shared_ptr<Resource>res;
    size_t seenUpdateTicks = 0;
    ResourceInput(std::shared_ptr<Resource>const&r):Input(InputKind::RESOURCE),res(r){}
};

class Function: public SignalReacter<Signal>{
  public:
    virtual void operator()(std::vector<float>&outputs,std::vector<float>const&inputs) = 0;
    Function(size_t i,size_t o):inputs(i),outputs(o){}
    virtual void bindInput(size_t i,std::shared_ptr<Function>const&f,size_t j){
      addSource(&*f);
      inputs.at(i) = std::make_shared<FunctionInput>(f,j);
    }
    virtual void bindInput(size_t i,std::shared_ptr<Resource>const&r){
      addSource(&*r);
      inputs.at(i) = std::make_shared<ResourceInput>(r);
    }
    std::shared_ptr<Resource>getOutput(size_t i)const{return outputs.at(i);}
    std::vector<std::shared_ptr<Input   >>inputs;
    std::vector<std::shared_ptr<Resource>>outputs;
};

/*
class Add: public Function{
  Add():Function(2,1){}
  void operator()(std::vector<float>&outputs,std::vector<float>const&inputs)override{

  }
};
*/

int main(){

  return 0;
}
