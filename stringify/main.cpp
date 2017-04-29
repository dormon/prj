#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#define BODY_OF_DO\
  int a;\
  a=32;\
  a*=2;\
  std::cout<<a<<std::endl
  
#define STRINGIFY(...) #__VA_ARGS__

int main(){
  BODY_OF_DO;
  std::cout<<STRINGIFY(BODY_OF_DO)<<std::endl;
  return 0;
}





