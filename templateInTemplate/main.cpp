#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<geCore/dtemplates.h>

template<typename TYPE>
class Ptr{
  public:
    TYPE*data;
};


/*
template<typename TYPE>std::string name();
template<template<typename...>typename CLASS,typename... ARGS>std::string name();
template<typename TYPE>std::string className();
template<template<typename...>typename CLASS>std::string className();
template<template<typename...>typename CLASS,typename... ARGS>std::string name(){return className<CLASS>()+"<"+">";}

template<>std::string name<int>()     {return "int";}
template<>std::string className<Ptr>(){return "Ptr";}

int main(){
  std::cout<<name<int>()<<std::endl;
  std::cout<<name<Ptr,int>()<<std::endl;
  return 0;
}

// */


template<typename TYPE>std::string name();


template<typename...ARGS>std::string names();
template<typename HEAD,typename...ARGS>std::string names_helper(){return name<HEAD>()+((sizeof...(ARGS)>0)?",":"")+names<ARGS...>();}
template<typename...ARGS>std::string names(){return names_helper<ARGS...>();}
template<>std::string names(){return std::string("");}

template<template<typename...>typename CLASS>std::string className();
template<template<typename...>typename CLASS,typename... ARGS>std::string name(){return className<CLASS>()+"<"+names<ARGS...>()+">";}

template<>std::string name<int>(){return "int";}
template<>std::string name<float>(){return "float";}
template<>std::string className<Ptr>(){return "Ptr";}


int main(){
  std::cout<<name<Ptr,int>()<<std::endl;
  return 0;
}
