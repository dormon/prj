#pragma once

#include<vulkan/vulkan.hpp>

void gpu_task(int argc,char*argv[]);

struct Vulkan{
  vk::Instance       instance      ;
  vk::PhysicalDevice physicalDevice;
  vk::Device         device        ;
  vk::DeviceMemory   deviceMemory  ;
  vk::Queue          queue         ;
  vk::CommandPool    commandPool   ;
  vk::DescriptorPool descriptorPool;
};

Vulkan init();
void   deinit(Vulkan&);
void   work(Vulkan&);

vk::Instance       createInstance      ();
vk::PhysicalDevice getPhysicalDevice   (vk::Instance instance);
uint32_t           getQueueFamilyIndex (vk::PhysicalDevice physicalDevice,vk::QueueFlags req);
uint32_t           getMemoryTypeIndex  (vk::PhysicalDevice physicalDevice,vk::MemoryPropertyFlags req);
vk::Device         createDevice        (vk::PhysicalDevice physicalDevice,uint32_t queueFamilyIndex);
vk::DeviceMemory   allocateMemory      (vk::Device device,size_t size,uint32_t memoryTypeIndex);
vk::CommandPool    createCommandPool   (vk::Device device,uint32_t queueFamilyIndex);
vk::DescriptorPool createDescriptorPool(vk::Device device);

vk::Buffer                    createBuffer             (vk::Device device,size_t size);
vk::ShaderModule              createShaderModule       (vk::Device device,std::string const&fileName);
vk::DescriptorSetLayout       createDescriptorSetLayout(vk::Device device);
std::vector<vk::DescriptorSet>allocateDescriptorSets   (vk::Device device,vk::DescriptorPool descriptorPool,vk::DescriptorSetLayout descriptorSetLayout);
vk::PipelineLayout            createPipelineLayout     (vk::Device device,vk::DescriptorSetLayout descriptorSetLayout);
vk::Pipeline                  createComputePipeline    (vk::Device device,vk::ShaderModule shaderModule,vk::PipelineLayout pipelineLayout,std::string const&entryPoint);

void*mapMemory       (Vulkan const&vulkan);
void flushMemory     (Vulkan const&vulkan,size_t offset,size_t size);
void invalidateMemory(Vulkan const&vulkan,size_t offset,size_t size);
