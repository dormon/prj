#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstring>
#include <functional>
#include <typeinfo>
#include <set>
#include <chrono>
#include <stack>
#include <thread>
#include <cassert>

class Base{
  public:
    Base(){
      std::cerr << "Base::Base()" << std::endl;
    }
    Base(int a,float b):a(a),b(b){
      std::cerr << "Base::Base("<<a<<","<<b<<")" << std::endl;
    }
    ~Base(){
      std::cerr << "Base::~Base()" << std::endl;
    }
    int   a = 0;
    float b = 0;
};

int main(){
  {
    std::cerr << "emplace" << std::endl;
    std::map<std::string,Base>vars;
    {
      auto w = vars.emplace(std::piecewise_construct,std::forward_as_tuple("a"),std::forward_as_tuple(1,3.f));
      std::cerr << "first  : " << std::get<1>(w) << std::endl;
      std::cerr << "element: " << std::get<0>(w)->first << " " << std::get<0>(w)->second.a << " " << std::get<0>(w)->second.b << std::endl;
    }
    {
      auto w = vars.emplace(std::piecewise_construct,std::forward_as_tuple("a"),std::forward_as_tuple(2,6.f));
      std::cerr << "first  : " << std::get<1>(w) << std::endl;
      std::cerr << "element: " << std::get<0>(w)->first << " " << std::get<0>(w)->second.a << " " << std::get<0>(w)->second.b << std::endl;
    }
  }

  std::cerr << std::endl;

  {
    std::cerr << "try_emplace" << std::endl;
    std::map<std::string,Base>vars;
    {
      auto w = vars.try_emplace("a",1,3.f);
      std::cerr << "first  : " << std::get<1>(w) << std::endl;
      std::cerr << "element: " << std::get<0>(w)->first << " " << std::get<0>(w)->second.a << " " << std::get<0>(w)->second.b << std::endl;
    }
    {
      auto w = vars.try_emplace("a",2,6.f);
      std::cerr << "first  : " << std::get<1>(w) << std::endl;
      std::cerr << "element: " << std::get<0>(w)->first << " " << std::get<0>(w)->second.a << " " << std::get<0>(w)->second.b << std::endl;
    }
  }
  return 0;
}
