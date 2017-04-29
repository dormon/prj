#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<glm/glm.hpp>
#include<algorithm>

using FCEPTR = void(*)();
using VOIDPTR = void*;

int main(){
  VOIDPTR a = (VOIDPTR*)&main;
  FCEPTR  b = a;
  return 0;
}
