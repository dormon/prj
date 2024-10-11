#pragma once

#include<vulkan/vulkan.h>

struct PhysicalDevice{
  PhysicalDevice();
  PhysicalDevice(VkPhysicalDevice dev);
  VkPhysicalDevice           device    ;
  VkPhysicalDeviceProperties properties;
};

