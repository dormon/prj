#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

template<unsigned nofInputs,const char*name>
class Function{
  public:
    static const char* fceName = name;
    std::shared_ptr<Function> inputs[nofInputs];
};

//template<unsigned nofInputs,const char*name>
//static const std::string Function::fceName = std::string(name);

char asd[]="sin";

int main(){
  Function<1,asd>a;
  std::cout<<a.fceName<<std::endl;
  return 0;
}


