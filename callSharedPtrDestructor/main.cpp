#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

int main(){
  char*ptr=new char[sizeof(std::shared_ptr<int>)];
  new(ptr)std::shared_ptr<int>();

  

  auto p=std::make_shared<int>(10);
  *((std::shared_ptr<int>&)ptr)=123;
  std::cout<<*p<<std::endl;
  std::cout<<*((std::shared_ptr<int>&)ptr)<<std::endl;


  ((std::shared_ptr<int>*)ptr)->~shared_ptr();
  delete[]ptr;


  return 0;
}





