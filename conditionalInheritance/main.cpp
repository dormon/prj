#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>


class Base{
  public:
    int _baseData;
};

template<bool INHERITE>
class Derived: public std::enable_if<INHERITE,Base>::type{
  public:
    int _derivedData;
};

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

#define DEF_ATTRIBUTER(newClassName,type)\
  DEF_NAMED_ATTRIBUTE(newClassName,type,false,true)

#define DEF_ATTRIBUTEW(newClassName,type)\
  DEF_NAMED_ATTRIBUTE(newClassName,type,true,false)

#define DEF_ATTRIBUTERW(newClassName,type)\
  DEF_NAMED_ATTRIBUTE(newClassName,type,true,true)

DEF_ATTRIBUTERW(Weight,float);
DEF_ATTRIBUTER(Speed,float);

class Particle: public Weight, public Speed{
  public:
    Particle(float const&weight,float const&speed):Weight(weight),Speed(speed){}
};

int main(){

  Derived<true>A;
  A._baseData = 10;
  A._derivedData = 12;

  

  Particle particle(10.f,10.f);
  particle.Weight::set(23.f);
  particle.Speed::get();
  return 0;
}
