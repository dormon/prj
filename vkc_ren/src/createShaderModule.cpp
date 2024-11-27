#include<createShaderModule.hpp>
#include<error.hpp>
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

  auto info = VkShaderModuleCreateInfo{
    .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext    = nullptr                                    ,
    .flags    = 0                                          ,
    .codeSize = size                                       ,
    .pCode    = code                                       ,
  };

  VkShaderModule module;
  VK_CALL(vkCreateShaderModule,device,&info,nullptr,&module);
  delete[]code;
  return module;
}
