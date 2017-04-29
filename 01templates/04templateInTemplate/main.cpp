#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* Can template be template argument? YES!
 */

template<template<typename...>class CONTAINER,typename TYPE>
class ContainerOfShared: public CONTAINER<std::shared_ptr<TYPE>>{
};

int main(){
  ContainerOfShared<std::set,int>setOfShared;
  setOfShared.insert(std::make_shared<int>(23));

  ContainerOfShared<std::vector,int>vectorOfShared;
  vectorOfShared.push_back(std::make_shared<int>(32));

  return 0;
}
