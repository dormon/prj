#include<dlfcn.h>
#include<fstream>
#include<iostream>
#include<vulkan/vulkan.hpp>

#include"main.hpp"

int main(int argc,char*argv[]){
  try{
    gpu_task(argc,argv);
  }catch(std::exception const&e){
    std::cerr << e.what() << std::endl;
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
  auto queueFamilyIndex = getQueueFamilyIndex   (vulkan.physicalDevice,vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer);
  auto memoryTypeIndex  = getMemoryTypeIndex    (vulkan.physicalDevice,vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible);
  vulkan.device         = createDevice          (vulkan.physicalDevice,queueFamilyIndex);
  vulkan.deviceMemory   = allocateMemory        (vulkan.device,1024*1024*128,memoryTypeIndex);
  vulkan.queue          = vulkan.device.getQueue(queueFamilyIndex,0);
  vulkan.commandPool    = createCommandPool     (vulkan.device,queueFamilyIndex);
  vulkan.descriptorPool = createDescriptorPool  (vulkan.device);
  return vulkan;
}

void deinit(Vulkan&vulkan){
  vulkan.device  .destroyDescriptorPool(vulkan.descriptorPool);
  vulkan.device  .freeMemory           (vulkan.deviceMemory  );
  vulkan.device  .destroyCommandPool   (vulkan.commandPool   );
  vulkan.device  .destroy              (                     );
  vulkan.instance.destroy              (                     );
}

void work(Vulkan&vulkan){
  auto buffer                   = createBuffer(vulkan.device,1024*sizeof(uint32_t));
  auto bufferMemoryRequirements = vulkan.device.getBufferMemoryRequirements(buffer);

  vulkan.device.bindBufferMemory(buffer,vulkan.deviceMemory,0);

  auto shaderModule             = createShaderModule(vulkan.device,"shader.spv");

  auto descriptorSetLayout      = createDescriptorSetLayout(vulkan.device);
  auto descriptorSet            = allocateDescriptorSets(vulkan.device,vulkan.descriptorPool,descriptorSetLayout);


  auto descriptorBufferInfo     = vk::DescriptorBufferInfo(buffer,0,vk::WholeSize);
  auto writeDescriptorSet       = vk::WriteDescriptorSet(descriptorSet.at(0),0,0,1,vk::DescriptorType::eStorageBuffer,{},&descriptorBufferInfo);
  vulkan.device.updateDescriptorSets(1u,&writeDescriptorSet,0,nullptr);

  auto pipelineLayout = createPipelineLayout (vulkan.device,descriptorSetLayout);
  auto pipeline       = createComputePipeline(vulkan.device,shaderModule,pipelineLayout,"main");

  auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo(vulkan.commandPool,vk::CommandBufferLevel::ePrimary,1);
  auto commandBuffers            = vulkan.device.allocateCommandBuffers(commandBufferAllocateInfo);

  auto commandBufferBeginInfo = vk::CommandBufferBeginInfo();
  commandBuffers.at(0).begin(commandBufferBeginInfo);
  commandBuffers.at(0).bindPipeline(vk::PipelineBindPoint::eCompute,pipeline);
  commandBuffers.at(0).bindDescriptorSets(vk::PipelineBindPoint::eCompute,pipelineLayout,0,1,descriptorSet.data(),0,{});
  commandBuffers.at(0).dispatch(1,1,1);
  commandBuffers.at(0).end();

  auto submitInfo = vk::SubmitInfo({},{},{},commandBuffers.size(),commandBuffers.data());

  auto readBuffer = mapMemory(vulkan);
  flushMemory(vulkan,0,1024*sizeof(uint32_t));

  vulkan.queue.submit(submitInfo);
  vulkan.queue.waitIdle();

  invalidateMemory(vulkan,0,1024*sizeof(uint32_t));

  auto up=(uint32_t*)readBuffer;
  for(int i=0;i<64;++i)
    std::cerr << up[i] << std::endl;


  vulkan.device.unmapMemory(vulkan.deviceMemory);

  vulkan.device.destroyPipeline           (pipeline);
  vulkan.device.destroyPipelineLayout     (pipelineLayout);
  vulkan.device.destroyDescriptorSetLayout(descriptorSetLayout);
  vulkan.device.destroyBuffer             (buffer);
  vulkan.device.destroyShaderModule       (shaderModule);
}

void*mapMemory(Vulkan const&vulkan){
  void*readBuffer = nullptr;
  if(vulkan.device.mapMemory(vulkan.deviceMemory,0,1024*sizeof(uint32_t),{},&readBuffer) != vk::Result::eSuccess)
    throw("cannot map memory");
  return readBuffer;
}

void flushMemory(Vulkan const&vulkan,size_t offset,size_t size){
  auto mappedMemoryRange = vk::MappedMemoryRange(vulkan.deviceMemory,offset,size);

  if(vulkan.device.flushMappedMemoryRanges(1,&mappedMemoryRange) != vk::Result::eSuccess)
    throw("cannot flush mapped memory ranges");
}

void invalidateMemory(Vulkan const&vulkan,size_t offset,size_t size){

  auto mappedMemoryRange = vk::MappedMemoryRange(vulkan.deviceMemory,offset,size);
  if(vulkan.device.invalidateMappedMemoryRanges(1,&mappedMemoryRange) != vk::Result::eSuccess)
    throw("cannot invalidate mapped memory ranges");
}



vk::Instance createInstance(){
  auto layers = std::vector<char const*>{
    "VK_LAYER_KHRONOS_validation"
  };
  auto instanceCreateInfo = vk::InstanceCreateInfo({},{},layers.size(),layers.data());
  auto instance           = vk::createInstance(instanceCreateInfo);
  return instance;
}

vk::PhysicalDevice getPhysicalDevice(vk::Instance instance){
  auto physicalDevices = instance.enumeratePhysicalDevices();
  for(auto const&physicalDevice:physicalDevices){
    auto properties = physicalDevice.getProperties();
    if(properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
      return physicalDevice;
  }
  return nullptr;
}

uint32_t getQueueFamilyIndex(vk::PhysicalDevice physicalDevice,vk::QueueFlags req){
  auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
  uint32_t queueFamilyIndex=0;
  for(auto queueFamilyProperty:queueFamilyProperties){
    if(queueFamilyProperty.queueCount >= 1 && (queueFamilyProperty.queueFlags & req) == req)break;
    ++queueFamilyIndex;
  }
  if(queueFamilyIndex >= queueFamilyProperties.size())throw("cannot find queue family");
  return queueFamilyIndex;
}

uint32_t getMemoryTypeIndex(vk::PhysicalDevice physicalDevice,vk::MemoryPropertyFlags req){
  auto memoryProperties = physicalDevice.getMemoryProperties();
  uint32_t memoryTypeIndex=0;
  for(auto const&memoryProperty:memoryProperties.memoryTypes){
    if((memoryProperty.propertyFlags & req) == req)break;
    ++memoryTypeIndex;
  }
  if(memoryTypeIndex >= memoryProperties.memoryTypeCount)throw("cannot find memory type");
  return memoryTypeIndex;
}

vk::Device createDevice(vk::PhysicalDevice physicalDevice,uint32_t queueFamilyIndex){
  auto queuePriorities = std::vector<float>{1.f};
  auto deviceQueueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>{
    vk::DeviceQueueCreateInfo({},queueFamilyIndex,1u,queuePriorities.data())
  };

  auto deviceCreateInfo = vk::DeviceCreateInfo({},deviceQueueCreateInfos.size(),deviceQueueCreateInfos.data());
  return physicalDevice.createDevice(deviceCreateInfo);
}

vk::DeviceMemory allocateMemory(vk::Device device,size_t size,uint32_t memoryTypeIndex){
  auto memoryAllocationInfo = vk::MemoryAllocateInfo(size,memoryTypeIndex);
  return device.allocateMemory(memoryAllocationInfo);
}


vk::CommandPool createCommandPool(vk::Device device,uint32_t queueFamilyIndex){
  auto commandPoolCreateInfo = vk::CommandPoolCreateInfo({},queueFamilyIndex);
  return device.createCommandPool(commandPoolCreateInfo);
}

vk::DescriptorPool createDescriptorPool(vk::Device device){
  auto descriptorPoolSizes = std::vector<vk::DescriptorPoolSize>{
    vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer,1)
  };

  auto descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo({},1,descriptorPoolSizes.size(),descriptorPoolSizes.data());
  return device.createDescriptorPool(descriptorPoolCreateInfo);
}

vk::Buffer createBuffer(vk::Device device,size_t size){
  auto bufferCreateInfo = vk::BufferCreateInfo({},size,vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc);
  return device.createBuffer(bufferCreateInfo);
}

vk::ShaderModule createShaderModule(vk::Device device,std::string const&fileName){
  auto iStream = std::ifstream(fileName, std::ios::binary);
  if(!iStream)throw("cannot open "+fileName);
  auto code = std::vector<char>((std::istreambuf_iterator<char>(iStream)),std::istreambuf_iterator<char>());
  iStream.close();

  auto shaderModuleCreateInfo = vk::ShaderModuleCreateInfo({},code.size(),(uint32_t*)code.data());
  return device.createShaderModule(shaderModuleCreateInfo);
}

vk::DescriptorSetLayout createDescriptorSetLayout(vk::Device device){
  auto descriptorSetLayoutBindings = std::vector<vk::DescriptorSetLayoutBinding>{
    vk::DescriptorSetLayoutBinding(0,vk::DescriptorType::eStorageBuffer,1,vk::ShaderStageFlagBits::eCompute)
  };

  auto descriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo({},descriptorSetLayoutBindings.size(),descriptorSetLayoutBindings.data());
  return device.createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
}

std::vector<vk::DescriptorSet> allocateDescriptorSets(vk::Device device,vk::DescriptorPool descriptorPool,vk::DescriptorSetLayout descriptorSetLayout){
  auto descriptorSetAllocateInfo = vk::DescriptorSetAllocateInfo(descriptorPool,1,&descriptorSetLayout);
  return device.allocateDescriptorSets(descriptorSetAllocateInfo);
}

vk::PipelineLayout createPipelineLayout(vk::Device device,vk::DescriptorSetLayout descriptorSetLayout){
  auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo({},1,&descriptorSetLayout);
  return device.createPipelineLayout(pipelineLayoutCreateInfo);
}

vk::Pipeline createComputePipeline(vk::Device device,vk::ShaderModule shaderModule,vk::PipelineLayout pipelineLayout,std::string const&entryPoint){
  auto pipelineShaderStageCreateInfo = vk::PipelineShaderStageCreateInfo({},vk::ShaderStageFlagBits::eCompute,shaderModule,entryPoint.c_str());
  auto computePipelineCreateInfo     = vk::ComputePipelineCreateInfo({},pipelineShaderStageCreateInfo,pipelineLayout);
  auto pipeline                      = device.createComputePipeline({},computePipelineCreateInfo);
  return pipeline.value;
}
