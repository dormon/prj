#include<iostream>
#include<sstream>
#include<vector>
#include<memory>

#include"dormonvu.hpp"

#if 0
#include <dlfcn.h>
#include "comp.hpp"

#define VK_NO_PROTOTYPES
#include<vulkan/vulkan.h>

#define ___ std::cerr << __LINE__ << std::endl




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


VkPhysicalDevice selectDevice(std::vector<VkPhysicalDevice>const&physicalDevices){
  uint32_t selectedDevice = 0;
  return physicalDevices.at(selectedDevice);
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

#endif

#include<functional>

int main(int argc,char*argv[]){
  Vulkan vk;
  vk.start();
  vk.printPhysicalDeviceGroups();
  vk.printPhysicalDeviceProperties();
  vk.printQueueProperties();
  vk.stop();

    

#if 0

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

  VkDescriptorBufferInfo descriptorBufferInfo{
    .buffer = buffer       ,
    .offset = 0            ,
    .range  = VK_WHOLE_SIZE,
  };

  VkWriteDescriptorSet writeDescriptorSet{
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


  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{
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


  VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo{
    .sType                = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext                = nullptr                                            ,
    .flags                = 0                                                  ,
    .stage                = VK_SHADER_STAGE_COMPUTE_BIT                        ,
    .module               = shaderModule                                       ,
    .pName                = "main"                                             ,
    .pSpecializationInfo  = nullptr                                            ,
  };

  VkComputePipelineCreateInfo computePipelineCreateInfo{
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


  VkMappedMemoryRange mappedMemoryRange{
    .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext  = nullptr                              ,
    .memory = deviceMemory                         ,
    .offset = 0                                    ,
    .size   = 1024*1024*sizeof(float)              ,
  };

  VK_CALL(vkFlushMappedMemoryRanges,device,1,&mappedMemoryRange);


  vkUnmapMemory(device,deviceMemory);

  auto commandBuffer            = createCommandBuffer(device,commandPool);




  VkCommandBufferBeginInfo commandBufferBeginInfo{
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

  VkSubmitInfo submitInfo{
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

