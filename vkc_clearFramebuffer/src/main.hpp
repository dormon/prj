#pragma once

#include<vulkan/vulkan.h>

struct Error{
  char const*file     = nullptr   ;
  int        line     = -1        ;
  char const*fceName  = nullptr   ;
  VkResult   result   = VK_SUCCESS;
  char const*msg      = nullptr   ;
  Error(char const*file,int line,char const*fce,VkResult r,char const*m);
  void print()const;
};

#define VK_CALL(fce,...) \
  if(auto err = fce(__VA_ARGS__);err != VK_SUCCESS)\
    throw Error(__FILE__,__LINE__,#fce,err,"")

#define VK_ERROR(msg)\
  throw Error(__FILE__,__LINE__,"",VK_SUCCESS,msg)


struct Vulkan{
  VkInstance       instance        ;
  uint32_t         nofPhysicalDevices;
  VkPhysicalDevice*physicalDevices ;
  VkPhysicalDevice physicalDevice  ;
  VkDevice         device          ;
  VkDeviceMemory   deviceMemory    ;
  uint32_t         queueFamilyIndex;
  uint32_t         memoryTypeIndex ;
  VkQueue          queue           ;
  VkCommandPool    commandPool     ;
  VkImage          image           ;
  VkImageView      imageView       ;
  VkFramebuffer    framebuffer     ;
  VkRenderPass     renderPass      ;
};

void   gpu_task(int argc,char*argv[]);
Vulkan init();
void   deinit(Vulkan&);
void   work  (Vulkan&);

VkPhysicalDevice getPhysicalDevice    (VkInstance instance);
uint32_t         getQueueFamilyIndex  (VkPhysicalDevice physicalDevice,VkQueueFlags req);
uint32_t         getMemoryTypeIndex   (VkPhysicalDevice physicalDevice,VkMemoryPropertyFlags req);
VkPhysicalDevice*getPhysicalDevices   (VkInstance instance,uint32_t n);
VkDevice         createDevice         (VkPhysicalDevice physicalDevice,uint32_t queueFamilyIndex);
VkQueue          getDeviceQueue       (VkDevice device,int queueFamilyIndex);
VkDeviceMemory   allocateMemory       (VkDevice device,size_t size,uint32_t memoryTypeIndex);
VkCommandPool    createCommandPool    (VkDevice device,uint32_t queueFamilyIndex);
VkDescriptorPool createDescriptorPool (VkDevice device);
uint32_t         getNofPhysicalDevices(VkInstance instance);
void             invalidate           (VkDevice device,VkDeviceMemory memory,size_t offset,size_t size);
void             flush                (VkDevice device,VkDeviceMemory memory,size_t offset,size_t size);
void             unmap                (VkDevice device,VkDeviceMemory memory);
void*            map                  (VkDevice device,VkDeviceMemory memory,size_t offset,size_t size);
void             bind                 (VkDevice device,VkBuffer buffer,VkDeviceMemory memory,size_t offset);
void             submit               (VkQueue  queue,VkCommandBuffer cmdBuffer);
void             begin                (VkCommandBuffer commandBuffer);
void             end                  (VkCommandBuffer commandBuffer);
VkCommandBuffer  allocateCommandBuffer(VkDevice device,VkCommandPool pool);
VkInstance       createInstance       ();
VkFramebuffer    createFramebuffer    (VkDevice device,VkRenderPass renderPass,VkImageView imageView);
VkRenderPass     createRenderPass     (VkDevice device);
VkImage          createImage          (VkDevice device,uint32_t queueFamilyIndex);
VkImageView      createImageView      (VkDevice device,VkImage image);
void             barrier              (VkCommandBuffer commandBuffer,uint32_t queueFamilyIndex,VkImage image);
void             beginRenderPass      (VkCommandBuffer commandBuffer,VkRenderPass renderPass,VkFramebuffer framebuffer);
void             clear                (VkCommandBuffer commandBuffer);
void             endRenderPass        (VkCommandBuffer commandBuffer);
void             clearMemory          (Vulkan&vulkan);
void             printMemory          (Vulkan&vulkan);
