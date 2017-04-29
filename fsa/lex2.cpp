#include"lex2.h"
#include<sstream>
#include<cstring>

using namespace ge::util;

Lex2::Lex2(void*data,unsigned size,bool shouldDeallocate){
  this->_data             = data            ;
  this->_size             = size            ;
  this->_shouldDeallocate = shouldDeallocate;
}

Lex2::~Lex2(){
  if(this->_shouldDeallocate&&this->_data)delete[](char*)this->_data;
}

void* Lex2::getData(){
  return this->_data;
}

unsigned Lex2::getSize(){
  return this->_size;
}

int Lex2::cmp(const void*data,unsigned size,unsigned offset){
  if(offset-size<this->getSize())return-2;
  return std::memcmp(this->getData(),(unsigned char*)data+offset,this->getSize());
}

int Lex2::cmp(Lex2&b){
  return this->cmp(b.getData(),b.getSize(),0);
}

int Lex2::cmpPrefix(Lex2&b){
  if(this->getSize()<b.getSize()){
    if(!this->cmp(b   . getData(),this->getSize(),0))return-1;
  }else{
    if(!b   . cmp(this->getData(),b   . getSize(),0))return+1;
  }
  return 0;
}

bool Lex2::isSpaceBetween(Lex2&b){
  int c=this->cmp(b);
  if(c==0)return false;
  unsigned char*aptr=(unsigned char*)(c<0?this->getData():b.getData());
  unsigned char*bptr=(unsigned char*)(c>0?this->getData():b.getData());
  if(std::memcmp(aptr,bptr,this->getSize()-1)!=0)return true;
  if(
      ((unsigned char*)bptr)[this->getSize()-1]-
      ((unsigned char*)aptr)[this->getSize()-1]>1)return true;
  return false;
}

std::string Lex2::toStr(){
  std::stringstream ss;
  ss<<this->_data<<":"<<this->_size<<std::endl;
  return ss.str();
}
