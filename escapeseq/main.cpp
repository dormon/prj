#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>



int main(){
  //std::cout<<"a\hoj"<<std::endl;
  //std::cout<<"\x1ff"<<std::endl;
  std::cout<<"\777"<<std::endl;
  std::cout<<"\x00000000000000000000ff\x"<<std::endl;
  auto a="\U0001f34c";
  unsigned char const*p=(unsigned char const*)a;
  std::cout<<(unsigned)(unsigned char)p[0]<<std::endl;
  std::cout<<(unsigned)(unsigned char)p[1]<<std::endl;
  std::cout<<(unsigned)(unsigned char)p[2]<<std::endl;
  std::cout<<(unsigned)(unsigned char)p[3]<<std::endl;
  return 0;
}
