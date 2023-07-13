#pragma once

#include<string>
#include<vector>
#include<sstream>
#include<exception>
#define VK_NO_PROTOTYPES
#include<vulkan/vulkan.h>

constexpr char const*vkResultString(VkResult const&err){
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


inline void throwError(std::string const&file,uint32_t line,std::string const&fce,std::string const&msg){
  std::stringstream ss;
  ss << file << "/" << line << "/" << fce << " " << msg;
  throw std::runtime_error(ss.str());
}

inline void throwError(std::string const&file,uint32_t line,std::string const&fce,VkResult err){
  throwError(file,line,fce,vkResultString(err));
}

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
  std::vector<VkPhysicalDevice>physicalDevices;
  std::vector<VkPhysicalDeviceProperties>physicalDeviceProperties;
  struct Device{
    std::vector<VkQueueFamilyProperties>queueFamilyProperties;
  };
  std::vector<Device>devices;

  VkInstance instance;
  NO_INSTANCE_FUNCTION_LIST(DECLARE);
  INSTANCE_FUNCTION_LIST(_,DECLARE2);

  void start();
  void stop();
  private:
    void*vulkanLib = nullptr;
    void load_vkGetInstanceProcAddr(void*vulkanLib);
    void createInstance();
    void getPhysicalDevices();
    void getPhysicalDeviceProperties();
};
