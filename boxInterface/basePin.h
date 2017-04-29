#pragma once

#include"pin.h"
#include"typeToId.h"

template<typename TYPE>
class BasePin: public Pin{
  protected:
    unsigned _type;
  public:
    BasePin(
        void*    data                       ,
        bool     read              = true   ,
        bool     write             = true   ,
        CALLBACK writeCallback     = nullptr,
        void*    writeCallbackData = NULL   ,
        CALLBACK readCallback      = nullptr,
        void*    readCallbackData  = NULL   ):Pin(data,read,write,writeCallback,writeCallbackData,readCallback,readCallbackData){
      this->_type=typeToId<TYPE>();
    }
    BasePin(
        bool     read              = true   ,
        bool     write             = true   ,
        CALLBACK writeCallback     = nullptr,
        void*    writeCallbackData = NULL   ,
        CALLBACK readCallback      = nullptr,
        void*    readCallbackData  = NULL   ):Pin(read,write,writeCallback,writeCallbackData,readCallback,readCallbackData){
      this->_type = typeToId<TYPE>();
    }
    TYPE& operator=(TYPE const&data){
      *((TYPE*)this->_data) = data;
      return *this;
    };
    operator TYPE&(){
      return *((TYPE*)this->_data);
    }
    std::string toStr(){return typeToStr<TYPE>();}
};

