#pragma once

#include<iostream>
#include<cmath>

#ifndef WARP
#define WARP 64u
#endif//WARP

#ifndef WINDOW_X
#define WINDOW_X 512u
#endif//WINDOW_X

#ifndef WINDOW_Y
#define WINDOW_Y 512u
#endif//WINDOW_Y

#ifndef MIN_Z
#define MIN_Z 512u
#endif//MIN_Z

struct uint3{
  public:
    uint32_t      &operator[](size_t i)     {return data[i];}
    uint32_t const&operator[](size_t i)const{return data[i];}
    uint32_t      &x()     {return data[0];}
    uint32_t      &y()     {return data[1];}
    uint32_t      &z()     {return data[2];}
    uint32_t const&x()const{return data[0];}
    uint32_t const&y()const{return data[1];}
    uint32_t const&z()const{return data[2];}
    uint3(uint32_t a,uint32_t b,uint32_t c){
      data[0] = a;
      data[1] = b;
      data[2] = c;
    }
  private:
    uint32_t data[3];
};

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

#define uint(XXX) static_cast<uint32_t>(XXX)

// m - length of 3 bits together
// n - length of 2 bits together
// o - length of 1 bit  alone
//
//                      adding_1bit|2bit_shifts|1bit_offset|2bit_offset
// ..*|   .**|.**|.**|       o     |     n-1   |   2n+3m   |  2i+2+3m
// .*.|   .**|.**|.**|       o     |     n-1   |   2n+3m   |  2i+2+3m
// ...|   .**|.**|.**|       o     |     n-1   |   2n+3m   |  2i+2+3m
// ####################################################################
// ..*|   *.*|*.*|*.*|       o-1   |     n     |   2n+3m   |  2i+1+3m
// *..|   *.*|*.*|*.*|       o+1   |     n-1   |   2n+3m-1 |  2i+1+3m
// ...|   *.*|*.*|*.*|       0     |     n     |   2n+3m   |  2i+1+3m
// ####################################################################
// .*.|   **.|**.|**.|       o     |     n     |   2n+3m   |  2i+0+3m
// *..|   **.|**.|**.|       o     |     n     |   2n+3m   |  2i+0+3m
// ...|   **.|**.|**.|       o     |     n     |   2n+3m   |  2i+0+3m
// ####################################################################
// ..*|   ...|...|...|       o     |     n     |   2n+3m   |  xxxxxxx
// .*.|   ...|...|...|       o     |     n     |   2n+3m   |  xxxxxxx
// ...|   ...|...|...|       o     |     n     |   2n+3m   |  xxxxxxx

#define REQUIRED_BITS(x)  uint(ceil(log2(float(x))))
#define DIV_ROUND_UP(x,y) (uint(x/y) + uint(x%y != 0u))

#define WARP_BITS(      w        ) REQUIRED_BITS(w)
#define WARP_BITS_X(    w        ) DIV_ROUND_UP(WARP_BITS(w),2u)
#define WARP_BITS_Y(    w        ) uint(WARP_BITS(w)-WARP_BITS_X(w))
#define WARP_X(         w        ) uint(1u<<WARP_BITS_X(w))
#define WARP_Y(         w        ) uint(1u<<WARP_BITS_Y(w))
#define UINTS_PER_WARP( w        ) uint((w)/32u)
#define CLUSTERS_X(     w,x      ) DIV_ROUND_UP((x),WARP_X(w))
#define CLUSTERS_Y(     w,  y    ) DIV_ROUND_UP((y),WARP_Y(w))
#define X_BITS(         w,x      ) REQUIRED_BITS(CLUSTERS_X(w,x))
#define Y_BITS(         w,  y    ) REQUIRED_BITS(CLUSTERS_Y(w,y))
#define Z_BITS(         w,x,y,z  ) max(max(X_BITS(w,x),Y_BITS(w,y)),z)
#define CLUSTERS_Z(     w,x,y,z  ) uint(1u<<Z_BITS(w,x,y,z))
#define ALL_BITS(       w,x,y,z  ) uint(X_BITS(w,x) + Y_BITS(w,y) + Z_BITS(w,x,y,z))
#define NOF_LEVELS(     w,x,y,z  ) DIV_ROUND_UP(ALL_BITS(w,x,y,z),WARP_BITS(w))
#define SHORTEST(       w,x,y,z  ) min(min(X_BITS(w,x),Y_BITS(w,y)),Z_BITS(w,x,y,z))
#define MIDDLE(         w,x,y,z  ) max(max(min(X_BITS(w,x),Y_BITS(w,y)),min(X_BITS(w,x),Z_BITS(w,x,y,z))),min(Y_BITS(w,y),Z_BITS(w,x,y,z)))
#define LONGEST(        w,x,y,z  ) max(max(X_BITS(w,x),Y_BITS(w,y)),Z_BITS(w,x,y,z))
#define BITS3_LENGTH(   w,x,y,z  ) SHORTEST(w,x,y,z)
#define BITS2_LENGTH(   w,x,y,z  ) uint(MIDDLE(w,x,y,z)-SHORTEST(w,x,y,z))
#define BITS1_LENGTH(   w,x,y,z  ) uint(LONGEST(w,x,y,z)-MIDDLE(w,x,y,z))
#define SHORTEST_LMASK( w,x,y,z  ) uint((1u<<(SHORTEST(w,x,y,z)*3u))-1u)
#define SHORTEST_HMASK( w,x,y,z  ) uint(~SHORTEST_LMASK(w,x,y,z))
#define MIDDLE_LMASK(   w,x,y,z  ) uint((1u<<(SHORTEST(w,x,y,z)*3u + MIDDLE(w,x,y,z)*2u))-1u)
#define MIDDLE_HMASK(   w,x,y,z  ) uint(~MIDDLE_LMASK(w,x,y,z))
#define SHORTEST_AXIS(  w,x,y,z  ) uint(uint(SHORTEST(w,x,y,z) == Y_BITS(w,y)) + uint(SHORTEST(w,x,y,z) == Z_BITS(w,x,y,z))*2u)
#define LONGEST_AXIS(   w,x,y,z  ) uint(uint(LONGEST( w,x,y,z) == Y_BITS(w,y)) + uint(LONGEST( w,x,y,z) == Z_BITS(w,x,y,z))*2u)

#define BITS2_SHIFTS(   w,x,y,z  ) uint(uint(BITS2_LENGTH(w,x,y,z) - uint(SHORTEST_AXIS(w,x,y,z)==2u || (SHORTEST_AXIS(w,x,y,z)==1u && LONGEST_AXIS(w,x,y,z) == 2u))) * uint(MIDDLE(w,x,y,z)>0))
#define BITS2_OFFSET(   w,x,y,z,i) uint(BITS3_LENGTH(w,x,y,z)*3u + SHORTEST_AXIS(w,x,y,z) + uint(i)*2u)
#define BITS2_LMASK(    w,x,y,z,i) uint((1u << BITS2_OFFSET(w,x,y,z,i))-1u)
#define BITS2_HMASK(    w,x,y,z,i) uint(~ BITS2_LMASK(w,x,y,z,i))

#define BITS1_COUNT(    w,x,y,z  ) uint(BITS1_LENGTH(w,x,y,z) - uint(SHORTEST_AXIS(w,x,y,z)==1u && LONGEST_AXIS(w,x,y,z)==0u) + uint(SHORTEST_AXIS(w,x,y,z)==1u && LONGEST_AXIS(w,x,y,z)==2u))
#define BITS1_OFFSET(   w,x,y,z  ) uint(BITS2_LENGTH(w,x,y,z)*2u + BITS3_LENGTH(w,x,y,z)*3u - uint(SHORTEST_AXIS(w,x,y,z)==1u && LONGEST_AXIS(w,x,y,z) == 2u))
#define BITS1_DST_MASK( w,x,y,z  ) uint((1u<<(BITS3_LENGTH(w,x,y,z)*3u + 2_BITS_LENGTH(w,x,y,z)*2u - uint(LONGEST_AXIS(w,x,y,z)==2u))) -1u)
#define BITS1_SRC_SHIFT(w,x,y,z  ) uint(LONGEST)
#define BITS1_SRC_MASK   

#define EXPAND_BITS0(V,O) ((uint(V) * (0x00010001u<<uint(O))) & (0xFF0000FFu<<uint(O)))
#define EXPAND_BITS1(V,O) ((uint(V) * (0x00000101u<<uint(O))) & (0x0F00F00Fu<<uint(O)))
#define EXPAND_BITS2(V,O) ((uint(V) * (0x00000011u<<uint(O))) & (0xC30C30C3u<<uint(O)))
#define EXPAND_BITS3(V,O) ((uint(V) * (0x00000005u<<uint(O))) & (0x49249249u<<uint(O)))

#define EXPAND_BITS(V,O) EXPAND_BITS3(EXPAND_BITS2(EXPAND_BITS1(EXPAND_BITS0(V,0),0),0),O)

#define MERGE_BITS(A,B,C) (EXPAND_BITS((A),0) | EXPAND_BITS((B),1) | EXPAND_BITS((C),2))

inline uint morton(uint3 v){

  uint res = MERGE_BITS(v[0],v[1],v[2]);

  //if(0  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 0))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 0));
  if(1  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 1))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 1));
  //if(2  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 2))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 2));
  //if(3  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 3))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 3));
  //if(4  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 4))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 4));
  //if(5  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 5))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 5));
  //if(6  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 6))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 6));
  //if(7  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 7))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 7));
  //if(8  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 8))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 8));
  //if(9  < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 9))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z, 9));
  //if(10 < BITS2_SHIFTS(WARP,WINDOW_X,WINDOW_Y,MIN_Z))res = ((res & BITS2_HMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z,10))>>1u) | (res & BITS2_LMASK(WARP,WINDOW_X,WINDOW_Y,MIN_Z,10));

  /*
  if(BITS1_COUNT != 0)
    res = uint(res & MIDDLE_LMASK) | uint((v[LONGEST_AXIS]<<uint(SHORTEST*2u + MIDDLE)) & MIDDLE_HMASK);

  */
  return res;
}

