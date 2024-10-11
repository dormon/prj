#pragma once

#include<vulkan/vulkan.h>
#include<physicalDevice.hpp>

struct Instance{
  Instance();
  ~Instance();
  VkInstance instance;
  PhysicalDevice*physicalDevices    = nullptr;
  uint32_t       nofPhysicalDevices = 0      ;
  void             initPhysicalDevices     (          );
  uint32_t         getNofPhysicalDevices   (          );
  VkPhysicalDevice*getPhysicalDevices      (uint32_t n);
  void             constructPhysicalDevices(VkPhysicalDevice*devs,uint32_t n);
};
