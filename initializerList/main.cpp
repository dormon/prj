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
    Program(ShaderPointers ss){
      this->_shaders = ss;
      for(auto const&x:ss){
        x->_programs.push_back(this->shared_from_this());
      }

    }
};

int main(){
  std::make_shared<Program>(Program::ShaderPointers({std::make_shared<Shader>(),std::make_shared<Shader>()}));
  return 0;
}


