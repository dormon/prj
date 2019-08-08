#pragma once
#include <vulkan/vulkan.h>
#include "vkStr.h"

void throwIfError(std::string const&name,std::string const&file,size_t line,VkResult err){
  if(err == VK_SUCCESS)return;
  std::stringstream ss;
  ss << file << " " << line << " " << name << " -  " << vkResult2String(err);
  throw std::runtime_error(ss.str());
}

template<typename...ARGS,typename...VARGS>
void call(std::string const&name,std::string const& file,size_t line,VkResult(*f)(ARGS...),VARGS...args){
  auto err = f(args...);
  throwIfError(name,file,line,err);
}

#define CALL(f,...)\
  call(#f,__FILE__,__LINE__,f,__VA_ARGS__)

