#include"createShaderModule.hpp"
#include<stdio.h>

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