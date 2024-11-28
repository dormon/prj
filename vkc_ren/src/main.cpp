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
  vulkan.physicalDevice   = getPhysicalDevice   (vulkan.instance.instance);
  vulkan.queueFamilyIndex = getQueueFamilyIndex (vulkan.physicalDevice,VK_QUEUE_GRAPHICS_BIT|VK_QUEUE_TRANSFER_BIT);
  vulkan.memoryTypeIndex  = getMemoryTypeIndex  (vulkan.physicalDevice,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  vulkan.device           = createDevice        (vulkan.physicalDevice,vulkan.queueFamilyIndex);
  vulkan.devMem           = allocateMemory      (vulkan.device,1024*1024*128,vulkan.memoryTypeIndex);
  vulkan.queue            = getDeviceQueue      (vulkan.device,vulkan.queueFamilyIndex);
  vulkan.commandPool      = createCommandPool   (vulkan.device,vulkan.queueFamilyIndex);
  vulkan.descriptorPool   = createDescriptorPool(vulkan.device);
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

  uint32_t nofDescriptorSetLayouts;

  VkSubpassDescription sds[] = {
    VkSubpassDescription{
      .flags                   = 0                              ,
      .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount    = 0                              ,
      .pInputAttachments       = nullptr                        ,
      .colorAttachmentCount    = 0                              ,
      .pColorAttachments       = nullptr                        ,
      .pResolveAttachments     = nullptr                        ,
      .pDepthStencilAttachment = nullptr                        ,
      .preserveAttachmentCount = 0                              ,
      .pPreserveAttachments    = 0                              ,
    },
  };

  auto ad = VkAttachmentDescription{
    .flags          = 0                               ,
    .format         = VK_FORMAT_R8G8B8A8_UINT         ,
    .samples        = VK_SAMPLE_COUNT_1_BIT           ,
    .loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE ,
    .storeOp        = VK_ATTACHMENT_STORE_OP_STORE    ,
    .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE ,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout  = VK_IMAGE_LAYOUT_GENERAL         ,
    .finalLayout    = VK_IMAGE_LAYOUT_GENERAL         ,
  };

  auto rpci = VkRenderPassCreateInfo{
    .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext           = nullptr                                  ,
    .flags           = 0                                        ,
    .attachmentCount = 1                                        ,
    .pAttachments    = &ad                                      ,
    .subpassCount    = sizeof(sds)/sizeof(sds[0])               ,
    .pSubpasses      = sds                                      ,
    .dependencyCount = 0                                        ,
    .pDependencies   = nullptr                                  ,
  };
  VkRenderPass renderPass;
  VK_CALL(vkCreateRenderPass,vulkan.device,&rpci,nullptr,&renderPass);



  uint32_t imageQueues[]={
    vulkan.queueFamilyIndex,
  };
  auto ici = VkImageCreateInfo{
    .sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO       ,
    .pNext                 = nullptr                                   ,
    .flags                 = 0                                         ,
    .imageType             = VK_IMAGE_TYPE_2D                          ,
    .format                = VK_FORMAT_R8G8B8A8_UINT                   ,
    .extent                = {64,64,1}                                 ,
    .mipLevels             = 1                                         ,
    .arrayLayers           = 1                                         ,
    .samples               = VK_SAMPLE_COUNT_1_BIT                     ,
    .tiling                = VK_IMAGE_TILING_LINEAR                    ,
    .usage                 = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT       ,
    .sharingMode           = VK_SHARING_MODE_EXCLUSIVE                 ,
    .queueFamilyIndexCount = sizeof(imageQueues)/sizeof(imageQueues[0]),
    .pQueueFamilyIndices   = imageQueues                               ,
  };
  VkImage image;
  VK_CALL(vkCreateImage,vulkan.device,&ici,nullptr,&image);
 
  VK_CALL(vkBindImageMemory,vulkan.device,image,vulkan.devMem.memory,0);


  auto isr = VkImageSubresourceRange{
    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
    .baseMipLevel   = 0                        ,
    .levelCount     = 1                        ,
    .baseArrayLayer = 0                        ,
    .layerCount     = 1                        ,
  };

  auto cm = VkComponentMapping{
    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
  };

  auto ivci = VkImageViewCreateInfo{
    .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext            = nullptr                                 ,
    .flags            = 0                                       ,
    .image            = image                                   ,
    .viewType         = VK_IMAGE_VIEW_TYPE_2D                   ,
    .format           = VK_FORMAT_R8G8B8A8_UINT                 ,
    .components       = cm                                      ,
    .subresourceRange = isr                                     ,
  };
  VkImageView imageView;
  VK_CALL(vkCreateImageView,vulkan.device,&ivci,nullptr,&imageView);

  

  auto fci = VkFramebufferCreateInfo{
    .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext           = nullptr                                  ,
    .flags           = 0                                        ,
    .renderPass      = renderPass                               ,
    .attachmentCount = 1                                        ,
    .pAttachments    = &imageView                               ,
    .width           = 64                                       ,
    .height          = 64                                       ,
    .layers          = 1                                        ,
  };
  VkFramebuffer framebuffer;
  VK_CALL(vkCreateFramebuffer,vulkan.device,&fci,nullptr,&framebuffer);


  auto commandBuffer = allocateCommandBuffer(vulkan);


  auto rpbi = VkRenderPassBeginInfo{
    .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext           = nullptr                                 ,
    .renderPass      = renderPass                              ,
    .framebuffer     = framebuffer                             ,
    .renderArea      = VkRect2D{.offset{0,0},.extent{64,64}}   ,
    .clearValueCount = 0                                       ,
    .pClearValues    = nullptr                                 ,
  };



  auto pmsscf = VkPipelineMultisampleStateCreateInfo{
    .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .pNext                 = nullptr                                                 ,
    .flags                 = 0                                                       ,
    .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT                                   ,
    .sampleShadingEnable   = VK_FALSE                                                ,
    .minSampleShading      = 0.f                                                     ,
    .pSampleMask           = nullptr                                                 ,
    .alphaToCoverageEnable = VK_FALSE                                                ,
    .alphaToOneEnable      = VK_FALSE                                                ,
  };

  auto vs = createShaderModule(vulkan.device,"shaders/vs.spv");
  auto fs = createShaderModule(vulkan.device,"shaders/fs.spv");

  VkPipelineShaderStageCreateInfo pssci[] = {
    VkPipelineShaderStageCreateInfo{
      .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext               = nullptr                                            ,
      .flags               = 0                                                  ,
      .stage               = VK_SHADER_STAGE_VERTEX_BIT                         ,
      .module              = vs                                                 ,
      .pName               = "main"                                             ,
      .pSpecializationInfo = nullptr                                            ,
    },
    VkPipelineShaderStageCreateInfo{
      .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext               = nullptr                                            ,
      .flags               = 0                                                  ,
      .stage               = VK_SHADER_STAGE_FRAGMENT_BIT                       ,
      .module              = fs                                                 ,
      .pName               = "main"                                             ,
      .pSpecializationInfo = nullptr                                            ,
    },
  };

  auto pvisci = VkPipelineVertexInputStateCreateInfo{
    .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext                           = nullptr                                                  ,
    .flags                           = 0                                                        ,
    .vertexBindingDescriptionCount   = 0                                                        ,
    .pVertexBindingDescriptions      = nullptr                                                  ,
    .vertexAttributeDescriptionCount = 0                                                        ,
    .pVertexAttributeDescriptions    = nullptr                                                  ,
  };

  auto piasci = VkPipelineInputAssemblyStateCreateInfo{
    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .pNext                  = nullptr                                                    ,
    .flags                  = 0                                                          ,
    .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST                        ,
    .primitiveRestartEnable = VK_FALSE                                                   ,
  };

  auto dslci = VkDescriptorSetLayoutCreateInfo{
    .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext        = nullptr                                            ,
    .flags        = 0                                                  ,
    .bindingCount = 0                                                  ,
    .pBindings    = nullptr                                            ,
  };

  VkDescriptorSetLayout renDescriptorSetLayout;

  VK_CALL(vkCreateDescriptorSetLayout,vulkan.device,&dslci,nullptr,&renDescriptorSetLayout);

  auto dsai = VkDescriptorSetAllocateInfo{
    .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext              = nullptr                                       ,
    .descriptorPool     = vulkan.descriptorPool                         ,
    .descriptorSetCount = 1                                             ,
    .pSetLayouts        = &renDescriptorSetLayout                       ,
  };
  VkDescriptorSet renDescriptorSet;
  VK_CALL(vkAllocateDescriptorSets,vulkan.device,&dsai,&renDescriptorSet);

  auto renPipelineLayout = createPipelineLayout(vulkan.device,1,&renDescriptorSetLayout);

  auto viewport = VkViewport{
    .x        = 0    ,
    .y        = 0    ,
    .width    = 64   ,
    .height   = 64   ,
    .minDepth =  0.0f,
    .maxDepth = +1.0f,
  };

  auto scissor = VkRect2D{
    .offset = {0 ,0 },
    .extent = {64,64},
  };

  auto pvsci = VkPipelineViewportStateCreateInfo{
    .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext         = nullptr                                              ,
    .flags         = 0                                                    ,
    .viewportCount = 1                                                    ,
    .pViewports    = &viewport                                            ,
    .scissorCount  = 1                                                    ,
    .pScissors     = &scissor                                             ,
  };

  auto ptsci = VkPipelineTessellationStateCreateInfo{
    .sType              = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
    .pNext              = nullptr                                                  ,
    .flags              = 0                                                        ,
    .patchControlPoints = 0                                                        ,
  };

  auto prsci = VkPipelineRasterizationStateCreateInfo{
    .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .pNext                   = nullptr                                                   ,
    .flags                   = 0                                                         ,
    .depthClampEnable        = VK_FALSE                                                  ,
    .rasterizerDiscardEnable = VK_FALSE                                                  ,
    .polygonMode             = VK_POLYGON_MODE_FILL                                      ,
    .cullMode                = VK_CULL_MODE_NONE                                         ,
    .frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE                           ,
    .depthBiasEnable         = VK_FALSE                                                  ,
    .depthBiasConstantFactor = 0.f                                                       ,
    .depthBiasClamp          = 0.f                                                       ,
    .depthBiasSlopeFactor    = 0.f                                                       ,
    .lineWidth               = 1.f                                                       ,
  };

  auto frontStencil = VkStencilOpState{
    .failOp      = VK_STENCIL_OP_KEEP  ,
    .passOp      = VK_STENCIL_OP_KEEP  ,
    .depthFailOp = VK_STENCIL_OP_KEEP  ,
    .compareOp   = VK_COMPARE_OP_ALWAYS,
    .compareMask = 0                   ,
    .writeMask   = 0                   ,
    .reference   = 0                   ,
  };

  auto backStencil = VkStencilOpState{
    .failOp      = VK_STENCIL_OP_KEEP  ,
    .passOp      = VK_STENCIL_OP_KEEP  ,
    .depthFailOp = VK_STENCIL_OP_KEEP  ,
    .compareOp   = VK_COMPARE_OP_ALWAYS,
    .compareMask = 0                   ,
    .writeMask   = 0                   ,
    .reference   = 0                   ,
  };

  auto pdssci = VkPipelineDepthStencilStateCreateInfo{
    .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
    .pNext                 = nullptr                                                   ,
    .flags                 = 0                                                         ,
    .depthTestEnable       = VK_FALSE                                                  ,
    .depthWriteEnable      = VK_FALSE                                                  ,
    .depthCompareOp        = VK_COMPARE_OP_LESS_OR_EQUAL                               ,
    .depthBoundsTestEnable = VK_FALSE                                                  ,
    .stencilTestEnable     = VK_FALSE                                                  ,
    .front                 = frontStencil                                              ,
    .back                  = backStencil                                               ,
  };

  VkPipelineColorBlendAttachmentState attachments[] = {
    VkPipelineColorBlendAttachmentState{
      .blendEnable         = VK_FALSE                                                                                           ,
      .srcColorBlendFactor = VK_BLEND_FACTOR_ONE                                                                                ,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE                                                                                ,
      .colorBlendOp        = VK_BLEND_OP_ADD                                                                                    ,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE                                                                                ,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE                                                                                ,
      .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT,
    },
  };
  auto pcbsci = VkPipelineColorBlendStateCreateInfo{
    .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .pNext           = nullptr                                                 ,
    .flags           = 0                                                       ,
    .logicOpEnable   = VK_FALSE                                                ,
    .logicOp         = VK_LOGIC_OP_CLEAR                                       ,
    .attachmentCount = sizeof(attachments)/sizeof(attachments[0])              ,
    .pAttachments    = attachments                                             ,
  };

  auto graphicsPipelineCreateInfo     = VkGraphicsPipelineCreateInfo{
    .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .pNext               = nullptr                                        ,
    .flags               = VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT    ,
    .stageCount          = sizeof(pssci)/sizeof(pssci[0])                 ,
    .pStages             = pssci                                          ,
    .pVertexInputState   = &pvisci                                        ,
    .pInputAssemblyState = &piasci                                        ,
    .pTessellationState  = &ptsci                                         ,
    .pViewportState      = &pvsci                                         ,
    .pRasterizationState = &prsci                                         ,
    .pMultisampleState   = &pmsscf                                        ,
    .pDepthStencilState  = &pdssci                                        ,
    .pColorBlendState    = &pcbsci                                        ,
    .layout              = renPipelineLayout                              ,
    .renderPass          = renderPass                                     ,
    .subpass             = 0                                              ,
    .basePipelineHandle  = nullptr                                        ,
    .basePipelineIndex   = 0                                              ,
  };
  VkPipeline renPipeline;
  VK_CALL(vkCreateGraphicsPipelines,vulkan.device,nullptr,1,&graphicsPipelineCreateInfo,nullptr,&renPipeline);


  auto imb = VkImageMemoryBarrier{
    .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext               = nullptr                               ,
    .srcAccessMask       = 0                                     ,
    .dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT  ,
    .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED             ,
    .newLayout           = VK_IMAGE_LAYOUT_GENERAL               ,
    .srcQueueFamilyIndex = vulkan.queueFamilyIndex               ,
    .dstQueueFamilyIndex = vulkan.queueFamilyIndex               ,
    .image               = image                                 ,
    .subresourceRange    = isr                                   ,
  };

  auto crr = VkRect2D{
    .offset = {0 ,0 },
    .extent = {64,64},
  };
  auto cr = VkClearRect{
    .rect           = crr,
    .baseArrayLayer = 0  ,
    .layerCount     = 1  ,
  };

  auto ccv = VkClearColorValue{
    .float32 = {0.f,0.f,0.f,0.f},
  };
  auto cv = VkClearValue{
    .color = ccv,
  };
  auto ca = VkClearAttachment{
    .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
    .colorAttachment = 0                        ,
    .clearValue      = cv                       ,
  };


  begin(commandBuffer);
  vkCmdBindPipeline      (commandBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,renPipeline);
  vkCmdBindDescriptorSets(commandBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,renPipelineLayout,0,1,&renDescriptorSet,0,nullptr);
  vkCmdPipelineBarrier   (commandBuffer,VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,0,0,nullptr,0,nullptr,1,&imb);
  vkCmdBeginRenderPass   (commandBuffer,&rpbi,VK_SUBPASS_CONTENTS_INLINE);
  //vkCmdClearAttachments  (commandBuffer,1,&ca,1,&cr);
  vkCmdDraw              (commandBuffer,3,1,0,0);
  vkCmdEndRenderPass(commandBuffer);
  end(commandBuffer);

  auto p = (uint32_t*)vulkan.devMem.map();
  for(uint32_t i=0;i<1024*16*sizeof(uint32_t);++i)
    p[i] = 0;
  vulkan.devMem.flush(0,VK_WHOLE_SIZE);
  vulkan.devMem.unmap();

  submit(vulkan,commandBuffer);
  vkQueueWaitIdle(vulkan.queue);

  auto ptr  = (uint8_t*)vulkan.devMem.map();
  vulkan.devMem.invalidate(0,VK_WHOLE_SIZE);
  for(int y=0;y<64;++y){
    for(int x=0;x<64;++x){
      int r = (int)((((float)ptr[(y*64+x)*4+0])/255.f)*9.f);
      fprintf(stderr,"%c",'0'+r);
    }
    fprintf(stderr,"\n");
  }
  vulkan.devMem.unmap();

  vkDestroyImageView          (vulkan.device,imageView             ,nullptr);
  vkDestroyImage              (vulkan.device,image                 ,nullptr);
  vkDestroyDescriptorSetLayout(vulkan.device,renDescriptorSetLayout,nullptr);
  vkDestroyPipelineLayout     (vulkan.device,renPipelineLayout     ,nullptr);
  vkDestroyShaderModule       (vulkan.device,vs                    ,nullptr);
  vkDestroyShaderModule       (vulkan.device,fs                    ,nullptr);
  vkDestroyPipeline           (vulkan.device,renPipeline           ,nullptr);
  vkDestroyFramebuffer        (vulkan.device,framebuffer           ,nullptr);
  vkDestroyRenderPass         (vulkan.device,renderPass            ,nullptr);

}


Instance createInstance(){
  char const* layers[] = {
    "VK_LAYER_KHRONOS_validation",
  };

  auto applicationInfo = VkApplicationInfo{
    .apiVersion = VK_MAKE_API_VERSION(0,1,3,0),
  };
  auto instanceCreateInfo = VkInstanceCreateInfo{
    .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext                   = nullptr                               ,
    .flags                   = 0                                     ,
    .pApplicationInfo        = &applicationInfo                      ,
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
  };

  VkDescriptorSetLayoutCreateInfo dslci[] = {
    descriptorSetLayoutCreateInfo(sizeof(dslb1)/sizeof(dslb1[0]),dslb1  ),
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
  delete[]devices;
}

void Instance::initPhysicalDevices(VkPhysicalDevice*devs,uint32_t n){
  for(uint32_t i=0;i<nofPhysicalDevices;++i)
    new(physicalDevices+i)PhysicalDevice(devs[i]);
}

void Instance::deletePhysicalDevices(){
  delete[]physicalDevices;
}
