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

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl


constexpr char const* vkResult2String(VkResult err){
#define CASE(NAME) case VK_##NAME:return "VK_"#NAME
  switch(err){
    CASE(SUCCESS                                           );
    CASE(NOT_READY                                         );
    CASE(TIMEOUT                                           );
    CASE(EVENT_SET                                         );
    CASE(EVENT_RESET                                       );
    CASE(INCOMPLETE                                        );
    CASE(ERROR_OUT_OF_HOST_MEMORY                          );
    CASE(ERROR_OUT_OF_DEVICE_MEMORY                        );
    CASE(ERROR_INITIALIZATION_FAILED                       );
    CASE(ERROR_DEVICE_LOST                                 );
    CASE(ERROR_MEMORY_MAP_FAILED                           );
    CASE(ERROR_LAYER_NOT_PRESENT                           );
    CASE(ERROR_EXTENSION_NOT_PRESENT                       );
    CASE(ERROR_FEATURE_NOT_PRESENT                         );
    CASE(ERROR_INCOMPATIBLE_DRIVER                         );
    CASE(ERROR_TOO_MANY_OBJECTS                            );
    CASE(ERROR_FORMAT_NOT_SUPPORTED                        );
    CASE(ERROR_FRAGMENTED_POOL                             );
    CASE(ERROR_OUT_OF_POOL_MEMORY                          );
    CASE(ERROR_INVALID_EXTERNAL_HANDLE                     );
    CASE(ERROR_SURFACE_LOST_KHR                            );
    CASE(ERROR_NATIVE_WINDOW_IN_USE_KHR                    );
    CASE(SUBOPTIMAL_KHR                                    );
    CASE(ERROR_OUT_OF_DATE_KHR                             );
    CASE(ERROR_INCOMPATIBLE_DISPLAY_KHR                    );
    CASE(ERROR_VALIDATION_FAILED_EXT                       );
    CASE(ERROR_INVALID_SHADER_NV                           );
    CASE(ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
    CASE(ERROR_FRAGMENTATION_EXT                           );
    CASE(ERROR_NOT_PERMITTED_EXT                           );
    CASE(ERROR_INVALID_DEVICE_ADDRESS_EXT                  );
    CASE(ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT         );
    CASE(RESULT_RANGE_SIZE                                 );
    CASE(RESULT_MAX_ENUM                                   );
    //CASE(ERROR_OUT_OF_POOL_MEMORY_KHR);
    //CASE(ERROR_INVALID_EXTERNAL_HANDLE_KHR);
    //CASE(RESULT_BEGIN_RANGE);
    //CASE(RESULT_END_RANGE);
  }
#undef CASE
  return "";
}

void throwIfError(std::string const&name,std::string const&file,size_t line,VkResult err){
  if(err == VK_SUCCESS)return;
  std::stringstream ss;
  ss << file << " " << line << " " << name << " -  " << vkResult2String(err);
  throw std::runtime_error(ss.str());
}

template<typename...ARGS,typename...VARGS>
void call(std::string const&name,std::string const& file,size_t line,VkResult(*f)(ARGS...),VARGS...args){
  auto err = f(args...);
  throwIfError(name,file,line,err);
}

#define CALL(f,...)\
  call(#f,__FILE__,__LINE__,f,__VA_ARGS__)

class Device{
  public:
    Device(VkDevice const&d):device(d){

    }
    ~Device(){
      vkDestroyDevice(device,nullptr);
    }
    VkDevice device;
};

std::string vkQueueFlags2String(VkQueueFlags f){
  std::stringstream ss;
#define IFF(NAME) if(f&VK_##NAME)ss << "VK_"#NAME " "
  IFF(QUEUE_GRAPHICS_BIT      );
  IFF(QUEUE_COMPUTE_BIT       );
  IFF(QUEUE_TRANSFER_BIT      );
  IFF(QUEUE_SPARSE_BINDING_BIT);
  IFF(QUEUE_PROTECTED_BIT     );
  IFF(QUEUE_FLAG_BITS_MAX_ENUM);
#undef IFF
  return ss.str();
}

constexpr char const*vkPhysicalDeviceType2String(VkPhysicalDeviceType f){
#define CASE(NAME) case VK_##NAME:return "VK_"#NAME
  switch(f){
    CASE(PHYSICAL_DEVICE_TYPE_OTHER         );
    CASE(PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
    CASE(PHYSICAL_DEVICE_TYPE_DISCRETE_GPU  );
    CASE(PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU   );
    CASE(PHYSICAL_DEVICE_TYPE_CPU           );
    CASE(PHYSICAL_DEVICE_TYPE_MAX_ENUM      );
    CASE(PHYSICAL_DEVICE_TYPE_RANGE_SIZE    );
  }
#undef CASE
  return "";
}

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
      VkDeviceQueueCreateInfo qInfo;
      qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      qInfo.pNext = nullptr;
      qInfo.flags = 0;
      qInfo.queueFamilyIndex = 0;
      qInfo.queueCount = 1;
      float priorities = 1;
      qInfo.pQueuePriorities = &priorities;



      VkDeviceCreateInfo info;
      info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      info.pNext = nullptr;
      info.flags = 0;
      info.queueCreateInfoCount = 1;
      info.pQueueCreateInfos = &qInfo;
      info.enabledLayerCount = 0;
      info.ppEnabledLayerNames = nullptr;
      info.enabledExtensionCount = 0;
      info.ppEnabledExtensionNames = nullptr;
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




  //std::vector<VkDevice>devices(nDevices);
  //for(size_t i=0;i<physicalDevices.size();++i){
  //  VkDeviceQueueCreateInfo qInfo;
  //  qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  //  qInfo.pNext = nullptr;
  //  qInfo.flags = 0;//TODO
  //  qInfo.queueFamilyIndex = 0;//TODO
  //  qInfo.queueCount = 1;//TODO
  //  qInfo.pQueuePriorities = nullptr;//TODO



  //  VkDeviceCreateInfo info;
  //  info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  //  info.pNext = nullptr;
  //  info.flags = 0;
  //  info.queueCreateInfoCount = 0; // TODO
  //  info.pQueueCreateInfos = nullptr; // TODO
  //  info.enabledLayerCount = 0;
  //  info.ppEnabledLayerNames = nullptr;
  //  info.enabledExtensionCount = 0;
  //  info.ppEnabledExtensionNames = nullptr;
  //  CALL(vkCreateDevice,physicalDevices[i],&info,nullptr,devices.data()+i);
  //}


  return 0;
}


