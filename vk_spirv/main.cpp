#include<iostream>
#include<sstream>
#include<vector>
#include<memory>

#include <dlfcn.h>

#define VK_NO_PROTOTYPES
#include<vulkan/vulkan.h>

#include<spirv/1.0/spirv.h>

#define ___ std::cerr << __LINE__ << std::endl


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

void throwError(std::string const&file,uint32_t line,std::string const&fce,VkResult err){
  std::stringstream ss;
  ss << file << "/" << line << "/" << fce << " " << vkResultString(err);
  throw std::runtime_error(ss.str());
}

#define VK_CALL(fce,...) \
  if(auto err = fce(__VA_ARGS__);err != VK_SUCCESS)\
    throwError(__FILE__,__LINE__,#fce,err)

std::string versionString(uint32_t version){
  std::stringstream ss;
  ss << VK_VERSION_MAJOR(version) << "." << VK_VERSION_MINOR(version) << "." << VK_VERSION_PATCH(version);
  return ss.str();
}

#define DECLARE(f) PFN_##f f
#define DECLARE2(i,f) PFN_##f f
#define LOAD(f)                      f = (decltype(f))dlsym(vulkanLib,#f);if(!f)std::cerr << #f << " == " << f << std::endl
#define LOAD_INSTANCE_FUNCTION(i,f)  f = (decltype(f))vkGetInstanceProcAddr(i,#f);if(!f)std::cerr << #f << " == " << f << std::endl
#define LOAD_NO_INSTANCE_FUNCTION(f) f = LOAD_INSTANCE_FUNCTION(nullptr,f)
#define LOAD_DEVICE_FUNCTION(i,f)    f = (decltype(f))vkGetDeviceProcAddr(i,#f);if(!f)std::cerr << #f << " == " << f << std::endl

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

NO_INSTANCE_FUNCTION_LIST(DECLARE);

INSTANCE_FUNCTION_LIST(_,DECLARE2);

DEVICE_FUNCTION_LIST(_,DECLARE2);

void*openVulkanLib(){
  void*vulkanLib = dlopen("libvulkan.so",RTLD_LAZY);
  if(!vulkanLib)std::cerr << "cannot open libvulkan.so" << std::endl;
  return vulkanLib;
}

void load_vkGetInstanceProcAddr(void*vulkanLib){
  vkGetInstanceProcAddr = LOAD(vkGetInstanceProcAddr);
  if(!vkGetInstanceProcAddr)std::cerr << "cannot load vkGetInstanceProcAddr" << std::endl;
}

void loadNoInstanceFunctions(){
  NO_INSTANCE_FUNCTION_LIST(LOAD_NO_INSTANCE_FUNCTION)
}

void loadInstanceFunctions(VkInstance instance){
  INSTANCE_FUNCTION_LIST(instance,LOAD_INSTANCE_FUNCTION);
}


void loadVulkanFunctions(VkDevice instance){
  DEVICE_FUNCTION_LIST(instance,LOAD_DEVICE_FUNCTION);
}

void loadVulkan(void*vulkanLib){
}

uint32_t getAPIVersion(){
  uint32_t apiVersion;
  VK_CALL(vkEnumerateInstanceVersion,&apiVersion);
  return apiVersion;
}

void printAPIVersion(uint32_t apiVersion){
  std::cerr << "apiVersion: " << versionString(apiVersion) << std::endl;
}

std::vector<VkLayerProperties>getLayerProperties(){
  uint32_t layerPropertyCount;
  VK_CALL(vkEnumerateInstanceLayerProperties,&layerPropertyCount,nullptr);
  std::vector<VkLayerProperties>layerProperties(layerPropertyCount);
  VK_CALL(vkEnumerateInstanceLayerProperties,&layerPropertyCount,layerProperties.data());
  return layerProperties;
}

void printLayerProperties(std::vector<VkLayerProperties>const&layerProperties){
  for(auto const&layerProperty:layerProperties){
    std::cerr << layerProperty.layerName << std::endl;
    //std::cerr << "layerName:" << layerProperty.layerName << std::endl;
    //std::cerr << "implementationVersion:" << versionString(layerProperty.implementationVersion) << std::endl;
    //std::cerr << "specVersion:" << versionString(layerProperty.specVersion) << std::endl;
    //std::cerr << layerProperty.description << std::endl;
  }
}

namespace vk{
  class InstanceImpl{
    public:
  };
  class Instance{
    public:
    protected:
      friend class InstanceImpl;
      std::shared_ptr<InstanceImpl>impl;
  };
  class createInstance{
    public:

  };
}

void*myalloc(void*userData,size_t size,size_t alignment,VkSystemAllocationScope scope){
  auto res = malloc(size);
  std::cerr << "alloc: " << res << " " << size << " " << alignment << " " << scope << std::endl;
  return res;
}

void myfree(void*userData,void*mem){
  std::cerr << "dealloc: " << mem << std::endl;
  free(mem);
}

void*myrealloc(void*userData,void*orig,size_t size,size_t alignment,VkSystemAllocationScope scope){
  auto res = realloc(orig,size);
  std::cerr << "realloc: " << res << " " << orig << " " << size << " " << alignment << " " << scope << std::endl;
  return res;
}

VkAllocationCallbacks clbs = {
  nullptr,
  myalloc,
  myrealloc,
  myfree,
  nullptr,
  nullptr,
};

#define WITH(T,N) T N;{auto&d=N//}
#define ENDWITH() }[]{}()


VkInstance createInstance(){
  VkApplicationInfo applicationInfo={
    .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext              = nullptr                           ,
    .pApplicationName   = ""                                ,
    .applicationVersion = 0                                 ,
    .pEngineName        = ""                                ,
    .engineVersion      = 0                                 ,
    .apiVersion         = VK_MAKE_VERSION(1,0,0)            ,
  };


  std::vector<char const*>enabledLayerNames = {
    //"VK_LAYER_KHRONOS_validation",
    //"VK_LAYER_KHRONOS_validation",
    //"VK_LAYER_LUNARG_api_dump",
    //"VK_LAYER_VALVE_steam_fossilize_64",
    //"VK_LAYER_VALVE_steam_overlay_32",
    //"VK_LAYER_VALVE_steam_overlay_64",
    //"VK_LAYER_VALVE_steam_fossilize_32",
    //"VK_LAYER_MESA_device_select",
    //"VK_LAYER_MESA_overlay",
  };
  VkInstanceCreateInfo instanceCreateInfo = {
    .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO         ,
    .pNext                   = nullptr                                        ,
    .flags                   = 0                                              ,
    .pApplicationInfo        = &applicationInfo                               ,
    .enabledLayerCount       = static_cast<uint32_t>(enabledLayerNames.size()),
    .ppEnabledLayerNames     = enabledLayerNames.data()                       ,
    .enabledExtensionCount   = 0                                              ,
    .ppEnabledExtensionNames = nullptr                                        ,
  };


  VkInstance instance;
  VK_CALL(vkCreateInstance,&instanceCreateInfo,nullptr,&instance);
  return instance;
}

std::vector<VkPhysicalDevice>getPhysicalDevices(VkInstance instance){
  uint32_t physicalDeviceCount;
  VK_CALL(vkEnumeratePhysicalDevices,instance,&physicalDeviceCount,nullptr);
  std::vector<VkPhysicalDevice>physicalDevices(physicalDeviceCount);
  VK_CALL(vkEnumeratePhysicalDevices,instance,&physicalDeviceCount,physicalDevices.data());
  return physicalDevices;
}

std::vector<VkPhysicalDeviceProperties>getPhysicalDeviceProperties(std::vector<VkPhysicalDevice>const&physicalDevices){
  std::vector<VkPhysicalDeviceProperties>physicalDeviceProperties(physicalDevices.size());
  for(size_t i=0;i<physicalDevices.size();++i){
    vkGetPhysicalDeviceProperties(physicalDevices.at(i),&physicalDeviceProperties.at(i));
  }
  return physicalDeviceProperties;
}

void printPhysicalDeviceProperties(std::vector<VkPhysicalDeviceProperties>const&physicalDeviceProperties){
  for(auto const&p:physicalDeviceProperties){
    std::cerr << p.deviceName << std::endl;
    std::cerr << "  apiVersion   : " << versionString(p.apiVersion) << std::endl;
    std::cerr << "  deviceID     : " << p.deviceID << std::endl;
    std::cerr << "  vendorID     : " << p.vendorID << std::endl;
    std::cerr << "  driverVersion: " << p.driverVersion << std::endl;
  }
}

VkPhysicalDevice selectDevice(std::vector<VkPhysicalDevice>const&physicalDevices){
  uint32_t selectedDevice = 0;
  return physicalDevices.at(selectedDevice);
}

std::vector<VkQueueFamilyProperties>getQueueFamilyProperties(VkPhysicalDevice const&physicalDevice){
  uint32_t queueFamilyPropertyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queueFamilyPropertyCount,nullptr);
  std::vector<VkQueueFamilyProperties>queueFamilyProperties(queueFamilyPropertyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queueFamilyPropertyCount,queueFamilyProperties.data());
  return queueFamilyProperties;
}

size_t getQueueFamilyIndex(std::vector<VkQueueFamilyProperties>const&queueFamilyProperties){
  VkQueueFlags requiredQueueFamilyFlags = VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
  size_t queueFamilyIndex=0;
  for(;queueFamilyIndex<queueFamilyProperties.size();++queueFamilyIndex){
    if((queueFamilyProperties.at(queueFamilyIndex).queueFlags & requiredQueueFamilyFlags) == requiredQueueFamilyFlags)break;
  }
  return queueFamilyIndex;
}

VkDevice createDevice(VkPhysicalDevice const&physicalDevice,uint32_t queueFamilyIndex){
  std::vector<float>queuePriorities = {1.f};

  auto deviceQueueCreateInfo = VkDeviceQueueCreateInfo{
    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .pNext            = nullptr,
    .flags            = 0,
    .queueFamilyIndex = queueFamilyIndex,
    .queueCount       = 1               ,
    .pQueuePriorities = queuePriorities.data(),
  };

  VkDeviceCreateInfo deviceCreateInfo{
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext                   = nullptr                             ,
    .flags                   = 0                                   ,
    .queueCreateInfoCount    = 1                                   ,
    .pQueueCreateInfos       = &deviceQueueCreateInfo              ,
    .enabledLayerCount       = 0                                   ,
    .ppEnabledLayerNames     = nullptr                             ,
    .enabledExtensionCount   = 0                                   ,
    .ppEnabledExtensionNames = nullptr                             ,
    .pEnabledFeatures        = nullptr                             ,
  };

  VkDevice device;

  VK_CALL(vkCreateDevice,physicalDevice,&deviceCreateInfo,nullptr,&device);

  return device;
}

VkQueue getQueue(VkDevice const&device,size_t queueFamilyIndex){
  VkQueue queue;
  vkGetDeviceQueue(device,queueFamilyIndex,0,&queue);
  return queue;
}

VkCommandPool createCommandPool(VkDevice const&device,uint32_t queueFamilyIndex){
  VkCommandPoolCreateInfo commandPoolCreateInfo{
    .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext            = nullptr                                   ,
    .flags            = 0                                         ,
    .queueFamilyIndex = queueFamilyIndex                          ,
  };
  VkCommandPool commandPool;
  VK_CALL(vkCreateCommandPool,device,&commandPoolCreateInfo,nullptr,&commandPool);
  return commandPool;
}

VkCommandBuffer createCommandBuffer(VkDevice device,VkCommandPool const&commandPool){
  VkCommandBufferAllocateInfo commandBufferAllocateInfo{
    .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext              = nullptr                                       ,
    .commandPool        = commandPool                                   ,
    .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY               ,
    .commandBufferCount = 1                                             ,
  };

  VkCommandBuffer commandBuffer;
  VK_CALL(vkAllocateCommandBuffers,device,&commandBufferAllocateInfo,&commandBuffer);
  return commandBuffer;
}

VkBuffer createBuffer(VkDevice device){
  VkBuffer buffer;
  VkBufferCreateInfo bufferCreateInfo{
    .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO                                 ,
    .pNext                 = nullptr                                                              ,
    .flags                 = 0                                                                    ,
    .size                  = 1024*1024*128                                                        ,
    .usage                 = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    .sharingMode           = VK_SHARING_MODE_EXCLUSIVE                                            ,
    .queueFamilyIndexCount = 0                                                                    ,
    .pQueueFamilyIndices   = nullptr                                                              ,
  };

  VK_CALL(vkCreateBuffer,device,&bufferCreateInfo,nullptr,&buffer);
  return buffer;
}

VkMemoryRequirements getMemoryRequirements(VkDevice device,VkBuffer buffer){
  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device,buffer,&memoryRequirements);
  return memoryRequirements;
}

VkPhysicalDeviceMemoryProperties getPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice){
  VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice,&physicalDeviceMemoryProperties);
  return physicalDeviceMemoryProperties;
}

void printMemoryHeaps(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties){
  for(uint32_t i=0;i<physicalDeviceMemoryProperties.memoryHeapCount;++i){
    std::cerr << "memoryHeaps["<<i<<"]"<<std::endl;
    std::cerr << "  .flags="<<std::endl;
    if(physicalDeviceMemoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
      std::cerr << "    VK_MEMORY_HEAP_DEVICE_LOCAL_BIT" << std::endl;
    if(physicalDeviceMemoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT)
      std::cerr << "    VK_MEMORY_HEAP_MULTI_INSTANCE_BIT" << std::endl;
    std::cerr << "  .size=" << std::endl;
    std::cerr << "    " << physicalDeviceMemoryProperties.memoryHeaps[i].size << std::endl;
  }
}

void printMemoryTypes(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties){
  for(uint32_t i=0;i<physicalDeviceMemoryProperties.memoryTypeCount;++i){
    std::cerr << "memoryTypes["<<i<<"]"<<std::endl;
    std::cerr << "  .heapIndex="<<physicalDeviceMemoryProperties.memoryTypes[i].heapIndex<<std::endl;
    std::cerr << "  .propertyFlags="<<std::endl;
    if(physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags&VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
      std::cerr << "    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT" << std::endl;
    if(physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags&VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
      std::cerr << "    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT" << std::endl;
  }
}

class Instance{
  protected:
    std::shared_ptr<VkInstance>instance;
};

VkInstance createInstance(void*vulkanLib){
  load_vkGetInstanceProcAddr(vulkanLib);
  loadNoInstanceFunctions();

  auto apiVersion               = getAPIVersion();

  printAPIVersion(apiVersion);

  auto layerProperties          = getLayerProperties();

  printLayerProperties(layerProperties);
  auto instance                 = createInstance();
  loadInstanceFunctions(instance);


  return instance;
}

int main(int argc,char*argv[]){

#if 1
  auto vulkanLib                = openVulkanLib();

  auto instance = createInstance(vulkanLib);

  auto physicalDevices          = getPhysicalDevices(instance);
  auto physicalDeviceProperties = getPhysicalDeviceProperties(physicalDevices);
  printPhysicalDeviceProperties(physicalDeviceProperties);
  auto physicalDevice           = selectDevice(physicalDevices);
  auto queueFamilyProperties    = getQueueFamilyProperties(physicalDevice);
  auto queueFamilyIndex         = getQueueFamilyIndex(queueFamilyProperties);
  auto device                   = createDevice(physicalDevice,queueFamilyIndex);
  loadVulkanFunctions(device);
  auto queue                    = getQueue(device,queueFamilyIndex);
  auto commandPool              = createCommandPool(device,queueFamilyIndex);
  auto physicalDeviceMemoryProperties = getPhysicalDeviceMemoryProperties(physicalDevice);

  //printMemoryHeaps(physicalDeviceMemoryProperties);

  //printMemoryTypes(physicalDeviceMemoryProperties);



  auto buffer = createBuffer(device);

  auto memoryRequirements = getMemoryRequirements(device,buffer);


  VkMemoryPropertyFlags requiredDeviceMemoryFlags = 
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    0;
  uint32_t memoryTypeIndex = 0;
  for(;memoryTypeIndex<physicalDeviceMemoryProperties.memoryTypeCount;++memoryTypeIndex)
    if((physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & requiredDeviceMemoryFlags) == requiredDeviceMemoryFlags)break;

  if(memoryTypeIndex == physicalDeviceMemoryProperties.memoryTypeCount)
    std::cerr << "cannot find device local and host visible memory type" << std::endl;

  VkMemoryAllocateInfo memoryAllocateInfo{
    .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext           = nullptr                               ,
    .allocationSize  = memoryRequirements.size               ,
    .memoryTypeIndex = memoryTypeIndex                       ,
  };
  
  VkDeviceMemory deviceMemory;
  VK_CALL(vkAllocateMemory,device,&memoryAllocateInfo,nullptr,&deviceMemory);

  VK_CALL(vkBindBufferMemory,device,buffer,deviceMemory,0);

  void*mappedBuffer=nullptr;
  VK_CALL(vkMapMemory,device,deviceMemory,0,1024*1024*sizeof(float),0,&mappedBuffer);


  uint32_t const code[]={
    SpvMagicNumber,
    SpvVersion,
    0xBADA55u,
    35u,
    0u,

    SpvOpCapability | (2u<<16u), // 0x00020011,
    SpvCapabilityShader,

    SpvOpExtInstImport | (6u<<16u), //0x06000B,
    1u, //%1
    'G'<<0u|'L'<<8u|'S'<<16u|'L'<<24u,
    '.'<<0u|'s'<<8u|'t'<<16u|'d'<<24u,
    '.'<<0u|'4'<<8u|'5'<<16u|'0'<<24u,
    0u,

    SpvOpMemoryModel | (3u<<16u), //0x0003000E
    SpvAddressingModelLogical,
    SpvMemoryModelGLSL450,

    SpvOpEntryPoint | (6u<<16u), //0x0006000F,
    SpvExecutionModelGLCompute,
    4u, // %main
    'm'<<0u|'a'<<8u|'i'<<16u|'n'<<24u,
    0u, 
    15u, // %gl_GlobalInvocationID

    SpvOpExecutionMode | (6u<<16u),
    4u, // %main
    SpvExecutionModeLocalSize,
    64u,
    1u,
    1u,

    SpvOpSource | (3u<<16u),
    SpvSourceLanguageGLSL,
    460u,

    SpvOpName | (4u<<16u),
    4u, // %main
    'm'<<0u|'a'<<8u|'i'<<16u|'n'<<24u,
    0u,

    SpvOpName | (4u<<16u),
    8u, // %Data
    'D'<<0u|'a'<<8u|'t'<<16u|'a'<<24u,
    0u,

    SpvOpMemberName | (5u<<16u),
    8u, // %Data
    0u,
    'd'<<0u|'a'<<8u|'t'<<16u|'a'<<24u,
    0u,

    SpvOpName | (3u<<16u),
    10u, // %_
    0u,

    SpvOpName | (8u<<16u),
    15u, // %gl_GlobalInvocationID
    'g'<<0u|'l' <<8u|'_' <<16u|'G' <<24u,
    'l'<<0u|'o' <<8u|'b' <<16u|'a' <<24u,
    'l'<<0u|'I' <<8u|'n' <<16u|'v' <<24u,
    'o'<<0u|'c' <<8u|'a' <<16u|'t' <<24u,
    'i'<<0u|'o' <<8u|'n' <<16u|'I' <<24u,
    'D'<<0u|'\0'<<8u|'\0'<<16u|'\0'<<24u,

    SpvOpDecorate | (4u<<16u),
    7u, // % _runtimearr_uint
    SpvDecorationArrayStride,
    4u,

    SpvOpMemberDecorate | (5u<<16u),
    8u, // %Data
    0u,
    SpvDecorationOffset,
    0u,

    SpvOpDecorate | (3u<<16u),
    8u, // %Data
    SpvDecorationBufferBlock,

    SpvOpDecorate | (4u<<16u),
    10u, // %_
    SpvDecorationDescriptorSet,
    0u,

    SpvOpDecorate | (4u<<16u),
    10u, // %_
    SpvDecorationBinding,
    0u,

    SpvOpDecorate | (4u<<16u),
    15u, // %gl_GlobalInvocationID
    SpvDecorationBuiltIn,
    SpvBuiltInGlobalInvocationId,

    SpvOpDecorate | (4u<<16u),
    26u, // %gl_WorkGroupSize
    SpvDecorationBuiltIn,
    SpvBuiltInWorkgroupSize,

    SpvOpTypeVoid | (2u<<16u),
    2u, // %void

    SpvOpTypeFunction | (3u<<16u),
    3u, // %3
    2u, // %void

    SpvOpTypeInt | (4u<<16u),
    6u, // %uint
    32u,
    0u,

    SpvOpTypeRuntimeArray | (3u<<16u),
    7u, // %_runtimearr_uint
    6u, // %uint

    SpvOpTypeStruct | (3u<<16u),
    8u, // %Data
    7u, // %_runtimearr_uint

    SpvOpTypePointer | (4u<<16u),
    9u, // %_ptr_Uniform_Data
    SpvStorageClassUniform,
    8u,

    SpvOpVariable | (4u<<16u),
    9u,  //%_ptr_Uniform_Data
    10u, //%_
    SpvStorageClassUniform,

    SpvOpTypeInt | (4u<<16u),
    11u, //%int
    32u,
    1u,

    SpvOpConstant | (4u<<16u),
    11u, //%int
    12u, //%int_0
    0u,

    SpvOpTypeVector | (4u<<16u),
    13u, //%v3uint
    6u,  //%uint
    3u,

    SpvOpTypePointer | (4u<<16u),
    14u, //%_ptr_Input_v3uint_Input
    SpvStorageClassInput,
    13u, //%v3uint

    SpvOpVariable | (4u<<16u),
    14u, //%_ptr_Input_v3uint_Input
    15u, //%gl_GlobalInvocationID
    SpvStorageClassInput,

    SpvOpConstant | (4u<<16u),
    6u,  //%uint
    16u, //%uint_0
    0u,

    SpvOpTypePointer | (4u<<16u),
    17u, //%_ptr_Input_uint
    SpvStorageClassInput,
    6u,  //%uint

    SpvOpTypePointer | (4u<<16u),
    22u, //%_ptr_Uniform_uint
    SpvStorageClassUniform,
    6u,  //%uint

    SpvOpConstant | (4u<<16u),
    6u,  //%uint
    24u, //%uint_64
    64u,

    SpvOpConstant | (4u<<16u),
    6u,  //%uint
    25u, //%uint_1
    1u,

    SpvOpConstantComposite | (6u<<16u),
    13u,  //%gl_WorkGroupSize
    26u,  //%v3uint
    24u,  //
    25u,
    25u,

    SpvOpFunction | (5u<<16u),
    2u, //%void
    4u, //%main
    SpvFunctionControlMaskNone,
    3u, //%3

    SpvOpLabel | (2u<<16u),
    29u, //%5

    SpvOpAccessChain | (5u<<16u),
    17u, //%_ptr_Input_uint
    30u, //%30
    15u, //%gl_GlobalInvocationID
    16u, //%uint_0

    SpvOpLoad | (4u<<16u),
    6u,  //%uint
    31u, //%31
    30u, //%30

    SpvOpAccessChain | (5u<<16u),
    17u, //%_ptr_Input_uint
    32u, //%32
    15u, //%gl_GlobalInvocationID
    16u, //%uint_0

    SpvOpLoad | (4u<<16u),
    6u,  //%uint
    33u, //%33
    32u, //%32

    SpvOpAccessChain | (6u<<16u),
    22u, //%_ptr_Uniform_uint
    34u, //%34
    10u, //%_
    16u, //%uint_0
    31u, //%31u

    SpvOpStore | (3u<<16u),
    34u, //%34
    33u, //%33



    SpvOpReturn | (1u<<16u),

    SpvOpFunctionEnd | (1u<<16u),
  };

  VkShaderModule shaderModule;
  VkShaderModuleCreateInfo shaderModuleCreateInfo{
    .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext    = nullptr                                    ,
    .flags    = 0                                          ,
    .codeSize = sizeof(code)                               ,
    .pCode    = code                                       ,
  };
  VK_CALL(vkCreateShaderModule,device,&shaderModuleCreateInfo,nullptr,&shaderModule);




  //descriptor madness
  /*

  VkDescriptorSetLayoutBinding
  VkDescriptorSetLayoutCreateInfo: +VkDescriptorSetLayoutBinding
  VkDescriptorSetLayout          : VkDescriptorSetLayoutCreateInfo
  
  VkDescriptorPoolSize
  VkDescriptorPoolCreateInfo     : +VkDescriptorPoolSize
  VkDescriptorPool               : VkDescriptorPoolCreateInfo
  VkDescriptorSetAllocateInfo    : VkDescriptorPool               ,+VkDescriptorSetLayout
  VkDescriptorSet                : VkDescriptorSetAllocateInfo

  */

  VkDescriptorSetLayoutBinding descriptorSetLayoutBindings{
    .binding            = 0                                ,
    .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    .descriptorCount    = 1                                ,
    .stageFlags         = VK_SHADER_STAGE_COMPUTE_BIT      ,
    .pImmutableSamplers = nullptr                          ,
  };


  auto setLayouts = std::vector<VkDescriptorSetLayout>(1);
  VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{
    .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext        = nullptr                                            ,
    .flags        = 0                                                  ,
    .bindingCount = 1                                                  ,
    .pBindings    = &descriptorSetLayoutBindings                       ,
  };
  VK_CALL(vkCreateDescriptorSetLayout,device,&descriptorSetLayoutCreateInfo,nullptr,&setLayouts.at(0));

  auto descriptorPoolSizes = std::vector<VkDescriptorPoolSize>(1);
  {
    auto&d=descriptorPoolSizes.at(0);
    d.type            = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    d.descriptorCount =                                 1;
  }

  VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{
    .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO    ,
    .pNext         = nullptr                                          ,
    .flags         = 0                                                ,
    .maxSets       = 1                                                ,
    .poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size()),
    .pPoolSizes    = descriptorPoolSizes.data()                       ,
  };


  VkDescriptorPool descriptorPool;
  VK_CALL(vkCreateDescriptorPool,device,&descriptorPoolCreateInfo,nullptr,&descriptorPool);

  VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{
    .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext              = nullptr                                       ,
    .descriptorPool     = descriptorPool                                ,
    .descriptorSetCount = static_cast<uint32_t>(setLayouts.size())      ,
    .pSetLayouts        = setLayouts.data()                             ,
  };





  VkDescriptorSet descriptorSet;
  VK_CALL(vkAllocateDescriptorSets,device,&descriptorSetAllocateInfo,&descriptorSet);

  auto descriptorBufferInfo = VkDescriptorBufferInfo{
    .buffer = buffer       ,
    .offset = 0            ,
    .range  = VK_WHOLE_SIZE,
  };

  auto writeDescriptorSet = VkWriteDescriptorSet{
    .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .pNext            = nullptr                               ,
    .dstSet           = descriptorSet                         ,
    .dstBinding       = 0                                     ,
    .dstArrayElement  = 0                                     ,
    .descriptorCount  = 1                                     ,
    .descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER     ,
    .pImageInfo       = nullptr                               ,
    .pBufferInfo      = &descriptorBufferInfo                 ,
    .pTexelBufferView = nullptr                               ,
  };

  vkUpdateDescriptorSets(device,1,&writeDescriptorSet, 0,nullptr);

  //


  auto pipelineLayoutCreateInfo = VkPipelineLayoutCreateInfo{
    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext                  = nullptr                                      ,
    .flags                  = 0                                            ,
    .setLayoutCount         = static_cast<uint32_t>(setLayouts.size())     ,
    .pSetLayouts            = setLayouts.data()                            ,
    .pushConstantRangeCount = 0                                            ,
    .pPushConstantRanges    = nullptr                                      ,
  };
  VkPipelineLayout pipelineLayout;
  VK_CALL(vkCreatePipelineLayout,device,&pipelineLayoutCreateInfo,nullptr,&pipelineLayout);


  auto pipelineShaderStageCreateInfo = VkPipelineShaderStageCreateInfo{
    .sType                = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext                = nullptr                                            ,
    .flags                = 0                                                  ,
    .stage                = VK_SHADER_STAGE_COMPUTE_BIT                        ,
    .module               = shaderModule                                       ,
    .pName                = "main"                                             ,
    .pSpecializationInfo  = nullptr                                            ,
  };

  auto computePipelineCreateInfo = VkComputePipelineCreateInfo{
    .sType                      = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    .pNext                      = nullptr                                       ,
    .flags                      = 0                                             ,
    .stage                      = pipelineShaderStageCreateInfo                 ,
    .layout                     = pipelineLayout                                ,
    .basePipelineHandle         = VK_NULL_HANDLE                                ,
    .basePipelineIndex          = -1                                            ,
  };
  VkPipeline pipeline;
  VK_CALL(vkCreateComputePipelines,device,VK_NULL_HANDLE,1,&computePipelineCreateInfo,nullptr,&pipeline);


  auto mappedMemoryRange = VkMappedMemoryRange{
    .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext  = nullptr                              ,
    .memory = deviceMemory                         ,
    .offset = 0                                    ,
    .size   = 1024*1024*sizeof(float)              ,
  };

  VK_CALL(vkFlushMappedMemoryRanges,device,1,&mappedMemoryRange);


  vkUnmapMemory(device,deviceMemory);

  auto commandBuffer            = createCommandBuffer(device,commandPool);




  auto commandBufferBeginInfo = VkCommandBufferBeginInfo{
    .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext            = nullptr                                    ,
    .flags            = 0                                          ,
    .pInheritanceInfo = nullptr                                    ,
  };
  VK_CALL(vkBeginCommandBuffer,commandBuffer,&commandBufferBeginInfo);

  vkCmdBindPipeline(commandBuffer,VK_PIPELINE_BIND_POINT_COMPUTE,pipeline);
  vkCmdBindDescriptorSets(commandBuffer,VK_PIPELINE_BIND_POINT_COMPUTE,pipelineLayout,0,1,&descriptorSet,0,nullptr);
  vkCmdDispatch(commandBuffer,1,1,1);

  VK_CALL(vkEndCommandBuffer,commandBuffer);

  auto submitInfo = VkSubmitInfo{
    .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext                = nullptr                      ,
    .waitSemaphoreCount   = 0                            ,
    .pWaitSemaphores      = nullptr                      ,
    .pWaitDstStageMask    = nullptr                      ,
    .commandBufferCount   = 1                            ,
    .pCommandBuffers      = &commandBuffer               ,
    .signalSemaphoreCount = 0                            ,
    .pSignalSemaphores    = nullptr                      ,
  };

  VK_CALL(vkQueueSubmit,queue,1,&submitInfo,VK_NULL_HANDLE);

  VK_CALL(vkQueueWaitIdle,queue);

  void*readBuffer=nullptr;
  VK_CALL(vkMapMemory,device,deviceMemory,0,1024*1024*sizeof(float),0,&readBuffer);
  if(readBuffer==nullptr){
    std::cerr << "readBuffer == nullptr" << std::endl;
    return 0;
  }

  VK_CALL(vkInvalidateMappedMemoryRanges,device,1,&mappedMemoryRange);

  auto up=(uint32_t*)readBuffer;
  for(int i=0;i<64;++i)
    std::cerr << up[i] << std::endl;

  vkUnmapMemory(device,deviceMemory);

  //VK_CALL(vkFreeDescriptorSets,device,descriptorPool,1,&descriptorSet);

  vkDestroyDescriptorPool(device,descriptorPool,nullptr);

  vkDestroyPipelineLayout(device,pipelineLayout,nullptr);
  vkDestroyDescriptorSetLayout(device,setLayouts.at(0),nullptr);
  vkDestroyPipeline(device,pipeline,nullptr);
  vkDestroyShaderModule(device,shaderModule,nullptr);

  vkFreeMemory(device,deviceMemory,nullptr);
  vkDestroyBuffer(device,buffer,nullptr);
  vkFreeCommandBuffers(device,commandPool,1,&commandBuffer);
  vkDestroyCommandPool(device,commandPool,nullptr);
  vkDestroyDevice(device,nullptr);
  if(!vkDestroyInstance)std::cerr << "neni vkDestroyInstance" << std::endl;
  vkDestroyInstance(instance,nullptr);
  dlclose(vulkanLib);
  return 0;
#endif
}

