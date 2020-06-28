#pragma once

#include <opencv2/opencv.hpp>

class Video{
  public:
    Video(std::string const&v){
      video = cv::VideoCapture(v.c_str());
      ifVideoIsNotOpenThrow();
      width     = video.get(cv::CAP_PROP_FRAME_WIDTH );
      height    = video.get(cv::CAP_PROP_FRAME_HEIGHT);
      nofFrames = video.get(cv::CAP_PROP_FRAME_COUNT ); 
      fps       = video.get(cv::CAP_PROP_FPS         ); 
    }
    ~Video(){
      video.release();
    }

    void readFrame(){
      video >> frame;
    }
    void onlyRead(){
      video >> frame;
      moveToPrevFrame();
    }
    void moveToNextFrame(){
      auto id = getFrameId();
      if(id+1 == nofFrames)return;
      video.set(cv::CAP_PROP_POS_FRAMES,id+1);
    }
    void moveToFrame(size_t f){
      if(f+1 >= nofFrames)f=nofFrames-1;
      video.set(cv::CAP_PROP_POS_FRAMES,f);
    }
    void moveToPrevFrame(){
      auto id = getFrameId();
      if(id == 0)return;
      video.set(cv::CAP_PROP_POS_FRAMES,id-1);
    }
    size_t getFrameId()const{
      return video.get(cv::CAP_PROP_POS_FRAMES);
    }
    void cloneFrame(cv::Mat&f){
      if(firstFrame){
        readFrame();
        firstFrame = false;
      }
      f = frame.clone();
    }
    cv::Mat frame;
    uint32_t width,height,nofFrames,fps;
  protected:
    void ifVideoIsNotOpenThrow(){
      if(video.isOpened())return;
      throw std::runtime_error("cannot open video");
    }
    cv::VideoCapture video;
    bool firstFrame = true;
};

