#include"range2.h"
#include<sstream>

using namespace ge::util;

Range2::Range2(Lex2 start,Lex2 end){
  this->_start = start;
  this->_end   = end  ;
}

Range2::Range2(Range2&a,Range2&b){
  switch(a.getAdjacency(b)){
    case START_END_LESS            :
      std::cerr<<"ranges: "<<a.toStr()<<" and "<<b.toStr()<<" cannot be joined"<<std::endl;
      this->_start = a._start;
      this->_end   = a._end  ;
      break;
    case START_LESS_END_TOUCHING   :
    case START_LESS_END_INSIDE     :
      this->_start = a._start;
      this->_end   = b._end  ;
      break;
    case START_LESS_END_GREATER    :
      this->_start = a._start;
      this->_end   = a._end  ;
      break;
    case START_END_INSIDE          :
      this->_start = b._start;
      this->_end   = b._end  ;
      break;
    case START_INSIDE_END_GREATER  :
      this->_start = b._start;
      this->_end   = a._end  ;
      break;
    case START_TOUCHING_END_GREATER:
      this->_start = b._start;
      this->_end   = a._end  ;
      break;
    case START_END_GREATER         :
      std::cerr<<"ranges: "<<a.toStr()<<" and "<<b.toStr()<<" cannot be joined"<<std::endl;
      this->_start = b._start;
      this->_end   = b._end  ;
      break;
  }
}
bool Range2::inside(const void*data,unsigned size,unsigned offset){
  if(!this->_end.getSize())return this->_start.cmp(data,size,offset)==0;
  int s=this->_start.cmp(data,size,offset);
  int e=this->_end  .cmp(data,size,offset);
  return s>=0&&e<=0&&e>=-1;
}

Lex2&Range2::getStart(){
  return this->_start;
}

Lex2&Range2::getEnd(){
  return this->_end;
}

void Range2::setStart(Lex2 start){
  this->_start = start;
}

void Range2::setEnd(Lex2 end){
  this->_end = end;
}

void Range2::set(Lex2 start,Lex2 end){
  this->_start = start;
  this->_end   = end  ;
}

Range2::Adjacency Range2::getAdjacency(Range2&b){
  int ss=this->_start.cmp(b._start);
  int se=this->_start.cmp(b._end  );
  int es=this->_end  .cmp(b._start);
  int ee=this->_end  .cmp(b._end  );
  bool est=this->_end.isSpaceBetween(b   . _start);
  bool set=b   . _end.isSpaceBetween(this->_start);
  if(ss==-1){
    if(es==-1){
      if(!est)return START_END_LESS;
      return START_LESS_END_TOUCHING;
    }
    if(ee<= 0)return START_LESS_END_INSIDE;
    return START_LESS_END_GREATER;
  }
  if(se<= 0){
    if(ee<= 0)return START_END_INSIDE;
    return START_INSIDE_END_GREATER;
  }
  if(se== 1&&set)return START_TOUCHING_END_GREATER;
  return START_END_GREATER;
}

bool Range2::canBeConnected(Range2&b){
  if(this->_start.getSize()!=b._start.getSize())return false;
  switch(this->getAdjacency(b)){
    case START_END_LESS            :return false;
    case START_LESS_END_TOUCHING   :return true ;
    case START_LESS_END_INSIDE     :return true ;
    case START_LESS_END_GREATER    :return true ;
    case START_END_INSIDE          :return true ;
    case START_INSIDE_END_GREATER  :return true ;
    case START_TOUCHING_END_GREATER:return true ;
    case START_END_GREATER         :return false;
    default                        :return false;
  }
}

std::string Range2::toStr(){
  std::stringstream ss;
  ss<<"<"<<this->_start.toStr()<<"-"<<this->_end.toStr()<<">";
  return ss.str();
}
