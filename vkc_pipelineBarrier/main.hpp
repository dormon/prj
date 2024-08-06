#pragma once

#include<vulkan/vulkan.h>

void gpu_task(int argc,char*argv[]);

struct Vulkan{
  VkInstance       instance      ;
  VkPhysicalDevice physicalDevice;
  VkDevice         device        ;
  VkDeviceMemory   deviceMemory  ;
  VkQueue          queue         ;
  VkCommandPool    commandPool   ;
  VkDescriptorPool descriptorPool;
};

Vulkan init();
void   deinit(Vulkan&);
void   work(Vulkan&);

VkInstance            createInstance           ();
VkPhysicalDevice      getPhysicalDevice        (VkInstance instance);
uint32_t              getQueueFamilyIndex      (VkPhysicalDevice physicalDevice,VkQueueFlags req);
uint32_t              getMemoryTypeIndex       (VkPhysicalDevice physicalDevice,VkMemoryPropertyFlags req);
VkDevice              createDevice             (VkPhysicalDevice physicalDevice,uint32_t queueFamilyIndex);
VkDeviceMemory        allocateMemory           (VkDevice device,size_t size,uint32_t memoryTypeIndex);
VkCommandPool         createCommandPool        (VkDevice device,uint32_t queueFamilyIndex);
VkDescriptorPool      createDescriptorPool     (VkDevice device);

VkBuffer              createBuffer             (VkDevice device,size_t size);
VkShaderModule        createShaderModule       (VkDevice device,char const*fileName);
VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device);
VkDescriptorSet*      allocateDescriptorSets   (VkDevice device,VkDescriptorPool descriptorPool,VkDescriptorSetLayout descriptorSetLayout);
VkPipelineLayout      createPipelineLayout     (VkDevice device,VkDescriptorSetLayout descriptorSetLayout);
VkPipeline            createComputePipeline    (VkDevice device,VkShaderModule shaderModule,VkPipelineLayout pipelineLayout,char const*entryPoint);

void*mapMemory       (Vulkan const&vulkan);
void flushMemory     (Vulkan const&vulkan,size_t offset,size_t size);
void invalidateMemory(Vulkan const&vulkan,size_t offset,size_t size);
