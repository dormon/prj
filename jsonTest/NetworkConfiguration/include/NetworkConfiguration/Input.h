#pragma once

#include<NetworkConfiguration/Layer.h>

class NETWORKCONFIGURATION_EXPORT cnnConf::Input: public cnnConf::Layer{
  public:
    Input(
        size_t w,
        size_t h,
        size_t c);
    size_t getNofChannels()const;
    size_t getWidth()const;
    size_t getHeight()const;
  protected:
    size_t width   ;
    size_t height  ;
    size_t channels;
};

