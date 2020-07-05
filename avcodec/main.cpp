/*
 * copyright (c) 2001 Fabrice Bellard
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <algorithm>
		


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

#include<iomanip>
#include<iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define ___ std::cerr << __LINE__ << std::endl

#define CHECK_ERR(ERR) {if ((ERR)<0) return -1; }



class Video{
  public:
    Video(char const*fileName,int id = 0){
      int ret;
      ret = avformat_open_input(&ctx, fileName,nullptr,nullptr);
      throwAVError(ret);
      ret = avformat_find_stream_info(ctx, nullptr);
      throwAVError(ret);
      av_dump_format(ctx, 0, fileName, false);

      int videoStreamCounter = 0;
      for (size_t i = 0; i < ctx->nb_streams; i++){
        if(ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
          if(videoStreamCounter == id){
            streamId = ctx->streams[i]->id;
            stream = ctx->streams[i];
            break;
          }
        }
      }
      if(stream == nullptr)throw std::runtime_error("cannot selected this video stream number");

      codec = avcodec_find_decoder(stream->codecpar->codec_id);
      if(!codec)throw std::runtime_error("cannot find decoder");

      codec_ctx = avcodec_alloc_context3(codec);

      ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
      throwAVError(ret);

      ret = avcodec_open2(codec_ctx, codec, nullptr);
      throwAVError(ret);

      pkt = av_packet_alloc();

      frame = av_frame_alloc();
      if(frame == nullptr)throw std::runtime_error("cannot allocate frame");
      getVideoInfo();
      frameCache = FrameCache(10,width*height*3);

      sws_ctx = sws_getContext(width, height,codec_ctx->pix_fmt,width, height,AV_PIX_FMT_RGB24,0, 0, 0, 0);
      if(!sws_ctx)throw std::runtime_error("cannot create sws_ctx");
    }

    std::string getAVError(int ret){
      size_t const bufSize = 4096;
      char buf[bufSize];
      av_strerror(ret,buf,bufSize);
      return std::string(buf);
    }
    
    void throwAVError(int ret){
      if(ret == 0)return;
      auto err = getAVError(ret);
      throw std::runtime_error(err);
    }

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

    FrameBytes const&getFrame(int64_t f){
      //std::cerr << "lru: " << std::endl;
      //for(auto const&x:frameCache.lru)std::cerr << x << " ";
      //std::cerr << std::endl;
      //std::cerr << "f2c: " << std::endl;
      //for(auto const&x:frameCache.frameToCache)std::cerr << x.first << "-" << x.second << " ";
      //std::cerr << std::endl;

      if(frameCache.has(f))return frameCache.get(f);
      auto&res = frameCache.reuseLRU(f);
      loadFrame(res,f);
      return res;
    }
    void loadFrame(FrameBytes&data,int64_t f){
      //std::cerr << "for frame: " << f << " ";
      //std::cerr << "we want dts: " << frameInfo.at(f).dts << " ";
      seek(f);
      //std::cerr << "we seeked to dts: " << pkt->dts << " ";
      read(data);
    }

    int64_t getNofFrames()const{
      return frameInfo.size();
    }
    void seek(int64_t f){
      //std::cerr << "f: " << f << std::endl;
      if(f >= getNofFrames())return;
      if(f == activeFrame)return;

      auto const&afi = frameInfo.at(activeFrame);
      auto const&fi  = frameInfo.at(f);
      
      int ret;
      if(
          afi.closestPrevKeyFrameDTS != fi.closestPrevKeyFrameDTS ||
          afi.dts > fi.dts
          ){
        avcodec_flush_buffers(codec_ctx);
        int flag = f < activeFrame?AVSEEK_FLAG_BACKWARD:0;
        ret = av_seek_frame(ctx,streamId,fi.closestPrevKeyFrameDTS,flag);
        throwAVError(ret);
      }else{
        //std::cerr << " we dont need av_seek_frame ";
      }

      activeFrame = f;

      if(fi.keyframe)return;

      auto const&pfi = frameInfo.at(f-1);

      if(afi.dts == pkt->dts)return;

      while((ret = av_read_frame(ctx,pkt))!=AVERROR_EOF){
        throwAVError(ret);
        //std::cerr <<"pkt->dts:" << pkt->dts << std::endl;

        if (pkt->stream_index != streamId )continue;

        ret = avcodec_send_packet(codec_ctx, pkt);
        if (ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)throwAVError(ret);
        ret = avcodec_receive_frame(codec_ctx, frame);
        if (ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)throwAVError(ret);

        int64_t dts   = pkt->dts;// == AV_NOPTS_VALUE?0:pkt->dts;
        if(dts >= pfi.dts)break;
        if(dts >= fi.dts)break;
      }

    }
    void read(FrameBytes&data){
      int ret;
      bool stop = false;

      while (1) {
        if ((ret = av_read_frame(ctx, pkt)) < 0)
           break;
        if (pkt->stream_index == streamId) {
          ret = avcodec_send_packet(codec_ctx, pkt);
          //std::cerr << " we get dts: " << pkt->dts << std::endl;
          throwAVError(ret);
          while (ret >= 0) {
            ret = avcodec_receive_frame(codec_ctx, frame);
            if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)break;
            else throwAVError(ret);

            //frame->pts = frame->best_effort_timestamp;
            
            int const dstStride[1] = {width*3};
            uint8_t*const dst [1] = {data.data()};
            sws_scale(sws_ctx,frame->data,frame->linesize,0,height,dst,dstStride);
            stop = true;
            av_frame_unref(frame);
          }
          activeFrame++;
        }
        av_packet_unref(pkt);
        if(stop)break;
      }
    }

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

    void getVideoInfo(){
      AVPacket packet;
      av_init_packet(&packet);
      packet.data = nullptr;
      packet.size = 0;
      auto myFrame = av_frame_alloc();

      int err;
      int64_t lastKeyFrameDTS = 0;
      int64_t elapsedTime = 0;
      while((err = av_read_frame(ctx,&packet))!=AVERROR_EOF){
        throwAVError(err);
        if (packet.stream_index != streamId )continue;
        bool    isKey = (packet.flags & AV_PKT_FLAG_KEY) != 0;
        int64_t dts   = packet.dts;// == AV_NOPTS_VALUE?0:packet.dts;
        if(isKey){
          keyFrames.emplace_back(dts);
          lastKeyFrameDTS = dts;
        }

        frameInfo.emplace_back(dts,lastKeyFrameDTS,elapsedTime,packet.pos,isKey);
        elapsedTime+=packet.duration;
      }

      err = av_seek_frame(ctx, streamId,0,AVSEEK_FLAG_BACKWARD);
      throwAVError(err);

      framerate = stream->r_frame_rate;
      width     = codec_ctx->width    ;
      height    = codec_ctx->height   ;

      av_frame_free(&myFrame);
      av_packet_unref(&packet);
    }

    ~Video(){
      sws_freeContext(sws_ctx);
      av_frame_free(&frame);
      avformat_close_input(&ctx);
      av_packet_unref(pkt);
      avcodec_free_context(&codec_ctx);
      avformat_close_input(&ctx);
    }

    AVFrame*         frame     = nullptr;
    int              streamId  = 0      ;
    AVPacket*        pkt       = nullptr;
    AVFormatContext* ctx       = nullptr;
    AVStream*        stream    = nullptr;
    AVCodec*         codec     = nullptr;
    AVCodecContext*  codec_ctx = nullptr;
    SwsContext*      sws_ctx   = nullptr;
};

int main(int argc, char **argv) {
  if(argc<2)return 0;
  auto video = Video(argv[1]);
  //for(size_t i=0;i<10;++i)
  //  std::cerr << video.frameInfo.at(i).str() << std::endl;

  size_t frameCounter = 0;
  for(int i=0;i<500;++i){
    std::stringstream ss;
    std::cerr << "saving frame: " << frameCounter << std::endl;
    ss << "frame" << std::setfill('0') << std::setw(7) << frameCounter << ".jpg";
    auto d = video.getFrame(i).data();
    stbi_write_jpg(ss.str().c_str(),video.width,video.height,3,d,100);
    frameCounter++;
  }
  for(int i=0;i<10;++i){
    std::stringstream ss;
    std::cerr << "saving frame: " << frameCounter << std::endl;
    ss << "frame" << std::setfill('0') << std::setw(7) << frameCounter << ".jpg";
    auto d = video.getFrame(400+i).data();
    stbi_write_jpg(ss.str().c_str(),video.width,video.height,3,d,100);
    frameCounter++;
  }


  return 0;
}

