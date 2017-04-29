#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Test{
  public:
    int a =1;
    Test(){
      std::cout<<"Test::Test(): "<<this<<std::endl;
    }
    ~Test(){
      std::cout<<"Test::~Test(): "<<this<<std::endl;
    }
};

int main(){
  uint8_t*ptr = new uint8_t[sizeof(std::shared_ptr<Test>)];
  //std::cout<<"ptr: "<<ptr<<std::endl;
  new(ptr)std::shared_ptr<Test>(new Test());
  //std::cout<<"t: "<<t<<std::endl;
  //new(&*t)Test();


  //delete ((Test*)(&*((std::shared_ptr<Test>*)ptr)));
  ((std::shared_ptr<Test>*)ptr)->~shared_ptr();
  delete[]ptr;



  //char*ptr=new char[sizeof(std::shared_ptr<int>)];
  //new(ptr)std::shared_ptr<int>();

  

  //auto p=std::make_shared<int>(10);
  //*((std::shared_ptr<int>&)ptr)=123;
  //std::cout<<*p<<std::endl;
  //std::cout<<*((std::shared_ptr<int>&)ptr)<<std::endl;


  //((std::shared_ptr<int>*)ptr)->~shared_ptr();
  //delete[]ptr;


  return 0;
}





