#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

int fce(int a){
  return a*2;
}

int main(){
  if(auto v=fce(2);v>10)
    std::cerr << v;
  return 0;
}


