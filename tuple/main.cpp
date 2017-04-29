#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<algorithm>
#include<functional>
#include<utility>
#include<glm/glm.hpp>
#include<geCore/Text.h>
#include<geGL/geGL.h>

int main(int,char*[]){
  using Properties = std::tuple<int,float,std::string>;
  Properties asd;
  enum TupleParts{SIZE,LENGTH,NAME};
  std::get<SIZE>(asd) = 12;
  std::cout<<std::get<0>(asd);
  return 0;
}
