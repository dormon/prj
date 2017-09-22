#pragma once

#include<memory>

class CompositeNumber    ;
class FiniteDigitNumber  ;
class InfiniteDigitNumber;
class Nullary            ;

class Number: public std::enable_shared_from_this<Number>{
  public:
    enum Type{
      NULLARY  ,
      COMPOSITE,
    }type;
    Number(Type const&t):type(t){}
    bool isNullary  ()const{return type == NULLARY  ;}
    bool isComposite()const{return type == COMPOSITE;}
    std::shared_ptr<Nullary>toNullary(){
      return std::dynamic_pointer_cast<Nullary>(this->shared_from_this());
    }
    std::shared_ptr<CompositeNumber>toComposite(){
      return std::dynamic_pointer_cast<CompositeNumber>(this->shared_from_this());
    }
};

class CompositeNumber: public Number{
  public:
    enum Type{
      FINITE  ,
      INFINITE,
    }type;
    std::shared_ptr<Number>digitType = nullptr;
    CompositeNumber(Type const&t,std::shared_ptr<Number>const&d):Number(Number::COMPOSITE),type(t),digitType(d){}
    bool isFinite  ()const{return type == FINITE  ;}
    bool isInfinite()const{return type == INFINITE;}
    std::shared_ptr<FiniteDigitNumber>toFinite(){
      return std::dynamic_pointer_cast<FiniteDigitNumber>(this->shared_from_this());
    }
    std::shared_ptr<InfiniteDigitNumber>toInfinite(){
      return std::dynamic_pointer_cast<InfiniteDigitNumber>(this->shared_from_this());
    }
};

class FiniteDigitNumber: public CompositeNumber{
  public:
    FiniteDigitNumber(std::shared_ptr<Number>const&d):CompositeNumber(CompositeNumber::FINITE,d){}
};

class InfiniteDigitNumber: public CompositeNumber{
  public:
    InfiniteDigitNumber(std::shared_ptr<Number>const&d):CompositeNumber(CompositeNumber::INFINITE,d){}
};

class Nullary: public Number{
  public:
    Nullary():Number(Number::NULLARY){}
};
