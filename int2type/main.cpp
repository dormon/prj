#include<iostream>
#include<map>
#include<vector>
#include<typeinfo>

template<size_t>struct Int2Type                          ;
template<      >struct Int2Type<0>{using type = int8_t ;};
template<      >struct Int2Type<1>{using type = int16_t;};
template<      >struct Int2Type<2>{using type = int32_t;};
template<      >struct Int2Type<3>{using type = int64_t;};

int main(){
  Int2Type<3>::type a = 32;
  static_cast<void>(a);
  return 0;
}
