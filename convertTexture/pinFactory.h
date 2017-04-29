#pragma once

#include"basePin.h"
#include"toVoidPtr.h"

template<typename TYPE>
std::shared_ptr<Pin> pinFactory(
    TYPE&         data                       ,
    bool          read              = true   ,
    bool          write             = true   ,
    Pin::CALLBACK writeCallback     = nullptr,
    void*         writeCallbackData = NULL   ,
    Pin::CALLBACK readCallback      = nullptr,
    void*         readCallbackData  = NULL   ){
  return std::make_shared<BasePin<TYPE>>(toVoidPtr(&data),read,write,writeCallback,writeCallbackData,readCallback,readCallbackData);
}

template<typename TYPE>
std::shared_ptr<Pin> pinFactory(
    bool          read              = true   ,
    bool          write             = true   ,
    Pin::CALLBACK writeCallback     = nullptr,
    void*         writeCallbackData = NULL   ,
    Pin::CALLBACK readCallback      = nullptr,
    void*         readCallbackData  = NULL   ){
  return std::make_shared<BasePin<TYPE>>(read,write,writeCallback,writeCallbackData,readCallback,readCallbackData);
}


