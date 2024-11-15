#pragma once

#include<vulkan/vulkan.h>

struct Error{
  char const*file     = nullptr   ;
  int        line     = -1        ;
  char const*fceName  = nullptr   ;
  VkResult   result   = VK_SUCCESS;
  Error();
  Error(char const*file,int line,char const*fce,VkResult r);
  void print()const;
};

#define VK_CALL(fce,...) \
  if(auto err = fce(__VA_ARGS__);err != VK_SUCCESS)\
    throw Error(__FILE__,__LINE__,#fce,err)

