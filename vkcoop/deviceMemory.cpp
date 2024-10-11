#include <deviceMemory.hpp>

void*DeviceMemory::map(size_t offset,size_t size){
  void*readBuffer = nullptr;
  auto result = vkMapMemory(device,memory,offset,size,0,&readBuffer);
  if(result != VK_SUCCESS)throw "cannot map memory";
  return readBuffer;
}

void DeviceMemory::unmap(){
  vkUnmapMemory(device,memory);
}

void DeviceMemory::flush(size_t offset,size_t size){
  auto mappedMemoryRange = VkMappedMemoryRange{
    .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext  = nullptr                              ,
    .memory = memory                               ,
    .offset = offset                               ,
    .size   = size                                 ,
  };
  auto result = vkFlushMappedMemoryRanges(device,1,&mappedMemoryRange);
  if(result != VK_SUCCESS)throw "cannot flush memory";
}

void DeviceMemory::invalidate(size_t offset,size_t size){
  auto mappedMemoryRange = VkMappedMemoryRange{
    .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext  = nullptr                              ,
    .memory = memory                               ,
    .offset = 0                                    ,
    .size   = VK_WHOLE_SIZE                        ,
  };
  auto result = vkInvalidateMappedMemoryRanges(device,1,&mappedMemoryRange);
  if(result != VK_SUCCESS)throw "cannot invalidate memory";
}
