#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

template<typename TYPE,unsigned S>
uint32_t size(const TYPE(&ar)[S]){
  return S;
}

enum Type{
  AHOJ   = 10,
  SVETE  = 12,
  TADY   = 1 ,
  JE     = 32,
  DORMON = 4 ,
};

std::string const names[]={
  "ahoj"  ,
  "svete" ,
  "tady"  ,
  "je"    ,
  "dormon",
};

Type const values[]={
  AHOJ  ,
  SVETE ,
  TADY  ,
  JE    ,
  DORMON,
};


template<typename TYPE,unsigned S>
const std::string (&getSomething())[S];



template<typename TYPE>
const TYPE*getValues();

template<typename TYPE>
const std::string*getStrings();

template<typename TYPE>
std::size_t getSize();

template<typename TYPE>
std::string translate(TYPE value){
  for(uint32_t i=0;i<getSize<TYPE>();++i)
    if(getValues<TYPE>()[i]==value)return getStrings<TYPE>()[i];
  return"unknown";
}


template<>const Type*       getValues <Type>(){return values                     ;}
template<>const std::string*getStrings<Type>(){return names                      ;}
template<>std::size_t       getSize   <Type>(){return sizeof(values)/sizeof(Type);}

int main(){
  Type t=TADY;
  std::cout<<translate(t)<<std::endl;
  //auto asd=getValues<unsigned>();
  //std::cout<<size(getValues<unsigned>())<<std::endl;
  std::cout<<size(names)<<std::endl;
  std::cout<<size(values)<<std::endl;
  return 0;
}
