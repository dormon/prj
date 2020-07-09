#include<iostream>
#include <bitset>

#include "glsl.h"

uvec3 getOld(uint xBits,uint yBits,uint zBits){
  const uint bitLength[3] = {
    min(min(xBits,yBits),zBits),
    max(max(min(xBits,yBits),min(xBits,zBits)),min(yBits,zBits)),
    max(max(xBits,yBits),zBits),
  };
  
  const uint bitTogether[3] = {
    bitLength[0]                   ,
    uint(bitLength[1]-bitLength[0]),
    uint(bitLength[2]-bitLength[1]),
  };
  
  const uint longestAxis  = 
    bitLength[2]==zBits?2u:
    bitLength[2]==yBits?1u:
    0u;

  const uint shortestAxis = 
    bitLength[0]==xBits?0u:
    bitLength[0]==yBits?1u:
    2u;
  const uint middleAxis   = 
    shortestAxis==0u?(longestAxis==1u?2u:1u):
    (shortestAxis==1u?(longestAxis==0u?2u:0u):
    (longestAxis==0u?1u:0u));
  
  const uint twoLongest[] = {
    0==shortestAxis?1u:0u,
    2==shortestAxis?1u:2u,
  };
  
  (void)middleAxis;

  const uint allBits         = xBits + yBits + zBits;
  
  const uint noAxis          = 3u;
  
#if 0
  std::cerr << "longestAxis : " << longestAxis  << std::endl;
  std::cerr << "middleAxis  : " << middleAxis   << std::endl;
  std::cerr << "shortestAxis: " << shortestAxis << std::endl;
  std::cerr << "twoLongest [0]: " << twoLongest [0] << std::endl;
  std::cerr << "twoLongest [1]: " << twoLongest [1] << std::endl;
  std::cerr << "bitTogether[0]: " << bitTogether[0] << std::endl;
  std::cerr << "bitTogether[1]: " << bitTogether[1] << std::endl;
  std::cerr << "bitTogether[2]: " << bitTogether[2] << std::endl;
#endif

  #define WHICH_AXIS_ON_BIT(b)   uint(                                           \
      b<bitTogether[0]*3u                  ?b%3u                                :\
      b<bitTogether[0]*3u+bitTogether[1]*2u?twoLongest[(b-bitTogether[0]*3u)%2u]:\
      b<allBits                            ?longestAxis                         :\
      noAxis)
  #define IS_BIT_THIS_AXIS(b,a)  uint(WHICH_AXIS_ON_BIT(b) == a)
  #define SET_BIT(b,a)           uint(IS_BIT_THIS_AXIS(b,a) << b)
  
  const uvec3 bitPosition = uvec3(
    SET_BIT( 0u,0u)| 
    SET_BIT( 1u,0u)| 
    SET_BIT( 2u,0u)| 
    SET_BIT( 3u,0u)| 
    SET_BIT( 4u,0u)| 
    SET_BIT( 5u,0u)| 
    SET_BIT( 6u,0u)| 
    SET_BIT( 7u,0u)| 
    SET_BIT( 8u,0u)| 
    SET_BIT( 9u,0u)| 
    SET_BIT(10u,0u)| 
    SET_BIT(11u,0u)| 
    SET_BIT(12u,0u)| 
    SET_BIT(13u,0u)| 
    SET_BIT(14u,0u)| 
    SET_BIT(15u,0u)| 
    SET_BIT(16u,0u)| 
    SET_BIT(17u,0u)| 
    SET_BIT(18u,0u)| 
    SET_BIT(19u,0u)| 
    SET_BIT(20u,0u)| 
    SET_BIT(21u,0u)| 
    SET_BIT(22u,0u)| 
    SET_BIT(23u,0u)| 
    SET_BIT(24u,0u)| 
    SET_BIT(25u,0u)| 
    SET_BIT(26u,0u)| 
    SET_BIT(27u,0u)| 
    SET_BIT(28u,0u)| 
    SET_BIT(29u,0u)| 
    SET_BIT(30u,0u)| 
    SET_BIT(31u,0u),
    SET_BIT( 0u,1u)| 
    SET_BIT( 1u,1u)| 
    SET_BIT( 2u,1u)| 
    SET_BIT( 3u,1u)| 
    SET_BIT( 4u,1u)| 
    SET_BIT( 5u,1u)| 
    SET_BIT( 6u,1u)| 
    SET_BIT( 7u,1u)| 
    SET_BIT( 8u,1u)| 
    SET_BIT( 9u,1u)| 
    SET_BIT(10u,1u)| 
    SET_BIT(11u,1u)| 
    SET_BIT(12u,1u)| 
    SET_BIT(13u,1u)| 
    SET_BIT(14u,1u)| 
    SET_BIT(15u,1u)| 
    SET_BIT(16u,1u)| 
    SET_BIT(17u,1u)| 
    SET_BIT(18u,1u)| 
    SET_BIT(19u,1u)| 
    SET_BIT(20u,1u)| 
    SET_BIT(21u,1u)| 
    SET_BIT(22u,1u)| 
    SET_BIT(23u,1u)| 
    SET_BIT(24u,1u)| 
    SET_BIT(25u,1u)| 
    SET_BIT(26u,1u)| 
    SET_BIT(27u,1u)| 
    SET_BIT(28u,1u)| 
    SET_BIT(29u,1u)| 
    SET_BIT(30u,1u)| 
    SET_BIT(31u,1u),
    SET_BIT( 0u,2u)| 
    SET_BIT( 1u,2u)| 
    SET_BIT( 2u,2u)| 
    SET_BIT( 3u,2u)| 
    SET_BIT( 4u,2u)| 
    SET_BIT( 5u,2u)| 
    SET_BIT( 6u,2u)| 
    SET_BIT( 7u,2u)| 
    SET_BIT( 8u,2u)| 
    SET_BIT( 9u,2u)| 
    SET_BIT(10u,2u)| 
    SET_BIT(11u,2u)| 
    SET_BIT(12u,2u)| 
    SET_BIT(13u,2u)| 
    SET_BIT(14u,2u)| 
    SET_BIT(15u,2u)| 
    SET_BIT(16u,2u)| 
    SET_BIT(17u,2u)| 
    SET_BIT(18u,2u)| 
    SET_BIT(19u,2u)| 
    SET_BIT(20u,2u)| 
    SET_BIT(21u,2u)| 
    SET_BIT(22u,2u)| 
    SET_BIT(23u,2u)| 
    SET_BIT(24u,2u)| 
    SET_BIT(25u,2u)| 
    SET_BIT(26u,2u)| 
    SET_BIT(27u,2u)| 
    SET_BIT(28u,2u)| 
    SET_BIT(29u,2u)| 
    SET_BIT(30u,2u)| 
    SET_BIT(31u,2u)
  );
  return bitPosition;
}

uvec3 getNew(uint xBits,uint yBits,uint zBits){
  const uint bitLength[3] = {
    min(min(xBits,yBits),zBits),
    max(max(min(xBits,yBits),min(xBits,zBits)),min(yBits,zBits)),
    max(max(xBits,yBits),zBits),
  };
  
  const uint bitTogether[3] = {
    bitLength[0]                   ,
    uint(bitLength[1]-bitLength[0]),
    uint(bitLength[2]-bitLength[1]),
  };
  
  const uint longestAxis  = 
    bitLength[2]==zBits?2u:
    bitLength[2]==yBits?1u:
    0u;

  const uint shortestAxis = 
    bitLength[0]==xBits?0u:
    bitLength[0]==yBits?1u:
    2u;
  const uint middleAxis   = 
    shortestAxis==0u?(longestAxis==1u?2u:1u):
    (shortestAxis==1u?(longestAxis==0u?2u:0u):
    (longestAxis==0u?1u:0u));
  
  const uint twoLongest[] = {
    0==shortestAxis?1u:0u,
    2==shortestAxis?1u:2u,
  };

  
  const uint allBits         = xBits + yBits + zBits;
  
  const uint noAxis          = 3u;

  (void)bitLength;
  (void)bitTogether;
  (void)longestAxis;
  (void)middleAxis;
  (void)shortestAxis;
  (void)twoLongest;
  (void)allBits;
  (void)noAxis;

  #define GET_MASK(x) uint((x)==32u?0xffffffffu:uint(uint(1u<<(x))-1u))

  const uvec3 bitPosition = uvec3(
    ((0x49249249u<<0u) & GET_MASK(bitTogether[0]*3u))|
    (uint(shortestAxis != 0) * (((0x55555555u<<0u                     ) & GET_MASK(bitTogether[1]*2u)) << (bitTogether[0]*3u)))|
    ((GET_MASK(bitTogether[2]) << (bitTogether[0]*3u + bitTogether[1]*2u))*uint(longestAxis == 0u)),

    ((0x49249249u<<1u) & GET_MASK(bitTogether[0]*3u))|
    (uint(shortestAxis != 1) * (((0x55555555u<<uint(shortestAxis != 0)) & GET_MASK(bitTogether[1]*2u)) << (bitTogether[0]*3u)))|
    ((GET_MASK(bitTogether[2]) << (bitTogether[0]*3u + bitTogether[1]*2u))*uint(longestAxis == 1u)),

    ((0x49249249u<<2u) & GET_MASK(bitTogether[0]*3u))|
    (uint(shortestAxis != 2) * (((0x55555555u<<1u                     ) & GET_MASK(bitTogether[1]*2u)) << (bitTogether[0]*3u)))|
    ((GET_MASK(bitTogether[2]) << (bitTogether[0]*3u + bitTogether[1]*2u))*uint(longestAxis == 2u))
  );

  return bitPosition;
}

void myTest(uint xBits,uint yBits,uint zBits){
  auto oldBits = getOld(xBits,yBits,zBits);
  auto newBits = getNew(xBits,yBits,zBits);
  if(oldBits == newBits) return;
  std::cerr << xBits << " " << yBits << " " << zBits << ": " << std::endl;
  for(int i=0;i<3;++i)
    if(oldBits[i] != newBits[i]){
      std::cerr << "xyz"[i] << ": " << std::endl;
      std::cerr << "o " << std::bitset<32>(oldBits[i]) << std::endl;
      std::cerr << "n " << std::bitset<32>(newBits[i]) << std::endl;
      std::cerr << "d " << std::bitset<32>(oldBits[i]^newBits[i]) << std::endl;
    }
}

int main(int argc,char*argv[]){
#if 0
  auto vv = getOld(0,0,32);
  std::cerr << std::bitset<32>(vv[0]) << std::endl << std::bitset<32>(vv[1]) << std::endl << std::bitset<32>(vv[2]) << std::endl;
  std::cerr << std::endl;
  auto ww = getNew(0,0,32);
  std::cerr << std::bitset<32>(ww[0]) << std::endl << std::bitset<32>(ww[1]) << std::endl << std::bitset<32>(ww[2]) << std::endl;
  return 0;
#endif
  myTest(10,10,10);
  myTest(9,9,9);
  myTest(10,10,9);
  myTest(10,10,8);
  myTest(9,9,8);
  myTest(9,9,7);
  myTest(10,9,8);
  for(uint x=0;x<=10;++x)
    for(uint y=0;y<=10;++y)
      for(uint z=0;z<=10;++z)
        myTest(x,y,z);

  for(uint x=0;x<=32;++x)
    for(uint y=0;y<=32-x;++y)
      for(uint z=0;z<=32-x-y;++z)
        myTest(x,y,z);

  return 0;
}
