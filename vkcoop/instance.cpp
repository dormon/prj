#include<instance.hpp>

Instance::Instance(){
  char const* layers[] = {
    "VK_LAYER_KHRONOS_validation"
  };

  auto instanceCreateInfo = VkInstanceCreateInfo{
    .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext                   = nullptr                               ,
    .flags                   = 0                                     ,
    .pApplicationInfo        = nullptr                               ,
    .enabledLayerCount       = sizeof(layers)/sizeof(char const*)    ,
    .ppEnabledLayerNames     = layers                                ,
    .enabledExtensionCount   = 0                                     ,
    .ppEnabledExtensionNames = nullptr                               ,
  };

  auto result = vkCreateInstance(&instanceCreateInfo,nullptr,&instance);
  if(result != VK_SUCCESS)throw "cannot create instance";
}

Instance::~Instance(){
  vkDestroyInstance(instance,nullptr);
}

void Instance::constructPhysicalDevices(VkPhysicalDevice*devs,uint32_t n){
  for(uint32_t i=0;i<nofPhysicalDevices;++i)
    new(physicalDevices+i)PhysicalDevice(devs[i]);
}

void Instance::initPhysicalDevices(){
  nofPhysicalDevices = getNofPhysicalDevices();
  auto devices = getPhysicalDevices(nofPhysicalDevices);
  physicalDevices = new PhysicalDevice[nofPhysicalDevices];
  constructPhysicalDevices(devices,nofPhysicalDevices);
  delete devices;
}

uint32_t Instance::getNofPhysicalDevices(){
  uint32_t n = 0;
  vkEnumeratePhysicalDevices(instance,&n,nullptr);
  return n;
}

VkPhysicalDevice* Instance::getPhysicalDevices(uint32_t n){
  auto physicalDevices = new VkPhysicalDevice[n];
  vkEnumeratePhysicalDevices(instance,&n,physicalDevices);
  return physicalDevices;
}

