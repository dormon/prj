#include"transition2.h"

using namespace ge::util;

Transition2::Transition2(
    State2*       next    ,
    Range2        range   ,
    RuleCallback2 callback,
    void*         data    ){
  this->_next     = next    ;
  this->_range    = range   ;
  this->_callback = callback;
  this->_data     = data    ;
}

bool Transition2::inside(const void*data,unsigned size,unsigned offset){
  return this->_range.inside(data,size,offset);
}

void Transition2::call(FSA2*fsa){
  if(this->_callback)this->_callback(fsa,this->_data);
}

State2*Transition2::getNext(){
  return this->_next;
}

RuleCallback2 Transition2::getCallback(){
  return this->_callback;
}

void*Transition2::getData(){
  return this->_data;
}

Range2&Transition2::getRange(){
  return this->_range;
}

void Transition2::setNext(State2*state){
  this->_next = state;
}

void Transition2::setCallback(RuleCallback2 callback,void*data){
  this->_callback = callback;
  this->_data     = data    ;
}

/*
   bool Transition2::canBeJoined(Transition2&b){
   if(this->getCallback()!=b.getCallback())return false;
   if(this->getData    ()!=b.getData    ())return false;
   if(this->getNext    ()!=b.getNext    ())return false;
   int ss=this->_start.cmp(b.getStart().getData(),b.getStart().getSize,0);
   int se=this->_start.cmp(b.getStart().getData(),b.getStart().getSize,0);
   }*/
