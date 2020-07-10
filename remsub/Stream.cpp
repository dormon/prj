#include <Stream.hpp>

#include <geGL/geGL.h>

#if defined(USE_OPENCV)
#include <video.hpp>
#else
#include <CachedVideo.hpp>
#endif

Stream::Stream(std::string const&name):file(name){
#if defined(USE_OPENCV)
  video = std::make_shared<Video>(name.c_str());
#else
  video = std::make_shared<cachedVideo::Video>(name.c_str());
#endif
  tex = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGB8UI,1,video->width,video->height);
  tex->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  tex->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  //tex->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  //tex->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  nofFrames = video->nofFrames;
  end = video->nofFrames;
  fps = video->fps;
  //keys.set<int>(0,"offset",0);
  //keys.set<float>(0,"contrast",1);
}

float Stream::getOffset(int f)const{
  return keys.get<float>(f,"offset",0);
}
float Stream::getContrast(int f)const{
  return keys.get<float>(f,"contrast",1);
}
glm::vec4 Stream::getOffsetScale(int f)const{
  return keys.get<glm::vec4>(f,"offsetScale",glm::vec4(0.f,0.f,1.f,1.f));
}
