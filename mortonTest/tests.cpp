#include"catch.hpp"

#include<iostream>
#include <bitset>

struct uvec3{
  public:
    uint32_t      &operator[](size_t i)     {return data[i];}
    uint32_t const&operator[](size_t i)const{return data[i];}
    uint32_t      &x()     {return data[0];}
    uint32_t      &y()     {return data[1];}
    uint32_t      &z()     {return data[2];}
    uint32_t const&x()const{return data[0];}
    uint32_t const&y()const{return data[1];}
    uint32_t const&z()const{return data[2];}
    uvec3(uint32_t a,uint32_t b,uint32_t c){
      data[0] = a;
      data[1] = b;
      data[2] = c;
    }
    uvec3(){data[0]=0;data[1]=0;data[2]=0;}
    bool operator == (uvec3 const&v)const{
      if(data[0] != v.data[0])return false;
      if(data[1] != v.data[1])return false;
      if(data[2] != v.data[2])return false;
      return true;
    }
    friend std::ostream& operator<< (std::ostream& stream, const uvec3& o) {
      stream << "(" << o.data[0] << "," << o.data[1] << "," << o.data[2] << ")";
      return stream;
    }
  private:
    uint32_t data[3];
};



#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

#define uint uint32_t 

// m - length of 3 bits together
// n - length of 2 bits together
// o - length of 1 bit  alone
//
//                      bits1Count|bits2shifts|1bit_offset|2bit_offset
// ..*|   .**|.**|.**|       o    |     n-1   |   2n+3m   |  2i+2+3m
// .*.|   .**|.**|.**|       o    |     n-1   |   2n+3m   |  2i+2+3m
// ...|   .**|.**|.**|       o    |     n-1   |   2n+3m   |  2i+2+3m
// ###################################################################
// ..*|   *.*|*.*|*.*|       o-1  |     n     |   2n+3m+1 |  2i+1+3m
// *..|   *.*|*.*|*.*|       o+1  |     n-1   |   2n+3m-1 |  2i+1+3m
// ...|   *.*|*.*|*.*|       o    |     n     |   2n+3m   |  2i+1+3m
// ###################################################################
// .*.|   **.|**.|**.|       o    |     n     |   2n+3m   |  2i+0+3m
// *..|   **.|**.|**.|       o    |     n     |   2n+3m   |  2i+0+3m
// ...|   **.|**.|**.|       o    |     n     |   2n+3m   |  2i+0+3m
// ###################################################################
// ..*|   ...|...|...|       o    |     n     |   2n+3m   |  xxxxxxx
// .*.|   ...|...|...|       o    |     n     |   2n+3m   |  xxxxxxx
// ...|   ...|...|...|       o    |     n     |   2n+3m   |  xxxxxxx



template<uint32_t WARP=64u,uint32_t WINDOW_X=512u,uint32_t WINDOW_Y=512u,uint32_t MIN_Z=9u>
uint morton2(uvec3 v){
  const uint warpBits      = uint(ceil(log2(float(WARP))));
  const uint warpBitsX     = 3u;
  const uint warpBitsY     = 3u;
  const uint warpX         = uint(1u<<warpBitsX);
  const uint warpY         = uint(1u<<warpBitsY);
  const uint clustersX     = uint(WINDOW_X/warpX) + uint(WINDOW_X%warpX != 0u);
  const uint clustersY     = uint(WINDOW_Y/warpY) + uint(WINDOW_Y%warpY != 0u);
  const uint xBits         = uint(ceil(log2(float(clustersX))));
  const uint yBits         = uint(ceil(log2(float(clustersY))));
  const uint zBits         = MIN_Z>0?MIN_Z:max(max(xBits,yBits),MIN_Z);
  uint res = 0;
  uint counters[3] = {0,0,0};
  const uint limits[3] = {xBits,yBits,zBits};
  const uint allBits = xBits + yBits + zBits;
  uint a = 0;
  for(uint b=0;b<allBits;++b){
    res |= ((v[a]>>counters[a])&1u) << b;
    counters[a]++;
    a = (a+1u)%3u;
    if(counters[a] >= limits[a])a = (a+1u)%3u;
    if(counters[a] >= limits[a])a = (a+1u)%3u;
  }
  return res;
}

template<uint32_t WARP=64u,uint32_t WINDOW_X=512u,uint32_t WINDOW_Y=512u,uint32_t MIN_Z=9u>
uvec3 demorton2(uint v){
  //std::cerr << std::bitset<32>(v) << std::endl;
  const uint warpBits      = uint(ceil(log2(float(WARP))));
  const uint warpBitsX     = 3u;
  const uint warpBitsY     = 3u;
  const uint warpX         = uint(1u<<warpBitsX);
  const uint warpY         = uint(1u<<warpBitsY);
  const uint clustersX     = uint(WINDOW_X/warpX) + uint(WINDOW_X%warpX != 0u);
  const uint clustersY     = uint(WINDOW_Y/warpY) + uint(WINDOW_Y%warpY != 0u);
  const uint xBits         = uint(ceil(log2(float(clustersX))));
  const uint yBits         = uint(ceil(log2(float(clustersY))));
  const uint zBits         = MIN_Z>0?MIN_Z:max(max(xBits,yBits),MIN_Z);
  uvec3 res;
  uint counters[3] = {0,0,0};
  const uint limits[3] = {xBits,yBits,zBits};
  const uint allBits = xBits + yBits + zBits;
  uint a = 0;
  for(uint b=0;b<allBits;++b){
    res[a] |= ((v>>b)&1u) << counters[a];
    counters[a]++;
    a = (a+1u)%3u;
    if(counters[a] >= limits[a])a = (a+1u)%3u;
    if(counters[a] >= limits[a])a = (a+1u)%3u;
  }
  return res;
}


template<uint32_t WARP=64u,uint32_t WINDOW_X=512u,uint32_t WINDOW_Y=512u,uint32_t MIN_Z=9u,bool BIT2 = true,bool BIT1 = true,uint32_t TILE_X=8u,uint32_t TILE_Y=8u>
uint morton(uvec3 v){
  //const uint warpBits      = uint(ceil(log2(float(WARP))));
  //const uint warpBitsX     = uint(warpBits/2u) + uint(warpBits%2 != 0u);
  //const uint warpBitsY     = uint(warpBits-warpBitsX);
  //const uint warpX         = uint(1u<<warpBitsX);
  //const uint warpY         = uint(1u<<warpBitsY);
  //const uint clustersX     = uint(WINDOW_X/warpX) + uint(WINDOW_X%warpX != 0u);
  //const uint clustersY     = uint(WINDOW_Y/warpY) + uint(WINDOW_Y%warpY != 0u);
  const uint clustersX     = uint(WINDOW_X/TILE_X) + uint(WINDOW_X%TILE_X != 0u);
  const uint clustersY     = uint(WINDOW_Y/TILE_Y) + uint(WINDOW_Y%TILE_Y != 0u);
  const uint xBits         = uint(ceil(log2(float(clustersX))));
  const uint yBits         = uint(ceil(log2(float(clustersY))));
  const uint zBits         = MIN_Z>0?MIN_Z:max(max(xBits,yBits),MIN_Z);
  const uint shortest      = min(min(xBits,yBits),zBits);
  const uint middle        = max(max(min(xBits,yBits),min(xBits,zBits)),min(yBits,zBits));
  const uint longest       = max(max(xBits,yBits),zBits);
  const uint bits3Length   = shortest;
  const uint bits2Length   = uint(middle-shortest);
  const uint bits1Length   = uint(longest-middle);
  const uint shortestAxis  = uint(uint(shortest == yBits) + uint(shortest == zBits)*2u);
  const uint longestAxis   = uint(uint(longest  == yBits) + uint(longest  == zBits)*2u);
  const uint shortestZ     = uint(shortestAxis == 2u);
  const uint shortestY     = uint(shortestAxis == 1u);
  const uint longestZ      = uint(longestAxis == 2u);
  const uint longestX      = uint(longestAxis == 0u);
  const uint isMiddle      = uint(middle > 0);
  const uint isLongest     = uint(longest > 0);
  const uint bits2Shifts   = uint(uint(bits2Length - uint(shortestZ | (shortestY & longestZ))) * isMiddle);

  const uint bits2OffsetB   = bits3Length*3u + shortestAxis;
  const uint bits2Offset00  = bits2OffsetB + 2* 0;
  const uint bits2Offset01  = bits2OffsetB + 2* 1;
  const uint bits2Offset02  = bits2OffsetB + 2* 2;
  const uint bits2Offset03  = bits2OffsetB + 2* 3;
  const uint bits2Offset04  = bits2OffsetB + 2* 4;
  const uint bits2Offset05  = bits2OffsetB + 2* 5;
  const uint bits2Offset06  = bits2OffsetB + 2* 6;
  const uint bits2Offset07  = bits2OffsetB + 2* 7;
  const uint bits2Offset08  = bits2OffsetB + 2* 8;
  const uint bits2Offset09  = bits2OffsetB + 2* 9;
  const uint bits2Offset10  = bits2OffsetB + 2*10;

  const uint bits2LMask00 = uint((1u << bits2Offset00)-1u);
  const uint bits2LMask01 = uint((1u << bits2Offset01)-1u);
  const uint bits2LMask02 = uint((1u << bits2Offset02)-1u);
  const uint bits2LMask03 = uint((1u << bits2Offset03)-1u);
  const uint bits2LMask04 = uint((1u << bits2Offset04)-1u);
  const uint bits2LMask05 = uint((1u << bits2Offset05)-1u);
  const uint bits2LMask06 = uint((1u << bits2Offset06)-1u);
  const uint bits2LMask07 = uint((1u << bits2Offset07)-1u);
  const uint bits2LMask08 = uint((1u << bits2Offset08)-1u);
  const uint bits2LMask09 = uint((1u << bits2Offset09)-1u);
  const uint bits2LMask10 = uint((1u << bits2Offset10)-1u);

  const uint bits2HMask00 = (~bits2LMask00)<<1u;
  const uint bits2HMask01 = (~bits2LMask01)<<1u;
  const uint bits2HMask02 = (~bits2LMask02)<<1u;
  const uint bits2HMask03 = (~bits2LMask03)<<1u;
  const uint bits2HMask04 = (~bits2LMask04)<<1u;
  const uint bits2HMask05 = (~bits2LMask05)<<1u;
  const uint bits2HMask06 = (~bits2LMask06)<<1u;
  const uint bits2HMask07 = (~bits2LMask07)<<1u;
  const uint bits2HMask08 = (~bits2LMask08)<<1u;
  const uint bits2HMask09 = (~bits2LMask09)<<1u;
  const uint bits2HMask10 = (~bits2LMask10)<<1u;

  uint res = 0;
  uint vv;
  vv   = (v[0] * (0x00010001u<<0u)) & (0xFF0000FFu<<0u);
  vv   = (vv   * (0x00000101u<<0u)) & (0x0F00F00Fu<<0u);
  vv   = (vv   * (0x00000011u<<0u)) & (0xC30C30C3u<<0u);
  res |= (vv   * (0x00000005u<<0u)) & (0x49249249u<<0u);

  vv   = (v[1] * (0x00010001u<<0u)) & (0xFF0000FFu<<0u);
  vv   = (vv   * (0x00000101u<<0u)) & (0x0F00F00Fu<<0u);
  vv   = (vv   * (0x00000011u<<0u)) & (0xC30C30C3u<<0u);
  res |= (vv   * (0x00000005u<<1u)) & (0x49249249u<<1u);

  vv   = (v[2] * (0x00010001u<<0u)) & (0xFF0000FFu<<0u);
  vv   = (vv   * (0x00000101u<<0u)) & (0x0F00F00Fu<<0u);
  vv   = (vv   * (0x00000011u<<0u)) & (0xC30C30C3u<<0u);
  res |= (vv   * (0x00000005u<<2u)) & (0x49249249u<<2u);


  /*
  std::cerr << std::endl;

  std::cerr << "mer          : " << std::bitset<32>(res) << std::endl;
  std::cerr << "bits2Length  : " << bits2Length << std::endl;
  std::cerr << "bits2Shifts  : " << bits2Shifts << std::endl;
  */

  if constexpr(BIT2){
  if(0  < bits2Shifts)res = ((res & bits2HMask00)>>1u) | (res & bits2LMask00);
  if(1  < bits2Shifts)res = ((res & bits2HMask01)>>1u) | (res & bits2LMask01);
  if(2  < bits2Shifts)res = ((res & bits2HMask02)>>1u) | (res & bits2LMask02);
  if(3  < bits2Shifts)res = ((res & bits2HMask03)>>1u) | (res & bits2LMask03);
  if(4  < bits2Shifts)res = ((res & bits2HMask04)>>1u) | (res & bits2LMask04);
  if(5  < bits2Shifts)res = ((res & bits2HMask05)>>1u) | (res & bits2LMask05);
  if(6  < bits2Shifts)res = ((res & bits2HMask06)>>1u) | (res & bits2LMask06);
  if(7  < bits2Shifts)res = ((res & bits2HMask07)>>1u) | (res & bits2LMask07);
  if(8  < bits2Shifts)res = ((res & bits2HMask08)>>1u) | (res & bits2LMask08);
  if(9  < bits2Shifts)res = ((res & bits2HMask09)>>1u) | (res & bits2LMask09);
  if(10 < bits2Shifts)res = ((res & bits2HMask10)>>1u) | (res & bits2LMask10);
  }
  
  //std::cerr << "2b           : " << std::bitset<32>(res) << std::endl;

  const uint bits1Count    = uint(bits1Length - uint(shortestY & longestX) + uint(shortestY & longestZ)) * isLongest;
  const uint bits1used     = longest - bits1Count;
  const uint bits1DstMask  = uint((1u<<(bits3Length*3u + bits2Length*2u + uint(shortestY & longestX) - uint(longestZ & shortestY))) -1u);
  const uint bits1SrcShift = bits3Length*3u + bits2Length*2u - uint(shortestY & longestZ) + uint(shortestY & longestX)  - bits1used;
  const uint bits1SrcMask  = ~((1u<<bits1used)-1u);
  /*
  std::cerr << "bits1DstMask : " << std::bitset<32>(bits1DstMask)  << std::endl;
  std::cerr << "bits1SrcMask : " << std::bitset<32>(bits1SrcMask)  << std::endl;
  std::cerr << "v[lo]        : " << std::bitset<32>(v[longestAxis]) << std::endl;
  std::cerr << "v[lo]&m      : " << std::bitset<32>(v[longestAxis]&bits1SrcMask) << std::endl;
  std::cerr << "bits1SrcShift: " << std::dec << bits1SrcShift << std::endl;
  std::cerr << "(v[lo]&m)<<s : " << std::bitset<32>((v[longestAxis]&bits1SrcMask)<<bits1SrcShift) << std::endl;
  std::cerr << "bits1Count   : " << std::dec << bits1Count    << std::endl;
  std::cerr << "bits3Length  : "             << bits3Length   << std::endl;
  std::cerr << "bits2Length  : "             << bits2Length   << std::endl;
  std::cerr << "bits1used    : "             << bits1used     << std::endl;
  std::cerr << "shortest     : "             << shortest      << std::endl;
  std::cerr << "middle       : "             << middle        << std::endl;
  std::cerr << "longest      : "             << longest       << std::endl;
  std::cerr << "xBits: " << xBits << std::endl;
  std::cerr << "yBits: " << yBits << std::endl;
  std::cerr << "zBits: " << zBits << std::endl;
  std::cerr << "res          : " << std::bitset<32>(res) << std::endl;
  std::cerr << "res&m        : " << std::bitset<32>(res&bits1DstMask) << std::endl;
  // */


  if constexpr(BIT1){

  if(bits1Count != 0)
    res = uint(res & bits1DstMask) | uint((v[longestAxis]&bits1SrcMask)<<bits1SrcShift);
  }

  //std::cerr << "1b           : " << std::bitset<32>(res) << std::endl;
  
  return res;
}


template<uint32_t WARP=64u,uint32_t WINDOW_X=512u,uint32_t WINDOW_Y=512u,uint32_t MIN_Z=9u,bool BIT2 = true,bool BIT1 = true,uint32_t TILE_X=8u,uint32_t TILE_Y=8u>
uvec3 demorton(uint res){
  
  //const uint warpBits      = uint(ceil(log2(float(WARP))));
  //const uint warpBitsX     = uint(warpBits/2u) + uint(warpBits%2 != 0u);
  //const uint warpBitsY     = uint(warpBits-warpBitsX);
  //const uint warpX         = uint(1u<<warpBitsX);
  //const uint warpY         = uint(1u<<warpBitsY);
  //const uint clustersX     = uint(WINDOW_X/warpX) + uint(WINDOW_X%warpX != 0u);
  //const uint clustersY     = uint(WINDOW_Y/warpY) + uint(WINDOW_Y%warpY != 0u);
  const uint clustersX     = uint(WINDOW_X/TILE_X) + uint(WINDOW_X%TILE_X != 0u);
  const uint clustersY     = uint(WINDOW_Y/TILE_Y) + uint(WINDOW_Y%TILE_Y != 0u);
  const uint xBits         = uint(ceil(log2(float(clustersX))));
  const uint yBits         = uint(ceil(log2(float(clustersY))));
  const uint zBits         = MIN_Z>0?MIN_Z:max(max(xBits,yBits),MIN_Z);
  const uint shortest      = min(min(xBits,yBits),zBits);
  const uint middle        = max(max(min(xBits,yBits),min(xBits,zBits)),min(yBits,zBits));
  const uint longest       = max(max(xBits,yBits),zBits);
  const uint bits3Length   = shortest;
  const uint bits2Length   = uint(middle-shortest);
  const uint bits1Length   = uint(longest-middle);
  const uint shortestAxis  = uint(uint(shortest == yBits) + uint(shortest == zBits)*2u);
  const uint longestAxis   = uint(uint(longest  == yBits) + uint(longest  == zBits)*2u);
  const uint shortestZ     = uint(shortestAxis == 2u);
  const uint shortestY     = uint(shortestAxis == 1u);
  const uint longestZ      = uint(longestAxis == 2u);
  const uint longestX      = uint(longestAxis == 0u);
  const uint isMiddle      = uint(middle > 0);
  const uint isLongest     = uint(longest > 0);
  const uint bits2Shifts   = uint(uint(bits2Length - uint(shortestZ | (shortestY & longestZ))) * isMiddle);

  const uint bits2OffsetB   = bits3Length*3u + shortestAxis;
  const uint bits2Offset00  = bits2OffsetB + 2* 0;
  const uint bits2Offset01  = bits2OffsetB + 2* 1;
  const uint bits2Offset02  = bits2OffsetB + 2* 2;
  const uint bits2Offset03  = bits2OffsetB + 2* 3;
  const uint bits2Offset04  = bits2OffsetB + 2* 4;
  const uint bits2Offset05  = bits2OffsetB + 2* 5;
  const uint bits2Offset06  = bits2OffsetB + 2* 6;
  const uint bits2Offset07  = bits2OffsetB + 2* 7;
  const uint bits2Offset08  = bits2OffsetB + 2* 8;
  const uint bits2Offset09  = bits2OffsetB + 2* 9;
  const uint bits2Offset10  = bits2OffsetB + 2*10;

  const uint bits2LMask00 = uint((1u << bits2Offset00)-1u);
  const uint bits2LMask01 = uint((1u << bits2Offset01)-1u);
  const uint bits2LMask02 = uint((1u << bits2Offset02)-1u);
  const uint bits2LMask03 = uint((1u << bits2Offset03)-1u);
  const uint bits2LMask04 = uint((1u << bits2Offset04)-1u);
  const uint bits2LMask05 = uint((1u << bits2Offset05)-1u);
  const uint bits2LMask06 = uint((1u << bits2Offset06)-1u);
  const uint bits2LMask07 = uint((1u << bits2Offset07)-1u);
  const uint bits2LMask08 = uint((1u << bits2Offset08)-1u);
  const uint bits2LMask09 = uint((1u << bits2Offset09)-1u);
  const uint bits2LMask10 = uint((1u << bits2Offset10)-1u);

  const uint bits2HMask00 = (~bits2LMask00)<<1u;
  const uint bits2HMask01 = (~bits2LMask01)<<1u;
  const uint bits2HMask02 = (~bits2LMask02)<<1u;
  const uint bits2HMask03 = (~bits2LMask03)<<1u;
  const uint bits2HMask04 = (~bits2LMask04)<<1u;
  const uint bits2HMask05 = (~bits2LMask05)<<1u;
  const uint bits2HMask06 = (~bits2LMask06)<<1u;
  const uint bits2HMask07 = (~bits2LMask07)<<1u;
  const uint bits2HMask08 = (~bits2LMask08)<<1u;
  const uint bits2HMask09 = (~bits2LMask09)<<1u;
  const uint bits2HMask10 = (~bits2LMask10)<<1u;


  const uint bits1Count    = uint(bits1Length - uint(shortestY & longestX) + uint(shortestY & longestZ)) * isLongest;
  const uint bits1used     = longest - bits1Count;
  const uint bits1DstMask  = uint((1u<<(bits3Length*3u + bits2Length*2u + uint(shortestY & longestX) - uint(longestZ & shortestY))) -1u);
  const uint bits1SrcShift = bits3Length*3u + bits2Length*2u - uint(shortestY & longestZ) + uint(shortestY & longestX)  - bits1used;
  const uint bits1SrcMask  = ~((1u<<bits1used)-1u);

  uvec3 v;

  //std::cerr << "resA         : " << std::bitset<32>(res) << std::endl;
  uint last = 0;
  if(bits1Count != 0){
    //res = uint(res & bits1DstMask) | uint((v[longestAxis]&bits1SrcMask)<<bits1SrcShift);
    last |= (res >> bits1SrcShift) & bits1SrcMask;
    res &= bits1DstMask;
  }
  //std::cerr << "resB         : " << std::bitset<32>(res) << std::endl;
  //std::cerr << "last         : " << std::bitset<32>(last) << std::endl;

  if(10 < bits2Shifts)res = ((res<<1u)&bits2HMask10) | (res & bits2LMask10);
  if(9  < bits2Shifts)res = ((res<<1u)&bits2HMask09) | (res & bits2LMask09);
  if(8  < bits2Shifts)res = ((res<<1u)&bits2HMask08) | (res & bits2LMask08);
  if(7  < bits2Shifts)res = ((res<<1u)&bits2HMask07) | (res & bits2LMask07);
  if(6  < bits2Shifts)res = ((res<<1u)&bits2HMask06) | (res & bits2LMask06);
  if(5  < bits2Shifts)res = ((res<<1u)&bits2HMask05) | (res & bits2LMask05);
  if(4  < bits2Shifts)res = ((res<<1u)&bits2HMask04) | (res & bits2LMask04);
  if(3  < bits2Shifts)res = ((res<<1u)&bits2HMask03) | (res & bits2LMask03);
  if(2  < bits2Shifts)res = ((res<<1u)&bits2HMask02) | (res & bits2LMask02);
  if(1  < bits2Shifts)res = ((res<<1u)&bits2HMask01) | (res & bits2LMask01);
  if(0  < bits2Shifts)res = ((res<<1u)&bits2HMask00) | (res & bits2LMask00);
  //std::cerr << "resC         : " << std::bitset<32>(res) << std::endl;



  v[2] = (res & 0x24924924u)>>2u;
  v[1] = (res & 0x92492492u)>>1u;
  v[0] = (res & 0x49249249u)>>0u;

  //std::cerr << "v[0]A: " <<  std::bitset<32>(v[0]) << std::endl;
  //std::cerr << "v[1]A: " <<  std::bitset<32>(v[1]) << std::endl;
  //std::cerr << "v[2]A: " <<  std::bitset<32>(v[2]) << std::endl;


  v[2] = (v[2] | (v[2]>> 2u)) & 0xc30c30c3u;
  v[2] = (v[2] | (v[2]>> 4u)) & 0x0f00f00fu;
  v[2] = (v[2] | (v[2]>> 8u)) & 0xff0000ffu;
  v[2] = (v[2] | (v[2]>>16u)) & 0x0000ffffu;

  v[1] = (v[1] | (v[1]>> 2u)) & 0xc30c30c3u;
  v[1] = (v[1] | (v[1]>> 4u)) & 0x0f00f00fu;
  v[1] = (v[1] | (v[1]>> 8u)) & 0xff0000ffu;
  v[1] = (v[1] | (v[1]>>16u)) & 0x0000ffffu;

  v[0] = (v[0] | (v[0]>> 2u)) & 0xc30c30c3u;
  v[0] = (v[0] | (v[0]>> 4u)) & 0x0f00f00fu;
  v[0] = (v[0] | (v[0]>> 8u)) & 0xff0000ffu;
  v[0] = (v[0] | (v[0]>>16u)) & 0x0000ffffu;
 
  //std::cerr << "v[0]B: " <<  std::bitset<32>(v[0]) << std::endl;
  //std::cerr << "v[1]B: " <<  std::bitset<32>(v[1]) << std::endl;
  //std::cerr << "v[2]B: " <<  std::bitset<32>(v[2]) << std::endl;

  if(bits1Count != 0){
  v[longestAxis] |= last;
  }

  //std::cerr << "v[0]C: " <<  std::bitset<32>(v[0]) << std::endl;
  //std::cerr << "v[1]C: " <<  std::bitset<32>(v[1]) << std::endl;
  //std::cerr << "v[2]C: " <<  std::bitset<32>(v[2]) << std::endl;
  
  return v;
}


#line 152
TEST_CASE("morton")
{

  REQUIRE(morton<64,4096*2,4096*2,10,false,false>({0x3ff,0x3ff,0x3ff}) == 0b111111111111111111111111111111);
  REQUIRE(morton<64,4096*2,4096*2,10,false,false>({0x3ff,0x000,0x000}) == 0b001001001001001001001001001001);
  REQUIRE(morton<64,4096*2,4096*2,10,false,false>({0x000,0x3ff,0x000}) == 0b010010010010010010010010010010);
  REQUIRE(morton<64,4096*2,4096*2,10,false,false>({0x000,0x000,0x3ff}) == 0b100100100100100100100100100100);
  REQUIRE(morton<64,4096*2,4096*2,10,false,false>({0x3ff,0x3ff,0x000}) == 0b011011011011011011011011011011);
  REQUIRE(morton<64,4096*2,4096*2,10,false,false>({0x000,0x3ff,0x3ff}) == 0b110110110110110110110110110110);
  REQUIRE(morton<64,4096*2,4096*2,10,false,false>({0x3ff,0x000,0x3ff}) == 0b101101101101101101101101101101);


  REQUIRE(uvec3{0x3ff,0x3ff,0x3ff} == demorton<64,4096*2,4096*2,10,false,false>(0b111111111111111111111111111111));
  REQUIRE(uvec3{0x3ff,0x000,0x000} == demorton<64,4096*2,4096*2,10,false,false>(0b001001001001001001001001001001));
  REQUIRE(uvec3{0x000,0x3ff,0x000} == demorton<64,4096*2,4096*2,10,false,false>(0b010010010010010010010010010010));
  REQUIRE(uvec3{0x000,0x000,0x3ff} == demorton<64,4096*2,4096*2,10,false,false>(0b100100100100100100100100100100));
  REQUIRE(uvec3{0x3ff,0x3ff,0x000} == demorton<64,4096*2,4096*2,10,false,false>(0b011011011011011011011011011011));
  REQUIRE(uvec3{0x000,0x3ff,0x3ff} == demorton<64,4096*2,4096*2,10,false,false>(0b110110110110110110110110110110));
  REQUIRE(uvec3{0x3ff,0x000,0x3ff} == demorton<64,4096*2,4096*2,10,false,false>(0b101101101101101101101101101101));


  REQUIRE(morton<64, 512, 512, 6            >({0x000,0x000,0x000}) ==                                0);
  REQUIRE(morton<64, 512, 512, 6            >({0x03f,0x03f,0x03f}) ==                          0x3ffff);
  REQUIRE(morton<64, 512, 512, 6            >({0x03f,0x000,0x000}) ==             0b001001001001001001);
  REQUIRE(morton<64, 512, 512, 6            >({0x000,0x03f,0x000}) ==             0b010010010010010010);
  REQUIRE(morton<64, 512, 512, 6            >({0x000,0x000,0x03f}) ==             0b100100100100100100);
  REQUIRE(morton<64, 512, 512,10            >({0x000,0x000,0x3ff}) ==         0b1111100100100100100100);
  REQUIRE(morton<64, 512, 512,10            >({0x03f,0x03f,0x200}) ==         0b1000011011011011011011);
  REQUIRE(morton<64,  16,  16,10            >({0x000,0x000,0x3ff}) ==                   0b111111111100);

  REQUIRE(uvec3({0x000,0x000,0x000}) == demorton<64, 512, 512, 6            >(                               0));
  REQUIRE(uvec3({0x03f,0x03f,0x03f}) == demorton<64, 512, 512, 6            >(                         0x3ffff));
  REQUIRE(uvec3({0x03f,0x000,0x000}) == demorton<64, 512, 512, 6            >(            0b001001001001001001));
  REQUIRE(uvec3({0x000,0x03f,0x000}) == demorton<64, 512, 512, 6            >(            0b010010010010010010));
  REQUIRE(uvec3({0x000,0x000,0x03f}) == demorton<64, 512, 512, 6            >(            0b100100100100100100));
  REQUIRE(uvec3({0x000,0x000,0x3ff}) == demorton<64, 512, 512,10            >(        0b1111100100100100100100));
  REQUIRE(uvec3({0x03f,0x03f,0x200}) == demorton<64, 512, 512,10            >(        0b1000011011011011011011));
  REQUIRE(uvec3({0x000,0x000,0x3ff}) == demorton<64,  16,  16,10            >(                  0b111111111100));



  // morton
  // z>y>x
  REQUIRE(morton<64,  16,4096,10,false,false>({0x000,0x000,0x3ff}) == 0b100100100100100100100100100100);
  REQUIRE(morton<64,  16,4096,10, true,false>({0x000,0x000,0x3ff}) ==         0b1001010101010101010100);
  REQUIRE(morton<64,  16,4096,10, true,true >({0x000,0x000,0x3ff}) ==           0b11010101010101010100);
  REQUIRE(morton<64,  16,4096,10, true,true >({0x001,0x1ff,0x3ff}) ==           0b11111111111111111111);
  REQUIRE(morton<64,  16,4096,10, true,true >({0x001,0x000,0x000}) ==                              0b1);
  REQUIRE(morton<64,  16,4096,10, true,true >({0x000,0x1ff,0x000}) ==             0b101010101010101010);
  // z>>y>x
  REQUIRE(morton<64,  16, 256,10, true,true >({0x001,0x000,0x000}) ==                              0b1);
  REQUIRE(morton<64,  16, 256,10, true,true >({0x000,0x01f,0x000}) ==                     0b1010101010);
  REQUIRE(morton<64,  16, 256,10, true,true >({0x000,0x000,0x3ff}) ==               0b1111110101010100);

  // z=y>x
  REQUIRE(morton<64,  16,4096, 9, true,true >({0x000,0x1ff,0x1ff}) ==            0b1111111111111111110);
  REQUIRE(morton<64,  16,4096, 9, true,true >({0x000,0x000,0x1ff}) ==            0b1010101010101010100);
  REQUIRE(morton<64,  16,4096, 9, true,true >({0x000,0x1ff,0x000}) ==            0b0101010101010101010);
  REQUIRE(morton<64,  16,4096, 9, true,true >({0x001,0x000,0x000}) ==                              0b1);

  // z>x>y
  REQUIRE(morton<64,4096,  16,10,false,false>({0x000,0x000,0x3ff}) == 0b100100100100100100100100100100);
  REQUIRE(morton<64,4096,  16,10, true,false>({0x000,0x000,0x3ff}) ==        0b10010010101010101010100);
  REQUIRE(morton<64,4096,  16,10, true,true >({0x000,0x000,0x3ff}) ==           0b11010101010101010100);
  REQUIRE(morton<64,4096,  16,10, true,true >({0x1ff,0x001,0x3ff}) ==           0b11111111111111111111);
  REQUIRE(morton<64,4096,  16,10, true,true >({0x1ff,0x000,0x000}) ==           0b00101010101010101001);
  REQUIRE(morton<64,4096,  16,10, true,true >({0x000,0x001,0x000}) ==                             0b10);

  // z>>x>y
  REQUIRE(morton<64, 256,  16,10, true,true >({0x01f,0x000,0x000}) ==                     0b1010101001);
  REQUIRE(morton<64, 256,  16,10, true,true >({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64, 256,  16,10, true,true >({0x000,0x000,0x3ff}) ==               0b1111110101010100);

  // z=x>y
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x1ff,0x000,0x1ff}) ==            0b1111111111111111101);
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x000,0x000,0x1ff}) ==            0b1010101010101010100);
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x001,0x000,0x000}) ==                              0b1);

  //demorton
  // z>y>x
  REQUIRE(uvec3{0x000,0x000,0x3ff} == demorton<64,  16,4096,10, true,true >(          0b11010101010101010100));
  REQUIRE(uvec3{0x001,0x1ff,0x3ff} == demorton<64,  16,4096,10, true,true >(          0b11111111111111111111));
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,  16,4096,10, true,true >(                             0b1));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,  16,4096,10, true,true >(            0b101010101010101010));
  // z>>y>x                                                           
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,  16, 256,10, true,true >(                             0b1));
  REQUIRE(uvec3{0x000,0x01f,0x000} == demorton<64,  16, 256,10, true,true >(                    0b1010101010));
  REQUIRE(uvec3{0x000,0x000,0x3ff} == demorton<64,  16, 256,10, true,true >(              0b1111110101010100));
                                                                                                          
  // z=y>x                                                            
  REQUIRE(uvec3{0x000,0x1ff,0x1ff} == demorton<64,  16,4096, 9, true,true >(           0b1111111111111111110));
  REQUIRE(uvec3{0x000,0x000,0x1ff} == demorton<64,  16,4096, 9, true,true >(           0b1010101010101010100));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,  16,4096, 9, true,true >(           0b0101010101010101010));
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,  16,4096, 9, true,true >(                             0b1));
                                                                                                          
  // z>x>y                                                            
  REQUIRE(uvec3{0x000,0x000,0x3ff} == demorton<64,4096,  16,10, true,true >(          0b11010101010101010100));
  REQUIRE(uvec3{0x1ff,0x001,0x3ff} == demorton<64,4096,  16,10, true,true >(          0b11111111111111111111));
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16,10, true,true >(          0b00101010101010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64,4096,  16,10, true,true >(                            0b10));
                                                                                                         
  // z>>x>y                                                           
  REQUIRE(uvec3{0x01f,0x000,0x000} == demorton<64, 256,  16,10, true,true >(                    0b1010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64, 256,  16,10, true,true >(                            0b10));
  REQUIRE(uvec3{0x000,0x000,0x3ff} == demorton<64, 256,  16,10, true,true >(              0b1111110101010100));
                                                                                                            
  // z=x>y                                                              
  REQUIRE(uvec3{0x1ff,0x000,0x1ff} == demorton<64,4096,  16, 9, true,true >(           0b1111111111111111101));
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16, 9, true,true >(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x000,0x1ff} == demorton<64,4096,  16, 9, true,true >(           0b1010101010101010100));
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,4096,  16, 9, true,true >(                             0b1));








  //morton
  // y>z>x
  REQUIRE(morton<64,16  ,4096, 8            >({0x001,0x000,0x000}) ==                               0b1);
  REQUIRE(morton<64,16  ,4096, 8            >({0x000,0x1ff,0x000}) ==              0b101010101010101010);
  REQUIRE(morton<64,16  ,4096, 8            >({0x000,0x000,0x0ff}) ==               0b10101010101010100);

  // y>>z>x
  REQUIRE(morton<64,16  ,4096, 5            >({0x001,0x000,0x000}) ==                               0b1);
  REQUIRE(morton<64,16  ,4096, 5            >({0x000,0x1ff,0x000}) ==                 0b111101010101010);
  REQUIRE(morton<64,16  ,4096, 5            >({0x000,0x000,0x01f}) ==                     0b10101010100);

  // y=z>x
  REQUIRE(morton<64,16  ,4096, 9            >({0x001,0x000,0x000}) ==                               0b1);
  REQUIRE(morton<64,16  ,4096, 9            >({0x000,0x1ff,0x000}) ==              0b101010101010101010);
  REQUIRE(morton<64,16  ,4096, 9            >({0x000,0x000,0x1ff}) ==             0b1010101010101010100);

  // y>x>z
  REQUIRE(morton<64,2048,4096, 1            >({0x0ff,0x000,0x000}) ==                0b1010101010101001);
  REQUIRE(morton<64,2048,4096, 1            >({0x000,0x1ff,0x000}) ==              0b110101010101010010);
  REQUIRE(morton<64,2048,4096, 1            >({0x000,0x000,0x001}) ==                             0b100);

  // y>>x>z
  REQUIRE(morton<64, 256,4096, 1            >({0x01f,0x000,0x000}) ==                      0b1010101001);
  REQUIRE(morton<64, 256,4096, 1            >({0x000,0x1ff,0x000}) ==                 0b111110101010010);
  REQUIRE(morton<64, 256,4096, 1            >({0x000,0x000,0x001}) ==                             0b100);

  // y=x>z
  REQUIRE(morton<64,4096,4096, 1            >({0x1ff,0x000,0x000}) ==              0b101010101010101001);
  REQUIRE(morton<64,4096,4096, 1            >({0x000,0x1ff,0x000}) ==             0b1010101010101010010);
  REQUIRE(morton<64,4096,4096, 1            >({0x000,0x000,0x001}) ==                             0b100);


  //demorton
  //y>z>x
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,16  ,4096, 8            >(                              0b1));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,16  ,4096, 8            >(             0b101010101010101010));
  REQUIRE(uvec3{0x000,0x000,0x0ff} == demorton<64,16  ,4096, 8            >(              0b10101010101010100));
                                                                          
  //y>>z>x                            
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,16  ,4096, 5            >(                              0b1));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,16  ,4096, 5            >(                0b111101010101010));
  REQUIRE(uvec3{0x000,0x000,0x01f} == demorton<64,16  ,4096, 5            >(                    0b10101010100));
                                                                          
  //y=z>x                                                                 
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,16  ,4096, 9            >(                              0b1));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,16  ,4096, 9            >(             0b101010101010101010));
  REQUIRE(uvec3{0x000,0x000,0x1ff} == demorton<64,16  ,4096, 9            >(            0b1010101010101010100));
                                                                          
  //y>x>z                                                                 
  REQUIRE(uvec3{0x0ff,0x000,0x000} == demorton<64,2048,4096, 1            >(               0b1010101010101001));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,2048,4096, 1            >(             0b110101010101010010));
  REQUIRE(uvec3{0x000,0x000,0x001} == demorton<64,2048,4096, 1            >(                            0b100));
                                                                          
  //y>>x>z                            
  REQUIRE(uvec3{0x01f,0x000,0x000} == demorton<64, 256,4096, 1            >(                     0b1010101001));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64, 256,4096, 1            >(                0b111110101010010));
  REQUIRE(uvec3{0x000,0x000,0x001} == demorton<64, 256,4096, 1            >(                            0b100));
                                                                          
  //y=x>z                                                                 
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,4096, 1            >(             0b101010101010101001));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,4096,4096, 1            >(            0b1010101010101010010));
  REQUIRE(uvec3{0x000,0x000,0x001} == demorton<64,4096,4096, 1            >(                            0b100));



  //morton
  //x>y>z
  REQUIRE(morton<64,4096,2048, 1, true,true >({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,2048, 1, true,true >({0x000,0x0ff,0x000}) ==              0b10101010101010010);
  REQUIRE(morton<64,4096,2048, 1, true,true >({0x000,0x000,0x001}) ==                            0b100);

  //x>>y>z
  REQUIRE(morton<64,4096, 256, 1, true,true >({0x1ff,0x000,0x000}) ==                0b111101010101001);
  REQUIRE(morton<64,4096, 256, 1, true,true >({0x000,0x01f,0x000}) ==                    0b10101010010);
  REQUIRE(morton<64,4096, 256, 1, true,true >({0x000,0x000,0x001}) ==                            0b100);

  //x=y>z
  REQUIRE(morton<64,4096,4096, 1, true,true >({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,4096, 1, true,true >({0x000,0x1ff,0x000}) ==            0b1010101010101010010);
  REQUIRE(morton<64,4096,4096, 1, true,true >({0x000,0x000,0x001}) ==                            0b100);

  //x>z>y
  REQUIRE(morton<64,4096,  16, 8, true,true >({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,  16, 8, true,true >({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64,4096,  16, 8, true,true >({0x000,0x000,0x0ff}) ==              0b10101010101010100);

  //x>>z>y
  REQUIRE(morton<64,4096,  16, 4, true,true >({0x1ff,0x000,0x000}) ==                 0b11111010101001);
  REQUIRE(morton<64,4096,  16, 4, true,true >({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64,4096,  16, 4, true,true >({0x000,0x000,0x00f}) ==                      0b101010100);

  //x=z>y
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x000,0x000,0x1ff}) ==            0b1010101010101010100);

  //demorton
  //x>y>z
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,2048, 1, true,true >(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x0ff,0x000} == demorton<64,4096,2048, 1, true,true >(             0b10101010101010010));
  REQUIRE(uvec3{0x000,0x000,0x001} == demorton<64,4096,2048, 1, true,true >(                           0b100));
                                              
  //x>>y>z
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096, 256, 1, true,true >(               0b111101010101001));
  REQUIRE(uvec3{0x000,0x01f,0x000} == demorton<64,4096, 256, 1, true,true >(                   0b10101010010));
  REQUIRE(uvec3{0x000,0x000,0x001} == demorton<64,4096, 256, 1, true,true >(                           0b100));
                                              
  //x=y>z                                     
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,4096, 1, true,true >(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,4096,4096, 1, true,true >(           0b1010101010101010010));
  REQUIRE(uvec3{0x000,0x000,0x001} == demorton<64,4096,4096, 1, true,true >(                           0b100));
                                              
  //x>z>y                                     
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16, 8, true,true >(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64,4096,  16, 8, true,true >(                            0b10));
  REQUIRE(uvec3{0x000,0x000,0x0ff} == demorton<64,4096,  16, 8, true,true >(             0b10101010101010100));
                                              
  //x>>z>y
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16, 4, true,true >(                0b11111010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64,4096,  16, 4, true,true >(                            0b10));
  REQUIRE(uvec3{0x000,0x000,0x00f} == demorton<64,4096,  16, 4, true,true >(                     0b101010100));
                                              
  //x=z>y                                     
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16, 9, true,true >(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64,4096,  16, 9, true,true >(                            0b10));
  REQUIRE(uvec3{0x000,0x000,0x1ff} == demorton<64,4096,  16, 9, true,true >(           0b1010101010101010100));


  for(uint32_t y=0;y<64;++y){
    for(uint32_t x=0;x<64;++x){
      for(uint32_t z=0;z<512;++z)
        REQUIRE(morton<64,512,512,9>({x,y,z}) == morton2<64,512,512,9>({x,y,z}));
    }
  }

  //auto const testDE = [&](uvec3 v){
  //  std::cerr << "v  : " << v << std::endl;
  //  std::cerr << "v.x: " << std::bitset<32>(v[0]) << std::endl;
  //  std::cerr << "v.y: " << std::bitset<32>(v[1]) << std::endl;
  //  std::cerr << "v.z: " << std::bitset<32>(v[2]) << std::endl;
  //  std::cerr << "M  : " << std::bitset<32>(morton <64,512,512,9>(v)) << std::endl;
  //  std::cerr << "M2 : " << std::bitset<32>(morton2<64,512,512,9>(v))<< std::endl;
  //  std::cerr << "D  : " << demorton2<64,512,512,9>(morton2<64,512,512,9>(v)) << std::endl;
  //};

  ////testDE({64,0,0});
  for(uint32_t y=0;y<64;++y){
    for(uint32_t x=0;x<64;++x){
      for(uint32_t z=0;z<512;++z)
        REQUIRE(demorton2<64,512,512,9>(morton2<64,512,512,9>({x,y,z})) == uvec3(x,y,z));
    }
  }

  for(uint32_t y=0;y<64;++y){
    for(uint32_t x=0;x<64;++x){
      for(uint32_t z=0;z<512;++z)
        REQUIRE(demorton2<32,512,512,9>(morton2<32,512,512,9>({x,y,z})) == uvec3(x,y,z));
    }
  }


  for(uint32_t y=0;y<64;++y){
    for(uint32_t x=0;x<64;++x){
      for(uint32_t z=0;z<64;++z)
        REQUIRE(demorton<64,512,512,6>(morton<64,512,512,6>({x,y,z})) == uvec3(x,y,z));
    }
  }

  for(uint32_t y=0;y<64;++y){
    for(uint32_t x=0;x<64;++x){
      for(uint32_t z=0;z<512;++z)
        REQUIRE(demorton<64,512,512,9>(morton<64,512,512,9>({x,y,z})) == uvec3(x,y,z));
    }
  }


  for(uint32_t y=0;y<64;++y){
    for(uint32_t x=0;x<64;++x){
      for(uint32_t z=0;z<512;++z)
        REQUIRE(demorton<32,512,512,9>(morton<32,512,512,9>({x,y,z})) == uvec3(x,y,z));
    }
  }

}
