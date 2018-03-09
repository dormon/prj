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

class Object{
  public:
    Object(float a):data(a){
      std::cout << "this: " << this << " ";
      std::cout << "Object::Object(float)" << std::endl;
    }
    ~Object(){
      std::cout << "this: " << this << " ";
      std::cout << "Object::~Object()" << std::endl;
      data = 0;
    }
    void print()const{
      if(this == nullptr){
        std::cout << "ERROR - this == nullptr" << std::endl;
        return;
      }
      std::cout << "this: " << this << " ";
      std::cout << "Object::data == " << data << std::endl;
    }
    Object(Object const&o):data(o.data){
      std::cout << "this: " << this << " ";
      std::cout << "Object::Object(Object const&)" << std::endl;
    }
    float data;
};

void refFunction(Object const&o){
  o.print();
}

void valFunction(Object o){
  o.print();
}

std::function<void()>createRefRefCommand(Object const&obj){
  return std::bind(refFunction,std::ref(obj));
}

std::function<void()>createRefCommand(Object const&obj){
  std::cout << __LINE__ << std::endl;
  return std::bind(refFunction,obj);
}

std::function<void()>createValCommand(Object const&obj){
  return std::bind(valFunction,obj);
}

std::function<void()>createRefValCommand(Object const&obj){
  return std::bind(valFunction,std::ref(obj));
}

void b(std::vector<float>const& a){
  for(auto const&x:a)
    std::cout << x << std::endl;
}

int main(){
  std::function<void()>d;
  d = std::bind(b,std::vector<float>{1.f,2.f});
  d();



  std::function<void()>cmd;
  {
    Object a(23);
    cmd = createValCommand(a);
  }
  cmd();
  return 0;
};
