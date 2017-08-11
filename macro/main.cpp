#include<iostream>
#include<sstream>
#include<functional>
#include<map>
#include<vector>
#include<typeinfo>
#include<tuple>
#include<limits>


#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>


#include"macros.h"

std::vector<std::string>splitString(
    std::string const&txt     ,
    std::string const&splitter){
  std::vector<std::string>result;
  size_t endPosition  = 0;
  size_t lastPosition = 0;
  size_t const splitterSize = splitter.size();
  while((endPosition = txt.find(splitter,lastPosition)) != std::string::npos){
    result.push_back(txt.substr(lastPosition,endPosition - lastPosition));
    lastPosition = endPosition + splitterSize;
  }
  result.push_back(txt.substr(lastPosition,txt.size() - lastPosition));
  return result;
}

std::map<size_t,std::string>getDefineList(
    std::vector<size_t>const&values,
    std::string        const&names ){
  std::map<size_t,std::string>result;
  std::vector<std::string>splitNames = splitString(names,",");
  for(size_t i = 0; i < splitNames.size(); ++i)
    result[values.at(i)] = splitNames.at(i);
  return result;
}

#define DEFINE_LIST(name,...)\
  std::map<size_t,std::string>static const name = getDefineList({__VA_ARGS__},#__VA_ARGS__)

#define CL_STATUS 123
#define CL_ERROR  32
#define CL_POWER  1233

DEFINE_LIST(clParams,CL_STATUS,CL_ERROR,CL_POWER);

int main(){
  std::cout << clParams.at(CL_STATUS) << std::endl;
  for(auto const&x:splitString("DormonasdDomronasdDormon","asd"))
    std::cout << x << std::endl;
  return 0;
}
