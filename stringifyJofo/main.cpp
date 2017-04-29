#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

#define STRINGIFY(src) #src

std::string s=STRINGIFY(float4 make_float4(float a, float b, float c, float d)
      {
          return float4(a, b, c, d);
          }));


int main(){
  return 0;
}
