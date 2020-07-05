#pragma once

#include <string>
#include <exception>
#include <stdexcept>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <sstream>

#define __STDC_CONSTANT_MACROS
extern "C"{
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/mathematics.h>
#include <libavutil/common.h>
}

namespace cachedVideo{

using FrameBytes = std::vector<uint8_t>;

struct FrameInfo{
  int64_t dts;
  int64_t closestPrevKeyFrameDTS;
  int64_t time;
  int64_t pos ;
  bool keyframe;
  FrameInfo(int64_t dts,int64_t prevKeyDTS,int64_t time,int64_t pos,bool key):
    dts(dts),
    closestPrevKeyFrameDTS(prevKeyDTS),
    time(time),
    pos(pos),
    keyframe(key){}
  std::string str()const{
    std::stringstream ss;
    ss << "dts: " << dts;
    ss << " prev: " << closestPrevKeyFrameDTS;
    ss << " time: " << time;
    ss << " pos: " << pos;
    ss << " key: " << (int)keyframe;
    return ss.str();
  }
};

struct FrameCache{
  std::vector<FrameBytes>cache;
  std::map<int64_t,size_t>frameToCache;
  std::list<int64_t>lru;
  FrameCache(int64_t n,size_t frameSize);
  bool has(int64_t f)const;
  FrameBytes&get(int64_t f);
  FrameBytes&reuseLRU(int64_t f);
  FrameCache();
};

class Video{
  public:
    Video(char const*fileName,int id = 0,size_t cacheSize = 100);
    ~Video();

    std::string      getAVError(int ret);
    void             throwAVError(int ret);

    FrameBytes const&getFrame(int64_t f);
    void             loadFrame(FrameBytes&data,int64_t f);
    int64_t          getNofFrames()const;
    void             seek(int64_t f);
    void             read(FrameBytes&data);
    int64_t          getTS(int64_t frame);
    void             getVideoInfo();
    


    int64_t activeFrame = 0;
    AVRational framerate;
    int64_t    nofFrames;
    int        width;
    int        height;

    FrameCache frameCache;

    std::vector<FrameInfo>frameInfo;
    std::vector<int64_t>keyFrames;

    AVFrame*         frame     = nullptr;
    int              streamId  = 0      ;
    AVPacket*        pkt       = nullptr;
    AVFormatContext* ctx       = nullptr;
    AVStream*        stream    = nullptr;
    AVCodec*         codec     = nullptr;
    AVCodecContext*  codec_ctx = nullptr;
    SwsContext*      sws_ctx   = nullptr;
};

}
