#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <iomanip>

float reductionArray[64*3];

using uint = uint32_t;


void reduceAdvanced(){
  float AB[64][2];                                                             
  uint  W[64];

  #define ab AB[gl_LocalInvocationIndex]
  #define w W[gl_LocalInvocationIndex]

#define PRINT(C,O)\
  for(size_t i=O;i<O+C;++i){\
    std::cerr << std::setfill(' ') << std::setw(3) << reductionArray[i] << " ";\
  }\
  std::cerr << std::endl

#define PRINTW()\
  for(size_t i=0;i<64;++i)\
    std::cerr << std::setfill(' ') << std::setw(3) << W[i] << " ";\
  std::cerr << std::endl

#define PARALLEL()\
  for(uint gl_LocalInvocationIndex=0;gl_LocalInvocationIndex<64;++gl_LocalInvocationIndex)


  PRINT(64,64*0);
  PRINT(64,64*1);
  PRINT(64,64*2);
  std::cerr << std::endl;

  //////(64t -> 32 min + 32 max) * 3
  PARALLEL(){
  ab[0] = reductionArray[64u*0u+(uint(gl_LocalInvocationIndex)&0x1fu)+ 0u];       
  ab[1] = reductionArray[64u*0u+(uint(gl_LocalInvocationIndex)&0x1fu)+32u];       
  w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x20u)!=0u)) > 0.f);
  }
  PARALLEL()
  reductionArray[64u*0u+gl_LocalInvocationIndex] = ab[w];                         

  PARALLEL(){
  ab[0] = reductionArray[64u*1u+(uint(gl_LocalInvocationIndex)&0x1fu)+ 0u];       
  ab[1] = reductionArray[64u*1u+(uint(gl_LocalInvocationIndex)&0x1fu)+32u];       
  w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x20u)!=0u)) > 0.f);
  }
  PARALLEL()
  reductionArray[64u*1u+gl_LocalInvocationIndex] = ab[w];                         

  PARALLEL(){
  ab[0] = reductionArray[64u*2u+(uint(gl_LocalInvocationIndex)&0x1fu)+ 0u];       
  ab[1] = reductionArray[64u*2u+(uint(gl_LocalInvocationIndex)&0x1fu)+32u];       
  w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x20u)!=0u)) > 0.f);
  }
  PARALLEL()
  reductionArray[64u*2u+gl_LocalInvocationIndex] = ab[w];                         

  PRINT(32,32*0);
  PRINT(32,32*1);
  PRINT(32,32*2);
  PRINT(32,32*3);
  PRINT(32,32*4);
  PRINT(32,32*5);
  std::cerr << std::endl;

  /////(32t -> 16 minx + 16 maxx) + (32t -> 16miny + 16 maxy)
  PARALLEL(){
  ab[0] = reductionArray[(uint(gl_LocalInvocationIndex)&0xfu) +  0u + (uint(gl_LocalInvocationIndex)&0x30u)*2u];                 
  ab[1] = reductionArray[(uint(gl_LocalInvocationIndex)&0xfu) + 16u + (uint(gl_LocalInvocationIndex)&0x30u)*2u];                 
  w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x10u)!=0u)) > 0.f);
  }
  PARALLEL()
  reductionArray[gl_LocalInvocationIndex] = ab[w];

  PRINT(16,16*0);
  PRINT(16,16*1);
  PRINT(16,16*2);
  PRINT(16,16*3);
  std::cerr << std::endl;
  /////(32t -> 16 minz + 16 maxz)
  PARALLEL()
  if(gl_LocalInvocationIndex < 32){                         
    ab[0] = reductionArray[64u*2u+(uint(gl_LocalInvocationIndex)&0xfu) +  0u + (uint(gl_LocalInvocationIndex)&0x10u)*2u];                 
    ab[1] = reductionArray[64u*2u+(uint(gl_LocalInvocationIndex)&0xfu) + 16u + (uint(gl_LocalInvocationIndex)&0x10u)*2u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float(gl_LocalInvocationIndex>15)) > 0.f);
  }
  PARALLEL()
  if(gl_LocalInvocationIndex < 32)                         
    reductionArray[64u + gl_LocalInvocationIndex] = ab[w];

  PRINT(16,16*4);
  PRINT(16,16*5);
  std::cerr << std::endl;


  /////(48t -> 8 minx + 8 maxx + 8miny + 8maxy + 8minz + 8maxz)
  PARALLEL()
  if(gl_LocalInvocationIndex < 6u*8u){                         
    ab[0] = reductionArray[(uint(gl_LocalInvocationIndex)&0x7u) +  0u + (uint(gl_LocalInvocationIndex)>>3u)*16u];                 
    ab[1] = reductionArray[(uint(gl_LocalInvocationIndex)&0x7u) +  8u + (uint(gl_LocalInvocationIndex)>>3u)*16u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x8u) != 0u)) > 0.f);
  }
  PARALLEL()
  if(gl_LocalInvocationIndex < 6u*8u)                         
    reductionArray[gl_LocalInvocationIndex] = ab[w];

  PRINT(8,8*0);
  PRINT(8,8*1);
  PRINT(8,8*2);
  PRINT(8,8*3);
  PRINT(8,8*4);
  PRINT(8,8*5);
  std::cerr << std::endl;


  PARALLEL()
  /////(24t -> 4 minx + 4 maxx + 4miny + 4maxy + 4minz + 4maxz)
  if(gl_LocalInvocationIndex < 6u*4u){                         
    ab[0] = reductionArray[(uint(gl_LocalInvocationIndex)&0x3u) +  0u + (uint(gl_LocalInvocationIndex)>>2u)*8u];                 
    ab[1] = reductionArray[(uint(gl_LocalInvocationIndex)&0x3u) +  4u + (uint(gl_LocalInvocationIndex)>>2u)*8u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x4u) != 0u)) > 0.f);
  }                                                                        
  PARALLEL()
  if(gl_LocalInvocationIndex < 6u*4u)                         
    reductionArray[gl_LocalInvocationIndex] = ab[w];

  PRINT(4,4*0);
  PRINT(4,4*1);
  PRINT(4,4*2);
  PRINT(4,4*3);
  PRINT(4,4*4);
  PRINT(4,4*5);
  std::cerr << std::endl;

  /////(12t -> 2 minx + 2 maxx + 2miny + 2maxy + 2minz + 2maxz)
  PARALLEL()
  if(gl_LocalInvocationIndex < 6u*2u){                         
    ab[0] = reductionArray[(uint(gl_LocalInvocationIndex)&0x1u) +  0u + (uint(gl_LocalInvocationIndex)>>1u)*4u];                 
    ab[1] = reductionArray[(uint(gl_LocalInvocationIndex)&0x1u) +  2u + (uint(gl_LocalInvocationIndex)>>1u)*4u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x2u) != 0u)) > 0.f);
  }                                                                        
  PARALLEL()
  if(gl_LocalInvocationIndex < 6u*2u)                       
    reductionArray[gl_LocalInvocationIndex] = ab[w];

  PRINT(2,2*0);
  PRINT(2,2*1);
  PRINT(2,2*2);
  PRINT(2,2*3);
  PRINT(2,2*4);
  PRINT(2,2*5);
  std::cerr << std::endl;

  /////(6t -> 1 minx + 1 maxx + 1miny + 1maxy + 1minz + 1maxz)
  PARALLEL()
  if(gl_LocalInvocationIndex < 6u*1u){                         
    ab[0] = reductionArray[(uint(gl_LocalInvocationIndex)&0x0u) +  0u + (uint(gl_LocalInvocationIndex)>>0u)*2u];                 
    ab[1] = reductionArray[(uint(gl_LocalInvocationIndex)&0x0u) +  1u + (uint(gl_LocalInvocationIndex)>>0u)*2u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x1u) != 0u)) > 0.f);
  }                                                       
  PARALLEL()
  if(gl_LocalInvocationIndex < 6u*1u)
    reductionArray[gl_LocalInvocationIndex] = ab[w];


  PRINT(1,1*0);
  PRINT(1,1*1);
  PRINT(1,1*2);
  PRINT(1,1*3);
  PRINT(1,1*4);
  PRINT(1,1*5);
  std::cerr << std::endl;

#undef PRINT
#undef PRINTW
}

void reduce(){
  float AB[64][2];                                                             
  uint  W[64];


  #define ab AB[gl_LocalInvocationIndex]
  #define w W[gl_LocalInvocationIndex]

#define PRINT()\
  for(size_t i=0;i<64;++i)\
    std::cerr << std::setfill(' ') << std::setw(2) << reductionArray[i] << " ";\
  std::cerr << std::endl

#define PRINTW()\
  for(size_t i=0;i<64;++i)\
    std::cerr << std::setfill(' ') << std::setw(2) << W[i] << " ";\
  std::cerr << std::endl

#define PARALLEL()\
  for(uint gl_LocalInvocationIndex=0;gl_LocalInvocationIndex<64;++gl_LocalInvocationIndex)


  PRINT();

  PARALLEL()
  {
    ab[0] = reductionArray[(uint(gl_LocalInvocationIndex)&0x1fu)+ 0u];       
    ab[1] = reductionArray[(uint(gl_LocalInvocationIndex)&0x1fu)+32u];       
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x20u)!=0)) > 0.f);
  }
  PRINTW();
  PARALLEL()
    reductionArray[gl_LocalInvocationIndex] = ab[w];                         


  PRINT();
   
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x10u) == 0u){                         
    ab[0] = reductionArray[gl_LocalInvocationIndex +  0u];                 
    ab[1] = reductionArray[gl_LocalInvocationIndex + 16u];                 
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x20u)!=0)) > 0.f);
  }
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x10u) == 0u){                         
    reductionArray[gl_LocalInvocationIndex - (uint((gl_LocalInvocationIndex)&0x20u) != 0)*16u] = ab[w];
  }                                                                        
  PRINT();
                                                                           
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x28u) == 0u){                         
    ab[0] = reductionArray[gl_LocalInvocationIndex + 0u];                  
    ab[1] = reductionArray[gl_LocalInvocationIndex + 8u];                  
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x10u)!=0)) > 0.f);
  }                                                                        
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x28u) == 0u){                         
    reductionArray[gl_LocalInvocationIndex - (uint((gl_LocalInvocationIndex)&0x10u) != 0)*8u] = ab[w];
  }                                                                        
  PRINT();
                                                                           
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x34u) == 0u){                         
    ab[0] = reductionArray[gl_LocalInvocationIndex + 0u];                  
    ab[1] = reductionArray[gl_LocalInvocationIndex + 4u];                  
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x8u)!=0)) > 0.f);
  }                                                                        
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x34u) == 0u){                         
    reductionArray[gl_LocalInvocationIndex - (uint((gl_LocalInvocationIndex)&0x8u) != 0)*4u] = ab[w];
  }                                                                        
  PRINT();
                                                                           
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x3au) == 0u){                         
    ab[0] = reductionArray[gl_LocalInvocationIndex + 0u];                  
    ab[1] = reductionArray[gl_LocalInvocationIndex + 2u];                  
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x4u)!=0)) > 0.f);
  }                                                                        
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x3au) == 0u){                         
    reductionArray[gl_LocalInvocationIndex - (uint((gl_LocalInvocationIndex)&0x4u) != 0)*2u] = ab[w];
  }                                                                        
  PRINT();
                                                                           
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x3du) == 0u){                         
    ab[0] = reductionArray[gl_LocalInvocationIndex + 0u];                  
    ab[1] = reductionArray[gl_LocalInvocationIndex + 1u];                  
    w = uint((ab[1]-ab[0])*(-1.f+2.f*float((uint(gl_LocalInvocationIndex)&0x2u)!=0)) > 0.f);
  }
  PARALLEL()
  if((uint(gl_LocalInvocationIndex)&0x3du) == 0u){                         
    reductionArray[gl_LocalInvocationIndex - (uint((gl_LocalInvocationIndex)&0x2u) != 0)*1u] = ab[w];
  }
  PRINT();
}


int main()
{
  for(uint i=0;i<64*3;++i)
    reductionArray[i] = (64*3-i);
  //reduce();

  reduceAdvanced();

  std::cerr << reductionArray[0] << std::endl;
  std::cerr << reductionArray[1] << std::endl;
  std::cerr << reductionArray[2] << std::endl;
  std::cerr << reductionArray[3] << std::endl;
  std::cerr << reductionArray[4] << std::endl;
  std::cerr << reductionArray[5] << std::endl;
  return 0;
}
