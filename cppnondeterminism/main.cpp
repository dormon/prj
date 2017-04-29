#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>
#include<cassert>
#include<functional>
#include<typeinfo>
#include<typeindex>

class B0;
class A0{
  public:
    int data;
    A0(int const&d = 0):data(d){}
    A0(B0 const&d);
    B0 toB()const;
};

class B0{
  public:
    float data;
    B0(float const&d = 0):data(d){}
    B0(A0 const&d):data(d.data){}
    A0 toA()const{return A0(this->data);}
};

B0 A0::toB()const{
  return B0(this->data);
}

A0::A0(B0 const&d):data(d.data){}

template<
  typename   OUTPUT   ,
  typename   CLASS    ,
  typename...ARGUMENTS,
  typename...VALUES   >
void uber(OUTPUT(CLASS::*fce)(ARGUMENTS...),OUTPUT*output,CLASS*cls,VALUES...values){
  *output = (cls.*fce)(values...);
}

int main(){
  return 0;
}


