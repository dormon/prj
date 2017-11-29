#include<iostream>
#include<vector>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>
#include<string>
#include<type_traits>
#include<sstream>
#include<typeinfo>
#include<utility>
#include<tuple>

class Object{
  public:
    Object(Object const&){
      std::cout << "Object(Object const&)" << std::endl;
    }
    Object(){
      std::cout << "Object::Object()" << std::endl;
    }
    ~Object(){
      std::cout << "Object::~Object()" << std::endl;
    }
};

using Type = std::tuple<Object const&,Object const,Object&,Object,void>;

typename std::tuple_element<4,Type>::type fce0(typename std::tuple_element<0,Type>::type o){static_cast<void>(o);}
typename std::tuple_element<4,Type>::type fce1(typename std::tuple_element<1,Type>::type o){static_cast<void>(o);}
typename std::tuple_element<4,Type>::type fce2(typename std::tuple_element<2,Type>::type o){static_cast<void>(o);}
typename std::tuple_element<4,Type>::type fce3(typename std::tuple_element<3,Type>::type o){static_cast<void>(o);}

int main(){
  std::cout << "create object" << std::endl;
  Object a;
  std::cout << std::endl;

  std::cout << "call const&" << std::endl;
  fce0(a);
  std::cout << std::endl;

  std::cout << "call const" << std::endl;
  fce1(a);
  std::cout << std::endl;

  std::cout << "call &" << std::endl;
  fce2(a);
  std::cout << std::endl;

  std::cout << "call " << std::endl;
  fce3(a);
  std::cout << std::endl;

  return 0;
}
