#include <devMem.hpp>
#include <error.hpp>

void DevMem::bind(VkBuffer buffer,size_t offset){
  VK_CALL(vkBindBufferMemory,device,buffer,memory,offset);
}

void*DevMem::map(size_t offset,size_t size){
  void*readBuffer = nullptr;
  auto result = vkMapMemory(device,memory,offset,size,0,&readBuffer);
  if(result != VK_SUCCESS)throw "cannot map memory";
  return readBuffer;
}

void DevMem::unmap(){
  vkUnmapMemory(device,memory);
}

void DevMem::flush(size_t offset,size_t size){
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

void DevMem::invalidate(size_t offset,size_t size){
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
