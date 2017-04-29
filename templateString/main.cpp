#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<geCore/dtemplates.h>

template<unsigned...ARGS>
typename std::enable_if<sizeof...(ARGS)==0,void>::type printTemplateList(){
}
template<unsigned HEAD,unsigned... TAIL>
void printTemplateList(){
  std::cout<<HEAD<<std::endl;
  printTemplateList<TAIL...>();
}



template<char c>
void idOf(){
  std::cout<<c<<std::endl;
}
template<char c,typename...CHARS>
void idOf(){
  std::cout<<c<<std::endl;
  idOf<CHARS...>();
}
/*
template<unsigned L>
constexpr void len(const char (&s)[L]){}

template<unsigned...ARGS>
void pt2(ge::core::seq<ARGS....>){
  idOf
}
*/
template<unsigned L,typename...ARGS>
ge::core::gen_seq<L>sd(const char (&s)[L],ge::core::gen_seq<L>p){
  ge::core::seq<ARGS...>asd=p;
}

void asd(const char(&s)[])
*/
int main(){
  printTemplateList<'a','h','o','j'>();
  //sd("ahoj");
  return 0;
}


