#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Class{
  public:
    int32_t a;
    Class(){
      std::cout<<"Class::Class()"<<std::endl;
    }
    ~Class(){
      std::cout<<"Class::~Class()"<<std::endl;
    }
};

int main(){
  uint8_t*ptr=new uint8_t[sizeof(std::shared_ptr<Class>)];
  new(ptr)std::shared_ptr<Class>(new Class());

  

  /*
  auto p=std::make_shared<Class>();
  ((std::shared_ptr<Class>&)ptr)->a=123;
  std::cout<<p->a<<std::endl;
  */

  *((std::shared_ptr<Class>*)ptr) = std::make_shared<Class>();


  ((std::shared_ptr<Class>*)ptr)->~shared_ptr();
  delete[]ptr;


  return 0;
}





