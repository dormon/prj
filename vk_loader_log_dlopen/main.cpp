#include <dlfcn.h>
#define VK_NO_PROTOTYPES
#include<vulkan/vulkan.h>

#include<stdio.h>

#define ___  fprintf(stderr,"%s:%d\n",__FILE__,__LINE__)
#define LOAD_CHECK(f)if(!f)fprintf(stderr,"cannot load %s\n",#f)

int main(int argc,char*argv[]){
  ___;
  void*vulkanLib = dlopen("libvulkan.so",RTLD_LAZY);
  if(!vulkanLib)fprintf(stderr,"cannot open libvulkan.so\n");
  ___;

  PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
  PFN_vkCreateInstance      vkCreateInstance      = nullptr;
  PFN_vkDestroyInstance     vkDestroyInstance     = nullptr;
  ___;


  vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(vulkanLib,"vkGetInstanceProcAddr");
  LOAD_CHECK(vkGetInstanceProcAddr);
  ___;

  vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(nullptr,"vkCreateInstance");
  LOAD_CHECK(vkCreateInstance);
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
    .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext                   = nullptr                               ,
    .flags                   = 0                                     ,
    .pApplicationInfo        = &applicationInfo                      ,
    .enabledLayerCount       = 0                                     ,
    .ppEnabledLayerNames     = nullptr                               ,
    .enabledExtensionCount   = 0                                     ,
    .ppEnabledExtensionNames = nullptr                               ,
  };
  VkInstance instance;
  ___;
  vkCreateInstance(&instanceCreateInfo,nullptr,&instance);
  ___;

  vkDestroyInstance = (PFN_vkDestroyInstance)vkGetInstanceProcAddr(instance,"vkDestroyInstance");
  LOAD_CHECK(vkDestroyInstance);
  ___;

  vkDestroyInstance(instance,nullptr);
  ___;


  dlclose(vulkanLib);
  ___;

  return 0;
}

