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


/*
void AV_seek(AV * av, size_t frame)
{
    int frame_delta = frame - av->frame_id;
    if (frame_delta < 0 || frame_delta > 5)
        av_seek_frame(av->fmt_ctx, av->video_stream_id,
                frame, AVSEEK_FLAG_BACKWARD);
    while (av->frame_id != frame)
        AV_read_frame(av);
}

void AV_read_frame(AV * av)
{
    AVPacket packet;
    int frame_done;

    while (av_read_frame(av->fmt_ctx, &packet) >= 0) {
        if (packet.stream_index == av->video_stream_id) {
            avcodec_decode_video2(av->codec_ctx, av->frame, &frame_done, &packet);
            if (frame_done) {
                ...
                av->frame_id = packet.dts;
                av_free_packet(&packet);
                return;
            }
        }
        av_free_packet(&packet);
    }
}
*/


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
      //std::cerr << " we get dts: " << pkt->dts << std::endl;
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
      //while((ret = av_read_frame(ctx,pkt))!=AVERROR_EOF){
      //  throwAVError(ret);
      //  if (pkt->stream_index == streamId ){

      //    ret = avcodec_send_packet(codec_ctx, pkt);
      //    throwAVError(ret);

      //    while(ret>=0){
      //      ret = avcodec_receive_frame(codec_ctx, frame);
      //      if (ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)throwAVError(ret);
      //    }


      //    std::cerr << "linesize[0]: " << frame->linesize[0] << std::endl;
      //    std::cerr << "linesize[1]: " << frame->linesize[1] << std::endl;
      //    std::cerr << "linesize[2]: " << frame->linesize[2] << std::endl;
      //    std::cerr << "linesize[3]: " << frame->linesize[3] << std::endl;
      //    std::cerr << "channels: " << frame->channels << std::endl;
      //    std::cerr << "codec_ctx->channels: "<<codec_ctx->channels<<std::endl;
      //    std::cerr << "frame->data[0]: " << frame->data[0] << std::endl;
      //    for(int y=0;y<height;++y)
      //      for(int x=0;x<width;++x){
      //        data[(y*width+x)*3+0] = 0 ;//frame->data[0][(y*frame->linesize[0]+x)];
      //        data[(y*width+x)*3+1] = 255;//frame->data[0][(y*frame->linesize[0]+x)];
      //        data[(y*width+x)*3+2] = 0;//frame->data[0][(y*frame->linesize[0]+x)];
      //      }
      //    av_frame_unref(frame);
      //    stop = true;

      //  }
      //  av_packet_unref(pkt);
      //  if(stop)break;
      //}

      while (1) {
        if ((ret = av_read_frame(ctx, pkt)) < 0)
           break;
        if (pkt->stream_index == streamId) {
          ret = avcodec_send_packet(codec_ctx, pkt);
          throwAVError(ret);
          while (ret >= 0) {
            ret = avcodec_receive_frame(codec_ctx, frame);
            if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)break;
            else throwAVError(ret);

            //frame->pts = frame->best_effort_timestamp;
            
            //do stuff

            int const dstStride[1] = {width*3};
            uint8_t*const dst [1] = {data.data()};
            sws_scale(sws_ctx,frame->data,frame->linesize,0,height,dst,dstStride);
            stop = true;
            av_frame_unref(frame);
          }
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
      bool keyframe;
      FrameInfo(int64_t dts,int64_t prevKeyDTS,int64_t time,bool key):
        dts(dts),
        closestPrevKeyFrameDTS(prevKeyDTS),
        time(time),
        keyframe(key){}
      std::string str()const{
        std::stringstream ss;
        ss << "dts: " << dts;
        ss << "prev: " << closestPrevKeyFrameDTS;
        ss << "time: " << time;
        ss << "key: " << (int)keyframe;
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

        frameInfo.emplace_back(dts,lastKeyFrameDTS,elapsedTime,isKey);
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

  //for(size_t i=0;i<100;++i){
  //  auto const&fi = video.frameInfo.at(i);
  //  std::cerr << "frame: " << i << " prev: " << fi.closestPrevKeyFrameDTS;
  //  std::cerr << " dts: " << fi.dts << " time: " << fi.time << " key: " << (int)fi.keyframe << std::endl;
  //}
  //return 0;
  for(int i=0;i<100;++i){
    std::stringstream ss;
    std::cerr << "saving frame: " << i << std::endl;
    ss << "frame" << std::setfill('0') << std::setw(7) << i << ".jpg";
    auto d = video.getFrame(i).data();
    stbi_write_jpg(ss.str().c_str(),video.width,video.height,3,d,100);
  }
  //auto n = atoi(argv[2]);
  //auto s = atoi(argv[3]);
  //std::cerr << n << " " << s << std::endl;
  //std::cerr << std::endl;
  //std::cerr << std::endl;
  //std::cerr << std::endl;
  //for(int i=0;i<n;++i)video.read();
  //video.seek(s);
  //for(int i=0;i<n;++i)video.read();
  //for(int i=0;i<500;++i)video.read();
  ////video.seek(2000);
  //video.AV_seek(400);
  //for(int i=0;i<100;++i)video.read();
  return 0;
}

#if 0
int main(int argc, char **argv) {
    AVFormatContext* ctx_format = nullptr;
    AVCodecContext* ctx_codec = nullptr;
    AVCodec* codec = nullptr;
    AVFrame* frame = av_frame_alloc();
    int stream_idx;
    //SwsContext* ctx_sws = nullptr;
    const char* fin = argv[1];
    AVStream *vid_stream = nullptr;
    AVPacket* pkt = av_packet_alloc();

    //av_register_all();

    if (int ret = avformat_open_input(&ctx_format, fin, nullptr, nullptr) != 0) {
        std::cout << 1 << std::endl;
        return ret;
    }
    if (avformat_find_stream_info(ctx_format, nullptr) < 0) {
        std::cout << 2 << std::endl;
        return -1; // Couldn't find stream information
    }
    av_dump_format(ctx_format, 0, fin, false);

    for (size_t i = 0; i < ctx_format->nb_streams; i++)
        if (ctx_format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            stream_idx = i;
            vid_stream = ctx_format->streams[i];
            break;
    }
    if (vid_stream == nullptr) {
        std::cout << 4 << std::endl;
        return -1;
    }

    codec = avcodec_find_decoder(vid_stream->codecpar->codec_id);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }
    ctx_codec = avcodec_alloc_context3(codec);

    if(avcodec_parameters_to_context(ctx_codec, vid_stream->codecpar)<0)
        std::cout << 512;
    if (avcodec_open2(ctx_codec, codec, nullptr)<0) {
        std::cout << 5;
        return -1;
    }

    //av_new_packet(pkt, pic_size);

    while(av_read_frame(ctx_format, pkt) >= 0){
      if (pkt->stream_index == stream_idx) {
        int ret = avcodec_send_packet(ctx_codec, pkt);
        if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
          std::cout << "avcodec_send_packet: " << ret << std::endl;
          break;
        }
        while (ret  >= 0) {
          ret = avcodec_receive_frame(ctx_codec, frame);
          if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            //std::cout << "avcodec_receive_frame: " << ret << std::endl;
            break;
          }
          std::cerr << " width   : " << frame->width    ;
          std::cerr << " height  : " << frame->height   ;
          std::cerr << " channels: " << frame->channels ;
          std::cerr << " keyframe: " << frame->key_frame;
          std::cerr << " channel_layout: " << frame->channel_layout;
          std::cerr << " best_effort_timestamp: " << frame->best_effort_timestamp;
          std::cerr << " format : " << frame->format;
          std::cerr << " frame   : " << ctx_codec->frame_number;
          std::cerr << " channels: " << ctx_codec->channels;
          std::cerr << " colorspace: " << frame->colorspace;

          std::stringstream ss;
          ss << "frame" << ctx_codec->frame_number << ".bgm";
          auto xsize = frame->width;
          auto ysize = frame->height;
          FILE *f;
          int i;
          f = fopen(ss.str().c_str(),"w");
          fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
          for (i = 0; i < ysize; i++)
              fwrite(frame->data[0] + i * frame->linesize[0], 1, xsize, f);
          fclose(f);

          
          std::cerr << std::endl;
          //std::cout << "frame: " << ctx_codec->frame_number << std::endl;
        }
      }
      av_packet_unref(pkt);
    }


    avformat_close_input(&ctx_format);
    av_packet_unref(pkt);
    avcodec_free_context(&ctx_codec);
    avformat_free_context(ctx_format);
    return 0;
}
#endif
