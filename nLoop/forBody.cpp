#include"forBody.h"

void forBody(
    size_t&a ,
    size_t i0,
    size_t i1,
    size_t i2,
    size_t i3,
    size_t i4,
    size_t i5,
    size_t i6,
    size_t i7
    ){
#if 0
  a+=i;
#else
  static_cast<void>(i0);
  static_cast<void>(i1);
  static_cast<void>(i2);
  static_cast<void>(i3);
  static_cast<void>(i4);
  static_cast<void>(i5);
  static_cast<void>(i6);
  static_cast<void>(i7);
  a++;
#endif
}
