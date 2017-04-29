#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<algorithm>
#include<functional>
#include<utility>
#include<glm/glm.hpp>
#include<geCore/Text.h>
#include<geGL/geGL.h>

int main(int,char*[]){
  std::map<std::string,int>data;
  data["a"]=32;
  data["b"]=42;
  auto it = data.find("c");
  if(it){
    std::cout<<"it existuje"<<std::endl;
  }
  return 0;
}
