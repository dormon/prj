#include<iostream>
#include <tuple>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <sstream>
#include <map>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

uint32_t nor(uint32_t a,uint32_t b){
  return ~(a|b);
}

uint32_t nand(uint32_t a,uint32_t b){
  return ~(a&b);
}


uint32_t nor_fce(uint32_t f,uint32_t b1,uint32_t b0){
  switch(f){
    case 0x0u:return nor(nor(b0,b0),b0);
    case 0x1u:return nor(b0,b1);
    case 0x2u:return nor(nor(b0,b0),b1);
    case 0x3u:return nor(b1,b1);
    case 0x4u:return nor(nor(b1,b1),b0);
    case 0x5u:return nor(b0,b0);
    case 0x6u:return nor(nor(nor(b0,b0),nor(b1,b1)),nor(b0,b1));
    case 0x7u:return nor(nor(nor(b0,b0),nor(b1,b1)),nor(nor(b0,b0),nor(b1,b1)));
    case 0x8u:return nor(nor(b0,b0),nor(b1,b1));
    case 0x9u:return nor(nor(nor(b0,b0),b1),nor(nor(b1,b1),b0));
    case 0xau:return nor(nor(b0,b0),nor(b0,b0));
    case 0xbu:return nor(nor(nor(b1,b1),b0),nor(nor(b1,b1),b0));
    case 0xcu:return nor(nor(b1,b1),nor(b1,b1));
    case 0xdu:return nor(nor(nor(b0,b0),b1),nor(nor(b0,b0),b1));
    case 0xeu:return nor(nor(b0,b1),nor(b0,b1));
    case 0xfu:return nor(nor(nor(b0,b0),b0),nor(nor(b0,b0),b0));
    default :return 0                ;
  }
}

uint32_t nand_fce(uint32_t f,uint32_t b1,uint32_t b0){
  switch(f){
    case 0x0u:return nand(nand(nand(b0,b0),b0),nand(nand(b0,b0),b0));
    case 0x1u:return nand(nand(nand(b0,b0),nand(b1,b1)),nand(nand(b0,b0),nand(b1,b1)));
    case 0x2u:return nand(nand(nand(b1,b1),b0),nand(nand(b1,b1),b0));
    case 0x3u:return nand(b1,b1);
    case 0x4u:return nand(nand(b1,nand(b0,b0)),nand(b1,nand(b0,b0)));
    case 0x5u:return nand(b0,b0);
    case 0x6u:return nand(nand(nand(b0,b0),b1),nand(nand(b1,b1),b0));
    case 0x7u:return nand(b0,b1);
    case 0x8u:return nand(nand(b0,b1),nand(b0,b1));
    case 0x9u:return nand(nand(b0,b1),nand(nand(b0,b0),nand(b1,b1)));
    case 0xau:return nand(nand(b0,b0),nand(b0,b0));
    case 0xbu:return nand(nand(b0,b0),b1);
    case 0xcu:return nand(nand(b1,b1),nand(b1,b1));
    case 0xdu:return nand(nand(b1,b1),b0);
    case 0xeu:return nand(nand(b0,b0),nand(b1,b1));
    case 0xfu:return nand(nand(b0,b0),b0);
    default :return 0                ;
  }
}

using Fce = uint32_t(*)(uint32_t a,uint32_t b);

struct Exp;

using In=std::tuple<bool,Exp,uint32_t>;

struct Exp{
  Exp(){}
  Exp(uint32_t a,uint32_t b){op[0] = std::make_tuple<bool,Exp,uint32_t>(false,{},a);}
  std::vector<In>op[2];
};

void searcher(Fce const&component,Fce const&target,uint32_t maxDepth){
  for(uint32_t d=0;d<maxDepth;++d){
  }
}

int main(){
  for(uint32_t f=0;f<16;++f){
    uint32_t finalV = 0;
    for(uint32_t v=0;v<4;++v){
      uint32_t b0=(v>>0u)&1u;
      uint32_t b1=(v>>1u)&1u;
      uint32_t r=0x1u&nand_fce(f,b1,b0);
      finalV |= r<<v;
    }
    if(finalV != f){
      std::cerr << "f" << f << " == " << finalV << "!=" << f << std::endl;
    }
  }
  return 0;
}


