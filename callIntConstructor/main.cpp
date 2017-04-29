#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>


int main(){

  char*ptr=new char[sizeof(int)];
  new(ptr)int(10);
  std::cout<<*((int*)ptr)<<std::endl;
  delete[]ptr;

  return 0;
}
