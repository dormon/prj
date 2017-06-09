#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
//#include<geGL/geGL.h>
#include<GL/glew.h>
#include<cstdlib>
#include<cstring>

template<typename CONTAINER,typename...ARGS>struct is_map:std::false_type{};
template<typename...ARGS>struct is_map<std::map<ARGS...>>:std::true_type{};

template<typename CONTAINER,typename std::enable_if<is_map<CONTAINER>::value,unsigned>::type = 0>
void funkce(CONTAINER const&){
  std::cout << "jsem mapa" << std::endl;
}

template<typename CONTAINER,typename std::enable_if<!is_map<CONTAINER>::value,unsigned>::type = 0>
void funkce(CONTAINER const&){
  std::cout << "nejsem mapa" << std::endl;
}

int main(){
  if(is_map<std::vector<int>>::value)
    std::cout << "vector je mapa" << std::endl;
  if(is_map<std::map<std::string,float>>::value)
    std::cout << "mapa je mapa" << std::endl;

  funkce(std::vector<float>());
  funkce(std::map<std::string,float>());
  return 0;
}





