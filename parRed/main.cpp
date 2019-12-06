#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <iomanip>
#include <cmath>

#define WINDOW_X 512
#define WINDOW_Y 512
#define TILE_X 8
#define TILE_Y 8
#define MIN_Z_BITS 9

//float reductionArray[WARP*3];

using uint = uint32_t;


template<typename T>
T max(T const& x,T const& y){return x>y?x:y;}

template<uint32_t WARP = 64,bool VERBOSE = false>
void reduceAdvanced(std::vector<float>&reductionArray){
  //const uint warpBits        = uint(ceil(log2(float(WARP))));
  //const uint clustersX       = uint(WINDOW_X/TILE_X) + uint(WINDOW_X%TILE_X != 0u);
  //const uint clustersY       = uint(WINDOW_Y/TILE_Y) + uint(WINDOW_Y%TILE_Y != 0u);
  //const uint xBits           = uint(ceil(log2(float(clustersX))));
  //const uint yBits           = uint(ceil(log2(float(clustersY))));
  //const uint zBits           = MIN_Z_BITS>0?MIN_Z_BITS:max(max(xBits,yBits),uint(MIN_Z_BITS));
  //const uint allBits         = xBits + yBits + zBits;
  //const uint nofLevels       = uint(allBits/warpBits) + uint(allBits%warpBits != 0u);
  //const uint uintsPerWarp    = uint(WARP/32u);
  //const uint floatsPerAABB   = 6u;

  const uint halfWarp        = WARP / 2u;
  const uint halfWarpMask    = uint(halfWarp - 1u);

  float AB[WARP][2];                                                             
  uint  W[WARP];

  #define ab AB[gl_LocalInvocationIndex]
  #define w W[gl_LocalInvocationIndex]

#define PRINT(C,O)\
  for(size_t i=O;i<O+C;++i){\
    std::cerr << std::setfill(' ') << std::setw(3) << reductionArray[i] << " ";\
  }\
  std::cerr << std::endl

#define PRINTW()\
  for(size_t i=0;i<WARP;++i)\
    std::cerr << std::setfill(' ') << std::setw(3) << W[i] << " ";\
  std::cerr << std::endl

#define PARALLEL()\
  for(uint gl_LocalInvocationIndex=0;gl_LocalInvocationIndex<WARP;++gl_LocalInvocationIndex)


  if(VERBOSE){
  std::cerr << "input: " << std::endl;
  PRINT(WARP,WARP*0);
  PRINT(WARP,WARP*1);
  PRINT(WARP,WARP*2);
  std::cerr << std::endl;
  }


  //////(64t -> 32 min + 32 max) * 3
  PARALLEL(){
  ab[0] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)&halfWarpMask)+ 0u     ];       
  ab[1] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)&halfWarpMask)+halfWarp];       
  w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&halfWarp)!=0u)) > 0.f);
  }PARALLEL()
  reductionArray[WARP*0u+gl_LocalInvocationIndex] = ab[w];                         

  PARALLEL(){
  ab[0] = reductionArray[WARP*1u+(uint(gl_LocalInvocationIndex)&halfWarpMask)+ 0u     ];       
  ab[1] = reductionArray[WARP*1u+(uint(gl_LocalInvocationIndex)&halfWarpMask)+halfWarp];       
  w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&halfWarp)!=0u)) > 0.f);
  }PARALLEL()
  reductionArray[WARP*1u+gl_LocalInvocationIndex] = ab[w];                         

  PARALLEL(){
  ab[0] = reductionArray[WARP*2u+(uint(gl_LocalInvocationIndex)&halfWarpMask)+ 0u     ];       
  ab[1] = reductionArray[WARP*2u+(uint(gl_LocalInvocationIndex)&halfWarpMask)+halfWarp];       
  w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&halfWarp)!=0u)) > 0.f);
  }PARALLEL()
  reductionArray[WARP*2u + gl_LocalInvocationIndex] = ab[w];                         


  if(VERBOSE){
  std::cerr << "after 3 warps to find min maxs" << std::endl;
  PRINT(WARP/2,WARP/2*0);
  PRINT(WARP/2,WARP/2*1);
  PRINT(WARP/2,WARP/2*2);
  PRINT(WARP/2,WARP/2*3);
  PRINT(WARP/2,WARP/2*4);
  PRINT(WARP/2,WARP/2*5);
  std::cerr << std::endl;
  }


  /////(32t -> 16 minx + 16 maxx) + (32t -> 16miny + 16 maxy)
  PARALLEL(){
  ab[0] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 0u];                 
  ab[1] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 1u];                 
  w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&(WARP>>2u))!=0u)) > 0.f);
  }PARALLEL()
  reductionArray[WARP*0u + gl_LocalInvocationIndex] = ab[w];


  if(VERBOSE){
  std::cerr << "first of 2 warps" << std::endl;
  PRINT(WARP/4,WARP/4*0);
  PRINT(WARP/4,WARP/4*1);
  PRINT(WARP/4,WARP/4*2);
  PRINT(WARP/4,WARP/4*3);
  PRINT(WARP/4,WARP/4*4);
  PRINT(WARP/4,WARP/4*5);
  std::cerr << std::endl;
  }

  /////(32t -> 16 minz + 16 maxz)
  PARALLEL()
  if(gl_LocalInvocationIndex < (WARP>>1u)){
    ab[0] = reductionArray[WARP*2u+(uint(gl_LocalInvocationIndex)<<1u) + 0u];                 
    ab[1] = reductionArray[WARP*2u+(uint(gl_LocalInvocationIndex)<<1u) + 1u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&(WARP>>2u))!=0u)) > 0.f);
  }
  PARALLEL()
  if(gl_LocalInvocationIndex < (WARP>>1u))
    reductionArray[WARP*1u + gl_LocalInvocationIndex] = ab[w];

  if(VERBOSE){
  std::cerr << "second of 2 warps" << std::endl;
  PRINT(WARP/4,WARP/4*0);
  PRINT(WARP/4,WARP/4*1);
  PRINT(WARP/4,WARP/4*2);
  PRINT(WARP/4,WARP/4*3);
  PRINT(WARP/4,WARP/4*4);
  PRINT(WARP/4,WARP/4*5);
  std::cerr << std::endl;
  }

  /////(48t -> 8 minx + 8 maxx + 8miny + 8maxy + 8minz + 8maxz)
  PARALLEL()
  if((WARP>>3u) > 0u){
    ab[0] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 0u];                 
    ab[1] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 1u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&(WARP>>3u)) != 0u)) > 0.f);
  }
  PARALLEL()
  if((WARP>>3u) > 0u)
    reductionArray[WARP*0u + gl_LocalInvocationIndex] = ab[w];

  if(VERBOSE){
  PRINT(WARP/8,WARP/8*0);
  PRINT(WARP/8,WARP/8*1);
  PRINT(WARP/8,WARP/8*2);
  PRINT(WARP/8,WARP/8*3);
  PRINT(WARP/8,WARP/8*4);
  PRINT(WARP/8,WARP/8*5);
  std::cerr << std::endl;
  }

  /////(24t -> 4 minx + 4 maxx + 4miny + 4maxy + 4minz + 4maxz)
  PARALLEL()
  if((WARP>>4u) > 0u){
    ab[0] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 0u];                 
    ab[1] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 1u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&(WARP>>4u)) != 0u)) > 0.f);
  }
  PARALLEL()
  if((WARP>>4u) > 0u)
    reductionArray[WARP*0u + gl_LocalInvocationIndex] = ab[w];

  if(VERBOSE){
  PRINT(WARP/16,WARP/16*0);
  PRINT(WARP/16,WARP/16*1);
  PRINT(WARP/16,WARP/16*2);
  PRINT(WARP/16,WARP/16*3);
  PRINT(WARP/16,WARP/16*4);
  PRINT(WARP/16,WARP/16*5);
  std::cerr << std::endl;
  }

  /////(12t -> 2 minx + 2 maxx + 2miny + 2maxy + 2minz + 2maxz)
  PARALLEL()
  if((WARP>>5u) > 0u){
    ab[0] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 0u];                 
    ab[1] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 1u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&(WARP>>5u)) != 0u)) > 0.f);
  }
  PARALLEL()
  if((WARP>>5u) > 0u)
    reductionArray[WARP*0u + gl_LocalInvocationIndex] = ab[w];
  
  if(VERBOSE){
  PRINT(WARP/32,WARP/32*0);
  PRINT(WARP/32,WARP/32*1);
  PRINT(WARP/32,WARP/32*2);
  PRINT(WARP/32,WARP/32*3);
  PRINT(WARP/32,WARP/32*4);
  PRINT(WARP/32,WARP/32*5);
  std::cerr << std::endl;
  }


  /////(6t -> 1 minx + 1 maxx + 1miny + 1maxy + 1minz + 1maxz)
  PARALLEL()
  if((WARP>>6u) > 0u){
    ab[0] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 0u];                 
    ab[1] = reductionArray[WARP*0u+(uint(gl_LocalInvocationIndex)<<1u) + 1u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&(WARP>>6u)) != 0u)) > 0.f);
  }
  PARALLEL()
  if((WARP>>6u) > 0u)
    reductionArray[WARP*0u + gl_LocalInvocationIndex] = ab[w];

  if(VERBOSE){
  PRINT(WARP/64,WARP/64*0);
  PRINT(WARP/64,WARP/64*1);
  PRINT(WARP/64,WARP/64*2);
  PRINT(WARP/64,WARP/64*3);
  PRINT(WARP/64,WARP/64*4);
  PRINT(WARP/64,WARP/64*5);
  std::cerr << std::endl;
  }

#undef PRINT
#undef PRINTW
}

template<uint32_t WARP=64>
void basicReduce(std::vector<float>&reductionArray){
  float aabb[6] = {
    +1e32f,
    -1e32f,
    +1e32f,
    -1e32f,
    +1e32f,
    -1e32f,
  };
  for(uint32_t k=0;k<3;++k){
    for(uint32_t i=0;i<WARP;++i){
      if(reductionArray[k*WARP+i] < aabb[k*2+0])aabb[k*2+0] = reductionArray[k*WARP+i];
      if(reductionArray[k*WARP+i] > aabb[k*2+1])aabb[k*2+1] = reductionArray[k*WARP+i];
    }
  }
  for(int i=0;i<6;++i)
    reductionArray[i] = aabb[i];

}

template<uint32_t WARP = 64>
std::vector<float>getData(){
  std::vector<float>ar(WARP*3);
  for(uint i=0;i<WARP*3;++i)
    ar[i] = (WARP*3-i);
  return ar;
}

template<uint32_t WARP = 64>
void test(std::vector<float>const&ar){

  std::cerr << "starting test for WARP: " << WARP << std::endl;
  auto aData = ar;
  auto bData = ar;

  reduceAdvanced<WARP>(aData);
  basicReduce   <WARP>(bData);

  aData.resize(6);
  bData.resize(6);
  bool suc = true;
  if(aData != bData){
    suc = false;
    std::cerr << "test failed for: WARP: " << WARP << std::endl;
    for(int i=0;i<6;++i)
      std::cerr << "  " << aData[i] << " should be " << bData[i] << std::endl;
  }
  std::cerr << "test for WARP: " << WARP << " had ended ";
  if(suc)std::cerr << "successfully";
  else std::cerr << "with error";
  std::cerr << std::endl;
}

int main()
{
  std::cerr << std::endl;
  test<64>(getData<64>());
  std::cerr << std::endl;
  test<32>(getData<32>());
  std::cerr << std::endl;
  test<16>(getData<16>());
  std::cerr << std::endl;
  test<8>(getData<8>());
  std::cerr << std::endl;
  test<4>(getData<4>());
  std::cerr << std::endl;
  return 0;
}
