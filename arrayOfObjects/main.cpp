#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Class{
  public:
    int32_t*a = nullptr;
    Class(){
      std::cout<<"Class::Class()"<<std::endl;
      this->a = new int32_t;
    }
    ~Class(){
      std::cout<<"Class::~Class()"<<std::endl;
      delete this->a;
    }
};

int main(){
  Class*ar = new Class[10];
  delete[]ar;
  return 0;
}





