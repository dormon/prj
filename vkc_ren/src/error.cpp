#include<error.hpp>
#include<stdio.h>
#include<vulkan/vk_enum_string_helper.h>

Error::Error(){}
Error::Error(char const*file,int line,char const*fce,VkResult r):
  file(file),
  line(line),
  fceName(fce),
  result(r){}
void Error::print()const{
  fprintf(stderr,"ERROR:\n");
  if(file   )fprintf(stderr,"  FILE    : %s\n",file);
  if(line>=0)fprintf(stderr,"  LINE    : %i\n",line);
  if(fceName)fprintf(stderr,"  FCE     : %s\n",fceName);
  fprintf(stderr,"  VkResult: %s\n",string_VkResult(result));
  fprintf(stderr,"\n");
}
