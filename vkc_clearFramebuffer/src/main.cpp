#include<exception>
#include<stdio.h>

#include<vulkan/vulkan.h>
#include<vulkan/vk_enum_string_helper.h>

#include<main.hpp>

int main(int argc,char*argv[]){
  try{
    gpu_task(argc,argv);
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
  Vulkan vk;
  vk.instance           = createInstance       ();
  vk.nofPhysicalDevices = getNofPhysicalDevices(vk.instance);
  vk.physicalDevices    = getPhysicalDevices   (vk.instance,vk.nofPhysicalDevices);
  vk.physicalDevice     = getPhysicalDevice    (vk.instance);
  vk.queueFamilyIndex   = getQueueFamilyIndex  (vk.physicalDevice,VK_QUEUE_GRAPHICS_BIT|VK_QUEUE_TRANSFER_BIT);
  vk.memoryTypeIndex    = getMemoryTypeIndex   (vk.physicalDevice,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  vk.device             = createDevice         (vk.physicalDevice,vk.queueFamilyIndex);
  vk.deviceMemory       = allocateMemory       (vk.device,1024*1024*128,vk.memoryTypeIndex);
  vk.queue              = getDeviceQueue       (vk.device,vk.queueFamilyIndex);
  vk.commandPool        = createCommandPool    (vk.device,vk.queueFamilyIndex);
  vk.renderPass         = createRenderPass     (vk.device);
  vk.image              = createImage          (vk.device,vk.queueFamilyIndex);

  VK_CALL(vkBindImageMemory,vk.device,vk.image,vk.deviceMemory,0);

  vk.imageView          = createImageView      (vk.device,vk.image);
  vk.framebuffer        = createFramebuffer    (vk.device,vk.renderPass,vk.imageView);
  return vk;
}

void work(Vulkan&vk){
  auto commandBuffer = allocateCommandBuffer(vk.device,vk.commandPool);

  begin          (commandBuffer);
  barrier        (commandBuffer,vk.queueFamilyIndex,vk.image);
  beginRenderPass(commandBuffer,vk.renderPass,vk.framebuffer);
  clear          (commandBuffer);
  endRenderPass  (commandBuffer);
  end            (commandBuffer);

  submit(vk.queue,commandBuffer);
  vkQueueWaitIdle(vk.queue);

  printMemory(vk);
}

void deinit(Vulkan&vk){
  vkDestroyImageView  (vk.device,vk.imageView   ,nullptr);
  vkDestroyImage      (vk.device,vk.image       ,nullptr);
  vkDestroyFramebuffer(vk.device,vk.framebuffer ,nullptr);
  vkDestroyRenderPass (vk.device,vk.renderPass  ,nullptr);
  vkDestroyCommandPool(vk.device,vk.commandPool ,nullptr);
  vkFreeMemory        (vk.device,vk.deviceMemory,nullptr);
  vkDestroyDevice     (vk.device                ,nullptr);
  vkDestroyInstance   (vk.instance              ,nullptr);
}




VkInstance createInstance(){
  char const* layers[] = {
    "VK_LAYER_KHRONOS_validation",
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

  VkInstance res;
  VK_CALL(vkCreateInstance,&instanceCreateInfo,nullptr,&res);
  return res;
}

uint32_t getNofPhysicalDevices(VkInstance instance){
  uint32_t n = 0;
  vkEnumeratePhysicalDevices(instance,&n,nullptr);
  return n;
}

VkPhysicalDevice*getPhysicalDevices(VkInstance instance,uint32_t n){
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
    if(
        properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ||
        properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU   ){
      delete[]devices;
      return device;
    }
  }
  VK_ERROR("getPhysicalDevice cannot locate descrete GPU");
  return nullptr;
}

uint32_t getNofQueueFamilyProperties(VkPhysicalDevice dev){
  uint32_t count;
  vkGetPhysicalDeviceQueueFamilyProperties(dev,&count,nullptr);
  return count;
}



uint32_t getQueueFamilyIndex(VkPhysicalDevice dev,VkQueueFlags req){
  uint32_t count;
  auto props = getQueueFamilyProperties(dev,&count);
  auto index = loopQueuesAndFindIndex(count,props,req);
  throwErrorIfQueueIndexOutOfRange(index,count);
  delete[]props;
  return index;
}

VkQueueFamilyProperties*getQueueFamilyProperties(VkPhysicalDevice dev,uint32_t*count){
  *count = getNofQueueFamilyProperties(dev);
  auto props = new VkQueueFamilyProperties[*count];
  vkGetPhysicalDeviceQueueFamilyProperties(dev,count,props);
  return props;
}

uint32_t loopQueuesAndFindIndex(uint32_t count,VkQueueFamilyProperties*props,VkQueueFlags req){
  uint32_t index=0;
  for(;index < count;++index){
    if(isQueueOK(props[index],req))break;
    ++index;
  }
  return index;
}

void throwErrorIfQueueIndexOutOfRange(uint32_t index,uint32_t count){
  if(index < count)return;
  VK_ERROR("cannot find queue family");
}

bool isQueueOK(VkQueueFamilyProperties prop,VkQueueFlags req){
  return prop.queueCount >= 1 && (prop.queueFlags & req) == req;
}

uint32_t getMemoryTypeIndex(VkPhysicalDevice dev,VkMemoryPropertyFlags req){
  VkPhysicalDeviceMemoryProperties props;
  vkGetPhysicalDeviceMemoryProperties(dev,&props);

  uint32_t index=0;
  for(auto const&prop:props.memoryTypes){
    if((prop.propertyFlags & req) == req)break;
    ++index;
  }
  if(index >= props.memoryTypeCount)VK_ERROR("cannot find memory type");
  return index;
}

VkDevice createDevice(VkPhysicalDevice physicalDevice,uint32_t queueFamilyIndex){
  float priorities[] = {1.f};
  VkDeviceQueueCreateInfo infos[] = {
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
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext                   = nullptr                             ,
    .flags                   = 0                                   ,
    .queueCreateInfoCount    = sizeof(infos)/sizeof(infos[0])      ,
    .pQueueCreateInfos       = infos                               ,
    .enabledLayerCount       = 0                                   ,
    .ppEnabledLayerNames     = nullptr                             ,
    .enabledExtensionCount   = 0                                   ,
    .ppEnabledExtensionNames = nullptr                             ,
    .pEnabledFeatures        = nullptr                             ,

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

VkDeviceMemory allocateMemory(VkDevice device,size_t size,uint32_t memoryTypeIndex){
  auto info = VkMemoryAllocateInfo{
    .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext           = nullptr                               ,
    .allocationSize  = size                                  ,
    .memoryTypeIndex = memoryTypeIndex                       ,
  };
  VkDeviceMemory memory;
  VK_CALL(vkAllocateMemory,device,&info,nullptr,&memory);
  return memory;
}


VkCommandPool createCommandPool(VkDevice device,uint32_t queueFamilyIndex){
  auto info = VkCommandPoolCreateInfo{
    .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext            = nullptr                                   ,
    .flags            = 0                                         ,
    .queueFamilyIndex = queueFamilyIndex                          ,
  };
  VkCommandPool pool;
  VK_CALL(vkCreateCommandPool,device,&info,nullptr,&pool);
  return pool;
}



VkRenderPass createRenderPass(VkDevice device){
  auto ar = VkAttachmentReference{
    .attachment = 0                      ,
    .layout     = VK_IMAGE_LAYOUT_GENERAL,
  };

  VkSubpassDescription sds[] = {
    VkSubpassDescription{
      .flags                   = 0                              ,
      .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount    = 0                              ,
      .pInputAttachments       = nullptr                        ,
      .colorAttachmentCount    = 1                              ,
      .pColorAttachments       = &ar                            ,
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
  VkRenderPass pass;
  VK_CALL(vkCreateRenderPass,device,&rpci,nullptr,&pass);
  return pass;
}

VkImage createImage(VkDevice device,uint32_t queueFamilyIndex){
  uint32_t queues[]={
    queueFamilyIndex,
  };
  auto ici = VkImageCreateInfo{
    .sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .pNext                 = nullptr                            ,
    .flags                 = 0                                  ,
    .imageType             = VK_IMAGE_TYPE_2D                   ,
    .format                = VK_FORMAT_R8G8B8A8_UINT            ,
    .extent                = {64,64,1}                          ,
    .mipLevels             = 1                                  ,
    .arrayLayers           = 1                                  ,
    .samples               = VK_SAMPLE_COUNT_1_BIT              ,
    .tiling                = VK_IMAGE_TILING_LINEAR             ,
    .usage                 = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .sharingMode           = VK_SHARING_MODE_EXCLUSIVE          ,
    .queueFamilyIndexCount = sizeof(queues)/sizeof(queues[0])   ,
    .pQueueFamilyIndices   = queues                             ,
  };
  VkImage image;
  VK_CALL(vkCreateImage,device,&ici,nullptr,&image);
  return image;
}

VkImageView createImageView(VkDevice device,VkImage image){
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
  VK_CALL(vkCreateImageView,device,&ivci,nullptr,&imageView);
  return imageView;
}

VkFramebuffer createFramebuffer(VkDevice device,VkRenderPass renderPass,VkImageView imageView){
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
  VK_CALL(vkCreateFramebuffer,device,&fci,nullptr,&framebuffer);
  return framebuffer;
}

void clear(VkCommandBuffer commandBuffer){
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
    .float32 = {1.f,1.f,1.f,1.f},
  };
  auto cv = VkClearValue{
    .color = ccv,
  };
  auto ca = VkClearAttachment{
    .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
    .colorAttachment = 0                        ,
    .clearValue      = cv                       ,
  };
  vkCmdClearAttachments(commandBuffer,1,&ca,1,&cr);
}

void beginRenderPass(VkCommandBuffer commandBuffer,VkRenderPass renderPass,VkFramebuffer framebuffer){
  auto rpbi = VkRenderPassBeginInfo{
    .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext           = nullptr                                 ,
    .renderPass      = renderPass                              ,
    .framebuffer     = framebuffer                             ,
    .renderArea      = VkRect2D{.offset{0,0},.extent{64,64}}   ,
    .clearValueCount = 0                                       ,
    .pClearValues    = nullptr                                 ,
  };
  vkCmdBeginRenderPass(commandBuffer,&rpbi,VK_SUBPASS_CONTENTS_INLINE);
}

void endRenderPass(VkCommandBuffer commandBuffer){
  vkCmdEndRenderPass(commandBuffer);
}

void printMemory(Vulkan&vulkan){
  auto ptr  = (uint8_t*)map(vulkan.device,vulkan.deviceMemory,0,VK_WHOLE_SIZE);
  invalidate(vulkan.device,vulkan.deviceMemory,0,VK_WHOLE_SIZE);
  for(int y=0;y<64;++y){
    for(int x=0;x<64;++x){
      int r = (int)((((float)ptr[(y*64+x)*4+0])/255.f)*9.f);
      if(r == 0)fprintf(stderr,".");
      else      fprintf(stderr,"%i",r);
    }
    fprintf(stderr,"\n");
  }
  unmap(vulkan.device,vulkan.deviceMemory);
}

void barrier(VkCommandBuffer commandBuffer,uint32_t queueFamilyIndex,VkImage image){
  auto isr = VkImageSubresourceRange{
    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
    .baseMipLevel   = 0                        ,
    .levelCount     = 1                        ,
    .baseArrayLayer = 0                        ,
    .layerCount     = 1                        ,
  };

  auto imb = VkImageMemoryBarrier{
    .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext               = nullptr                               ,
    .srcAccessMask       = 0                                     ,
    .dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT  ,
    .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED             ,
    .newLayout           = VK_IMAGE_LAYOUT_GENERAL               ,
    .srcQueueFamilyIndex = queueFamilyIndex                      ,
    .dstQueueFamilyIndex = queueFamilyIndex                      ,
    .image               = image                                 ,
    .subresourceRange    = isr                                   ,
  };
  vkCmdPipelineBarrier   (commandBuffer,VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,0,0,nullptr,0,nullptr,1,&imb);
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

void submit(VkQueue queue,VkCommandBuffer commandBuffer){
  auto info = VkSubmitInfo{
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
  VK_CALL(vkQueueSubmit,queue,1,&info,VK_NULL_HANDLE);
}

VkCommandBuffer allocateCommandBuffer(VkDevice device,VkCommandPool pool){
  auto info = VkCommandBufferAllocateInfo{
    .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext              = nullptr                                       ,
    .commandPool        = pool                                          ,
    .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY               ,
    .commandBufferCount = 1                                             ,
  };
  VkCommandBuffer commandBuffer;
  VK_CALL(vkAllocateCommandBuffers,device,&info,&commandBuffer);
  return commandBuffer;
}

void bind(VkDevice device,VkBuffer buffer,VkDeviceMemory memory,size_t offset){
  VK_CALL(vkBindBufferMemory,device,buffer,memory,offset);
}

void*map(VkDevice device,VkDeviceMemory memory,size_t offset,size_t size){
  void*readBuffer = nullptr;
  VK_CALL(vkMapMemory,device,memory,offset,size,0,&readBuffer);
  return readBuffer;
}

void unmap(VkDevice device,VkDeviceMemory memory){
  vkUnmapMemory(device,memory);
}

void invalidate(VkDevice device,VkDeviceMemory memory,size_t offset,size_t size){
  auto range = VkMappedMemoryRange{
    .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext  = nullptr                              ,
    .memory = memory                               ,
    .offset = 0                                    ,
    .size   = VK_WHOLE_SIZE                        ,
  };
  VK_CALL(vkInvalidateMappedMemoryRanges,device,1,&range);
}

Error::Error(char const*file,int line,char const*fce,VkResult r,char const*m):
  file(file),
  line(line),
  fceName(fce),
  result(r),
  msg(m){}
void Error::print()const{
  fprintf(stderr,"ERROR:\n");
  if(file   )fprintf(stderr,"  FILE    : %s\n",file                   );
  if(line>=0)fprintf(stderr,"  LINE    : %i\n",line                   );
  if(fceName)fprintf(stderr,"  FCE     : %s\n",fceName                );
  if(msg    )fprintf(stderr,"  MSG     : %s\n",msg                    );
  if(true   )fprintf(stderr,"  VkResult: %s\n",string_VkResult(result));
  fprintf(stderr,"\n");
}
