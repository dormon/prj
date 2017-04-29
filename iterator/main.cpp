#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<utility>
#include<cassert>
#include<limits>
#include<algorithm>

template<typename RandomIt>
void print(RandomIt first,RandomIt last){
  while(first!=last){
    std::cout<<*first<<std::endl;
    first++;
  }
}

int main(){
  std::vector<int32_t>asd = {1,2,3,4};
  print(asd.begin(),asd.end());
  return 0;
}
