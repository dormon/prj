#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<vulkan/vulkan.h>
#include"vulkanStrings.h"



int main(){
  VkResult result;

  uint32_t          pPropertyCount = 0;
  VkLayerProperties*pProperties    = nullptr;
  result = vkEnumerateInstanceLayerProperties(&pPropertyCount,nullptr);
  if(result==VK_SUCCESS){
    pProperties = new VkLayerProperties[pPropertyCount];
    result = vkEnumerateInstanceLayerProperties(&pPropertyCount,pProperties);
    if(result==VK_SUCCESS){
      for(uint32_t p=0;p<pPropertyCount;++p){
        std::cout<<"layerName: "            <<pProperties[p].layerName            <<std::endl;
        std::cout<<"specVersion: "          <<pProperties[p].specVersion          <<std::endl;
        std::cout<<"implementationVersion: "<<pProperties[p].implementationVersion<<std::endl;
        std::cout<<"description: "          <<pProperties[p].description          <<std::endl;
        uint32_t pExtensionCount = 0;
        result = vkEnumerateInstanceExtensionProperties(pProperties[p].layerName,&pExtensionCount,nullptr);
        if(result == VK_SUCCESS){
          VkExtensionProperties*pExtensions = new VkExtensionProperties[pExtensionCount];
          result = vkEnumerateInstanceExtensionProperties(pProperties[p].layerName,&pExtensionCount,pExtensions);
          for(uint32_t e=0;e<pExtensionCount;++e){
            std::cout<<"  extensionName: "<<pExtensions[e].extensionName<<std::endl;
            std::cout<<"  specVersion: "<<pExtensions[e].specVersion<<std::endl;
          }

        }else std::cout<<"ERROR: vkEnumerateInstanceExtensionProperties: "<<translate(result)<<std::endl;
      }
    }else std::cout<<"ERROR: vkEnumerateInstanceLayerProperties: "<<translate(result)<<std::endl;
  }else std::cerr<<"ERROR: vkEnumerateInstanceLayerProperties: "<<translate(result)<<std::endl;

  VkApplicationInfo vkApplicationInfo;
  vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  vkApplicationInfo.pNext = nullptr;
  vkApplicationInfo.pApplicationName = "vulkanTest";
  vkApplicationInfo.applicationVersion = 1;
  vkApplicationInfo.pEngineName = "ge";
  vkApplicationInfo.engineVersion = 1;
  vkApplicationInfo.apiVersion = 1;

  VkInstanceCreateInfo vkInstanceCreateInfo;
  vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  vkInstanceCreateInfo.pNext = nullptr;
  vkInstanceCreateInfo.flags = 0;
  vkInstanceCreateInfo.pApplicationInfo = &vkApplicationInfo;
  vkInstanceCreateInfo.enabledLayerCount = 0;
  vkInstanceCreateInfo.ppEnabledLayerNames = nullptr;
  vkInstanceCreateInfo.enabledExtensionCount = 0;
  vkInstanceCreateInfo.ppEnabledExtensionNames = nullptr;

  VkInstance vkInstance;
  result=vkCreateInstance(&vkInstanceCreateInfo,nullptr,&vkInstance);
  std::cout<<translate(result)<<std::endl;

  if(result!=VK_SUCCESS)return 0;
  vkDestroyInstance(vkInstance,nullptr);
  return 0;
}
