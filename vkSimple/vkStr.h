#pragma once

#include <iostream>
#include <sstream>
#include <vulkan/vulkan.h>

constexpr char const* vkResult2String(VkResult err){
#define CASE(NAME) case VK_##NAME:return "VK_"#NAME
  switch(err){
    CASE(SUCCESS                                           );
    CASE(NOT_READY                                         );
    CASE(TIMEOUT                                           );
    CASE(EVENT_SET                                         );
    CASE(EVENT_RESET                                       );
    CASE(INCOMPLETE                                        );
    CASE(ERROR_OUT_OF_HOST_MEMORY                          );
    CASE(ERROR_OUT_OF_DEVICE_MEMORY                        );
    CASE(ERROR_INITIALIZATION_FAILED                       );
    CASE(ERROR_DEVICE_LOST                                 );
    CASE(ERROR_MEMORY_MAP_FAILED                           );
    CASE(ERROR_LAYER_NOT_PRESENT                           );
    CASE(ERROR_EXTENSION_NOT_PRESENT                       );
    CASE(ERROR_FEATURE_NOT_PRESENT                         );
    CASE(ERROR_INCOMPATIBLE_DRIVER                         );
    CASE(ERROR_TOO_MANY_OBJECTS                            );
    CASE(ERROR_FORMAT_NOT_SUPPORTED                        );
    CASE(ERROR_FRAGMENTED_POOL                             );
    CASE(ERROR_OUT_OF_POOL_MEMORY                          );
    CASE(ERROR_INVALID_EXTERNAL_HANDLE                     );
    CASE(ERROR_SURFACE_LOST_KHR                            );
    CASE(ERROR_NATIVE_WINDOW_IN_USE_KHR                    );
    CASE(SUBOPTIMAL_KHR                                    );
    CASE(ERROR_OUT_OF_DATE_KHR                             );
    CASE(ERROR_INCOMPATIBLE_DISPLAY_KHR                    );
    CASE(ERROR_VALIDATION_FAILED_EXT                       );
    CASE(ERROR_INVALID_SHADER_NV                           );
    CASE(ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
    CASE(ERROR_FRAGMENTATION_EXT                           );
    CASE(ERROR_NOT_PERMITTED_EXT                           );
    CASE(ERROR_INVALID_DEVICE_ADDRESS_EXT                  );
    CASE(ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT         );
    CASE(RESULT_RANGE_SIZE                                 );
    CASE(RESULT_MAX_ENUM                                   );
    //CASE(ERROR_OUT_OF_POOL_MEMORY_KHR);
    //CASE(ERROR_INVALID_EXTERNAL_HANDLE_KHR);
    //CASE(RESULT_BEGIN_RANGE);
    //CASE(RESULT_END_RANGE);
  }
#undef CASE
  return "";
}

std::string vkQueueFlags2String(VkQueueFlags f){
  std::stringstream ss;
#define IFF(NAME) if(f&VK_##NAME)ss << "VK_"#NAME " "
  IFF(QUEUE_GRAPHICS_BIT      );
  IFF(QUEUE_COMPUTE_BIT       );
  IFF(QUEUE_TRANSFER_BIT      );
  IFF(QUEUE_SPARSE_BINDING_BIT);
  IFF(QUEUE_PROTECTED_BIT     );
  IFF(QUEUE_FLAG_BITS_MAX_ENUM);
#undef IFF
  return ss.str();
}

constexpr char const*vkPhysicalDeviceType2String(VkPhysicalDeviceType f){
#define CASE(NAME) case VK_##NAME:return "VK_"#NAME
  switch(f){
    CASE(PHYSICAL_DEVICE_TYPE_OTHER         );
    CASE(PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
    CASE(PHYSICAL_DEVICE_TYPE_DISCRETE_GPU  );
    CASE(PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU   );
    CASE(PHYSICAL_DEVICE_TYPE_CPU           );
    CASE(PHYSICAL_DEVICE_TYPE_MAX_ENUM      );
    CASE(PHYSICAL_DEVICE_TYPE_RANGE_SIZE    );
  }
#undef CASE
  return "";
}
