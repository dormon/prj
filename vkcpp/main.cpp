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
  auto ici = vk::InstanceCreateInfo{};
  ici.enabledLayerCount = layers.size();
  ici.ppEnabledLayerNames = layers.data();
  auto instance = vk::createInstance(ici);
  return instance;
}

vk::PhysicalDevice getPhysicalDevice(vk::Instance instance){
  auto physicalDevices = instance.enumeratePhysicalDevices();
  for(auto const&physicalDevice:physicalDevices){
    auto p = physicalDevice.getProperties();
    if(p.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
      return physicalDevice;
  }
  return nullptr;
}

uint32_t getQueueFamilyIndex(vk::PhysicalDevice physicalDevice,vk::QueueFlags req){
  auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
  uint32_t queueFamilyIndex=0;
  for(auto queueFamilyProperty:queueFamilyProperties){
    if(queueFamilyProperty.queueCount >= 1 && (queueFamilyProperty.queueFlags & req) == req){
      break;
    }
    ++queueFamilyIndex;
  }
  if(queueFamilyIndex >= queueFamilyProperties.size())throw("cannot find queue family");
  return queueFamilyIndex;
}

uint32_t getMemoryTypeIndex(vk::PhysicalDevice physicalDevice,vk::MemoryPropertyFlags req){
  auto memoryProperties = physicalDevice.getMemoryProperties();
  uint32_t memoryTypeIndex=0;
  for(auto const&memoryProperty:memoryProperties.memoryTypes){
    if((memoryProperty.propertyFlags & req) == req){
      break;
    }
    ++memoryTypeIndex;
  }
  if(memoryTypeIndex >= memoryProperties.memoryTypeCount)throw("cannot find memory type");
  return memoryTypeIndex;
}

vk::Device createDevice(vk::PhysicalDevice physicalDevice,uint32_t queueFamilyIndex){
  auto deviceQueueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>{1};
  auto queuePriorities = std::vector<float>{1.f};
  deviceQueueCreateInfos.at(0).queueFamilyIndex = queueFamilyIndex;
  deviceQueueCreateInfos.at(0).queueCount       = 1u;
  deviceQueueCreateInfos.at(0).pQueuePriorities = queuePriorities.data();

  auto deviceCreateInfo = vk::DeviceCreateInfo{};
  deviceCreateInfo.queueCreateInfoCount = deviceQueueCreateInfos.size();
  deviceCreateInfo.pQueueCreateInfos    = deviceQueueCreateInfos.data();
  
  auto device = physicalDevice.createDevice(deviceCreateInfo);
  return device;
}

vk::CommandPool createCommandPool(vk::Device device,uint32_t queueFamilyIndex){
  auto cpci = vk::CommandPoolCreateInfo{};
  cpci.queueFamilyIndex = queueFamilyIndex;
  auto commandPool = device.createCommandPool(cpci);
  return commandPool;
}

vk::Buffer createBuffer(vk::Device device,size_t size){
  auto bci = vk::BufferCreateInfo{};
  bci.size = size;
  bci.usage = vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc;
  bci.sharingMode = vk::SharingMode::eExclusive;
  auto buffer = device.createBuffer(bci);
  return buffer;
}

vk::DeviceMemory allocateMemory(vk::Device device,size_t size,uint32_t memoryTypeIndex){
  auto mai = vk::MemoryAllocateInfo{};
  mai.allocationSize  = size;
  mai.memoryTypeIndex = memoryTypeIndex;
  auto dm = device.allocateMemory(mai);
  return dm;
}

vk::ShaderModule createShaderModule(vk::Device device,std::string const&fileName){
  auto smci = vk::ShaderModuleCreateInfo{};
  auto iStream = std::ifstream(fileName, std::ios::binary);
  if(!iStream)throw("cannot open "+fileName);
  auto code = std::vector<char>((std::istreambuf_iterator<char>(iStream)),
                          std::istreambuf_iterator<char>());
  iStream.close();
  smci.codeSize = code.size();
  smci.pCode    = (uint32_t*)code.data();
  auto sm = device.createShaderModule(smci);
  return sm;
}

vk::DescriptorSetLayout createDescriptorSetLayout(vk::Device device){
  auto dslbs = std::vector<vk::DescriptorSetLayoutBinding>(1);
  dslbs.at(0).binding         = 0;
  dslbs.at(0).descriptorType  = vk::DescriptorType::eStorageBuffer;
  dslbs.at(0).descriptorCount = 1;
  dslbs.at(0).stageFlags      = vk::ShaderStageFlagBits::eCompute;

  auto dslci = vk::DescriptorSetLayoutCreateInfo{};
  dslci.bindingCount = dslbs.size();
  dslci.pBindings = dslbs.data();
  auto dsl = device.createDescriptorSetLayout(dslci);
  return dsl;
}

vk::DescriptorPool createDescriptorPool(vk::Device device){
  auto dps = std::vector<vk::DescriptorPoolSize>(1);
  dps.at(0).type = vk::DescriptorType::eStorageBuffer;
  dps.at(0).descriptorCount = 1;

  auto dpci = vk::DescriptorPoolCreateInfo{};
  dpci.maxSets = 1;
  dpci.poolSizeCount = dps.size();
  dpci.pPoolSizes    = dps.data();
  auto descriptorPool = device.createDescriptorPool(dpci);
  return descriptorPool;
}


std::vector<vk::DescriptorSet> allocateDescriptorSets(vk::Device device,vk::DescriptorPool descriptorPool,vk::DescriptorSetLayout descriptorSetLayout){
  auto dsai = vk::DescriptorSetAllocateInfo{};
  dsai.descriptorPool = descriptorPool;
  dsai.descriptorSetCount = 1;
  dsai.pSetLayouts = &descriptorSetLayout;
  auto descriptorSet = device.allocateDescriptorSets(dsai);
  return descriptorSet;
}

vk::PipelineLayout createPipelineLayout(vk::Device device,vk::DescriptorSetLayout descriptorSetLayout){
  auto plci = vk::PipelineLayoutCreateInfo{};
  plci.setLayoutCount = 1;
  plci.pSetLayouts = &descriptorSetLayout;

  auto pipelineLayout = device.createPipelineLayout(plci);
  return pipelineLayout;
}

vk::Pipeline createComputePipeline(vk::Device device,vk::ShaderModule shaderModule,vk::PipelineLayout pipelineLayout,std::string const&entryPoint){
  auto pssci = vk::PipelineShaderStageCreateInfo{};
  pssci.stage = vk::ShaderStageFlagBits::eCompute;
  pssci.module = shaderModule;
  pssci.pName = entryPoint.c_str();

  auto cpici = vk::ComputePipelineCreateInfo{};
  cpici.stage  = pssci;
  cpici.layout = pipelineLayout;
  cpici.basePipelineHandle = nullptr;
  cpici.basePipelineIndex = -1;
  auto pipeline = device.createComputePipeline({},cpici);
  return pipeline.value;
}

int main(int argc,char*argv[]){
  try{
  auto instance = createInstance();
  auto physicalDevice = getPhysicalDevice(instance);
  auto physicalDevices = instance.enumeratePhysicalDevices();

  auto queueFamilyIndex         = getQueueFamilyIndex               (physicalDevice,vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer);
  auto memoryTypeIndex          = getMemoryTypeIndex                (physicalDevice,vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible);
  auto device                   = createDevice                      (physicalDevice,queueFamilyIndex);
  auto queue                    = device.getQueue                   (queueFamilyIndex,0);
  auto commandPool              = createCommandPool                 (device,queueFamilyIndex);
  auto buffer                   = createBuffer                      (device,10124*sizeof(uint32_t));
  auto bufferMemoryRequirements = device.getBufferMemoryRequirements(buffer);
  auto deviceMemory             = allocateMemory                    (device,bufferMemoryRequirements.size,memoryTypeIndex);

  device.bindBufferMemory(buffer,deviceMemory,0);

  auto mappedBuffer             = device.mapMemory(deviceMemory,0,1024*sizeof(uint32_t));
  auto shaderModule             = createShaderModule(device,"shader.spv");
  auto descriptorSetLayout      = createDescriptorSetLayout(device);

  auto descriptorPool           = createDescriptorPool(device);
  auto descriptorSet            = allocateDescriptorSets(device,descriptorPool,descriptorSetLayout);

  auto dbi = vk::DescriptorBufferInfo{};
  dbi.buffer = buffer;
  dbi.offset = 0;
  dbi.range = vk::WholeSize;

  auto wds = vk::WriteDescriptorSet{};
  wds.dstSet = descriptorSet.at(0);
  wds.dstBinding = 0;
  wds.dstArrayElement = 0;
  wds.descriptorCount = 1;
  wds.descriptorType = vk::DescriptorType::eStorageBuffer;
  wds.pBufferInfo = &dbi;

  device.updateDescriptorSets(1u,&wds,0,nullptr);

  auto pipelineLayout = createPipelineLayout (device,descriptorSetLayout);
  auto pipeline       = createComputePipeline(device,shaderModule,pipelineLayout,"main");

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
  }catch(std::exception const&e){
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

