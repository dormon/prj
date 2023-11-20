#include"dormonvu.hpp"
#include<iostream>
#include <dlfcn.h>

char const*vkResultString(VkResult const&err);
void throwError(std::string const&file,uint32_t line,std::string const&fce,std::string const&msg){
  std::stringstream ss;
  ss << file << "/" << line << "/" << fce << " " << msg;
  throw std::runtime_error(ss.str());
}

void throwError(std::string const&file,uint32_t line,std::string const&fce,VkResult err){
  throwError(file,line,fce,vkResultString(err));
}
void throwError(std::string const&file,uint32_t line,std::string const&fce,std::string const&msg);
void throwError(std::string const&file,uint32_t line,std::string const&fce,VkResult err);

template<typename FCE>struct FceReturnType;                                
template<typename OUTPUT,typename... ARGS>                                 
struct FceReturnType<OUTPUT(ARGS...)>{                                     
  using type = OUTPUT;                                                     
};

template<typename FCE,typename... ARGS>
void callAndThrowIfError(
    std::string const&file,
    uint32_t line,
    std::string const&fceName,
    FCE*fce,
    ARGS... args){
  if(!fce)throwError(file,line,fceName,"is nullptr");
  if constexpr (std::is_same<typename FceReturnType<FCE>::type,void>::value){
    fce(args...);
  }else{
    auto err = fce(args...);
    if(err != VK_SUCCESS)
      throwError(file,line,fceName,err);
  }
}

#define VK_CALL(fce,...) \
  callAndThrowIfError(__FILE__,__LINE__,#fce,fce,__VA_ARGS__)



#define IF_ERR_RETURN(x,...)if(err==x)return #x;
char const*vkResultString(VkResult const&err){
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
}

#define LIST_QUEUE_FLAGS(f,...)\
  f(VK_QUEUE_GRAPHICS_BIT      ,__VA_ARGS__)\
  f(VK_QUEUE_COMPUTE_BIT       ,__VA_ARGS__)\
  f(VK_QUEUE_TRANSFER_BIT      ,__VA_ARGS__)\
  f(VK_QUEUE_PROTECTED_BIT     ,__VA_ARGS__)\
  f(VK_QUEUE_SPARSE_BINDING_BIT,__VA_ARGS__)

#define LIST_COMMA(x,...) x,
#define IF_ERR_RETURN(x,...)if(err==x)return #x;


char const*queueFlagToStr(VkQueueFlags err){

  std::vector<int>asd{
    LIST_QUEUE_FLAGS(LIST_COMMA)
  };

  LIST_QUEUE_FLAGS(IF_ERR_RETURN);


  return "";
}
#undef IF_ERR_RETURN



#define LOAD(f)                        f = (decltype(f))dlsym(vulkanLib,#f);if(!f)std::cerr << #f << " == " << f << std::endl
#define LOAD_INSTANCE_FUNCTION(i,f)    f = (decltype(f))vkGetInstanceProcAddr(i,#f);if(!f)std::cerr << #f << " == " << f << std::endl
#define LOAD_NO_INSTANCE_FUNCTION(i,f) f = LOAD_INSTANCE_FUNCTION(nullptr,f)

void*openVulkanLib(){
  void*vulkanLib = dlopen("libvulkan.so",RTLD_LAZY);
  if(!vulkanLib)std::cerr << "cannot open libvulkan.so" << std::endl;
  return vulkanLib;
}

void Vulkan::load_physicalDeviceFunctions(){
#define LOAD_PHYSICAL_DEVICE_FUNCTION(i,f)\
  physicalDeviceFunctions.f = (decltype(physicalDeviceFunctions.f))vkGetInstanceProcAddr(i,#f);\
  if(!physicalDeviceFunctions.f)std::cerr << #f << " == " << physicalDeviceFunctions.f << std::endl
  PHYSICAL_DEVICE_FUNCTION_LIST(instance,LOAD_PHYSICAL_DEVICE_FUNCTION);
#undef LOAD_PHYSICAL_DEVICE_FUNCTION
}

void Vulkan::start(){
  vulkanLib = openVulkanLib();
  load_vkGetInstanceProcAddr(vulkanLib);
  NO_INSTANCE_FUNCTION_LIST(_,LOAD_NO_INSTANCE_FUNCTION)
  createInstance();
  INSTANCE_FUNCTION_LIST(instance,LOAD_INSTANCE_FUNCTION);
  load_physicalDeviceFunctions();
  getPhysicalDeviceGroupProperties();
  getPhysicalDevices();
  getPhysicalDeviceProperties();
  getQueueFamilyProperties();
  createLogicalDevices();
}

void Vulkan::stop(){
  for(auto&pdev:physicalDevices){
    for(auto&ldev:pdev.logicalDevices){
      vkDestroyDevice(ldev.device,nullptr);
    }
  }
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
    for(uint32_t i=0;i<x.physicalDeviceCount;++i)
      std::cerr << "  physicalDevices[" << i << "]" << " : " << x.physicalDevices[0] << std::endl;

  }
}

uint32_t Vulkan::getNofPhysicalDevices(){
  uint32_t count;
  VK_CALL(vkEnumeratePhysicalDevices,instance,&count,nullptr);
  return count;
}

void Vulkan::getPhysicalDevices(){
  auto count = getNofPhysicalDevices();
  auto devs = std::vector<VkPhysicalDevice>(count);
  VK_CALL(vkEnumeratePhysicalDevices,instance,&count,devs.data());

  for(auto const&dev:devs)
    physicalDevices.emplace_back(dev,physicalDeviceFunctions);
}

Vulkan::PhysicalDevice::PhysicalDevice(
    VkPhysicalDevice        const&pd ,
    PhysicalDeviceFunctions const&pdf):physicalDevice(pd),ctx(pdf){
  getProperties();
}

void Vulkan::PhysicalDevice::getProperties(){
  ctx.vkGetPhysicalDeviceProperties(physicalDevice,&properties);
}

void Vulkan::getPhysicalDeviceProperties(){
  for(auto&dev:physicalDevices){
    physicalDeviceFunctions.vkGetPhysicalDeviceProperties(dev.physicalDevice,&dev.properties);
  }
}

std::string versionString(uint32_t version){
  std::stringstream ss;
  ss << VK_VERSION_MAJOR(version) << "." << VK_VERSION_MINOR(version) << "." << VK_VERSION_PATCH(version);
  return ss.str();
}

template<typename T>
void printValue(int indent,int padding,std::string const&name,T const&v){
  for(int i=0;i<indent;++i)std::cerr << " ";
  std::cerr << name;
  auto ep = padding-name.length();
  for(int i=0;i<ep;++i)std::cerr << " ";
  std::cerr << ": " << v << std::endl;
}

void Vulkan::printPhysicalDeviceProperties(){
  for(auto const&dev:physicalDevices){
    auto const&p = dev.properties;
    std::cerr << p.deviceName << std::endl;
    printValue(2,13,"apiVersion"   ,versionString(p.apiVersion   ));
    printValue(2,13,"deviceID"     ,              p.deviceID      );
    printValue(2,13,"vendorID"     ,              p.vendorID      );
    printValue(2,13,"driverVersion",              p.driverVersion );
  }
}

//uint32_t getPhysicalDeviceQueueFamilyPropertiesCount(VkPhysicalDevice const&pd){
//  uint32_t count;
//  vkGetPhysicalDeviceQueueFamilyProperties(pd,&count,nullptr);
//  return count;
//}

void Vulkan::getQueueFamilyProperties(){
  for(auto&dev:physicalDevices){
    auto const&pd = dev.physicalDevice;
    uint32_t count;
    physicalDeviceFunctions.vkGetPhysicalDeviceQueueFamilyProperties(pd,&count,nullptr);
    std::vector<VkQueueFamilyProperties>qfp;
    qfp.resize(count);
    dev.queueFamilies.resize(count);
    physicalDeviceFunctions.vkGetPhysicalDeviceQueueFamilyProperties(pd,&count,qfp.data());
    for(size_t i=0;i<qfp.size();++i)
      dev.queueFamilies.at(i).properties = qfp.at(i);
  }
}


std::string queueFlagsToStr(VkQueueFlags f){
  std::stringstream ss;
  std::vector<VkQueueFlags>const flags = {
    LIST_QUEUE_FLAGS(LIST_COMMA)
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
    for(auto const&qf:dev.queueFamilies){
      auto const&q = qf.properties;

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

void Vulkan::throwIfPhysicalDeviceOutOfRange(uint32_t dev){
  if(dev < physicalDevices.size())return;
  throw std::runtime_error("Cannot find queue family, physical device out of range");
}

bool Vulkan::areQueueFlagsIncompatible(QueueFamility const&family,VkQueueFlags required){
  return (family.properties.queueFlags & required) != required;
}

bool Vulkan::notEnoughFamilyCapacity(QueueFamility const&family,uint32_t count){
  return count + family.used > family.properties.queueCount;
}

bool Vulkan::isQueueFamilyIncopatible(QueueFamility family,VkQueueFlags required,uint32_t count){
  return 
    areQueueFlagsIncompatible(family,required) ||
    notEnoughFamilyCapacity(family,count);
}

uint32_t Vulkan::findQueueFamily(uint32_t physicalDevice,VkQueueFlags requiredFlags,uint32_t count){
  throwIfPhysicalDeviceOutOfRange(physicalDevice);

  auto const&qf = physicalDevices[physicalDevice].queueFamilies;
  for(size_t i=0;i<qf.size();++i){
    if(isQueueFamilyIncopatible(qf.at(i),requiredFlags,count))continue;
    return i;
  }

  throw std::runtime_error("Cannot find queue family");
  return 0;
}

VkDeviceQueueCreateInfo deviceQueueCreateInfo(uint32_t index,uint32_t count,float const*priorities){
  return VkDeviceQueueCreateInfo{
    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .pNext            = nullptr                                   ,
    .flags            = 0                                         ,
    .queueFamilyIndex = index                                     ,
    .queueCount       = count                                     ,
    .pQueuePriorities = priorities                                ,
  };
}

VkDeviceCreateInfo deviceCreateInfo(uint32_t count,VkDeviceQueueCreateInfo const*infos){
  return VkDeviceCreateInfo{
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext                   = nullptr                             ,
    .flags                   = 0                                   ,
    .queueCreateInfoCount    = count                               ,
    .pQueueCreateInfos       = infos                               ,
    .enabledLayerCount       = 0                                   ,
    .ppEnabledLayerNames     = nullptr                             ,
    .enabledExtensionCount   = 0                                   ,
    .ppEnabledExtensionNames = nullptr                             ,
    .pEnabledFeatures        = nullptr                             ,
  };
}


VkDeviceCreateInfo Vulkan::allocateDeviceCreateInfo(CreateDeviceInfo const&cdi){
  uint32_t n = cdi.queueCreateInfos.size();
  return VkDeviceCreateInfo{
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext                   = nullptr                             ,
    .flags                   = 0                                   ,
    .queueCreateInfoCount    = n                                   ,
    .pQueueCreateInfos       = new VkDeviceQueueCreateInfo[n]      ,
    .enabledLayerCount       = 0                                   ,
    .ppEnabledLayerNames     = nullptr                             ,
    .enabledExtensionCount   = 0                                   ,
    .ppEnabledExtensionNames = nullptr                             ,
    .pEnabledFeatures        = nullptr                             ,
  };
}

void Vulkan::fillDeviceCreateInfo(VkDeviceCreateInfo&dci,CreateDeviceInfo const&cdi){
  auto d = (VkDeviceQueueCreateInfo*)dci.pQueueCreateInfos;
  for(auto&qci:cdi.queueCreateInfos){
    uint32_t index = findQueueFamily(cdi.physicalDevice,qci.flags,qci.count);
    d->sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    d->pNext            = nullptr                                   ;
    d->flags            = 0                                         ;
    d->queueFamilyIndex = index                                     ;
    d->queueCount       = qci.count                                 ;
    d->pQueuePriorities = qci.priorities.data()                     ;
    d++;
  }
}

VkDeviceCreateInfo Vulkan::createDeviceCreateInfo(CreateDeviceInfo const&cdi){
  auto dci = allocateDeviceCreateInfo(cdi);
  fillDeviceCreateInfo(dci,cdi);
  return dci;
}

Vulkan::PhysicalDevice&Vulkan::getPhysicalDevice(CreateDeviceInfo const&cdi){
  return physicalDevices.at(cdi.physicalDevice);
}

void Vulkan::destroyDeviceCreateInfo(VkDeviceCreateInfo&info){
  delete[]info.pQueueCreateInfos;
}

void Vulkan::createLogicalDevice(PhysicalDevice&pd,VkDeviceCreateInfo&ci){
  pd.logicalDevices.push_back({});
  auto&pdev = pd.physicalDevice;
  auto&ldev = pd.logicalDevices.back();
  VK_CALL(vkCreateDevice,pdev,&ci,nullptr,&ldev.device);
  //ldev.queueFamily.resize(ci.queueCreateInfoCount);
  //for(uint32_t i=0;i<ci.queueCreateInfoCount;++i){
  //  auto&qf = ldev.queueFamily[i];
  //  qf.familyIndex=ci.pQueueCreateInfos[i].queueFamilyIndex;
  //  qf.queues.resize(ci.pQueueCreateInfos[i].queueCount);
  //  ldev.vkGetDeviceQueue(ldev.device,qf.familyIndex,0,qf.queues.data()+0);
  //}
}

void Vulkan::loadDeviceFunctions(PhysicalDevice&pdev){
  auto&ld = pdev.logicalDevices.back();
  
#define LOAD_DEVICE_FUNCTION(i,f)\
  ld.f = (decltype(ld.f))vkGetDeviceProcAddr(i,#f);\
  if(!ld.f)std::cerr << #f << " == " << ld.f << std::endl

  DEVICE_FUNCTION_LIST(ld.device,LOAD_DEVICE_FUNCTION);
}

void Vulkan::createLogicalDevice(
    CreateDeviceInfo const&cdi){
  auto deviceCreateInfo = createDeviceCreateInfo(cdi);
  auto&pdev             = getPhysicalDevice(cdi);
  createLogicalDevice    (pdev,deviceCreateInfo);
  loadDeviceFunctions    (pdev                 );
  destroyDeviceCreateInfo(deviceCreateInfo     );
}

void Vulkan::createLogicalDevices(){
  for(auto const&cdi:createDeviceInfos)
    createLogicalDevice(cdi);
}

//void Vulkan::LogicalDevice::createCommandPools(){
//  VkCommandPoolCreateInfo commandPoolCreateInfo{
//    .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
//    .pNext            = nullptr                                   ,
//    .flags            = 0                                         ,
//    .queueFamilyIndex = queueFamilyIndex                          ,
//  };
//  VkCommandPool commandPool;
//  VK_CALL(vkCreateCommandPool,device,&commandPoolCreateInfo,nullptr,&commandPool);
//
//}
