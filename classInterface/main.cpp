#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Argument{
};

class Interface{
  public:
    virtual std::string getMethodName(unsigned id){return "";}
    virtual unsigned    getNofMethods(){return 0;}
    virtual void call(std::string method,std::vector<Argument>const&arguments){}
};

int main(){
  return 0;
}
