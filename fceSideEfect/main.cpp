#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

int fce(int data,bool remember){
  static int _data;
  if(remember)_data=data;
  return _data;
}

int main(){
  std::cout<<fce(32 ,true)<<std::endl;
  std::cout<<fce(100,false)<<std::endl;
  std::cout<<fce(101,true)<<std::endl;
  std::cout<<fce(12,false)<<std::endl;
  return 0;
}
