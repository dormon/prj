#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<sstream>

float fce(float a,float b){
  return a*b;
}

int main(){
  auto const fce = [](float a){
    return a*a*fce(10,200);
  };

  std::cout << fce(100) << std::endl;

  return 0;
};
