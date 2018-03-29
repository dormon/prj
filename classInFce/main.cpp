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

class Base{
};

class Child: private Base{
  private:
    Child(){}
};

class Function{
  public:

};

template<typename RETURN_TYPE,typename... ARGUMENT_TYPES>
Base*factory( RETURN_TYPE(*fce)(ARGUMENT_TYPE...) ){
  class Child: public Base{
    public:
      Child(){}
    protected:
      float a;
  };
  return new Child();
}


int main(){
  return 0;
}

