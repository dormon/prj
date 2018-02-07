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
    Object(){
      std::cout << "this: " << this << " ";
      std::cout << "Object::Object()" << std::endl;
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

void print(std::vector<std::reference_wrapper<Object const>>const&objs){
  for(auto const&x:objs)
    x.get().print();
}

std::function<void()>createCommand(std::map<std::string,Object>const&objs){
  std::vector<std::reference_wrapper<Object const>>refs;
  for(auto const&x:objs)
    refs.push_back(std::ref(x.second));
  return std::bind(print,refs);
};

int main(){
  std::function<void()>cmd;

  std::cout << "0##########################" << std::endl;
  //{
    std::cout << "1##########################" << std::endl;
    std::map<std::string,Object>objs;
    objs["a"] = Object(1);
    objs["b"] = Object(2);
    objs["c"] = Object(3);
    std::cout << "2##########################" << std::endl;
    cmd = createCommand(objs);
    std::cout << "3##########################" << std::endl;
  //}
  std::cout << "4##########################" << std::endl;
  cmd();
  std::cout << "5##########################" << std::endl;
  auto const cmd2 = cmd;
  std::cout << "6##########################" << std::endl;
  return 0;
};
