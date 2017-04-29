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

template<typename...ARGS,size_t...I>
void printTuple(std::tuple<ARGS...>const&t){
  std::index_sequence<I...>seq = std::make_index_sequence<sizeof...(ARGS)>{};
}

int main(int,char*[]){
  printTuple(std::tuple<int,std::string,char>(42,"word",'c'));
  return 0;
}
