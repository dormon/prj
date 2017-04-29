#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>




int main(){
  std::map<const char*,int>asd;
  asd["ahoj"]=32;
  asd["svete"]=42;
  std::string key="svete";
  std::cout<<asd[key.c_str()]<<std::endl;
  std::cout<<asd["svete"]<<std::endl;

  std::map<std::string,int>vsd;
  vsd["ahoj"]=32;
  vsd["svete"]=42;
  std::string vkey="svete";
  std::cout<<vsd[vkey]<<std::endl;
  std::cout<<vsd["svete"]<<std::endl;



  return 0;
}
