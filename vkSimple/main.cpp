#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <sstream>
#include <map>
#include <vulkan/vulkan.h>
#include "vkStr.h"
#include "vkCall.h"

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

class CommandPool{
  public:
    CommandPool(VkCommandPool const&p,VkDevice dev):pool(p),device(dev){}
    VkCommandPool pool;
    VkDevice      device ;
    ~CommandPool(){
      vkDestroyCommandPool(device,pool,nullptr);
    }
};

class Queue{
  public:
    Queue(VkQueue const&qq):q(qq){}
    VkQueue q;
};

class Device{
  public:
    Device(VkDevice const&d){
      device = std::shared_ptr<VkDevice>(new VkDevice,[](VkDevice*i){vkDestroyDevice(*i,nullptr);delete i;});
      *device = d;
    }
    ~Device(){
    }
    Queue getQueue(){
      VkQueue res;
      vkGetDeviceQueue(*device,0,0,&res);
      return Queue(res);
    }
    CommandPool getCommandPool(){
      VkCommandPool res;
      VkCommandPoolCreateInfo info;
      info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      info.pNext            = nullptr;
      info.flags            = 0;
      info.queueFamilyIndex = 0;
      CALL(vkCreateCommandPool,*device,&info,nullptr,&res);
      return CommandPool(res,*device);
    }
    std::shared_ptr<VkDevice> device;
};


class PhysicalDevice{
  public:
    PhysicalDevice(VkPhysicalDevice const&d):device(d){
      vkGetPhysicalDeviceProperties(device,&properties);
      std::cerr << properties.deviceName << std::endl;
      std::cerr << vkPhysicalDeviceType2String(properties.deviceType) << std::endl;
      uint32_t nFams;
      vkGetPhysicalDeviceQueueFamilyProperties(device,&nFams,nullptr);
      queueFamilyProperties.resize(nFams);
      vkGetPhysicalDeviceQueueFamilyProperties(device,&nFams,queueFamilyProperties.data());
      for(auto const&qf:queueFamilyProperties){
        std::cerr << vkQueueFlags2String(qf.queueFlags) << std::endl;
        std::cerr << qf.queueCount << std::endl;
      }
    }
    ~PhysicalDevice(){
    }
    Device getDevice(){
      float priorities = 1;
      VkDeviceQueueCreateInfo qInfo;
      qInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      qInfo.pNext            = nullptr;
      qInfo.flags            = 0;
      qInfo.queueFamilyIndex = 0;
      qInfo.queueCount       = 1;
      qInfo.pQueuePriorities = &priorities;

      VkDeviceCreateInfo info;
      info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      info.pNext                   = nullptr;
      info.flags                   = 0;
      info.queueCreateInfoCount    = 1;
      info.pQueueCreateInfos       = &qInfo;
      info.enabledLayerCount       = 0;
      info.ppEnabledLayerNames     = nullptr;
      info.enabledExtensionCount   = 0;
      info.ppEnabledExtensionNames = nullptr;
      info.pEnabledFeatures        = nullptr;
      VkDevice logicalDevice;
      CALL(vkCreateDevice,device,&info,nullptr,&logicalDevice);
      return Device(logicalDevice);
    }
    VkPhysicalDevice device;
    VkPhysicalDeviceProperties properties;
    std::vector<VkQueueFamilyProperties>queueFamilyProperties;
};


std::vector<char const*>getLayerNames(){
  uint32_t pPropertyCount;
  vkEnumerateInstanceLayerProperties(&pPropertyCount,0);
  std::vector<VkLayerProperties>pProperties(pPropertyCount);
  vkEnumerateInstanceLayerProperties(&pPropertyCount,pProperties.data());

  //std::vector<char const*>layerNames;
  //for(auto const&p:pProperties){
  //  std::cerr << p.layerName << std::endl;
  //  if(std::string(p.layerName) == "VK_LAYER_LUNARG_vktrace")continue;
  //  //std::cerr << p.description << std::endl;
  //  layerNames.push_back(p.layerName);
  //}

  std::vector<char const*>layerNames = {
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_GOOGLE_unique_objects",
    "VK_LAYER_GOOGLE_threading",
    "VK_LAYER_LUNARG_standard_validation",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_starter_layer",
    "VK_LAYER_GOOGLE_threading",
    //"VK_LAYER_LUNARG_device_simulation",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_screenshot",
    "VK_LAYER_LUNARG_monitor",
    "VK_LAYER_GOOGLE_unique_objects",
    "VK_LAYER_KHRONOS_validation",
    //"VK_LAYER_LUNARG_vktrace",
    //"VK_LAYER_LUNARG_api_dump",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_LUNARG_demo_layer",
    "VK_LAYER_LUNARG_standard_validation",
    //"VK_LAYER_LUNARG_assistant_layer",
    "VK_LAYER_RENDERDOC_Capture",
    "VK_LAYER_VALVE_steam_fossilize_64",
    "VK_LAYER_VALVE_steam_overlay_64",
    "VK_LAYER_VALVE_steam_overlay_32",
    "VK_LAYER_VALVE_steam_fossilize_32",
  };
  return layerNames;
}

class Instance{
  public:
    Instance(){
      instance = std::shared_ptr<VkInstance>(new VkInstance,[](VkInstance*i){vkDestroyInstance(*i,nullptr);delete i;});

      auto layerNames = getLayerNames();

      VkInstanceCreateInfo info;
      info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      info.pNext                   = nullptr;
      info.flags                   = 0;
      info.pApplicationInfo        = nullptr;
      info.enabledLayerCount       = layerNames.size();
      info.ppEnabledLayerNames     = layerNames.data();
      info.enabledExtensionCount   = 0;
      info.ppEnabledExtensionNames = nullptr;



      CALL(vkCreateInstance,&info,nullptr,&(*instance));
    }
    ~Instance(){
    }
    std::vector<PhysicalDevice> getDevices(){
      uint32_t nDevices;
      CALL(vkEnumeratePhysicalDevices,*instance,&nDevices,nullptr);

      std::vector<VkPhysicalDevice>physicalDevices(nDevices);
      CALL(vkEnumeratePhysicalDevices,*instance,&nDevices,physicalDevices.data());
      std::vector<PhysicalDevice>result;
      for(auto const&d:physicalDevices)
        result.emplace_back(d);
      return result;
    }
    std::shared_ptr<VkInstance> instance;
};

int main(){

  auto instance = Instance();
  auto physicalDevices = instance.getDevices();
  auto dev = physicalDevices[0].getDevice();
  auto queue = dev.getQueue();
  auto pool  = dev.getCommandPool();
  (void)queue;

  return 0;
}


