#pragma once

template<typename TYPE>
const std::string* getEnumValueNames();
template<typename TYPE>
const TYPE* getEnumValues();
template<typename TYPE>
std::size_t getEnumSize();

template<typename TYPE>
std::string translate(TYPE value){
  for(std::size_t i=0;i<getEnumSize<TYPE>();++i)
    if(getEnumValues<TYPE>()[i]==value)return getEnumValueNames<TYPE>()[i];
  return "unknown";
}

#define DEFINE_ENUM_NAMES(TYPE)\
template<>const std::string* getEnumValueNames<TYPE>(){return TYPE##Names                          ;}\
template<>const TYPE*        getEnumValues    <TYPE>(){return TYPE##Values                         ;}\
template<>std::size_t        getEnumSize      <TYPE>(){return sizeof(TYPE##Values)/sizeof(VkResult);}


