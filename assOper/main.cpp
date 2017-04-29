#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>



int main(){
  int a,b,c;
  a=12;
  b=1233;
  c=123;
  a=b*=c=123;
  std::cout<<a<<std::endl;
  std::cout<<b<<std::endl;
  std::cout<<c<<std::endl;


  return 0;
}
