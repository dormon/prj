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

int main(){
  std::function<float(float)>const fce = [&](float a){
    if(a<0)return 1.f;
    return fce(a-10)*2;
  };

  std::cout << fce(100) << std::endl;

  return 0;
};
