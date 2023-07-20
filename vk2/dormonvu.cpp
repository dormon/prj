#include"dormonvu.hpp"
#include<iostream>
#include <dlfcn.h>

void throwError(std::string const&file,uint32_t line,std::string const&fce,std::string const&msg){
  std::stringstream ss;
  ss << file << "/" << line << "/" << fce << " " << msg;
  throw std::runtime_error(ss.str());
}

void throwError(std::string const&file,uint32_t line,std::string const&fce,VkResult err){
  throwError(file,line,fce,vkResultString(err));
}



char const*vkResultString(VkResult const&err){
#define IF_ERR_RETURN(x)if(err==x)return #x
  IF_ERR_RETURN(VK_SUCCESS);
  IF_ERR_RETURN(VK_NOT_READY);
  IF_ERR_RETURN(VK_TIMEOUT);
  IF_ERR_RETURN(VK_EVENT_SET);
  IF_ERR_RETURN(VK_EVENT_RESET);
  IF_ERR_RETURN(VK_INCOMPLETE);
  IF_ERR_RETURN(VK_ERROR_OUT_OF_HOST_MEMORY);
  IF_ERR_RETURN(VK_ERROR_OUT_OF_DEVICE_MEMORY);
  IF_ERR_RETURN(VK_ERROR_INITIALIZATION_FAILED);
  IF_ERR_RETURN(VK_ERROR_DEVICE_LOST);
  IF_ERR_RETURN(VK_ERROR_MEMORY_MAP_FAILED);
  IF_ERR_RETURN(VK_ERROR_LAYER_NOT_PRESENT);
  IF_ERR_RETURN(VK_ERROR_EXTENSION_NOT_PRESENT);
  IF_ERR_RETURN(VK_ERROR_FEATURE_NOT_PRESENT);
  IF_ERR_RETURN(VK_ERROR_INCOMPATIBLE_DRIVER);
  IF_ERR_RETURN(VK_ERROR_TOO_MANY_OBJECTS);
  IF_ERR_RETURN(VK_ERROR_FORMAT_NOT_SUPPORTED);
  IF_ERR_RETURN(VK_ERROR_FRAGMENTED_POOL);
  IF_ERR_RETURN(VK_ERROR_UNKNOWN);
  IF_ERR_RETURN(VK_ERROR_OUT_OF_POOL_MEMORY);
  IF_ERR_RETURN(VK_ERROR_INVALID_EXTERNAL_HANDLE);
  IF_ERR_RETURN(VK_ERROR_FRAGMENTATION);
  IF_ERR_RETURN(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
  IF_ERR_RETURN(VK_ERROR_SURFACE_LOST_KHR);
  IF_ERR_RETURN(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
  IF_ERR_RETURN(VK_SUBOPTIMAL_KHR);
  IF_ERR_RETURN(VK_ERROR_OUT_OF_DATE_KHR);
  IF_ERR_RETURN(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
  IF_ERR_RETURN(VK_ERROR_VALIDATION_FAILED_EXT);
  IF_ERR_RETURN(VK_ERROR_INVALID_SHADER_NV);
  //IF_ERR_RETURN(VK_ERROR_INCOMPATIBLE_VERSION_KHR);
  IF_ERR_RETURN(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
  IF_ERR_RETURN(VK_ERROR_NOT_PERMITTED_EXT);
  IF_ERR_RETURN(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
  IF_ERR_RETURN(VK_THREAD_IDLE_KHR);
  IF_ERR_RETURN(VK_THREAD_DONE_KHR);
  IF_ERR_RETURN(VK_OPERATION_DEFERRED_KHR);
  IF_ERR_RETURN(VK_OPERATION_NOT_DEFERRED_KHR);
  IF_ERR_RETURN(VK_PIPELINE_COMPILE_REQUIRED_EXT);
  IF_ERR_RETURN(VK_ERROR_OUT_OF_POOL_MEMORY_KHR);
  IF_ERR_RETURN(VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR);
  IF_ERR_RETURN(VK_ERROR_FRAGMENTATION_EXT);
  IF_ERR_RETURN(VK_ERROR_INVALID_DEVICE_ADDRESS_EXT);
  IF_ERR_RETURN(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR);
  IF_ERR_RETURN(VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT);
  return "VK_RESULT_MAX_ENUM";
#undef IF_ERR_RETURN
}

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
  getPhysicalDeviceGroupProperties();
  getPhysicalDevices();
  getPhysicalDeviceProperties();
  getQueueFamilyProperties();
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

void Vulkan::getPhysicalDeviceGroupProperties(){
  uint32_t count;
  VK_CALL(vkEnumeratePhysicalDeviceGroups,instance,&count,nullptr);
  physicalDeviceGroups.resize(count);
  VK_CALL(vkEnumeratePhysicalDeviceGroups,instance,&count,physicalDeviceGroups.data());
}

void Vulkan::printPhysicalDeviceGroups(){
  int i=0;
  for(auto const&x:physicalDeviceGroups){
    std::cerr << "group:"<<i++<<std::endl;
    std::cerr << "  physicalDeviceCount : " << x.physicalDeviceCount << std::endl;

  }
}

void Vulkan::getPhysicalDevices(){
  uint32_t physicalDeviceCount;
  VK_CALL(vkEnumeratePhysicalDevices,instance,&physicalDeviceCount,nullptr);
  std::vector<VkPhysicalDevice>devs;
  devs.resize(physicalDeviceCount);
  VK_CALL(vkEnumeratePhysicalDevices,instance,&physicalDeviceCount,devs.data());

  physicalDevices.resize(physicalDeviceCount);
  for(uint32_t i=0;i<physicalDeviceCount;++i)
    physicalDevices.at(i).physicalDevice = devs.at(i);
}

void Vulkan::getPhysicalDeviceProperties(){
  for(auto&dev:physicalDevices){
    vkGetPhysicalDeviceProperties(dev.physicalDevice,&dev.properties);
  }
}

std::string versionString(uint32_t version){
  std::stringstream ss;
  ss << VK_VERSION_MAJOR(version) << "." << VK_VERSION_MINOR(version) << "." << VK_VERSION_PATCH(version);
  return ss.str();
}

template<typename T>
void printValue(std::string const&name,T const&v){
}

void Vulkan::printPhysicalDeviceProperties(){
  for(auto const&dev:physicalDevices){
    auto const&p = dev.properties;
    std::cerr << p.deviceName << std::endl;
    std::cerr << "  apiVersion   : " << versionString(p.apiVersion) << std::endl;
    std::cerr << "  deviceID     : " << p.deviceID << std::endl;
    std::cerr << "  vendorID     : " << p.vendorID << std::endl;
    std::cerr << "  driverVersion: " << p.driverVersion << std::endl;
  }
}

void Vulkan::getQueueFamilyProperties(){
  for(auto&dev:physicalDevices){
    auto const&pd = dev.physicalDevice;
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(pd,&count,nullptr);
    dev.queueFamilyProperties.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(pd,&count,dev.queueFamilyProperties.data());
  }
}

char const*queueFlagToStr(VkQueueFlags f){
  switch(f){
    case VK_QUEUE_GRAPHICS_BIT      :return"VK_QUEUE_GRAPHICS_BIT"      ;
    case VK_QUEUE_COMPUTE_BIT       :return"VK_QUEUE_COMPUTE_BIT"       ;
    case VK_QUEUE_TRANSFER_BIT      :return"VK_QUEUE_TRANSFER_BIT"      ;
    case VK_QUEUE_PROTECTED_BIT     :return"VK_QUEUE_PROTECTED_BIT"     ;
    case VK_QUEUE_SPARSE_BINDING_BIT:return"VK_QUEUE_SPARSE_BINDING_BIT";
  }
  return "";
}

std::string queueFlagsToStr(VkQueueFlags f){
  std::stringstream ss;
  std::vector<VkQueueFlags>const flags = {
    VK_QUEUE_GRAPHICS_BIT      ,
    VK_QUEUE_COMPUTE_BIT       ,
    VK_QUEUE_TRANSFER_BIT      ,
    VK_QUEUE_PROTECTED_BIT     ,
    VK_QUEUE_SPARSE_BINDING_BIT,
  };

  bool first = true;
  for(auto const&x:flags){
    if(x&f){
      if(first)first = false;
      else ss << " | ";
      ss << queueFlagToStr(x);
    }
  }
  return ss.str();
}

void Vulkan::printQueueProperties(){
  for(auto const&dev:physicalDevices){
    std::cerr << dev.properties.deviceName << std::endl;
    int i=0;
    for(auto const&q:dev.queueFamilyProperties){
      std::cerr << "  queueFamily:"<<(i++)<<std::endl;
      auto&itg = q.minImageTransferGranularity;
      std::cerr << "    minImageTransferGranularity.width  : " << itg.width    << std::endl;
      std::cerr << "    minImageTransferGranularity.height : " << itg.height   << std::endl;
      std::cerr << "    minImageTransferGranularity.depth  : " << itg.depth    << std::endl;
      std::cerr << "    queueCount                         : " << q.queueCount << std::endl;
      std::cerr << "    queueFlags                         : " << queueFlagsToStr(q.queueFlags) << std::endl;
      std::cerr << "    timestampValidBits                 : " << q.timestampValidBits << std::endl;
    }
  }
}
