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

class Video{
  public:
    Video(char const*fileName,int id = 0);

    std::string getAVError(int ret);
    void throwAVError(int ret);

    int64_t activeFrame = 0;
    AVRational framerate;
    int64_t    nofFrames;
    int        width;
    int        height;


    using FrameBytes = std::vector<uint8_t>;
    struct FrameCache{
      std::vector<FrameBytes>cache;
      std::map<int64_t,size_t>frameToCache;
      std::list<int64_t>lru;
      FrameCache(int64_t n,size_t frameSize){
        cache.resize(n);
        for(auto&f:cache)
          f.resize(frameSize);
      }
      bool has(int64_t f)const{
        return frameToCache.count(f)>0;
      }
      FrameBytes&get(int64_t f){
        auto&res = cache.at(frameToCache.at(f));
        lru.erase(std::find(std::cbegin(lru),std::cend(lru),f));
        lru.push_back(f);
        return res;
      }
      FrameBytes&reuseLRU(int64_t f){
        if(lru.size() < cache.size()){
          frameToCache[f] = lru.size();
        }else{
          auto lf = lru.front();
          lru.pop_front();
          auto cacheId = frameToCache.at(lf);
          frameToCache.erase(lf);
          frameToCache[f] = cacheId;
        }
        lru.push_back(f);
        return cache.at(frameToCache.at(f));
      }
      FrameCache(){}
    }frameCache;

    FrameBytes const&getFrame(int64_t f);
    
    void loadFrame(FrameBytes&data,int64_t f);
    int64_t getNofFrames()const;
    void seek(int64_t f);
    void read(FrameBytes&data);
    
    int64_t getTS(int64_t frame){
      auto timeBase = (int64_t(codec_ctx->time_base.num) * AV_TIME_BASE) / int64_t(codec_ctx->time_base.den);
      return int64_t(frame) * timeBase;
    }

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
    std::vector<FrameInfo>frameInfo;
    std::vector<int64_t>keyFrames;

    void getVideoInfo();
    

    ~Video();

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
