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
    uvec3(uint32_t a){data[0]=a;data[1]=a;data[2]=a;}
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

uint32_t clamp(uint32_t a,uint32_t b,uint32_t c){
  if(a<b)return b;
  if(a>c)return c;
  return a;
}

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



template<uint32_t WARP=64u,uint32_t WINDOW_X=512u,uint32_t WINDOW_Y=512u,uint32_t MIN_Z_BITS=9u,uint32_t TILE_X=8u,uint32_t TILE_Y=8u>
uint morton2(uvec3 v){
  const uint clustersX     = uint(WINDOW_X/TILE_X) + uint(WINDOW_X%TILE_X != 0u);
  const uint clustersY     = uint(WINDOW_Y/TILE_Y) + uint(WINDOW_Y%TILE_Y != 0u);
  const uint xBits         = uint(ceil(log2(float(clustersX))));
  const uint yBits         = uint(ceil(log2(float(clustersY))));
  const uint zBits         = MIN_Z_BITS>0?MIN_Z_BITS:max(max(xBits,yBits),MIN_Z_BITS);
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

template<uint32_t WARP=64u,uint32_t WINDOW_X=512u,uint32_t WINDOW_Y=512u,uint32_t MIN_Z_BITS=9u,uint32_t TILE_X=8u,uint32_t TILE_Y=8u>
uvec3 demorton2(uint v){
  //std::cerr << std::bitset<32>(v) << std::endl;
  const uint clustersX     = uint(WINDOW_X/TILE_X) + uint(WINDOW_X%TILE_X != 0u);
  const uint clustersY     = uint(WINDOW_Y/TILE_Y) + uint(WINDOW_Y%TILE_Y != 0u);
  const uint xBits         = uint(ceil(log2(float(clustersX))));
  const uint yBits         = uint(ceil(log2(float(clustersY))));
  const uint zBits         = MIN_Z_BITS>0?MIN_Z_BITS:max(max(xBits,yBits),MIN_Z_BITS);
  uvec3 res = uvec3(0);
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


template<uint32_t WARP=64u,uint32_t WINDOW_X=512u,uint32_t WINDOW_Y=512u,uint32_t MIN_Z_BITS=9u,uint32_t TILE_X=8u,uint32_t TILE_Y=8u>
uint morton(uvec3 v){

#define DIV_ROUND_UP(x,y) uint(uint(uint(x)/uint(y)) + uint((uint(x) % uint(y)) != 0u))
#define BITS_REQUIRED(x) uint(ceil(log2(float(x))))
#line 31
const uint tileBitsX       = BITS_REQUIRED(TILE_X);
const uint tileBitsY       = BITS_REQUIRED(TILE_Y);
const uint tileMaskX       = uint(TILE_X-1u);
const uint tileMaskY       = uint(TILE_Y-1u);
const uint warpBits        = BITS_REQUIRED(WARP);
const uint clustersX       = DIV_ROUND_UP(WINDOW_X,TILE_X);
const uint clustersY       = DIV_ROUND_UP(WINDOW_Y,TILE_Y);
const uint xBits           = BITS_REQUIRED(clustersX);
const uint yBits           = BITS_REQUIRED(clustersY);
const uint zBits           = MIN_Z_BITS>0?MIN_Z_BITS:max(max(xBits,yBits),MIN_Z_BITS);
const uint clustersZ       = 1u << zBits;
const uint allBits         = xBits + yBits + zBits;
const uint nofLevels       = DIV_ROUND_UP(allBits,warpBits);
const uint uintsPerWarp    = uint(WARP/32u);
const uint noAxis          = 3u;
#line 47
const uint bitLength[3] = {
  min(min(xBits,yBits),zBits),
  max(max(min(xBits,yBits),min(xBits,zBits)),min(yBits,zBits)),
  max(max(xBits,yBits),zBits),
};
#line 53
const uint bitTogether[3] = {
  bitLength[0]                   ,
  uint(bitLength[1]-bitLength[0]),
  uint(bitLength[2]-bitLength[1]),
};

const uint longestAxis  = clamp(uint(uint(bitLength[2] == yBits) + uint(bitLength[2] == zBits)*2u),0u,2u);
const uint middleAxis   = clamp(uint(uint(bitLength[1] == yBits) + uint(bitLength[1] == zBits)*2u),0u,2u);
const uint shortestAxis = clamp(uint(uint(bitLength[0] == yBits) + uint(bitLength[0] == zBits)*2u),0u,2u);

#define QUANTIZE_Z(z) clamp(uint(log(-z/NEAR) / log(1.f+2.f*tan(FOVY/2.f)/clustersY)),0u,clustersZ-1u)
#define CLUSTER_TO_Z(i) (-NEAR * exp((i)*log(1.f + 2.f*tan(FOVY/2.f)/clustersY)))

// | 2n/(R-L)  0          (R+L)/(R-L)  0          |   |x|
// | 0         2n/(T-B)   (T+B)/(T-B)  0          | * |y|
// | 0         0         -(f+n)/(f-n)  -2fn/(f-n) |   |z|
// | 0         0         -1            0          |   |1|
//
// ndcdepth<-1,1> = (-(f+n)/(f-n)*z  -2fn/(f-n)*1)/(-z)
// d = (-(f+n)/(f-n)*z  -2fn/(f-n)*1)/(-z)
// d = (f+n)/(f-n) + 2fn/(f-n)/z
// d-(f+n)/(f-n) = 2fn/(f-n)/z
// z = 2fn/(f-n) / (d-(f+n)/(f-n))
// z = 2fn/(f-n) / ((d*(f-n)-(f+n))/(f-n))
// z = 2fn / ((d*(f-n)-(f+n)))
// z = 2fn / ((d*(f-n)-f-n)))
//

#ifdef FAR_IS_INFINITE
  #define DEPTH_TO_Z(d) (2.f*NEAR    /((d) - 1.f))
  #define Z_TO_DEPTH(z) ((2.f*NEAR)/(z)+1.f)
#else
  #define DEPTH_TO_Z(d) (2.f*NEAR*FAR/((d)*(FAR-NEAR)-FAR-NEAR))
  #define Z_TO_DEPTH(z) (((2.f*NEAR*FAR/(z))+FAR+NEAR)/(FAR-NEAR))
#endif

const uint twoLongest[] = {
  middleAxis ,
  longestAxis,
};


  const uint shortestZ     = uint(shortestAxis == 2u);
  const uint shortestY     = uint(shortestAxis == 1u);
  const uint isMiddle      = uint(bitTogether[1] > 0u);
  const uint isLongest     = uint(bitTogether[2] > 0u);
  const uint longestZ      = uint(longestAxis == 2u) * isLongest;
  const uint longestX      = uint(longestAxis == 0u) * isLongest;

  const uint bits2Shifts   = uint(uint(bitTogether[1] - uint(shortestZ | (shortestY & longestZ))) * isMiddle);

  const uint bits2OffsetB   = bitTogether[0]*3u + shortestAxis;
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

  const uint bits1Count    = uint(bitTogether[2] - uint(shortestY & longestX) + uint(shortestY & longestZ)) * isLongest;
  const uint bits1used     = bitLength[2] - bits1Count;
  const uint bits1DstMask  = uint((1u<<(bitTogether[0]*3u + bitTogether[1]*2u + uint(shortestY & longestX) - uint(longestZ & shortestY))) -1u);
  const uint bits1SrcShift = bitTogether[0]*3u + bitTogether[1]*2u - uint(shortestY & longestZ) + uint(shortestY & longestX)  - bits1used;
  const uint bits1SrcMask  = ~((1u<<bits1used)-1u);

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

  if(bits1Count != 0)
    res = uint(res & bits1DstMask) | uint((v[longestAxis]&bits1SrcMask)<<bits1SrcShift);

  return res;
#if 0


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
  // */

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

  /*
  std::cerr << "2b           : " << std::bitset<32>(res) << std::endl;
  // */

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


  if(bits1Count != 0)
    res = uint(res & bits1DstMask) | uint((v[longestAxis]&bits1SrcMask)<<bits1SrcShift);

  /*
  std::cerr << "1b           : " << std::bitset<32>(res) << std::endl;
  // */
  
  return res;
#endif
}


template<uint32_t WARP=64u,uint32_t WINDOW_X=512u,uint32_t WINDOW_Y=512u,uint32_t MIN_Z_BITS=9u,uint32_t TILE_X=8u,uint32_t TILE_Y=8u>
uvec3 demorton(uint res){
  const uint clustersX     = uint((WINDOW_X)/(TILE_X)) + uint(((WINDOW_X)%(TILE_X)) != 0u);
  const uint clustersY     = uint((WINDOW_Y)/(TILE_Y)) + uint(((WINDOW_Y)%(TILE_Y)) != 0u);
  const uint xBits         = uint(ceil(log2(float(clustersX))));
  const uint yBits         = uint(ceil(log2(float(clustersY))));
  const uint zBits         = (MIN_Z_BITS)>0u?(MIN_Z_BITS):max(max(xBits,yBits),(MIN_Z_BITS));
  const uint shortest      = min(min(xBits,yBits),zBits);
  const uint middle        = max(max(min(xBits,yBits),min(xBits,zBits)),min(yBits,zBits));
  const uint longest       = max(max(xBits,yBits),zBits);
  const uint bits3Length   = shortest;
  const uint bits2Length   = uint(middle-shortest);
  const uint bits1Length   = uint(longest-middle);
  const uint shortestAxis  = uint(uint(shortest == yBits) + uint(shortest == zBits)*2u);
  const uint longestAxis   = clamp(uint(uint(longest  == yBits) + uint(longest  == zBits)*2u),0u,2u);
  const uint shortestZ     = uint(shortestAxis == 2u);
  const uint shortestY     = uint(shortestAxis == 1u);
  const uint isMiddle      = uint(bits2Length > 0u);
  const uint isLongest     = uint(bits1Length > 0u);
  const uint longestZ      = uint(longestAxis == 2u) * isLongest;
  const uint longestX      = uint(longestAxis == 0u) * isLongest;
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

  uvec3 v = uvec3(0);

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
  REQUIRE(morton<64,  16,4096,10>({0x000,0x000,0x3ff}) ==           0b11010101010101010100);
  REQUIRE(morton<64,  16,4096,10>({0x001,0x1ff,0x3ff}) ==           0b11111111111111111111);
  REQUIRE(morton<64,  16,4096,10>({0x001,0x000,0x000}) ==                              0b1);
  REQUIRE(morton<64,  16,4096,10>({0x000,0x1ff,0x000}) ==             0b101010101010101010);
  // z>>y>x
  REQUIRE(morton<64,  16, 256,10>({0x001,0x000,0x000}) ==                              0b1);
  REQUIRE(morton<64,  16, 256,10>({0x000,0x01f,0x000}) ==                     0b1010101010);
  REQUIRE(morton<64,  16, 256,10>({0x000,0x000,0x3ff}) ==               0b1111110101010100);

  // z=y>x
  REQUIRE(morton<64,  16,4096, 9>({0x000,0x1ff,0x1ff}) ==            0b1111111111111111110);
  REQUIRE(morton<64,  16,4096, 9>({0x000,0x000,0x1ff}) ==            0b1010101010101010100);
  REQUIRE(morton<64,  16,4096, 9>({0x000,0x1ff,0x000}) ==            0b0101010101010101010);
  REQUIRE(morton<64,  16,4096, 9>({0x001,0x000,0x000}) ==                              0b1);

  // z>x>y
  REQUIRE(morton<64,4096,  16,10>({0x000,0x000,0x3ff}) ==           0b11010101010101010100);
  REQUIRE(morton<64,4096,  16,10>({0x1ff,0x001,0x3ff}) ==           0b11111111111111111111);
  REQUIRE(morton<64,4096,  16,10>({0x1ff,0x000,0x000}) ==           0b00101010101010101001);
  REQUIRE(morton<64,4096,  16,10>({0x000,0x001,0x000}) ==                             0b10);

  // z>>x>y
  REQUIRE(morton<64, 256,  16,10>({0x01f,0x000,0x000}) ==                     0b1010101001);
  REQUIRE(morton<64, 256,  16,10>({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64, 256,  16,10>({0x000,0x000,0x3ff}) ==               0b1111110101010100);

  // z=x>y
  REQUIRE(morton<64,4096,  16, 9>({0x1ff,0x000,0x1ff}) ==            0b1111111111111111101);//<<<<<<<<
  REQUIRE(morton<64,4096,  16, 9>({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,  16, 9>({0x000,0x000,0x1ff}) ==            0b1010101010101010100);
  REQUIRE(morton<64,4096,  16, 9>({0x001,0x000,0x000}) ==                              0b1);

  //demorton
  // z>y>x
  REQUIRE(uvec3{0x000,0x000,0x3ff} == demorton<64,  16,4096,10>(          0b11010101010101010100));
  REQUIRE(uvec3{0x001,0x1ff,0x3ff} == demorton<64,  16,4096,10>(          0b11111111111111111111));
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,  16,4096,10>(                             0b1));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,  16,4096,10>(            0b101010101010101010));
  // z>>y>x                                                   
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,  16, 256,10>(                             0b1));
  REQUIRE(uvec3{0x000,0x01f,0x000} == demorton<64,  16, 256,10>(                    0b1010101010));
  REQUIRE(uvec3{0x000,0x000,0x3ff} == demorton<64,  16, 256,10>(              0b1111110101010100));
                                                                                              
  // z=y>x                                                    
  REQUIRE(uvec3{0x000,0x1ff,0x1ff} == demorton<64,  16,4096, 9>(           0b1111111111111111110));
  REQUIRE(uvec3{0x000,0x000,0x1ff} == demorton<64,  16,4096, 9>(           0b1010101010101010100));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,  16,4096, 9>(           0b0101010101010101010));
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,  16,4096, 9>(                             0b1));
                                                                                              
  // z>x>y                                                    
  REQUIRE(uvec3{0x000,0x000,0x3ff} == demorton<64,4096,  16,10>(          0b11010101010101010100));
  REQUIRE(uvec3{0x1ff,0x001,0x3ff} == demorton<64,4096,  16,10>(          0b11111111111111111111));
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16,10>(          0b00101010101010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64,4096,  16,10>(                            0b10));
                                                                                             
  // z>>x>y                                                   
  REQUIRE(uvec3{0x01f,0x000,0x000} == demorton<64, 256,  16,10>(                    0b1010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64, 256,  16,10>(                            0b10));
  REQUIRE(uvec3{0x000,0x000,0x3ff} == demorton<64, 256,  16,10>(              0b1111110101010100));
                                                                                                
  // z=x>y                                                    
  REQUIRE(uvec3{0x1ff,0x000,0x1ff} == demorton<64,4096,  16, 9>(           0b1111111111111111101));
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16, 9>(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x000,0x1ff} == demorton<64,4096,  16, 9>(           0b1010101010101010100));
  REQUIRE(uvec3{0x001,0x000,0x000} == demorton<64,4096,  16, 9>(                             0b1));








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
  REQUIRE(morton<64,4096,2048, 1>({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,2048, 1>({0x000,0x0ff,0x000}) ==              0b10101010101010010);
  REQUIRE(morton<64,4096,2048, 1>({0x000,0x000,0x001}) ==                            0b100);

  //x>>y>z
  REQUIRE(morton<64,4096, 256, 1>({0x1ff,0x000,0x000}) ==                0b111101010101001);
  REQUIRE(morton<64,4096, 256, 1>({0x000,0x01f,0x000}) ==                    0b10101010010);
  REQUIRE(morton<64,4096, 256, 1>({0x000,0x000,0x001}) ==                            0b100);

  //x=y>z
  REQUIRE(morton<64,4096,4096, 1>({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,4096, 1>({0x000,0x1ff,0x000}) ==            0b1010101010101010010);
  REQUIRE(morton<64,4096,4096, 1>({0x000,0x000,0x001}) ==                            0b100);

  //x>z>y
  REQUIRE(morton<64,4096,  16, 8>({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,  16, 8>({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64,4096,  16, 8>({0x000,0x000,0x0ff}) ==              0b10101010101010100);

  //x>>z>y
  REQUIRE(morton<64,4096,  16, 4>({0x1ff,0x000,0x000}) ==                 0b11111010101001);
  REQUIRE(morton<64,4096,  16, 4>({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64,4096,  16, 4>({0x000,0x000,0x00f}) ==                      0b101010100);

  //x=z>y
  REQUIRE(morton<64,4096,  16, 9>({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,  16, 9>({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64,4096,  16, 9>({0x000,0x000,0x1ff}) ==            0b1010101010101010100);

  //demorton
  //x>y>z
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,2048, 1>(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x0ff,0x000} == demorton<64,4096,2048, 1>(             0b10101010101010010));
  REQUIRE(uvec3{0x000,0x000,0x001} == demorton<64,4096,2048, 1>(                           0b100));
                                              
  //x>>y>z
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096, 256, 1>(               0b111101010101001));
  REQUIRE(uvec3{0x000,0x01f,0x000} == demorton<64,4096, 256, 1>(                   0b10101010010));
  REQUIRE(uvec3{0x000,0x000,0x001} == demorton<64,4096, 256, 1>(                           0b100));
                                              
  //x=y>z                                     
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,4096, 1>(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x1ff,0x000} == demorton<64,4096,4096, 1>(           0b1010101010101010010));
  REQUIRE(uvec3{0x000,0x000,0x001} == demorton<64,4096,4096, 1>(                           0b100));
                                              
  //x>z>y                                     
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16, 8>(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64,4096,  16, 8>(                            0b10));
  REQUIRE(uvec3{0x000,0x000,0x0ff} == demorton<64,4096,  16, 8>(             0b10101010101010100));
                                              
  //x>>z>y
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16, 4>(                0b11111010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64,4096,  16, 4>(                            0b10));
  REQUIRE(uvec3{0x000,0x000,0x00f} == demorton<64,4096,  16, 4>(                     0b101010100));
                                              
  //x=z>y                                     
  REQUIRE(uvec3{0x1ff,0x000,0x000} == demorton<64,4096,  16, 9>(            0b101010101010101001));
  REQUIRE(uvec3{0x000,0x001,0x000} == demorton<64,4096,  16, 9>(                            0b10));
  REQUIRE(uvec3{0x000,0x000,0x1ff} == demorton<64,4096,  16, 9>(           0b1010101010101010100));


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
