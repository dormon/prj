#include"dormonvu.hpp"
#include<iostream>
#include <dlfcn.h>

#define LOAD(f)                      f = (decltype(f))dlsym(vulkanLib,#f);if(!f)std::cerr << #f << " == " << f << std::endl
#define LOAD_INSTANCE_FUNCTION(i,f)  f = (decltype(f))vkGetInstanceProcAddr(i,#f);if(!f)std::cerr << #f << " == " << f << std::endl
#define LOAD_NO_INSTANCE_FUNCTION(f) f = LOAD_INSTANCE_FUNCTION(nullptr,f)
#define LOAD_DEVICE_FUNCTION(i,f)    f = (decltype(f))vkGetDeviceProcAddr(i,#f);if(!f)std::cerr << #f << " == " << f << std::endl

void*openVulkanLib(){
  void*vulkanLib = dlopen("libvulkan.so",RTLD_LAZY);
  if(!vulkanLib)std::cerr << "cannot open libvulkan.so" << std::endl;
  return vulkanLib;
}

void Vulkan::start(){
  vulkanLib = openVulkanLib();
  load_vkGetInstanceProcAddr(vulkanLib);
  NO_INSTANCE_FUNCTION_LIST(LOAD_NO_INSTANCE_FUNCTION)
  createInstance();
  INSTANCE_FUNCTION_LIST(instance,LOAD_INSTANCE_FUNCTION);
  getPhysicalDevices();
  getPhysicalDeviceProperties();
}

void Vulkan::stop(){
  VK_CALL(vkDestroyInstance,instance,nullptr);
  dlclose(vulkanLib);
};

void Vulkan::load_vkGetInstanceProcAddr(void*vulkanLib){
  vkGetInstanceProcAddr = LOAD(vkGetInstanceProcAddr);
  if(!vkGetInstanceProcAddr)std::cerr << "cannot load vkGetInstanceProcAddr" << std::endl;
}


void Vulkan::createInstance(){
  VkApplicationInfo applicationInfo={
    .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext              = nullptr                           ,
    .pApplicationName   = applicationName.c_str()           ,
    .applicationVersion = applicationVersion                ,
    .pEngineName        = engineName.c_str()                ,
    .engineVersion      = engineVersion                     ,
    .apiVersion         = apiVersion                        ,
  };

  std::vector<char const*>eln;
  for(auto const&x:enabledLayers)
    eln.push_back(x.c_str());

  std::vector<char const*>een;
  for(auto const&x:enabledExtensions)
    een.push_back(x.c_str());

  VkInstanceCreateInfo instanceCreateInfo = {
    .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext                   = nullptr                               ,
    .flags                   = 0                                     ,
    .pApplicationInfo        = &applicationInfo                      ,
    .enabledLayerCount       = static_cast<uint32_t>(eln.size())     ,
    .ppEnabledLayerNames     = eln.data()                            ,
    .enabledExtensionCount   = static_cast<uint32_t>(een.size())     ,
    .ppEnabledExtensionNames = een.data()                            ,
  };

  VK_CALL(vkCreateInstance,&instanceCreateInfo,nullptr,&instance);
}

void Vulkan::getPhysicalDevices(){
  uint32_t physicalDeviceCount;
  VK_CALL(vkEnumeratePhysicalDevices,instance,&physicalDeviceCount,nullptr);
  physicalDevices.resize(physicalDeviceCount);
  VK_CALL(vkEnumeratePhysicalDevices,instance,&physicalDeviceCount,physicalDevices.data());
}

void Vulkan::getPhysicalDeviceProperties(){
  physicalDeviceProperties.resize(physicalDevices.size());
  for(size_t i=0;i<physicalDevices.size();++i)
    vkGetPhysicalDeviceProperties(physicalDevices.at(i),&physicalDeviceProperties.at(i));
}
