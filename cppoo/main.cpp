#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>
#include<cassert>
#include<functional>
#include<limits>
#include<typeinfo>
#include<typeindex>

class True{};
class False{};

class Bool{
  public:
    Bool(True  const&){this->_value = true ;}
    Bool(False const&){this->_value = false;}
  private:
    bool _value;
};

class Uint32{
  public:
    Uint32():_value(0.f){}
    Uint32(Uint32 const&o):_value(o._value){}
    Bool   operator== (Uint32 const&o)const{if(this->_value == o._value)return Bool(True{});return Bool(False{});}
    Bool   operator<= (Uint32 const&o)const{if(this->_value <= o._value)return Bool(True{});return Bool(False{});}
    Bool   operator>= (Uint32 const&o)const{if(this->_value >= o._value)return Bool(True{});return Bool(False{});}
    Bool   operator!= (Uint32 const&o)const{if(this->_value != o._value)return Bool(True{});return Bool(False{});}
    Bool   operator<  (Uint32 const&o)const{if(this->_value <  o._value)return Bool(True{});return Bool(False{});}
    Bool   operator>  (Uint32 const&o)const{if(this->_value >  o._value)return Bool(True{});return Bool(False{});}
    Uint32 operator+  (Uint32 const&o)const{return Uint32(this->_value +  o._value);}
    Uint32 operator-  (Uint32 const&o)const{return Uint32(this->_value -  o._value);}
    Uint32 operator*  (Uint32 const&o)const{return Uint32(this->_value *  o._value);}
    Uint32 operator/  (Uint32 const&o)const{return Uint32(this->_value /  o._value);}
    Uint32 operator%  (Uint32 const&o)const{return Uint32(this->_value %  o._value);}
    Uint32 operator<< (Uint32 const&o)const{return Uint32(this->_value << o._value);}
    Uint32 operator>> (Uint32 const&o)const{return Uint32(this->_value >> o._value);}
    Uint32 operator+= (Uint32 const&o)     {this->_value +=  o._value;return Uint32(this->_value);}
    Uint32 operator-= (Uint32 const&o)     {this->_value -=  o._value;return Uint32(this->_value);}
    Uint32 operator*= (Uint32 const&o)     {this->_value *=  o._value;return Uint32(this->_value);}
    Uint32 operator/= (Uint32 const&o)     {this->_value /=  o._value;return Uint32(this->_value);}
    Uint32 operator%= (Uint32 const&o)     {this->_value %=  o._value;return Uint32(this->_value);}
    Uint32 operator>>=(Uint32 const&o)     {this->_value >>= o._value;return Uint32(this->_value);}
    Uint32 operator<<=(Uint32 const&o)     {this->_value <<= o._value;return Uint32(this->_value);}
    Uint32 operator-  (              )const{return Uint32(-this->_value);}
    Uint32 operator+  (              )const{return Uint32(+this->_value);}
  private:
    Uint32(uint32_t const&v):_value(v){}
    uint32_t _value;
};


class Float32{
  public:
    Float32():_value(0.f){}
    Float32(Float32 const&o):_value(o._value){}
    Bool    operator==(Float32 const&o)const{if(this->_value == o._value)return Bool(True{});return Bool(False{});}
    Bool    operator<=(Float32 const&o)const{if(this->_value <= o._value)return Bool(True{});return Bool(False{});}
    Bool    operator>=(Float32 const&o)const{if(this->_value >= o._value)return Bool(True{});return Bool(False{});}
    Bool    operator!=(Float32 const&o)const{if(this->_value != o._value)return Bool(True{});return Bool(False{});}
    Bool    operator< (Float32 const&o)const{if(this->_value <  o._value)return Bool(True{});return Bool(False{});}
    Bool    operator> (Float32 const&o)const{if(this->_value >  o._value)return Bool(True{});return Bool(False{});}
    Float32 operator+ (Float32 const&o)const{return Float32(this->_value + o._value);}
    Float32 operator- (Float32 const&o)const{return Float32(this->_value - o._value);}
    Float32 operator* (Float32 const&o)const{return Float32(this->_value * o._value);}
    Float32 operator/ (Float32 const&o)const{return Float32(this->_value / o._value);}
    Float32 operator+=(Float32 const&o)     {this->_value+=o._value;return Float32(this->_value);}
    Float32 operator-=(Float32 const&o)     {this->_value-=o._value;return Float32(this->_value);}
    Float32 operator*=(Float32 const&o)     {this->_value*=o._value;return Float32(this->_value);}
    Float32 operator/=(Float32 const&o)     {this->_value/=o._value;return Float32(this->_value);}
    Float32 operator- (               )const{return Float32(-this->_value);}
    Float32 operator+ (               )const{return Float32(+this->_value);}
  private:
    Float32(float const&v):_value(v){}
    float _value;
};

int main(){
  Float32 a;
  Float32 b;
  auto c=a+b;
  c=-c;
  c=+c;
  c=c;
  c*=c;
  c/=c;
  c=c-=c;
  (void)c;
  return 0;
}


