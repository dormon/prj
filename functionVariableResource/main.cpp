#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

class Statement;

template<typename T>
class Instance{
  public:
    Instance(T const&d):data(d){}
    T data;
};

template<typename T>
class Variable{
  public:
    std::shared_ptr<Instance<T>>instance;
};

class Statement{
  public:
    virtual void operator()() = 0;
};

class StatementList: public Statement{
  public:
    virtual void operator()() override {
      for(auto const&x:statements)
        (*x)();
    }
    std::vector<std::shared_ptr<Statement>>statements;
};

class If: public Statement{
  public:
    virtual void operator()() override{
      if(conditionVariable.instance->data)
        (*statement.instance->data)();
    }
    Variable<bool>conditionVariable;
    Variable<std::shared_ptr<Statement>>statement;
};

class While: public Statement{
  public:
    virtual void operator()() override{
      while(conditionVariable.instance->data)
        (*statement.instance->data)();
    }
    Variable<bool>conditionVariable;
    Variable<std::shared_ptr<Statement>>statement;
};

int main() {
  return 0;
}
