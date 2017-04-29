#pragma once

#include<iostream>

template<typename TYPE>
class BasePin;

class Pin{
  public:
    typedef void(*CALLBACK)(void*);
  protected:
    bool  _read              = true   ;
    bool  _write             = true   ;
    void* _data              = NULL   ;
    CALLBACK _writeCallback  = nullptr;
    CALLBACK _readCallback   = nullptr;
    void* _readCallbackData  = NULL   ;
    void* _writeCallbackData = NULL   ;
  public:
    Pin(
        void*    data                       ,
        bool     read              = true   ,
        bool     write             = true   ,
        CALLBACK writeCallback     = nullptr,
        void*    writeCallbackData = NULL   ,
        CALLBACK readCallback      = nullptr,
        void*    readCallbackData  = NULL   );
    Pin(
        bool     read              = true   ,
        bool     write             = true   ,
        CALLBACK writeCallback     = nullptr,
        void*    writeCallbackData = NULL   ,
        CALLBACK readCallback      = nullptr,
        void*    readCallbackData  = NULL   );
    bool getRead ();
    bool getWrite();
    void bind(void*data);
    void setWriteCallback(
        CALLBACK writeCallback     = nullptr,
        void*    writeCallbackData = NULL   );
    void setReadCallback(
        CALLBACK readCallback     = nullptr,
        void*    readCallbackData = NULL   );
    virtual std::string toStr();
    template<typename TYPE>
      void set(TYPE const&data){
        if(!this->_write){
          std::cerr<<"ERROR: you cannot write to readonly pin"<<std::endl;
          return;
        }
        *((TYPE*)this->_data)=data;
        if(this->_writeCallback)this->_writeCallback(this->_writeCallbackData);
      }
    template<typename TYPE>
      TYPE&get(){
        if(!this->_read)
          std::cerr<<"ERROR: you cannot read from writeonly pin"<<std::endl;
        if(this->_readCallback)this->_readCallback(this->_readCallbackData);
        return *((TYPE*)this->_data);
      }

    template<typename TYPE>
      TYPE& operator=(TYPE const&data){
        *((TYPE*)this->_data) = data;
        return *((TYPE*)this->_data);
      }
    template<typename TYPE>
      operator TYPE&(){
        return *((TYPE*)this->_data);
      }
};

