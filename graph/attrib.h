#pragma once

template<typename TYPE,bool SET,bool GET>
class Attrib{
  protected:
    TYPE _value;
  public:
    Attrib(TYPE const&value){this->_value = value;}
    Attrib(){}
    template<typename T = TYPE>
    inline typename std::enable_if<GET,T>::type&get(){return this->_value;}
    template<typename T = TYPE>
    inline void set(typename std::enable_if<SET,T>::type const&value){this->_value = value;}
};

#define DEF_NAMED_ATTRIBUTE(newClassName,type,set,get)\
  class newClassName: public Attrib<type,set,get>{\
    public:\
      newClassName(const type&val):Attrib<type,set,get>(val){}\
  }

#define DEF_ATTRIBUTE(newClassName,type)\
  DEF_NAMED_ATTRIBUTE(newClassName,type,false,false)

#define DEF_ATTRIBUTE_R(newClassName,type)\
  DEF_NAMED_ATTRIBUTE(newClassName,type,false,true)

#define DEF_ATTRIBUTE_W(newClassName,type)\
  DEF_NAMED_ATTRIBUTE(newClassName,type,true,false)

#define DEF_ATTRIBUTE_RW(newClassName,type)\
  DEF_NAMED_ATTRIBUTE(newClassName,type,true,true)

