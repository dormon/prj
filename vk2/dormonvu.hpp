#pragma once

#include<string>
#include<vector>
#include<sstream>
#include<exception>
#define VK_NO_PROTOTYPES
#include<vulkan/vulkan.h>

char const*vkResultString(VkResult const&err);
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


#define DECLARE(f)    PFN_##f f = nullptr;
#define DECLARE2(i,f) PFN_##f f = nullptr;

#define NO_INSTANCE_FUNCTION_LIST(f)\
  f(vkEnumerateInstanceLayerProperties);\
  f(vkEnumerateInstanceVersion);\
  f(vkCreateInstance);\


#define INSTANCE_FUNCTION_LIST(i,f)\
  f(i,vkDestroyInstance);\
  f(i,vkGetInstanceProcAddr);\
  f(i,vkEnumeratePhysicalDevices);\
  f(i,vkGetDeviceProcAddr);\
  f(i,vkGetPhysicalDeviceProperties);\
  f(i,vkGetPhysicalDeviceQueueFamilyProperties);\
  f(i,vkCreateDevice);\
  f(i,vkDestroyDevice);\
  f(i,vkGetPhysicalDeviceMemoryProperties);\
  f(i,vkEnumeratePhysicalDeviceGroups);\
      


#define DEVICE_FUNCTION_LIST(i,f)\
  f(i,vkGetDeviceQueue);\
  f(i,vkCreateCommandPool);\
  f(i,vkAllocateCommandBuffers);\
  f(i,vkBeginCommandBuffer);\
  f(i,vkEndCommandBuffer);\
  f(i,vkCreateBuffer);\
  f(i,vkGetBufferMemoryRequirements);\
  f(i,vkAllocateMemory);\
  f(i,vkBindBufferMemory);\
  f(i,vkMapMemory);\
  f(i,vkFlushMappedMemoryRanges);\
  f(i,vkInvalidateMappedMemoryRanges);\
  f(i,vkUnmapMemory);\
  f(i,vkCreateShaderModule);\
  f(i,vkCreatePipelineLayout);\
  f(i,vkCreateDescriptorSetLayout);\
  f(i,vkCreateComputePipelines);\
  f(i,vkFreeMemory);\
  f(i,vkDestroyBuffer);\
  f(i,vkFreeCommandBuffers);\
  f(i,vkDestroyCommandPool);\
  f(i,vkDestroyShaderModule);\
  f(i,vkDestroyPipelineLayout);\
  f(i,vkDestroyPipeline);\
  f(i,vkDestroyDescriptorSetLayout);\
  f(i,vkCmdDispatch);\
  f(i,vkCmdBindPipeline);\
  f(i,vkCmdBindDescriptorSets);\
  f(i,vkAllocateDescriptorSets);\
  f(i,vkCreateDescriptorPool);\
  f(i,vkFreeDescriptorSets);\
  f(i,vkDestroyDescriptorPool);\
  f(i,vkUpdateDescriptorSets);\
  f(i,vkQueueSubmit);\
  f(i,vkQueueWaitIdle);\

struct Vulkan{
  std::string applicationName    = "";
  uint32_t    applicationVersion = 0 ;
  std::string engineName         = "";
  uint32_t    engineVersion      = 0 ;
  uint32_t    apiVersion         = VK_MAKE_VERSION(1,0,0);
  std::vector<std::string> enabledLayers  = {};
  std::vector<std::string> enabledExtensions  = {};

  void start();
  void stop();
  void printPhysicalDeviceGroups();
  void printPhysicalDeviceProperties();
  void printQueueProperties();

  std::vector<VkPhysicalDeviceGroupProperties>physicalDeviceGroups;

  struct LogicalDevice{
    VkDevice device;

  };

  struct PhysicalDevice{
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    std::vector<VkQueueFamilyProperties>queueFamilyProperties;
    std::vector<LogicalDevice>logicalDevices;
  };
  std::vector<PhysicalDevice>physicalDevices;

  VkInstance instance;
  NO_INSTANCE_FUNCTION_LIST(DECLARE);
  INSTANCE_FUNCTION_LIST(_,DECLARE2);

  private:
    void*vulkanLib = nullptr;
    void load_vkGetInstanceProcAddr(void*vulkanLib);
    void createInstance();
    void getPhysicalDevices();
    void getPhysicalDeviceProperties();
    void getQueueFamilyProperties();
    void getPhysicalDeviceGroupProperties();
};
