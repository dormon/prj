#include"pin.h"


Pin::Pin(
    void*    data             ,
    bool     read             ,
    bool     write            ,
    CALLBACK writeCallback    ,
    void*    writeCallbackData,
    CALLBACK readCallback     ,
    void*    readCallbackData ){
  this->_read              = read             ;
  this->_write             = write            ;
  this->_data              = data             ;
  this->_writeCallback     = writeCallback    ;
  this->_writeCallbackData = writeCallbackData;
  this->_readCallback      = readCallback     ;
  this->_readCallbackData  = readCallbackData ;

}

Pin::Pin(
    bool     read             ,
    bool     write            ,
    CALLBACK writeCallback    ,
    void*    writeCallbackData,
    CALLBACK readCallback     ,
    void*    readCallbackData ){
  this->_read              = read             ;
  this->_write             = write            ;
  this->_data              = NULL             ;
  this->_writeCallback     = writeCallback    ;
  this->_writeCallbackData = writeCallbackData;
  this->_readCallback      = readCallback     ;
  this->_readCallbackData  = readCallbackData ;
}

bool Pin::getRead (){
  return this->_read ;
}

bool Pin::getWrite(){
  return this->_write;
}

void Pin::bind(void*data){
  this->_data = data;
}

void Pin::setWriteCallback(
    CALLBACK writeCallback    ,
    void*    writeCallbackData){
  this->_writeCallback     = writeCallback    ;
  this->_writeCallbackData = writeCallbackData;
}

void Pin::setReadCallback(
    CALLBACK readCallback    ,
    void*    readCallbackData){
  this->_readCallback     = readCallback    ;
  this->_readCallbackData = readCallbackData;
}

std::string Pin::toStr(){
  return "";
}

