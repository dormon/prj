#include<vulkan/vulkan.h>
#include<vulkan/vk_enum_string_helper.h>
#include<exception>
#include<stdio.h>

#include<main.hpp>
#include<devMem.hpp>
#include<error.hpp>

#ifndef ROOT_DIR
#define ROOT_DIR "."
#endif


int main(int argc,char*argv[]){
  try{
    gpu_task(argc,argv);
  }catch(std::exception const&e){
    fprintf(stderr,"ERROR: %s",e.what());
  }catch(char const*e){
    fprintf(stderr,"ERROR: %s\n",e);
  }catch(Error const&e){
    e.print();
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
  vulkan.instance.getPhysicalDevices();
  vulkan.physicalDevice = getPhysicalDevice   (vulkan.instance.instance);
  auto queueFamilyIndex = getQueueFamilyIndex (vulkan.physicalDevice,VK_QUEUE_COMPUTE_BIT|VK_QUEUE_TRANSFER_BIT);
  auto memoryTypeIndex  = getMemoryTypeIndex  (vulkan.physicalDevice,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  vulkan.device         = createDevice        (vulkan.physicalDevice,queueFamilyIndex);
  vulkan.devMem         = allocateMemory      (vulkan.device,1024*1024*128,memoryTypeIndex);
  vulkan.queue          = getDeviceQueue      (vulkan.device,queueFamilyIndex);
  vulkan.commandPool    = createCommandPool   (vulkan.device,queueFamilyIndex);
  vulkan.descriptorPool = createDescriptorPool(vulkan.device);
  return vulkan;
}

void deinit(Vulkan&vulkan){
  vkDestroyDescriptorPool(vulkan.device           ,vulkan.descriptorPool,nullptr);
  vkDestroyCommandPool   (vulkan.device           ,vulkan.commandPool   ,nullptr);
  vkFreeMemory           (vulkan.devMem.device    ,vulkan.devMem.memory ,nullptr);
  vkDestroyDevice        (vulkan.device           ,nullptr);
  vkDestroyInstance      (vulkan.instance.instance,nullptr);
}

void work(Vulkan&vulkan){
  auto buffer0 = createBuffer(vulkan.device,64*sizeof(uint32_t));
  auto buffer1 = createBuffer(vulkan.device,64*sizeof(uint32_t));
  auto buffer2 = createBuffer(vulkan.device,64*sizeof(uint32_t));
  //auto bufferMemoryRequirements = vulkan.device.getBufferMemoryRequirements(buffer);

  vulkan.devMem.bind(buffer0,  0*sizeof(uint32_t));
  vulkan.devMem.bind(buffer1, 64*sizeof(uint32_t));
  vulkan.devMem.bind(buffer2,128*sizeof(uint32_t));

  auto shaderModule             = createShaderModule(vulkan.device,"shaders/shader.spv");

  uint32_t nofDescriptorSetLayouts;
  auto descriptorSetLayout      = createDescriptorSetLayout(vulkan.device,&nofDescriptorSetLayouts);
  auto descriptorSets           = allocateDescriptorSets(vulkan.device,vulkan.descriptorPool,descriptorSetLayout,nofDescriptorSetLayouts);

  VkDescriptorBufferInfo descriptorBufferInfo[] = {
    VkDescriptorBufferInfo{
      .buffer = buffer0      ,
      .offset = 0            ,
      .range  = VK_WHOLE_SIZE,
    },
    VkDescriptorBufferInfo{
      .buffer = buffer1      ,
      .offset = 0            ,
      .range  = VK_WHOLE_SIZE,
    },
    VkDescriptorBufferInfo{
      .buffer = buffer2      ,
      .offset = 0            ,
      .range  = VK_WHOLE_SIZE,
    },
  };

  VkWriteDescriptorSet writeDescriptorSet[] = {
    VkWriteDescriptorSet{
      .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext            = nullptr                               ,
      .dstSet           = descriptorSets[0]                     ,
      .dstBinding       = 0                                     ,
      .dstArrayElement  = 0                                     ,
      .descriptorCount  = 1                                     ,
      .descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER     ,
      .pImageInfo       = nullptr                               ,
      .pBufferInfo      = descriptorBufferInfo+0                ,
      .pTexelBufferView = nullptr                               ,
    },
    VkWriteDescriptorSet{
      .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext            = nullptr                               ,
      .dstSet           = descriptorSets[0]                     ,
      .dstBinding       = 3                                     ,
      .dstArrayElement  = 0                                     ,
      .descriptorCount  = 1                                     ,
      .descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER     ,
      .pImageInfo       = nullptr                               ,
      .pBufferInfo      = descriptorBufferInfo+1                ,
      .pTexelBufferView = nullptr                               ,
    },
    VkWriteDescriptorSet{
      .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext            = nullptr                               ,
      .dstSet           = descriptorSets[2]                     ,
      .dstBinding       = 0                                     ,
      .dstArrayElement  = 0                                     ,
      .descriptorCount  = 1                                     ,
      .descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER     ,
      .pImageInfo       = nullptr                               ,
      .pBufferInfo      = descriptorBufferInfo+2                ,
      .pTexelBufferView = nullptr                               ,
    },
  };
  vkUpdateDescriptorSets(vulkan.device,sizeof(writeDescriptorSet)/sizeof(VkWriteDescriptorSet),writeDescriptorSet,0,nullptr);


  auto pipelineLayout = createPipelineLayout (vulkan.device,3,descriptorSetLayout);
  auto pipeline       = createComputePipeline(vulkan.device,shaderModule,pipelineLayout,"main");

  auto commandBuffer = allocateCommandBuffer(vulkan);

  begin(commandBuffer);
  vkCmdBindPipeline(commandBuffer,VK_PIPELINE_BIND_POINT_COMPUTE,pipeline);
  vkCmdBindDescriptorSets(commandBuffer,VK_PIPELINE_BIND_POINT_COMPUTE,pipelineLayout,0,nofDescriptorSetLayouts,descriptorSets,0,nullptr);
  vkCmdDispatch(commandBuffer,1,1,1);
  end(commandBuffer);

  vulkan.devMem.map();
  vulkan.devMem.flush(0,2*64*sizeof(uint32_t));
  vulkan.devMem.unmap();

  submit(vulkan,commandBuffer);

  vkQueueWaitIdle(vulkan.queue);



  auto ptr  = (uint32_t*)vulkan.devMem.map();
  vulkan.devMem.invalidate(0,2*64*sizeof(uint32_t));
  for(int i=0;i<64*3;++i)
    fprintf(stderr,"Buf: %u\n",ptr[i]);
  vulkan.devMem.unmap();

  vkDestroyPipeline(vulkan.device,pipeline,nullptr);
  vkDestroyPipelineLayout(vulkan.device,pipelineLayout,nullptr);
  delete[]descriptorSets;
  for(uint32_t i=0;i<nofDescriptorSetLayouts;++i)
    vkDestroyDescriptorSetLayout(vulkan.device,descriptorSetLayout[i],nullptr);
  delete[]descriptorSetLayout;
  vkDestroyShaderModule(vulkan.device,shaderModule,nullptr);
  vkDestroyBuffer(vulkan.device,buffer2,nullptr);
  vkDestroyBuffer(vulkan.device,buffer1,nullptr);
  vkDestroyBuffer(vulkan.device,buffer0,nullptr);
}


Instance createInstance(){
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

  Instance res;
  VK_CALL(vkCreateInstance,&instanceCreateInfo,nullptr,&res.instance);
  return res;
}

auto getNofPhysicalDevices(VkInstance instance){
  uint32_t n = 0;
  vkEnumeratePhysicalDevices(instance,&n,nullptr);
  return n;
}

auto getPhysicalDevices(VkInstance instance,uint32_t n){
  auto physicalDevices = new VkPhysicalDevice[n];
  vkEnumeratePhysicalDevices(instance,&n,physicalDevices);
  return physicalDevices;
}

auto getPhysicalDeviceProperties(VkPhysicalDevice device){
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(device,&properties);
  return properties;
}

VkPhysicalDevice getPhysicalDevice(VkInstance instance){
  auto count   = getNofPhysicalDevices(instance);
  auto devices = getPhysicalDevices(instance,count);

  for(uint32_t i=0;i<count;++i){
    auto device = devices[i];
    auto properties = getPhysicalDeviceProperties(device);
    if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
      delete[]devices;
      return device;
    }
  }
  throw "getPhysicalDevice cannot locate descrete GPU";
  return nullptr;
}

uint32_t getNofQueueFamilyProperties(VkPhysicalDevice dev){
  uint32_t count;
  vkGetPhysicalDeviceQueueFamilyProperties(dev,&count,nullptr);
  return count;
}

uint32_t getQueueFamilyIndex(VkPhysicalDevice dev,VkQueueFlags req){
  auto count = getNofQueueFamilyProperties(dev);
  auto props = new VkQueueFamilyProperties[count];
  vkGetPhysicalDeviceQueueFamilyProperties(dev,&count,props);


  uint32_t index=0;
  for(;index < count;++index){
    auto prop = props[index];
    if(prop.queueCount >= 1 && (prop.queueFlags & req) == req)break;
    ++index;
  }
  if(index >= count)throw("cannot find queue family");
  delete[]props;
  return index;
}

uint32_t getMemoryTypeIndex(VkPhysicalDevice dev,VkMemoryPropertyFlags req){
  VkPhysicalDeviceMemoryProperties props;
  vkGetPhysicalDeviceMemoryProperties(dev,&props);

  uint32_t index=0;
  for(auto const&prop:props.memoryTypes){
    if((prop.propertyFlags & req) == req)break;
    ++index;
  }
  if(index >= props.memoryTypeCount)throw("cannot find memory type");
  return index;
}

VkDevice createDevice(VkPhysicalDevice physicalDevice,uint32_t queueFamilyIndex){
  float priorities[] = {1.f};
  VkDeviceQueueCreateInfo queueInfos[] = {
    VkDeviceQueueCreateInfo{
      .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext            = nullptr                                   ,
      .flags            = 0                                         ,
      .queueFamilyIndex = queueFamilyIndex                          ,
      .queueCount       = 1u                                        ,
      .pQueuePriorities = priorities                                ,
    },
  };

  auto createInfo = VkDeviceCreateInfo{
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO    ,
    .pNext                   = nullptr                                 ,
    .flags                   = 0                                       ,
    .queueCreateInfoCount    = sizeof(queueInfos)/sizeof(queueInfos[0]),
    .pQueueCreateInfos       = queueInfos                              ,
    .enabledLayerCount       = 0                                       ,
    .ppEnabledLayerNames     = nullptr                                 ,
    .enabledExtensionCount   = 0                                       ,
    .ppEnabledExtensionNames = nullptr                                 ,
    .pEnabledFeatures        = nullptr                                 ,

  };
  VkDevice device;
  VK_CALL(vkCreateDevice,physicalDevice,&createInfo,nullptr,&device);

  return device;
}

VkQueue getDeviceQueue(VkDevice device,int queueFamilyIndex){
  VkQueue queue;
  vkGetDeviceQueue(device,queueFamilyIndex,0,&queue);
  return queue;
}

DevMem allocateMemory(VkDevice device,size_t size,uint32_t memoryTypeIndex){
  DevMem res;
  res.device = device;
  auto info = VkMemoryAllocateInfo{
    .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext           = nullptr                               ,
    .allocationSize  = size                                  ,
    .memoryTypeIndex = memoryTypeIndex                       ,
  };
  VK_CALL(vkAllocateMemory,device,&info,nullptr,&res.memory);
  return res;
}


VkCommandPool createCommandPool(VkDevice device,uint32_t queueFamilyIndex){
  auto pool = VkCommandPoolCreateInfo{
    .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext            = nullptr                                   ,
    .flags            = 0                                         ,
    .queueFamilyIndex = queueFamilyIndex                          ,
  };
  VkCommandPool commandPool;
  VK_CALL(vkCreateCommandPool,device,&pool,nullptr,&commandPool);
  return commandPool;
}

VkDescriptorPool createDescriptorPool(VkDevice device){
  VkDescriptorPoolSize sizes[] = {
    VkDescriptorPoolSize{
      .type            = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .descriptorCount = 5u                               ,
    },
  };
  auto createInfo = VkDescriptorPoolCreateInfo{
    .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .pNext         = nullptr                                      ,
    .flags         = 0                                            ,
    .maxSets       = 5u                                           ,
    .poolSizeCount = sizeof(sizes)/sizeof(sizes[0])               ,
    .pPoolSizes    = sizes                                        ,
  };

  VkDescriptorPool pool;
  VK_CALL(vkCreateDescriptorPool,device,&createInfo,nullptr,&pool);

  return pool;
}

VkDescriptorSetLayoutBinding descriptorSetLayoutBinding_csbuf(uint32_t b,uint32_t c,VkSampler const*i=nullptr){
  return
  VkDescriptorSetLayoutBinding{
    .binding            = b                                ,
    .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    .descriptorCount    = c                                ,
    .stageFlags         = VK_SHADER_STAGE_COMPUTE_BIT      ,
    .pImmutableSamplers = i                                ,
  };
}

VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(uint32_t c,VkDescriptorSetLayoutBinding*s){
  return VkDescriptorSetLayoutCreateInfo{
    .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext        = nullptr                                            ,
    .flags        = 0                                                  ,
    .bindingCount = c                                                  ,
    .pBindings    = s                                                  ,
  };
}

VkDescriptorSetLayout*createDescriptorSetLayout(VkDevice device,uint32_t*n){
  VkDescriptorSetLayoutBinding dslb1[] = {
    VkDescriptorSetLayoutBinding{
      .binding            = 0                                ,
      .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .descriptorCount    = 1                                ,
      .stageFlags         = VK_SHADER_STAGE_COMPUTE_BIT      ,
      .pImmutableSamplers = nullptr                          ,
    },
    VkDescriptorSetLayoutBinding{
      .binding            = 3                                ,
      .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .descriptorCount    = 1                                ,
      .stageFlags         = VK_SHADER_STAGE_COMPUTE_BIT      ,
      .pImmutableSamplers = nullptr                          ,
    },
  };

  VkDescriptorSetLayoutBinding dslb2[] = {
    VkDescriptorSetLayoutBinding{
      .binding            = 0                                ,
      .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .descriptorCount    = 1                                ,
      .stageFlags         = VK_SHADER_STAGE_COMPUTE_BIT      ,
      .pImmutableSamplers = nullptr                          ,
    },
  };

  VkDescriptorSetLayoutCreateInfo dslci[] = {
    descriptorSetLayoutCreateInfo(sizeof(dslb1)/sizeof(dslb1[0]),dslb1  ),
    descriptorSetLayoutCreateInfo(0                             ,nullptr),
    descriptorSetLayoutCreateInfo(sizeof(dslb2)/sizeof(dslb2[0]),dslb2  ),
  };

  *n=sizeof(dslci)/sizeof(dslci[0]);
  VkDescriptorSetLayout*layouts = new VkDescriptorSetLayout[*n];
  VkResult result;
  for(uint32_t i=0;i<*n;++i)
    VK_CALL(vkCreateDescriptorSetLayout,device,dslci+i,nullptr,layouts+i);

  return layouts;
}

VkDescriptorSet* allocateDescriptorSets(VkDevice device,VkDescriptorPool pool,VkDescriptorSetLayout*layouts,uint32_t n){
  auto allocateInfo = VkDescriptorSetAllocateInfo{
    .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext              = nullptr                                       ,
    .descriptorPool     = pool                                          ,
    .descriptorSetCount = n                                             ,
    .pSetLayouts        = layouts                                       ,
  };

  VkDescriptorSet*sets = new VkDescriptorSet[n];
  VK_CALL(vkAllocateDescriptorSets,device,&allocateInfo,sets);

  return sets;
}

VkPipelineLayout createPipelineLayout(VkDevice device,uint32_t n,VkDescriptorSetLayout*layouts){
  auto createInfo = VkPipelineLayoutCreateInfo{
    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext                  = nullptr                                      ,
    .flags                  = 0                                            ,
    .setLayoutCount         = n                                            ,
    .pSetLayouts            = layouts                                      ,
    .pushConstantRangeCount = 0                                            ,
    .pPushConstantRanges    = nullptr                                      ,
  };
  VkPipelineLayout pipelineLayout;
  VK_CALL(vkCreatePipelineLayout,device,&createInfo,nullptr,&pipelineLayout);
  return pipelineLayout;
}

VkPipeline createComputePipeline(VkDevice device,VkShaderModule module,VkPipelineLayout layout,char const*entryPoint){
  auto pipelineShaderStageCreateInfo = VkPipelineShaderStageCreateInfo{
    .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext               = nullptr                                            ,
    .flags               = 0                                                  ,
    .stage               = VK_SHADER_STAGE_COMPUTE_BIT                        ,
    .module              = module                                             ,
    .pName               = entryPoint                                         ,
    .pSpecializationInfo = nullptr                                            ,
  };
  auto computePipelineCreateInfo     = VkComputePipelineCreateInfo{
    .sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    .pNext              = nullptr                                       ,
    .flags              = 0                                             ,
    .stage              = pipelineShaderStageCreateInfo                 ,
    .layout             = layout                                        ,
    .basePipelineHandle = nullptr                                       ,
    .basePipelineIndex  = -1                                            ,
  };
  VkPipeline pipeline;
  VK_CALL(vkCreateComputePipelines,device,nullptr,1,&computePipelineCreateInfo,nullptr,&pipeline);
  return pipeline;
}

void begin(VkCommandBuffer commandBuffer){
  auto info = VkCommandBufferBeginInfo{
    .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext            = nullptr                                    ,
    .flags            = 0                                          ,
    .pInheritanceInfo = nullptr                                    ,
  };
  VK_CALL(vkBeginCommandBuffer,commandBuffer,&info);
}

void end(VkCommandBuffer commandBuffer){
  VK_CALL(vkEndCommandBuffer,commandBuffer);
}

void submit(Vulkan const&vulkan,VkCommandBuffer commandBuffer){
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
  VK_CALL(vkQueueSubmit,vulkan.queue,1,&submitInfo,VK_NULL_HANDLE);
}

VkCommandBuffer allocateCommandBuffer(Vulkan const&vulkan){
  auto allocateInfo = VkCommandBufferAllocateInfo{
    .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext              = nullptr                                       ,
    .commandPool        = vulkan.commandPool                            ,
    .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY               ,
    .commandBufferCount = 1                                             ,
  };
  VkCommandBuffer commandBuffer;
  VK_CALL(vkAllocateCommandBuffers,vulkan.device,&allocateInfo,&commandBuffer);
  return commandBuffer;
}

PhysicalDevice::PhysicalDevice(){}
PhysicalDevice::PhysicalDevice(VkPhysicalDevice dev):device(dev){
  properties = getPhysicalDeviceProperties(dev);
}

void Instance::getPhysicalDevices(){
  nofPhysicalDevices = getNofPhysicalDevices(instance);
  auto devices = ::getPhysicalDevices(instance,nofPhysicalDevices);
  physicalDevices = new PhysicalDevice[nofPhysicalDevices];
  initPhysicalDevices(devices,nofPhysicalDevices);
  delete devices;
}

void Instance::initPhysicalDevices(VkPhysicalDevice*devs,uint32_t n){
  for(uint32_t i=0;i<nofPhysicalDevices;++i)
    new(physicalDevices+i)PhysicalDevice(devs[i]);
}

void Instance::deletePhysicalDevices(){
  delete[]physicalDevices;
}
