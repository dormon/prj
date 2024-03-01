#include<dlfcn.h>
#include<vulkan/vulkan.h>
#include<exception>
#include<stdio.h>

#include"main.hpp"

int main(int argc,char*argv[]){
  try{
    gpu_task(argc,argv);
  }catch(std::exception const&e){
    fprintf(stderr,"ERROR: %s",e.what());
  }catch(char const*e){
    fprintf(stderr,"ERROR: %s\n",e);
  }
  return 0;
}

void gpu_task(int argc,char*argv[]){
  auto vulkan = init();
  work(vulkan);
  deinit(vulkan);
}

Vulkan init(){
  Vulkan vulkan;
  vulkan.instance       = createInstance        ();
  vulkan.physicalDevice = getPhysicalDevice     (vulkan.instance);
  auto queueFamilyIndex = getQueueFamilyIndex   (vulkan.physicalDevice,VK_QUEUE_COMPUTE_BIT|VK_QUEUE_TRANSFER_BIT);
  auto memoryTypeIndex  = getMemoryTypeIndex    (vulkan.physicalDevice,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  vulkan.device         = createDevice          (vulkan.physicalDevice,queueFamilyIndex);
  vulkan.deviceMemory   = allocateMemory        (vulkan.device,1024*1024*128,memoryTypeIndex);
  vkGetDeviceQueue(vulkan.device,queueFamilyIndex,0,&vulkan.queue);
  vulkan.commandPool    = createCommandPool     (vulkan.device,queueFamilyIndex);
  vulkan.descriptorPool = createDescriptorPool  (vulkan.device);
  return vulkan;
}

void deinit(Vulkan&vulkan){
  vkDestroyDescriptorPool(vulkan.device  ,vulkan.descriptorPool,nullptr);
  vkDestroyCommandPool   (vulkan.device  ,vulkan.commandPool   ,nullptr);
  vkFreeMemory           (vulkan.device  ,vulkan.deviceMemory  ,nullptr);
  vkDestroyDevice        (vulkan.device  ,nullptr);
  vkDestroyInstance      (vulkan.instance,nullptr);
}

void work(Vulkan&vulkan){
  auto buffer                   = createBuffer(vulkan.device,1024*sizeof(uint32_t));
  //auto bufferMemoryRequirements = vulkan.device.getBufferMemoryRequirements(buffer);

  vkBindBufferMemory(vulkan.device,buffer,vulkan.deviceMemory,0);

  auto shaderModule             = createShaderModule(vulkan.device,"shader.spv");

  auto descriptorSetLayout      = createDescriptorSetLayout(vulkan.device);
  auto descriptorSets           = allocateDescriptorSets(vulkan.device,vulkan.descriptorPool,descriptorSetLayout);


  //auto descriptorBufferInfo     = vk::DescriptorBufferInfo(buffer,0,vk::WholeSize);
  //auto writeDescriptorSet       = vk::WriteDescriptorSet(descriptorSet.at(0),0,0,1,vk::DescriptorType::eStorageBuffer,{},&descriptorBufferInfo);
  //vulkan.device.updateDescriptorSets(1u,&writeDescriptorSet,0,nullptr);

  auto pipelineLayout = createPipelineLayout (vulkan.device,descriptorSetLayout);
  auto pipeline       = createComputePipeline(vulkan.device,shaderModule,pipelineLayout,"main");

  //auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo(vulkan.commandPool,vk::CommandBufferLevel::ePrimary,1);
  //auto commandBuffers            = vulkan.device.allocateCommandBuffers(commandBufferAllocateInfo);

  //auto commandBufferBeginInfo = vk::CommandBufferBeginInfo();
  //commandBuffers.at(0).begin(commandBufferBeginInfo);
  //commandBuffers.at(0).bindPipeline(vk::PipelineBindPoint::eCompute,pipeline);
  //commandBuffers.at(0).bindDescriptorSets(vk::PipelineBindPoint::eCompute,pipelineLayout,0,1,descriptorSet.data(),0,{});
  //commandBuffers.at(0).dispatch(1,1,1);
  //commandBuffers.at(0).end();

  //auto submitInfo = vk::SubmitInfo({},{},{},commandBuffers.size(),commandBuffers.data());

  //auto readBuffer = mapMemory(vulkan);
  //flushMemory(vulkan,0,1024*sizeof(uint32_t));

  //vulkan.queue.submit(submitInfo);
  //vulkan.queue.waitIdle();

  //invalidateMemory(vulkan,0,1024*sizeof(uint32_t));

  //auto up=(uint32_t*)readBuffer;
  //for(int i=0;i<64;++i)
  //  std::cerr << up[i] << std::endl;


  //vulkan.device.unmapMemory(vulkan.deviceMemory);

  //vulkan.device.destroyPipeline           (pipeline);
  vkDestroyPipelineLayout(vulkan.device,pipelineLayout,nullptr);
  delete[]descriptorSets;
  vkDestroyDescriptorSetLayout(vulkan.device,descriptorSetLayout,nullptr);
  vkDestroyShaderModule(vulkan.device,shaderModule,nullptr);
  vkDestroyBuffer(vulkan.device,buffer,nullptr);
}

void*mapMemory(Vulkan const&vulkan){
  void*readBuffer = nullptr;
  auto result = vkMapMemory(vulkan.device,vulkan.deviceMemory,0,1024*sizeof(uint32_t),0,&readBuffer);
  if(result != VK_SUCCESS)throw "cannot map memory";
  return readBuffer;
}

void flushMemory(Vulkan const&vulkan,size_t offset,size_t size){
  auto mappedMemoryRange = VkMappedMemoryRange{
    .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext  = nullptr                              ,
    .memory = vulkan.deviceMemory                  ,
    .offset = 0                                    ,
    .size   = VK_WHOLE_SIZE                        ,
  };
  auto result = vkFlushMappedMemoryRanges(vulkan.device,1,&mappedMemoryRange);
  if(result != VK_SUCCESS)throw "cannot flush memory";
}

void invalidateMemory(Vulkan const&vulkan,size_t offset,size_t size){
  auto mappedMemoryRange = VkMappedMemoryRange{
    .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext  = nullptr                              ,
    .memory = vulkan.deviceMemory                  ,
    .offset = 0                                    ,
    .size   = VK_WHOLE_SIZE                        ,
  };
  auto result = vkInvalidateMappedMemoryRanges(vulkan.device,1,&mappedMemoryRange);
  if(result != VK_SUCCESS)throw "cannot invalidate memory";
}

VkInstance createInstance(){
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

  VkInstance instance;
  auto result = vkCreateInstance(&instanceCreateInfo,nullptr,&instance);
  if(result != VK_SUCCESS)throw "cannot create instance";
  return instance;
}

VkPhysicalDevice getPhysicalDevice(VkInstance instance){
  uint32_t physicalDeviceCount = 0;
  vkEnumeratePhysicalDevices(instance,&physicalDeviceCount,nullptr);
  auto physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
  vkEnumeratePhysicalDevices(instance,&physicalDeviceCount,physicalDevices);

  for(uint32_t i=0;i<physicalDeviceCount;++i){
    auto physicalDevice = physicalDevices[i];
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physicalDevice,&properties);
    if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
      delete[]physicalDevices;
      return physicalDevice;
    }
  }
  return nullptr;
}

uint32_t getQueueFamilyIndex(VkPhysicalDevice physicalDevice,VkQueueFlags req){
  uint32_t queueFamilyPropertyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queueFamilyPropertyCount,nullptr);
  auto queueFamilyProperties = new VkQueueFamilyProperties[queueFamilyPropertyCount];
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queueFamilyPropertyCount,queueFamilyProperties);


  uint32_t queueFamilyIndex=0;
  for(;queueFamilyIndex < queueFamilyPropertyCount;++queueFamilyIndex){
    auto queueFamilyProperty = queueFamilyProperties[queueFamilyIndex];
    if(queueFamilyProperty.queueCount >= 1 && (queueFamilyProperty.queueFlags & req) == req)break;
    ++queueFamilyIndex;
  }
  if(queueFamilyIndex >= queueFamilyPropertyCount)throw("cannot find queue family");
  delete[]queueFamilyProperties;
  return queueFamilyIndex;
}

uint32_t getMemoryTypeIndex(VkPhysicalDevice physicalDevice,VkMemoryPropertyFlags req){
  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice,&memoryProperties);

  uint32_t memoryTypeIndex=0;
  for(auto const&memoryProperty:memoryProperties.memoryTypes){
    if((memoryProperty.propertyFlags & req) == req)break;
    ++memoryTypeIndex;
  }
  if(memoryTypeIndex >= memoryProperties.memoryTypeCount)throw("cannot find memory type");
  return memoryTypeIndex;
}

VkDevice createDevice(VkPhysicalDevice physicalDevice,uint32_t queueFamilyIndex){
  float queuePriorities[] = {1.f};
  VkDeviceQueueCreateInfo deviceQueueCreateInfos[] = {
    VkDeviceQueueCreateInfo{
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = nullptr                                   ,
      .flags = 0                                         ,
      .queueFamilyIndex = queueFamilyIndex               ,
      .queueCount       = 1u                             ,
      .pQueuePriorities = queuePriorities                ,
    },
  };

  auto deviceCreateInfo = VkDeviceCreateInfo{
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO                          ,
    .pNext                   = nullptr                                                       ,
    .flags                   = 0                                                             ,
    .queueCreateInfoCount    = sizeof(deviceQueueCreateInfos)/sizeof(VkDeviceQueueCreateInfo),
    .pQueueCreateInfos       = deviceQueueCreateInfos                                        ,
    .enabledLayerCount       = 0                                                             ,
    .ppEnabledLayerNames     = nullptr                                                       ,
    .enabledExtensionCount   = 0                                                             ,
    .ppEnabledExtensionNames = nullptr                                                       ,
    .pEnabledFeatures        = nullptr                                                       ,

  };
  VkDevice device;
  auto result = vkCreateDevice(physicalDevice,&deviceCreateInfo,nullptr,&device);
  if(result != VK_SUCCESS)throw "cannot create device";

  return device;
}

VkDeviceMemory allocateMemory(VkDevice device,size_t size,uint32_t memoryTypeIndex){
  auto memoryAllocationInfo = VkMemoryAllocateInfo{
    .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext           = nullptr                               ,
    .allocationSize  = size                                  ,
    .memoryTypeIndex = memoryTypeIndex                       ,
  };
  VkDeviceMemory deviceMemory;
  auto result = vkAllocateMemory(device,&memoryAllocationInfo,nullptr,&deviceMemory);
  if(result != VK_SUCCESS)throw "cannot allocate memory";
  return deviceMemory;
}


VkCommandPool createCommandPool(VkDevice device,uint32_t queueFamilyIndex){
  auto commandPoolCreateInfo = VkCommandPoolCreateInfo{
    .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext            = nullptr                                   ,
    .flags            = 0                                         ,
    .queueFamilyIndex = queueFamilyIndex                          ,
  };
  VkCommandPool commandPool;
  auto result = vkCreateCommandPool(device,&commandPoolCreateInfo,nullptr,&commandPool);
  if(result != VK_SUCCESS)throw "cannot create command pool";
  return commandPool;
}

VkDescriptorPool createDescriptorPool(VkDevice device){
  VkDescriptorPoolSize descriptorPoolSizes[] = {
    VkDescriptorPoolSize{
      .type            = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .descriptorCount = 1u                               ,
    },
  };
  auto descriptorPoolCreateInfo = VkDescriptorPoolCreateInfo{
    .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO           ,
    .pNext         = nullptr                                                 ,
    .flags         = 0                                                       ,
    .maxSets       = 1u                                                      ,
    .poolSizeCount = sizeof(descriptorPoolSizes)/sizeof(VkDescriptorPoolSize),
    .pPoolSizes    = descriptorPoolSizes                                     ,
  };

  VkDescriptorPool descriptorPool;
  auto result = vkCreateDescriptorPool(device,&descriptorPoolCreateInfo,nullptr,&descriptorPool);
  if(result != VK_SUCCESS)throw "cannot create descriptor pool";

  return descriptorPool;
}

VkBuffer createBuffer(VkDevice device,size_t size){
  auto bufferCreateInfo = VkBufferCreateInfo{
    .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO                                 ,
    .pNext                 = nullptr                                                              ,
    .flags                 = 0                                                                    ,
    .size                  = size                                                                 ,
    .usage                 = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    .sharingMode           = VK_SHARING_MODE_EXCLUSIVE                                            ,
    .queueFamilyIndexCount = 0u                                                                   ,
    .pQueueFamilyIndices   = nullptr                                                              ,
  };
  VkBuffer buffer;
  auto result = vkCreateBuffer(device,&bufferCreateInfo,nullptr,&buffer);
  if(result != VK_SUCCESS)throw "cannot create buffer";
  return buffer;
}

VkShaderModule createShaderModule(VkDevice device,char const*fileName){
  auto file = fopen(fileName,"rb");
  if(!file)throw "cannot open spirv shader";
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  auto code = new uint32_t[size/sizeof(uint32_t)];
  auto read = fread(code,1,size,file);
  if(read != size)throw "cannot read spirv shader";
  fclose(file);

  auto shaderModuleCreateInfo = VkShaderModuleCreateInfo{
    .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext    = nullptr                                    ,
    .flags    = 0                                          ,
    .codeSize = size                                       ,
    .pCode    = code                                       ,
  };

  VkShaderModule shaderModule;
  auto result = vkCreateShaderModule(device,&shaderModuleCreateInfo,nullptr,&shaderModule);
  if(result != VK_SUCCESS)throw "cannot create shader module";
  delete[]code;
  return shaderModule;
}

VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device){
  VkDescriptorSetLayoutBinding descriptorSetLayoutBindings[] = {
    VkDescriptorSetLayoutBinding{
      .binding = 0,
      .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .descriptorCount = 1,
      .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
      .pImmutableSamplers = nullptr,
    },
  };

  auto descriptorSetLayoutCreateInfo = VkDescriptorSetLayoutCreateInfo{
    .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO                     ,
    .pNext        = nullptr                                                                 ,
    .flags        = 0                                                                       ,
    .bindingCount = sizeof(descriptorSetLayoutBindings)/sizeof(VkDescriptorSetLayoutBinding),
    .pBindings    = descriptorSetLayoutBindings                                             ,
  };

  VkDescriptorSetLayout descriptorSetLayout;
  auto result = vkCreateDescriptorSetLayout(device,&descriptorSetLayoutCreateInfo,nullptr,&descriptorSetLayout);
  if(result != VK_SUCCESS)throw "cannot create descriptorSetLaoyut";

  return descriptorSetLayout;
}

VkDescriptorSet* allocateDescriptorSets(VkDevice device,VkDescriptorPool descriptorPool,VkDescriptorSetLayout descriptorSetLayout){
  auto descriptorSetAllocateInfo = VkDescriptorSetAllocateInfo{
    .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext              = nullptr                                       ,
    .descriptorPool     = descriptorPool,
    .descriptorSetCount = 1u,
    .pSetLayouts        = &descriptorSetLayout,
  };

  VkDescriptorSet*descriptorSets = new VkDescriptorSet[1u];
  auto result = vkAllocateDescriptorSets(device,&descriptorSetAllocateInfo,descriptorSets);
  if(result != VK_SUCCESS)throw "cannot allocate descriptorSets";

  return descriptorSets;
}

VkPipelineLayout createPipelineLayout(VkDevice device,VkDescriptorSetLayout descriptorSetLayout){
  auto pipelineLayoutCreateInfo = VkPipelineLayoutCreateInfo{
    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext                  = nullptr                                      ,
    .flags                  = 0                                            ,
    .setLayoutCount         = 1u                                           ,
    .pSetLayouts            = &descriptorSetLayout                         ,
    .pushConstantRangeCount = 0                                            ,
    .pPushConstantRanges    = nullptr                                      ,
  };
  VkPipelineLayout pipelineLayout;
  auto result = vkCreatePipelineLayout(device,&pipelineLayoutCreateInfo,nullptr,&pipelineLayout);
  if(result != VK_SUCCESS)throw "cannot create pipelineLayout";
  return pipelineLayout;
}

VkPipeline createComputePipeline(VkDevice device,VkShaderModule shaderModule,VkPipelineLayout pipelineLayout,char const*entryPoint){
  auto pipelineShaderStageCreateInfo = VkPipelineShaderStageCreateInfo{
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext = nullptr                                            ,
    .flags = 0                                                  ,
    .module = shaderModule,
    .pName = entryPoint,
    .pSpecializationInfo = nullptr,
    .stage = 
  };//({},vk::ShaderStageFlagBits::eCompute,shaderModule,entryPoint.c_str());
  auto computePipelineCreateInfo     = VkComputePipelineCreateInfo{
    .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    .pNext = nullptr                                       ,
    .flags = 0                                             ,
    .layout = pipelineLayout,
    .basePipelineHandle = nullptr,
    .basePipelineIndex = 0,
  };
  VkPipeline pipeline;
  auto result = vkCreateComputePipelines(device,nullptr,1,&computePipelineCreateInfo,nullptr,&pipeline);
  if(result != VK_SUCCESS)throw "cannot create compute pipeline";
  return pipeline;
}
