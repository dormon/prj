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
#include<array>

int main(){

  uint8_t*data = new uint8_t[10];

  struct MyComp{
    bool operator()(uint8_t*const&a,uint8_t*const&b)const{
      for(size_t i=0;i<2;++i){
        if(a[i]<b[i])return true;
        if(a[i]>b[i])return false;
      }
      return false;
    }
  };

  std::map<uint8_t*,std::string,MyComp>asd;

  delete[]data;
  return 0;
}
