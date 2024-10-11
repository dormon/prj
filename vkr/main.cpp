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
  vulkan.instance       = createInstance      ();
  vulkan.physicalDevice = getPhysicalDevice   (vulkan.instance);
  auto queueFamilyIndex = getQueueFamilyIndex (vulkan.physicalDevice,VK_QUEUE_COMPUTE_BIT|VK_QUEUE_TRANSFER_BIT);
  auto memoryTypeIndex  = getMemoryTypeIndex  (vulkan.physicalDevice,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  vulkan.device         = createDevice        (vulkan.physicalDevice,queueFamilyIndex);
  vulkan.deviceMemory   = allocateMemory      (vulkan.device,1024*1024*128,memoryTypeIndex);
  vulkan.queue          = getDeviceQueue      (vulkan.device,queueFamilyIndex);
  vulkan.commandPool    = createCommandPool   (vulkan.device,queueFamilyIndex);
  vulkan.descriptorPool = createDescriptorPool(vulkan.device);
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

  auto descriptorBufferInfo = VkDescriptorBufferInfo{
    .buffer = buffer       ,
    .offset = 0            ,
    .range  = VK_WHOLE_SIZE,
  };
  auto writeDescriptorSet = VkWriteDescriptorSet{
    .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .pNext            = nullptr                               ,
    .dstSet           = descriptorSets[0]                     ,
    .dstBinding       = 0                                     ,
    .dstArrayElement  = 0                                     ,
    .descriptorCount  = 1                                     ,
    .descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER     ,
    .pImageInfo       = nullptr                               ,
    .pBufferInfo      = &descriptorBufferInfo                 ,
    .pTexelBufferView = nullptr                               ,
  };
  vkUpdateDescriptorSets(vulkan.device,1,&writeDescriptorSet,0,nullptr);

  auto pipelineLayout = createPipelineLayout (vulkan.device,descriptorSetLayout);
  auto pipeline       = createComputePipeline(vulkan.device,shaderModule,pipelineLayout,"main");

  auto commandBufferAllocateInfo = VkCommandBufferAllocateInfo{
    .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext              = nullptr                                       ,
    .commandPool        = vulkan.commandPool                            ,
    .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY               ,
    .commandBufferCount = 1                                             ,
  };
  VkCommandBuffer commandBuffer;
  auto result = vkAllocateCommandBuffers(vulkan.device,&commandBufferAllocateInfo,&commandBuffer);
  if(result != VK_SUCCESS)throw "cannot allocate command buffer";

  auto commandBufferBeginInfo = VkCommandBufferBeginInfo{
    .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext            = nullptr                                    ,
    .flags            = 0                                          ,
    .pInheritanceInfo = nullptr                                    ,
  };
  auto res2 = vkBeginCommandBuffer(commandBuffer,&commandBufferBeginInfo);
  if(res2 != VK_SUCCESS)throw "cannot begin command buffer";
  vkCmdBindPipeline(commandBuffer,VK_PIPELINE_BIND_POINT_COMPUTE,pipeline);
  vkCmdBindDescriptorSets(commandBuffer,VK_PIPELINE_BIND_POINT_COMPUTE,pipelineLayout,0,1,&descriptorSets[0],0,nullptr);
  vkCmdDispatch(commandBuffer,1,1,1);
  auto res3 = vkEndCommandBuffer(commandBuffer);
  if(res3 != VK_SUCCESS)throw "cannot end command buffer";


  auto readBuffer = mapMemory(vulkan);
  flushMemory(vulkan,0,1024*sizeof(uint32_t));

  submit(vulkan.queue,commandBuffer);
  auto submitInfo = VkSubmitInfo{
    .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext                = nullptr                      ,
    .waitSemaphoreCount   = 0                            ,
    .pWaitSemaphores      = nullptr                      ,
    .pWaitDstStageMask    = nullptr                      ,
    .commandBufferCount   = 1                            ,
    .pCommandBuffers      = &commandBuffer               ,
    .signalSemaphoreCount = 0                            ,
    .pSignalSemaphores    = nullptr                      ,
  };
  vkQueueSubmit(vulkan.queue,1,&submitInfo,VK_NULL_HANDLE);

  vkQueueWaitIdle(vulkan.queue);

  invalidateMemory(vulkan,0,1024*sizeof(uint32_t));

  auto up=(uint32_t*)readBuffer;
  for(int i=0;i<64;++i)
    fprintf(stderr,"%u\n",up[i]);

  vkUnmapMemory(vulkan.device,vulkan.deviceMemory);

  vkDestroyPipeline(vulkan.device,pipeline,nullptr);
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
      .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext            = nullptr                                   ,
      .flags            = 0                                         ,
      .queueFamilyIndex = queueFamilyIndex                          ,
      .queueCount       = 1u                                        ,
      .pQueuePriorities = queuePriorities                           ,
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

VkQueue getDeviceQueue(VkDevice device,int queueFamilyIndex){
  VkQueue queue;
  vkGetDeviceQueue(device,queueFamilyIndex,0,&queue);
  return queue;
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
      .binding            = 0                                ,
      .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .descriptorCount    = 1                                ,
      .stageFlags         = VK_SHADER_STAGE_COMPUTE_BIT      ,
      .pImmutableSamplers = nullptr                          ,
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
    .descriptorPool     = descriptorPool                                ,
    .descriptorSetCount = 1u                                            ,
    .pSetLayouts        = &descriptorSetLayout                          ,
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
    .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext               = nullptr                                            ,
    .flags               = 0                                                  ,
    .stage               = VK_SHADER_STAGE_COMPUTE_BIT                        ,
    .module              = shaderModule                                       ,
    .pName               = entryPoint                                         ,
    .pSpecializationInfo = nullptr                                            ,
  };
  auto computePipelineCreateInfo     = VkComputePipelineCreateInfo{
    .sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    .pNext              = nullptr                                       ,
    .flags              = 0                                             ,
    .stage              = pipelineShaderStageCreateInfo                 ,
    .layout             = pipelineLayout                                ,
    .basePipelineHandle = nullptr                                       ,
    .basePipelineIndex  = -1                                            ,
  };
  VkPipeline pipeline;
  auto result = vkCreateComputePipelines(device,nullptr,1,&computePipelineCreateInfo,nullptr,&pipeline);
  if(result != VK_SUCCESS)throw "cannot create compute pipeline";
  return pipeline;
}
