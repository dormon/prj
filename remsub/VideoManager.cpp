#include <iomanip>

#include <VideoManager.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void VideoManager::saveFrame(int frame,int w,int h,std::vector<uint8_t>&data){
  std::stringstream ss;
  ss << outputName << std::setfill('0') << std::setw(7) << frame << "."<<outputExtension;
  if(outputExtension == "jpg"){
    stbi_write_jpg(ss.str().c_str(),w,h,3,data.data(),100);
  }
  if(outputExtension == "png"){
    stbi_write_png(ss.str().c_str(),w,h,3,data.data(),3);
  }
}
