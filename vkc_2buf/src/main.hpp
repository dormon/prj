#pragma once

#include<stdio.h>
#include<vulkan/vulkan.h>
#include<vulkan/vk_enum_string_helper.h>
#include"devMem.hpp"

void gpu_task(int argc,char*argv[]);

struct PhysicalDevice{
  PhysicalDevice();
  PhysicalDevice(VkPhysicalDevice dev);
  VkPhysicalDevice           device    ;
  VkPhysicalDeviceProperties properties;
};

struct Instance{
  VkInstance      instance                    ;
  PhysicalDevice* physicalDevices    = nullptr;
  uint32_t        nofPhysicalDevices = 0      ;
  void getPhysicalDevices();
  void initPhysicalDevices(VkPhysicalDevice*devs,uint32_t n);
  void deletePhysicalDevices();
};

struct Vulkan{
  Instance         instance      ;
  VkPhysicalDevice physicalDevice;
  VkDevice         device        ;
  DevMem           devMem        ;
  VkQueue          queue         ;
  VkCommandPool    commandPool   ;
  VkDescriptorPool descriptorPool;
};


Vulkan init();
void   deinit(Vulkan&);
void   work  (Vulkan&);

Instance              createInstance           ();
VkPhysicalDevice      getPhysicalDevice        (VkInstance instance);
uint32_t              getQueueFamilyIndex      (VkPhysicalDevice physicalDevice,VkQueueFlags req);
uint32_t              getMemoryTypeIndex       (VkPhysicalDevice physicalDevice,VkMemoryPropertyFlags req);
VkDevice              createDevice             (VkPhysicalDevice physicalDevice,uint32_t queueFamilyIndex);
VkQueue               getDeviceQueue           (VkDevice device,int queueFamilyIndex);
DevMem                allocateMemory           (VkDevice device,size_t size,uint32_t memoryTypeIndex);
VkCommandPool         createCommandPool        (VkDevice device,uint32_t queueFamilyIndex);
VkDescriptorPool      createDescriptorPool     (VkDevice device);

VkBuffer              createBuffer             (VkDevice device,size_t size);
VkShaderModule        createShaderModule       (VkDevice device,char const*fileName);
VkDescriptorSetLayout*createDescriptorSetLayout(VkDevice device,uint32_t*n);
VkDescriptorSet*      allocateDescriptorSets   (VkDevice device,VkDescriptorPool descriptorPool,VkDescriptorSetLayout*descriptorSetLayout,uint32_t n);
VkPipelineLayout      createPipelineLayout     (VkDevice device,uint32_t n,VkDescriptorSetLayout*descriptorSetLayout);
VkPipeline            createComputePipeline    (VkDevice device,VkShaderModule shaderModule,VkPipelineLayout pipelineLayout,char const*entryPoint);

void submit          (Vulkan const&vulakn,VkCommandBuffer cmdBuffer);
void begin(VkCommandBuffer commandBuffer);
void end  (VkCommandBuffer commandBuffer);
VkCommandBuffer allocateCommandBuffer(Vulkan const&vulkan);
