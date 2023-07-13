#ifndef _VKHUMAN_HPP_
#define _VKHUMAN_HPP_
#pragma once

#include<cstdint>

namespace vkh{

struct Result{
  int32_t v;
  Result(uint32_t v):v(v){}
  bool operator==(Result const&o)const{return v==o.v;}
};

namespace result{
  auto const success                 = Result(0);
  auto const notReady                = Result(1);
  auto const timeOut                 = Result(2);
  namespace event{
    auto const set                   = Result(3);
    auto const reset                 = Result(4);
  };
  auto const incomplete              = Result(5);
  namespace error{
    namespace out_of{
      auto const host_memory         = Result(-1);
      auto const device_memory       = Result(-2);
      auto const poolMemory          = Result(-1000069000);
      auto const dateKHR             = Result(-1000001004);
    };
    auto const initialization_failed = Result(-3);
    auto const device_lost           = Result(-4);
//    VK_ERROR_MEMORY_MAP_FAILED = -5,
//    VK_ERROR_LAYER_NOT_PRESENT = -6,
//    VK_ERROR_EXTENSION_NOT_PRESENT = -7,
//    VK_ERROR_FEATURE_NOT_PRESENT = -8,
//    VK_ERROR_INCOMPATIBLE_DRIVER = -9,
//    VK_ERROR_TOO_MANY_OBJECTS = -10,
//    VK_ERROR_FORMAT_NOT_SUPPORTED = -11,
//    VK_ERROR_FRAGMENTED_POOL = -12,
//    VK_ERROR_UNKNOWN = -13,
//    VK_ERROR_INVALID_EXTERNAL_HANDLE = -1000072003,
//    VK_ERROR_FRAGMENTATION = -1000161000,
//    VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS = -1000257000,
//    VK_ERROR_SURFACE_LOST_KHR = -1000000000,
//    VK_ERROR_NATIVE_WINDOW_IN_USE_KHR = -1000000001,
//    VK_ERROR_INCOMPATIBLE_DISPLAY_KHR = -1000003001,
//    VK_ERROR_VALIDATION_FAILED_EXT = -1000011001,
//    VK_ERROR_INVALID_SHADER_NV = -1000012000,
//    VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR = -1000023000,
//    VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR = -1000023001,
//    VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR = -1000023002,
//    VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR = -1000023003,
//    VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR = -1000023004,
//    VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR = -1000023005,
//    VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT = -1000158000,
//    VK_ERROR_NOT_PERMITTED_KHR = -1000174001,
//    VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT = -1000255000,
//    VK_ERROR_COMPRESSION_EXHAUSTED_EXT = -1000338000,
//    VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR = VK_ERROR_INVALID_EXTERNAL_HANDLE,
//    VK_ERROR_FRAGMENTATION_EXT = VK_ERROR_FRAGMENTATION,
//    VK_ERROR_NOT_PERMITTED_EXT = VK_ERROR_NOT_PERMITTED_KHR,
//    VK_ERROR_INVALID_DEVICE_ADDRESS_EXT = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
//    VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
//    VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT = VK_PIPELINE_COMPILE_REQUIRED,
  };

//    VK_THREAD_IDLE_KHR = 1000268000,
//    VK_THREAD_DONE_KHR = 1000268001,
//    VK_OPERATION_DEFERRED_KHR = 1000268002,
//    VK_OPERATION_NOT_DEFERRED_KHR = 1000268003,
//    VK_PIPELINE_COMPILE_REQUIRED_EXT = VK_PIPELINE_COMPILE_REQUIRED,


};

inline char const* str(Result const&t){
  if(t==result::success)return "VK_SUCCESS";
  return "";
}

//std::ostream& operator<<(std::ostream& os, Result const& obj){
//  os << obj.v;
//  return os;
//}
//
//void a(){
//  std::cerr << result::event::set << std::endl;
//}

}

#endif//_VKHUMAN_HPP_
