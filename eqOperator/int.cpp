#include"int.h"
#include"add.h"
#include"mul.h"
#include"sub.h"
#include"addeqmul.h"
#include"addeqsub.h"
#include"muleqsub.h"

class Add;
class Mul;
class Sub;

Int::Int(Type type){
  this->_type = type;
}

Int::~Int(){}

bool ifCompare(Int const&a,Int const&b){
  return false;
}

template<Int::Type A,Int::Type B,typename... ARGS>
bool ifCompare(Int const&a,Int const&b){
  if(a._type==A&&b._type==B)return a.to<A>()==b.to<B>();
  else return ifCompare<ARGS...>(a,b);
}

bool Int::operator==(Int const&other)const{
  std::cout<<"Int==Int"<<std::endl;
  return ifCompare<ADD,ADD,ADD,MUL,ADD,SUB,MUL,ADD,MUL,MUL,MUL,SUB,SUB,ADD,SUB,MUL,SUB,SUB>(*this,other);
  switch(this->_type){
    case ADD:
      switch(other._type){
        case ADD:return this->to<Add>()==other.to<Add>();
        case MUL:return this->to<Add>()==other.to<Mul>();
        case SUB:return this->to<Add>()==other.to<Sub>();
      }
    case MUL:
      switch(other._type){
        case ADD:return this->to<Mul>()==other.to<Add>();
        case MUL:return this->to<Mul>()==other.to<Mul>();
        case SUB:return this->to<Mul>()==other.to<Sub>();
      }
    case SUB:
      switch(other._type){
        case ADD:return this->to<Sub>()==other.to<Add>();
        case MUL:return this->to<Sub>()==other.to<Mul>();
        case SUB:return this->to<Sub>()==other.to<Sub>();
      }
  }
  return false;
}


