#pragma once

#include<memory>

#include<glm/glm.hpp>

#include <geGL/Fwd.h>

#include<keyframes.hpp>

#if defined(USE_OPENCV)
class Video;
using FwdVideo = Video;
#else
namespace cachedVideo{
  class Video;
}
using FwdVideo = cachedVideo::Video;
#endif


struct Stream{
  float   fps         = 25.f     ;
  float   offset      = 0        ;
  int32_t start       = 0        ;
  int32_t end         = 0        ;
  int32_t nofFrames   = 0        ;
  int32_t loadedFrame = -10000000;
  std::shared_ptr<FwdVideo>video;
  std::shared_ptr<ge::gl::Texture>tex;
  std::string file;
  keyframe::Keyframes keys;
  Stream(std::string const&name);
 
  float getOffset(int f)const;
  float getContrast(int f)const;
  glm::vec4 getOffsetScale(int f)const;
};
