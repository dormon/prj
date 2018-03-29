#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

class Parameter{
  public:
    virtual ~Parameter(){}
};

template<typename T>
class TypedParameter{
  public:
    TypedParameter(T const&d):data(d){}
    T data;
};

class Function{
  public:
    void operator()();
    virtual ~Function(){
      for(auto const&x:params)
        delete x.second;
    }
  protected:
    virtual void compute() = 0;
    std::map<std::string,Parameter*>params;
};

enum ParamTableOperation{
  REGISTER_PARAMETER,
  REGISTER_OUTPUT,
  GET_PARAMETER,
  SET_OUTPUT,
};
void paramTable(ParamTableOperation const&op,std::string const&name = "",float*data = nullptr){
  static std::map<std::string,float>params;
  static std::map<std::string,float*>outputs;
  if(op == REGISTER_PARAMETER)
    params[name] = *data;
  if(op == REGISTER_OUTPUT)
    outputs[name] = data;
  if(op == GET_PARAMETER)
    *data = params.at(name);
  if(op == SET_OUTPUT)
    *outputs[name] = *data;
}

void addParam(std::string const&name,float d = 0.f){
  paramTable(REGISTER_PARAMETER,name,&d);
}

void addOutput(std::string const&name,float*d = nullptr){
  paramTable(REGISTER_OUTPUT,name,d);
}

float getParam(std::string const&name){
  float res;
  paramTable(GET_PARAMETER,name,&res);
  return res;
}

void setOutput(std::string const&name,float d){
  paramTable(SET_OUTPUT,name,&d);
}

void factorial(){
  addParam("n");
  addOutput("result");

  float result = 1.f;
  float i=getParam("n");
  while(i>1.f){
    result *= i;
    i-=1.f;
  }

  setOutput("result",result);
};

class AtomicFunction{

};

int main() {
  return 0;
}
