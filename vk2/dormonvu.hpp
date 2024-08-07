#pragma once

#include<string>
#include<vector>
#include<sstream>
#include<exception>
#define VK_NO_PROTOTYPES
#include<vulkan/vulkan.h>


#define DECLARE(i,f) PFN_##f f = nullptr;

#define NO_INSTANCE_FUNCTION_LIST(i,f)\
  f(i,vkEnumerateInstanceLayerProperties);\
  f(i,vkEnumerateInstanceVersion);\
  f(i,vkCreateInstance);\


#define INSTANCE_FUNCTION_LIST(i,f)\
  f(i,vkDestroyInstance);\
  f(i,vkGetInstanceProcAddr);\
  f(i,vkEnumeratePhysicalDevices);\
  f(i,vkGetDeviceProcAddr);\
  f(i,vkCreateDevice);\
  f(i,vkDestroyDevice);\
  f(i,vkEnumeratePhysicalDeviceGroups);\
      

#define PHYSICAL_DEVICE_FUNCTION_LIST(i,f)\
  f(i,vkEnumeratePhysicalDevices);\
  f(i,vkGetPhysicalDeviceProperties);\
  f(i,vkGetPhysicalDeviceProperties2);\
  f(i,vkGetPhysicalDeviceQueueFamilyProperties);\
  f(i,vkGetPhysicalDeviceQueueFamilyProperties2);\
  f(i,vkGetPhysicalDeviceMemoryProperties);\
  f(i,vkGetPhysicalDeviceMemoryProperties2);\
  f(i,vkGetPhysicalDeviceDisplayPropertiesKHR);\
  f(i,vkGetPhysicalDeviceDisplayProperties2KHR);\
  f(i,vkGetPhysicalDeviceDisplayPlanePropertiesKHR);\
  f(i,vkGetPhysicalDeviceDisplayPlaneProperties2KHR);\
  f(i,vkGetDisplayPlaneSupportedDisplaysKHR);\
  f(i,vkGetDisplayModePropertiesKHR);\
  f(i,vkGetDisplayModeProperties2KHR);\
  f(i,vkCreateDisplayModeKHR);\
  f(i,vkGetDisplayPlaneCapabilitiesKHR);\
  f(i,vkGetDisplayPlaneCapabilities2KHR);\
  f(i,vkGetPhysicalDeviceSurfaceSupportKHR);\
  /*f(i,vkGetPhysicalDeviceWaylandPresentationSupportKHR);*/\
  /*f(i,vkGetPhysicalDeviceWin32PresentationSupportKHR);*/\
  /*f(i,vkGetPhysicalDeviceXcbPresentationSupportKHR);*/\
  /*f(i,vkGetPhysicalDeviceXlibPresentationSupportKHR);*/\
  f(i,vkGetPhysicalDeviceSurfaceCapabilitiesKHR);\
  f(i,vkGetPhysicalDeviceSurfaceCapabilities2KHR);\
  f(i,vkGetPhysicalDeviceSurfaceFormatsKHR);\
  f(i,vkGetPhysicalDeviceSurfaceFormats2KHR);\
  f(i,vkGetPhysicalDeviceSurfacePresentModesKHR);\
  f(i,vkGetPhysicalDevicePresentRectanglesKHR);\
  f(i,vkEnumerateDeviceLayerProperties);\
  f(i,vkEnumerateDeviceExtensionProperties);\
  f(i,vkGetPhysicalDeviceExternalBufferProperties);\
  f(i,vkGetPhysicalDeviceExternalSemaphoreProperties);\
  f(i,vkGetPhysicalDeviceExternalFenceProperties);\
  f(i,vkGetPhysicalDeviceFeatures);\
  f(i,vkGetPhysicalDeviceFeatures2);\
  f(i,vkGetPhysicalDeviceFormatProperties);\
  f(i,vkGetPhysicalDeviceFormatProperties2);\
  f(i,vkGetPhysicalDeviceImageFormatProperties);\
  f(i,vkGetPhysicalDeviceImageFormatProperties2);\


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

struct PhysicalDeviceFunctions{
  PHYSICAL_DEVICE_FUNCTION_LIST(_,DECLARE);
};


struct Vulkan{
  std::string applicationName    = "";
  uint32_t    applicationVersion = 0 ;
  std::string engineName         = "";
  uint32_t    engineVersion      = 0 ;
  uint32_t    apiVersion         = VK_MAKE_VERSION(1,0,0);
  std::vector<std::string> enabledLayers      = {};
  std::vector<std::string> enabledExtensions  = {};

  struct QueueCreateInfo{
    VkQueueFlags       flags      = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
    uint32_t           count      = 1    ;
    std::vector<float> priorities = {1.f};
  };
  struct CreateDeviceInfo{
    uint32_t                    physicalDevice   = 0;
    std::vector<QueueCreateInfo>queueCreateInfos = {
      {}
    };
  };

  std::vector<CreateDeviceInfo>createDeviceInfos = {
    {}
  };

  void start();
  void stop();
  void printPhysicalDeviceGroups();
  void printPhysicalDeviceProperties();
  void printQueueProperties();

  protected:
    PhysicalDeviceFunctions physicalDeviceFunctions;
    std::vector<VkPhysicalDeviceGroupProperties>physicalDeviceGroups;

    struct QueueFamily{
      uint32_t            familyIndex;
      VkCommandPool       commandPool;
      std::vector<VkQueue>queues     ;
    };

    struct LogicalDevice{
      VkDevice                device;
      std::vector<QueueFamily>queueFamily;
      void createCommandPools();
      DEVICE_FUNCTION_LIST(_,DECLARE);
    };

    struct QueueFamility{
      uint32_t                used       = 0;
      VkQueueFamilyProperties properties    ;
    };

    struct PhysicalDevice;
    std::vector<PhysicalDevice>physicalDevices;

    //struct LogicalDevice{
    //  VkDevice device;
    //};
    //std::vector<LogicalDevice>logicalDevices;

    VkInstance instance;
    NO_INSTANCE_FUNCTION_LIST(_,DECLARE);
    INSTANCE_FUNCTION_LIST(_,DECLARE);

  private:
    void*vulkanLib = nullptr;
    VkPhysicalDevice*vk_pDevs;
    uint32_t         npDevs;
    struct{
      VkPhysicalDeviceProperties       props   ;
      VkPhysicalDeviceMemoryProperties memProps;
      VkQueueFamilyProperties*         qfProps ;
      uint32_t                         nQF     ;
    };

    void load_vkGetInstanceProcAddr(void*vulkanLib);
    void load_physicalDeviceFunctions();
    void createInstance                  ();
    void getPhysicalDevices              ();
    void getPhysicalDeviceProperties     ();
    void getQueueFamilyProperties        ();
    void getPhysicalDeviceGroupProperties();
    void createLogicalDevices            ();
    void createLogicalDevice(CreateDeviceInfo const&);
    uint32_t findQueueFamily(
        uint32_t     physicalDevice,
        VkQueueFlags requiredFlags ,
        uint32_t     count         );
    void     throwIfPhysicalDeviceOutOfRange(
        uint32_t     dev           );
    bool areQueueFlagsIncompatible(
        QueueFamility const&family  ,
        VkQueueFlags        required);
    bool notEnoughFamilyCapacity(
        QueueFamility const&family,
        uint32_t            count );
    bool isQueueFamilyIncopatible(
        QueueFamility queueFlags,
        VkQueueFlags  required  ,
        uint32_t      count     );
    void loadDeviceFunctions(PhysicalDevice&pd);
    void createLogicalDevice(PhysicalDevice&pdev,VkDeviceCreateInfo&ci);
    VkDeviceCreateInfo createDeviceCreateInfo(CreateDeviceInfo const&cdi);
    void destroyDeviceCreateInfo(VkDeviceCreateInfo&info);
    PhysicalDevice&getPhysicalDevice(CreateDeviceInfo const&cdi);
    void fillDeviceCreateInfo(VkDeviceCreateInfo&dci,CreateDeviceInfo const&cdi);
    VkDeviceCreateInfo allocateDeviceCreateInfo(CreateDeviceInfo const&cdi);
    PhysicalDevice createPhysicalDevice(VkPhysicalDevice const&pd);
    uint32_t       getNofPhysicalDevices();

};

struct Vulkan::PhysicalDevice{
  PhysicalDevice(VkPhysicalDevice const&pd,PhysicalDeviceFunctions const&pdf);
  VkPhysicalDevice           physicalDevice;
  VkPhysicalDeviceProperties properties    ;
  std::vector<QueueFamility >queueFamilies ;
  std::vector<LogicalDevice >logicalDevices;
  PhysicalDeviceFunctions    ctx;
  void getProperties();
};

