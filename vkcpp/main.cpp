#include<dlfcn.h>
#include<fstream>
#include<iostream>
#include<vulkan/vulkan.hpp>
#include<vulkan/vulkan_enums.hpp>
#include<vulkan/vulkan_structs.hpp>

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
  auto device = physicalDevice.createDevice(deviceCreateInfo);
  return device;
}

vk::CommandPool createCommandPool(vk::Device device,uint32_t queueFamilyIndex){
  auto commandPoolCreateInfo = vk::CommandPoolCreateInfo({},queueFamilyIndex);
  auto commandPool           = device.createCommandPool(commandPoolCreateInfo);
  return commandPool;
}

vk::Buffer createBuffer(vk::Device device,size_t size){
  auto bufferCreateInfo = vk::BufferCreateInfo({},size,vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc);
  auto buffer           = device.createBuffer(bufferCreateInfo);
  return buffer;
}

vk::DeviceMemory allocateMemory(vk::Device device,size_t size,uint32_t memoryTypeIndex){
  auto memoryAllocationInfo = vk::MemoryAllocateInfo(size,memoryTypeIndex);
  auto deviceMemory         = device.allocateMemory(memoryAllocationInfo);
  return deviceMemory;
}

vk::ShaderModule createShaderModule(vk::Device device,std::string const&fileName){
  auto iStream = std::ifstream(fileName, std::ios::binary);
  if(!iStream)throw("cannot open "+fileName);
  auto code = std::vector<char>((std::istreambuf_iterator<char>(iStream)),std::istreambuf_iterator<char>());
  iStream.close();

  auto shaderModuleCreateInfo = vk::ShaderModuleCreateInfo({},code.size(),(uint32_t*)code.data());
  auto shaderModule           = device.createShaderModule(shaderModuleCreateInfo);
  return shaderModule;
}

vk::DescriptorSetLayout createDescriptorSetLayout(vk::Device device){
  auto descriptorSetLayoutBindings = std::vector<vk::DescriptorSetLayoutBinding>{
    vk::DescriptorSetLayoutBinding(0,vk::DescriptorType::eStorageBuffer,1,vk::ShaderStageFlagBits::eCompute)
  };

  auto descriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo({},descriptorSetLayoutBindings.size(),descriptorSetLayoutBindings.data());
  auto descriptorSetLayout           = device.createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
  return descriptorSetLayout;
}

vk::DescriptorPool createDescriptorPool(vk::Device device){
  auto descriptorPoolSizes = std::vector<vk::DescriptorPoolSize>{
    vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer,1)
  };

  auto descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo({},1,descriptorPoolSizes.size(),descriptorPoolSizes.data());
  auto descriptorPool           = device.createDescriptorPool(descriptorPoolCreateInfo);
  return descriptorPool;
}


std::vector<vk::DescriptorSet> allocateDescriptorSets(vk::Device device,vk::DescriptorPool descriptorPool,vk::DescriptorSetLayout descriptorSetLayout){
  auto descriptorSetAllocateInfo = vk::DescriptorSetAllocateInfo(descriptorPool,1,&descriptorSetLayout);
  auto descriptorSet             = device.allocateDescriptorSets(descriptorSetAllocateInfo);
  return descriptorSet;
}

vk::PipelineLayout createPipelineLayout(vk::Device device,vk::DescriptorSetLayout descriptorSetLayout){
  auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo({},1,&descriptorSetLayout);
  auto pipelineLayout           = device.createPipelineLayout(pipelineLayoutCreateInfo);
  return pipelineLayout;
}

vk::Pipeline createComputePipeline(vk::Device device,vk::ShaderModule shaderModule,vk::PipelineLayout pipelineLayout,std::string const&entryPoint){
  auto pipelineShaderStageCreateInfo = vk::PipelineShaderStageCreateInfo({},vk::ShaderStageFlagBits::eCompute,shaderModule,entryPoint.c_str());
  auto computePipelineCreateInfo     = vk::ComputePipelineCreateInfo({},pipelineShaderStageCreateInfo,pipelineLayout);
  auto pipeline                      = device.createComputePipeline({},computePipelineCreateInfo);
  return pipeline.value;
}

void work(int argc,char*argv[]){
  auto instance                 = createInstance();
  auto physicalDevice           = getPhysicalDevice(instance);
  auto physicalDevices          = instance.enumeratePhysicalDevices();
  auto queueFamilyIndex         = getQueueFamilyIndex               (physicalDevice,vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer);
  auto memoryTypeIndex          = getMemoryTypeIndex                (physicalDevice,vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible);
  auto device                   = createDevice                      (physicalDevice,queueFamilyIndex);
  auto queue                    = device.getQueue                   (queueFamilyIndex,0);
  auto commandPool              = createCommandPool                 (device,queueFamilyIndex);
  auto buffer                   = createBuffer                      (device,1024*sizeof(uint32_t));
  auto bufferMemoryRequirements = device.getBufferMemoryRequirements(buffer);
  auto deviceMemory             = allocateMemory                    (device,bufferMemoryRequirements.size,memoryTypeIndex);

  device.bindBufferMemory(buffer,deviceMemory,0);

  auto shaderModule             = createShaderModule(device,"shader.spv");
  auto descriptorSetLayout      = createDescriptorSetLayout(device);

  auto descriptorPool           = createDescriptorPool(device);
  auto descriptorSet            = allocateDescriptorSets(device,descriptorPool,descriptorSetLayout);

  auto descriptorBufferInfo = vk::DescriptorBufferInfo(buffer,0,vk::WholeSize);

  auto writeDescriptorSet = vk::WriteDescriptorSet(descriptorSet.at(0),0,0,1,vk::DescriptorType::eStorageBuffer,{},&descriptorBufferInfo);

  device.updateDescriptorSets(1u,&writeDescriptorSet,0,nullptr);

  auto pipelineLayout = createPipelineLayout (device,descriptorSetLayout);
  auto pipeline       = createComputePipeline(device,shaderModule,pipelineLayout,"main");

  void*readBuffer = nullptr;
  if(device.mapMemory(deviceMemory,0,1024*sizeof(uint32_t),{},&readBuffer) != vk::Result::eSuccess)
    throw("cannot map memory");
  
  auto mappedMemoryRange = vk::MappedMemoryRange(deviceMemory,0,1024*sizeof(uint32_t));

  if(device.flushMappedMemoryRanges(1,&mappedMemoryRange) != vk::Result::eSuccess)
    throw("cannot flush mapped memory ranges");

  device.unmapMemory(deviceMemory);

  auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo(commandPool,vk::CommandBufferLevel::ePrimary,1);
  auto commandBuffers = device.allocateCommandBuffers(commandBufferAllocateInfo);

  auto commandBufferBeginInfo = vk::CommandBufferBeginInfo();
  commandBuffers.at(0).begin(commandBufferBeginInfo);
  commandBuffers.at(0).bindPipeline(vk::PipelineBindPoint::eCompute,pipeline);
  commandBuffers.at(0).bindDescriptorSets(vk::PipelineBindPoint::eCompute,pipelineLayout,0,1,descriptorSet.data(),0,{});
  commandBuffers.at(0).dispatch(1,1,1);
  commandBuffers.at(0).end();

  auto submitInfo = vk::SubmitInfo({},{},{},commandBuffers.size(),commandBuffers.data());
  queue.submit(submitInfo);
  queue.waitIdle();

  if(device.mapMemory(deviceMemory,0,1024*sizeof(uint32_t),{},&readBuffer) != vk::Result::eSuccess)
    throw("cannot map memory");

  if(readBuffer == nullptr)throw("cannot map buffer for reading");

  if(device.invalidateMappedMemoryRanges(1,&mappedMemoryRange) != vk::Result::eSuccess)
    throw("cannot invalidate mapped memory ranges");

  auto up=(uint32_t*)readBuffer;
  for(int i=0;i<64;++i)
    std::cerr << up[i] << std::endl;


  device.unmapMemory(deviceMemory);


  device.destroyPipeline           (pipeline);
  device.destroyPipelineLayout     (pipelineLayout);
  device.destroyDescriptorPool     (descriptorPool);
  device.destroyDescriptorSetLayout(descriptorSetLayout);
  device.destroyShaderModule       (shaderModule);
  device.unmapMemory               (deviceMemory);
  device.freeMemory                (deviceMemory);
  device.destroyBuffer             (buffer);
  device.destroyCommandPool        (commandPool);
  
  device.destroy();
  instance.destroy();
}

int main(int argc,char*argv[]){
  try{
    work(argc,argv);
  }catch(std::exception const&e){
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

