#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<geCore/dtemplates.h>

int main(){
  using type = int8_t;
  type a=0,b=3,c=4;
  a=b*=c;
  return a;
}
