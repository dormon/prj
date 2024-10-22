#include<createBuffer.hpp>

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
