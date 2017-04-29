#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<algorithm>
#include<glm/glm.hpp>
#include<unordered_map>

int main(int,char*[]){
  std::unordered_map<void const*,void*>m;
  m.insert(std::pair<void const*,void*>(nullptr,nullptr));
  m[(void const*)1] = nullptr;
  auto x =float(2);
  (void)x;
  return 0;
}
