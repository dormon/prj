#include<vulkan/vulkan.h>
#include<stdio.h>

#define ___  fprintf(stderr,"%s:%d\n",__FILE__,__LINE__)

int main(int argc,char*argv[]){
  ___;
  auto applicationInfo = VkApplicationInfo{
    .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext              = nullptr                           ,
    .pApplicationName   = ""                                ,
    .applicationVersion = 0                                 ,
    .pEngineName        = ""                                ,
    .engineVersion      = 0                                 ,
    .apiVersion         = VK_MAKE_VERSION(1,0,0)            ,
  };

  auto instanceCreateInfo = VkInstanceCreateInfo{
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .pApplicationInfo        = nullptr,
    .enabledLayerCount       = 0,
    .ppEnabledLayerNames     = nullptr,
    .enabledExtensionCount   = 0,
    .ppEnabledExtensionNames = nullptr,
  };
  VkInstance instance;
  ___;
  vkCreateInstance(&instanceCreateInfo,nullptr,&instance);
  ___;
  vkDestroyInstance(instance,nullptr);
  ___;
  return 0;
}

