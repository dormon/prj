#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

//extern "C" is because undefined references WTF is this
extern "C" { 
#include <libavdevice/avdevice.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext_vdpau.h>
#include <libavcodec/vdpau.h>
}

#define ___ std::cerr << __FILE__ << "/" << __LINE__ << std::endl

void saveFrame(int frame,int w,int h,std::vector<uint8_t>&data){
  std::stringstream ss;
  std::string outputName = "test";
  std::string outputExtension = "jpg";
  ss << outputName << std::setfill('0') << std::setw(7) << frame << "."<<outputExtension;
  if(outputExtension == "jpg"){
    stbi_write_jpg(ss.str().c_str(),w,h,3,data.data(),100);
  }
  if(outputExtension == "png"){
    stbi_write_png(ss.str().c_str(),w,h,3,data.data(),3);
  }
}

int main(int argc,char*argv[]){
  std::string videoFile = "/media/papobackup/backup/video/허준/hur-jun-episode-2.mp4";
  auto formatContext = avformat_alloc_context();

  if(!formatContext)
    throw std::runtime_error("Cannot allocate format context memory");

  if(avformat_open_input(&formatContext, videoFile.c_str(), NULL, NULL) != 0)
    throw std::runtime_error("Cannot open the video file"); 

  if(avformat_find_stream_info(formatContext, NULL) < 0)
    throw std::runtime_error("Cannot get the stream info");


  AVCodec *codec;
  auto videoStreamId = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &codec, 0);
  if(videoStreamId < 0)
      throw std::runtime_error("No video stream available");

  if(!codec)
      throw std::runtime_error("No suitable codec found");

  auto codecContext = avcodec_alloc_context3(codec);
  if(!codecContext)
      throw std::runtime_error("Cannot allocate codec context memory");

  AVPixelFormat pixFmt;
  const AVCodecHWConfig *config;
  for(int i=0;; i++)
  {
      if(!(config = avcodec_get_hw_config(codec, i)))
          throw std::runtime_error("No HW config for codec");
      //vaapi libva-vdpau-driver for nvidia needed
      else if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX && config->device_type == AV_HWDEVICE_TYPE_VDPAU)
      {   
          pixFmt = config->pix_fmt;
          std::cerr << av_get_pix_fmt_name(pixFmt) << std::endl;
          break;
      }
      std::cerr << av_hwdevice_get_type_name(config->device_type) << std::endl;
  }

  AVBufferRef *deviceContext;
  if(av_hwdevice_ctx_create(&deviceContext, config->device_type, NULL, NULL, 0) < 0)
      throw std::runtime_error("Cannot create HW device");
  codecContext->hw_device_ctx = av_buffer_ref(deviceContext);

  if(avcodec_open2(codecContext, codec, NULL) < 0)
    throw std::runtime_error("Cannot open codec.");













  avformat_free_context(formatContext);
  return EXIT_SUCCESS;
}

