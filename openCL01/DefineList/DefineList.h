#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>

#include<TextUtils/Utils.h>

template<typename TYPE>
std::map<TYPE,std::string>getDefineList(
    std::vector<TYPE>const&values,
    std::string      const&names ){
  std::map<TYPE,std::string>result;
  std::vector<std::string>splitNames = splitString(names,",");
  std::for_each(splitNames.begin(),splitNames.end(),[](std::string&x){x = removeSpaces(x);});
  for(size_t i = 0; i < splitNames.size(); ++i)
    result[values.at(i)] = splitNames.at(i);
  return result;
}

template<typename TYPE>
std::map<std::string,TYPE>getDefineListByName(
    std::vector<TYPE>const&values,
    std::string      const&names ){
  std::map<std::string,TYPE>result;
  std::vector<std::string>splitNames = splitString(names,",");
  std::for_each(splitNames.begin(),splitNames.end(),[](std::string&x){x = removeSpaces(x);});
  for(size_t i = 0; i < splitNames.size(); ++i)
    result[splitNames.at(i)] = values.at(i);
  return result;
}

#define DEFINE_LIST(type,name,...)\
  std::map   <type,std::string>static const name         = getDefineList      <type>({__VA_ARGS__},#__VA_ARGS__);\
  std::vector<type            >static const name##List   =                           {__VA_ARGS__}              ;\
  std::map   <std::string,type>static const name##ByName = getDefineListByName<type>({__VA_ARGS__},#__VA_ARGS__)

