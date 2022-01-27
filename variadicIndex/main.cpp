#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstring>
#include <functional>
#include <typeinfo>
#include <set>
#include <chrono>
#include <stack>
#include <thread>
#include <cassert>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <tuple>


float arr[256] = {0};

template<size_t...I,typename...ARGS>
void set(ARGS const&... args){
  auto v = {(arr[I]=args,0)...};
  (void)v;
}


int main(){
  set<0,2,3,5>(10,20,50,100);

  for(size_t i=0;i<8;++i)
    std::cerr << arr[i] << std::endl;
  return 0;
}
