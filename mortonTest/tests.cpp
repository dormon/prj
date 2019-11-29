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

#line 409
#define REQUIRED_BITS(x)  uint(ceil(log2(float(x))))
#define DIV_ROUND_UP(x,y) (uint(x/y) + uint(x%y != 0u))

#define UINTS_PER_WARP( w        ) uint((w)/32u)

#line 452
#define EXPAND_BITS0(V,O) ((uint(V) * (0x00010001u<<uint(O))) & (0xFF0000FFu<<uint(O)))
#define EXPAND_BITS1(V,O) ((uint(V) * (0x00000101u<<uint(O))) & (0x0F00F00Fu<<uint(O)))
#define EXPAND_BITS2(V,O) ((uint(V) * (0x00000011u<<uint(O))) & (0xC30C30C3u<<uint(O)))
#define EXPAND_BITS3(V,O) ((uint(V) * (0x00000005u<<uint(O))) & (0x49249249u<<uint(O)))

#define EXPAND_BITS(V,O) EXPAND_BITS3(EXPAND_BITS2(EXPAND_BITS1(EXPAND_BITS0(V,0),0),0),O)

#define MERGE_BITS(A,B,C) (EXPAND_BITS((A),0) | EXPAND_BITS((B),1) | EXPAND_BITS((C),2))

#line 123

template<uint32_t WARP=64u,uint32_t WINDOW_X=512u,uint32_t WINDOW_Y=512u,uint32_t MIN_Z=9u,bool BIT2 = true,bool BIT1 = true>
uint morton(uvec3 v){
  const uint warpBits      = REQUIRED_BITS(WARP);
  const uint warpBitsX     = DIV_ROUND_UP(warpBits,2u);
  const uint warpBitsY     = uint(warpBits-warpBitsX);
  const uint warpX         = uint(1u<<warpBitsX);
  const uint warpY         = uint(1u<<warpBitsY);
  const uint clustersX     = DIV_ROUND_UP(WINDOW_X,warpX);
  const uint clustersY     = DIV_ROUND_UP(WINDOW_Y,warpY);
  const uint xBits         = REQUIRED_BITS(clustersX);
  const uint yBits         = REQUIRED_BITS(clustersY);
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

  #define BITS2_OFFSET(i) uint(bits3Length*3u + shortestAxis + uint(i)*2u)
  #define BITS2_LMASK( i) uint((1u << BITS2_OFFSET(i))-1u)
  #define BITS2_HMASK( i) uint(~ BITS2_LMASK(i))
  

  uint res = MERGE_BITS(v[0],v[1],v[2]);

  std::cerr << std::endl;
  std::cerr << "bits2Length  : " << bits2Length << std::endl;
  std::cerr << "bits2Shifts  : " << bits2Shifts << std::endl;
  std::cerr << "mer          : " << std::bitset<32>(res) << std::endl;

  if constexpr(BIT2){
  if(0  < bits2Shifts)res = ((res & BITS2_HMASK( 0))>>1u) | (res & BITS2_LMASK( 0));
  if(1  < bits2Shifts)res = ((res & BITS2_HMASK( 1))>>1u) | (res & BITS2_LMASK( 1));
  if(2  < bits2Shifts)res = ((res & BITS2_HMASK( 2))>>1u) | (res & BITS2_LMASK( 2));
  if(3  < bits2Shifts)res = ((res & BITS2_HMASK( 3))>>1u) | (res & BITS2_LMASK( 3));
  if(4  < bits2Shifts)res = ((res & BITS2_HMASK( 4))>>1u) | (res & BITS2_LMASK( 4));
  if(5  < bits2Shifts)res = ((res & BITS2_HMASK( 5))>>1u) | (res & BITS2_LMASK( 5));
  if(6  < bits2Shifts)res = ((res & BITS2_HMASK( 6))>>1u) | (res & BITS2_LMASK( 6));
  if(7  < bits2Shifts)res = ((res & BITS2_HMASK( 7))>>1u) | (res & BITS2_LMASK( 7));
  if(8  < bits2Shifts)res = ((res & BITS2_HMASK( 8))>>1u) | (res & BITS2_LMASK( 8));
  if(9  < bits2Shifts)res = ((res & BITS2_HMASK( 9))>>1u) | (res & BITS2_LMASK( 9));
  if(10 < bits2Shifts)res = ((res & BITS2_HMASK(10))>>1u) | (res & BITS2_LMASK(10));
  }
  
  std::cerr << "2b           : " << std::bitset<32>(res) << std::endl;

  if constexpr(BIT1){
  const uint bits1Count    = uint(bits1Length - uint(shortestY & longestX) + uint(shortestY & longestZ)) * isLongest;
  const uint bits1used     = longest - bits1Count;
  const uint bits1DstMask  = uint((1u<<(bits3Length*3u + bits2Length*2u + uint(shortestY & longestX)/*- longestZ*/)) -1u);
  const uint bits1SrcShift = bits3Length*3u + bits2Length*2u - uint(shortestY & longestZ) + uint(shortestY & longestX)  - bits1used;
  const uint bits1SrcMask  = ~((1u<<bits1used)-1u);

  //*
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
  }

  std::cerr << "1b           : " << std::bitset<32>(res) << std::endl;
  
  return res;
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


  // x>y>z
  REQUIRE(morton<64,4096,2048, 1, true,true >({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,2048, 1, true,true >({0x000,0x0ff,0x000}) ==              0b10101010101010010);
  REQUIRE(morton<64,4096,2048, 1, true,true >({0x000,0x000,0x001}) ==                            0b100);

  // x>>y>z
  REQUIRE(morton<64,4096, 256, 1, true,true >({0x1ff,0x000,0x000}) ==                0b111101010101001);
  REQUIRE(morton<64,4096, 256, 1, true,true >({0x000,0x01f,0x000}) ==                    0b10101010010);
  REQUIRE(morton<64,4096, 256, 1, true,true >({0x000,0x000,0x001}) ==                            0b100);

  // x=y>z
  REQUIRE(morton<64,4096,4096, 1, true,true >({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,4096, 1, true,true >({0x000,0x1ff,0x000}) ==            0b1010101010101010010);
  REQUIRE(morton<64,4096,4096, 1, true,true >({0x000,0x000,0x001}) ==                            0b100);

  // x>z>y
  REQUIRE(morton<64,4096,  16, 8, true,true >({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,  16, 8, true,true >({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64,4096,  16, 8, true,true >({0x000,0x000,0x0ff}) ==              0b10101010101010100);

  // x>>z>y
  REQUIRE(morton<64,4096,  16, 4, true,true >({0x1ff,0x000,0x000}) ==                 0b11111010101001);
  REQUIRE(morton<64,4096,  16, 4, true,true >({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64,4096,  16, 4, true,true >({0x000,0x000,0x00f}) ==                      0b101010100);

  // x=z>y
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x1ff,0x000,0x000}) ==             0b101010101010101001);
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x000,0x001,0x000}) ==                             0b10);
  REQUIRE(morton<64,4096,  16, 9, true,true >({0x000,0x000,0x1ff}) ==            0b1010101010101010100);
}
