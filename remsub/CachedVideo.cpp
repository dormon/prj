#include "CachedVideo.hpp"

#include <iostream>

cachedVideo::FrameCache::FrameCache(int64_t n,size_t frameSize){
  cache.resize(n);
  for(auto&f:cache)
    f.resize(frameSize);
}

bool cachedVideo::FrameCache::has(int64_t f)const{
  return frameToCache.count(f)>0;
}

cachedVideo::FrameBytes&cachedVideo::FrameCache::get(int64_t f){
  auto&res = cache.at(frameToCache.at(f));
  lru.erase(std::find(std::cbegin(lru),std::cend(lru),f));
  lru.push_back(f);
  return res;
}

cachedVideo::FrameBytes&cachedVideo::FrameCache::reuseLRU(int64_t f){
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
cachedVideo::FrameCache::FrameCache(){}


cachedVideo::Video::Video(char const*fileName,int id,size_t cacheSize){
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
  frameCache = FrameCache(cacheSize,width*height*3);

  sws_ctx = sws_getContext(width, height,codec_ctx->pix_fmt,width, height,AV_PIX_FMT_RGB24,0, 0, 0, 0);
  if(!sws_ctx)throw std::runtime_error("cannot create sws_ctx");
}


std::string cachedVideo::Video::getAVError(int ret){
  size_t const bufSize = 4096;
  char buf[bufSize];
  av_strerror(ret,buf,bufSize);
  return std::string(buf);
}

void cachedVideo::Video::throwAVError(int ret){
  if(ret == 0)return;
  auto err = getAVError(ret);
  throw std::runtime_error(err);
}


cachedVideo::FrameBytes const&cachedVideo::Video::getFrame(int64_t f){
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

void cachedVideo::Video::loadFrame(FrameBytes&data,int64_t f){
  //std::cerr << "for frame: " << f << " ";
  //std::cerr << "we want dts: " << frameInfo.at(f).dts << " ";
  seek(f);
  //std::cerr << "we seeked to dts: " << pkt->dts << " ";
  read(data);
}

int64_t cachedVideo::Video::getNofFrames()const{
  return frameInfo.size();
}

void cachedVideo::Video::seek(int64_t f){
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

void cachedVideo::Video::read(FrameBytes&data){
  int ret;
  bool stop = false;

  while (1) {
    if ((ret = av_read_frame(ctx, pkt)) < 0)
       break;
    if (pkt->stream_index == streamId) {
      ret = avcodec_send_packet(codec_ctx, pkt);
      //std::cerr << " we get dts: " << pkt->dts << std::endl;
      //throwAVError(ret);
      if(ret < 0)std::cerr << __LINE__ << ": " << getAVError(ret) << std::endl;
      while (ret >= 0) {
        ret = avcodec_receive_frame(codec_ctx, frame);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)break;
        else if(ret < 0){
          //throwAVError(ret);
          std::cerr << __LINE__ << ": " << getAVError(ret) << std::endl;
        }

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

int64_t cachedVideo::Video::getTS(int64_t frame){
  auto timeBase = (int64_t(codec_ctx->time_base.num) * AV_TIME_BASE) / int64_t(codec_ctx->time_base.den);
  return int64_t(frame) * timeBase;
}

void cachedVideo::Video::getVideoInfo(){
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

  nofFrames = frameInfo.size();

  fps = (float)framerate.num / (float)framerate.den;

  av_frame_free(&myFrame);
  av_packet_unref(&packet);
}

int64_t cachedVideo::Video::getFrameId()const{
  return activeFrame;
}

cachedVideo::Video::~Video(){
  sws_freeContext(sws_ctx);
  av_frame_free(&frame);
  avformat_close_input(&ctx);
  av_packet_unref(pkt);
  avcodec_free_context(&codec_ctx);
  avformat_close_input(&ctx);
}
