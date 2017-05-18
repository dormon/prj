#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

class Program;
class Shader{
  public:
    Shader(){};
    std::vector<std::weak_ptr<Program>>_programs;
};

class Program: public std::enable_shared_from_this<Program>{
  public:
    using ShaderPointer  = std::shared_ptr<Shader>;
    using ShaderPointers = std::vector<ShaderPointer>;
    ShaderPointers _shaders;
    Program(std::vector<std::shared_ptr<Shader>>const&ss):_shaders(ss){
      for(auto const&x:this->_shaders)
        x->_programs.push_back(this->shared_from_this());
    }
};

void test(std::initializer_list<int>const&a){
  std::vector<int>x = a;
}

class Simple{
  public:
    std::vector<int>a;
    Simple(std::initializer_list<int>const&x):a(x){}
};

class Complex{
  public:
    std::vector<std::shared_ptr<int>>a;
    Complex(std::initializer_list<std::shared_ptr<int>>const&x):a(x){}
};

int main(){
  std::make_shared<Program>(Program::ShaderPointers({std::make_shared<Shader>(),std::make_shared<Shader>()}));
  test({1,2,3});
  Simple({1,2,3});
  Complex({std::make_shared<int>(2),std::make_shared<int>(3)});
  //this does not work
  //std::make_shared<Simple>({1,2,3});
  return 0;
}


