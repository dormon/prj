#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

float addFloat(float const&a,float const&b){
  return a + b;
}

int32_t addInt32(int32_t const&a,int32_t const&b){
  return a + b;
}

template<typename TYPE>
TYPE add(TYPE const&a,TYPE const&b){
  return a + b;
}

template<typename TYPE0,typename TYPE1>
TYPE0 add(TYPE0 const&a,TYPE1 const&b){
  return a + b;
}


class A{
  public:
    float a[3];
    A operator+(A const&other)const{
      A result = {{0,0,0}};
      for(size_t i=0;i<3;++i)
        result.a[i] = this->a[i] + other.a[i];
      return result;
    }
};

int main(){
  auto const a = add(1,2);
  static_cast<void>(a);

  auto const b = add(1.f,3.1f);
  static_cast<void>(b);

  A x;
  A y;
  A z = add(x,y);

  add(1,32.3f);
  static_cast<void>(z);

  return 0;
}
