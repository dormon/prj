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

using DICE = std::function<int()>;

std::map<std::string,DICE>dices = {
  {"dice0"  ,[](){return 0;}},
  {"diceR"  ,[](){return rand()%6;}},
  {"diceMem",[](){static int mem=5;mem = (mem+1)%6;return mem;}},
};


template<typename T>
float avg(std::vector<T>const&arr){
  float c = 0.f;
  for(auto const&x:arr)
    c += x;
  c /= arr.size();
  return c;
}

template<typename T>
float sum(std::vector<T>const&arr){
  float c=0.f;
  for(auto const&x:arr)
    c+=x;
  return c;
}

template<typename T>
float relative_var(std::vector<T>const&arr){
  float a = avg(arr);
  float c = 0.f;
  for(auto const&x:arr)
    c+=(a-x)*(a-x);
  c = sqrt(c) / arr.size() / sum(arr);
  return c;
}


bool isDiceFair(DICE dice){
  std::vector<size_t>counters(6);
  std::vector<size_t>conditionalCounter(36);
  size_t samples = 6000000;

  int lastValue = 6;
  for(size_t i=0;i<samples;++i){
    auto t = dice();
    counters[t]++;
    if(lastValue<6){
      conditionalCounter[t*6+lastValue]++;
    }
    lastValue = t;
  }
  for(auto const&c:counters)
    std::cerr << c << " ";
  std::cerr << "AVG         : " << avg(counters) << " ";
  std::cerr << "Rel Variance: " << relative_var(counters) << " ";
  std::cerr << "cond: ";
  for(size_t i=0;i<conditionalCounter.size();++i){
    if(i%6==0)std::cerr << "[";
    std::cerr << conditionalCounter[i] << " ";
    if(i%6==5)std::cerr << "]";
  }
  return true;
}



int main(){
  srand(time(NULL));
  for(auto const&dice:dices){
    std::cerr << dice.first << " ";
    isDiceFair(dice.second);
    std::cerr << std::endl;
  }
  return 0;
}
