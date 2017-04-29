#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<glm/glm.hpp>
#include<algorithm>

int main(){
  std::cout<<std::atof("inf")<<std::endl;
  std::cout<<std::atof("Inf")<<std::endl;
  std::cout<<std::atof("INF")<<std::endl;
  std::cout<<std::atof("-inf")<<std::endl;
  std::cout<<std::atof("a1a")<<std::endl;
  std::cout<<std::atof("0.f")<<std::endl;
  std::cout<<std::atof("0.0f")<<std::endl;
  std::cout<<std::atof(".0f")<<std::endl;
  std::cout<<std::atof(".0000000f")<<std::endl;
  std::cout<<std::atof("-.0000000f")<<std::endl;
  return 0;
}
