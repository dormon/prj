#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<algorithm>
#include<cstring>

uint32_t myRand(uint32_t x){
  uint32_t r=1;
  r*=x;
  r+=1;
  r*=x;
  r+=1;
  r*=x;
  r+=1;
  r*=x;
  r+=1;
  r*=x;
  r+=1;
  return r;
}

int main(){
  size_t N = 0x100000000;
  std::vector<uint32_t>data(N);
  std::memset(data.data(),0,sizeof(uint32_t)*data.size());
  for(size_t i=0;i<N;++i){
    uint32_t w = myRand(i);
    if(data[w]!=0){
      std::cerr << "duplicate element: " << w << std::endl;
      break;
    }
    else
      data[w]++;
  }

  return 0;
}





