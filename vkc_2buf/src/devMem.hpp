#pragma once

#include<vulkan/vulkan.h>

struct DevMem{
  VkDevice       device;
  VkDeviceMemory memory;
  void invalidate(size_t offset = 0,size_t size = VK_WHOLE_SIZE);
  void flush     (size_t offset = 0,size_t size = VK_WHOLE_SIZE);
  void*map       (size_t offset = 0,size_t size = VK_WHOLE_SIZE);
  void unmap     ();
};
