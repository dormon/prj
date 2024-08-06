#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstring>
#include <functional>
#include <typeinfo>
#include <set>
#include <chrono>
#include <stack>
#include <thread>
#include <cassert>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <tuple>

enum Color{
  black      = 30,
  red        = 31,
  green      = 32,
  yellow     = 33,
  blue       = 34,
  magenta    = 35,
  cyan       = 36,
  white      = 37,
};

template<typename T>
std::string colored(T const&v,Color c){
  std::stringstream ss;
  ss << "\033[1;"<<c<<"m";
  ss << v;
  ss << "\033[0m";
  return ss.str();
}

std::string printAsBits(uint32_t a,uint32_t len){
  bool colorMantisa = false;
  std::stringstream ss;
  for(uint32_t i=0;i<len;++i){
    auto b = ((a>>(len-i-1))&1u);
    ss << b;
  }
  return ss.str();
}

std::string printAsBitsDiff(uint32_t v,uint32_t ref,uint32_t len){
  bool colorMantisa = false;
  std::stringstream ss;
  for(uint32_t i=0;i<len;++i){
    auto b = ((v  >>(len-i-1))&1u);
    auto r = ((ref>>(len-i-1))&1u);
    if(b!=r)ss << colored(b,red);
    else    ss << b;
  }
  return ss.str();

}

using MORTON = std::function<uint32_t(uint32_t)>;

uint32_t scatter(uint32_t v,uint32_t s){
  uint32_t res = 0;
  for(uint32_t i=0;i<32;++i)
    res |= ((v>>i)&1u) << (i*s);
  return res;
}

uint32_t compScatterLen(uint32_t s,uint32_t bits){
  return bits*s-1u;
}

std::string space(uint32_t n){
  std::stringstream ss;
  for(uint32_t i=0;i<n;++i)
    ss << " ";
  return ss.str();
}

uint32_t nofOnes(uint32_t a){
  uint32_t c=0;
  for(uint32_t i=0;i<32;++i)
    if((a>>i)&1)c++;
  return c;
}

uint32_t nofErr(uint32_t a,uint32_t ref){
  return nofOnes(a^ref);
}

std::string align(uint32_t v,uint32_t len){
  std::stringstream ss;
  ss << v;
  ss << space(len-ss.str().length());
  return ss.str();
}

void printMorton(uint32_t bits){
  uint32_t n=1<<bits;
  uint32_t st = 2;
  uint32_t stlen = compScatterLen(st,bits);

  std::cerr << space(bits) << " " << space(stlen) << " ";
  for(uint32_t i=0;i<n;++i)
    std::cerr << printAsBits(i,bits) << space(stlen-bits+1);
  std::cerr << std::endl;

  std::vector<uint32_t>errors(n);
  for(uint32_t i=0;i<n;++i){
    auto ref = scatter(i,st);
    auto mask = scatter(n-1,st);
    std::cerr << printAsBits(i,bits) << " " << printAsBits(ref,stlen) << " ";
    for(uint32_t j=0;j<n;++j){
      uint32_t newVal;
      if(j==1){
        newVal = (i*i)&mask;
      }else if(j==2){
        newVal = (i*i - 2*((i&3)*(i&6)+(i&2)*(i&2)))&mask;
      }else if(j==3){
        newVal = 
          (((i&0x55555555)*(i&0x55555555))&0x49249249) |
          (((i&0xaaaaaaaa)*(i&0xaaaaaaaa))&0x24924924)

          ;
      }else{
        newVal = (i*j)&mask;
      }
      std::cerr << printAsBitsDiff(newVal,ref,stlen) << " ";
      errors[j] += nofErr(newVal,ref);
    }
    std::cerr << std::endl;
  }
  std::cerr << space(bits) << " " << space(stlen) << " ";
  for(uint32_t i=0;i<n;++i)
    std::cerr << align(errors[i],stlen) << " ";


}


using CALL = std::tuple<std::function<uint32_t(uint32_t,uint32_t)>,uint32_t,std::string>;

struct Instruction{
  uint32_t reg[3];
  CALL     call;
};

std::vector<CALL>calls = {
  {[](uint32_t a,uint32_t b){return a+ b;},2,"+" },
  {[](uint32_t a,uint32_t b){return a- b;},2,"-" },
  {[](uint32_t a,uint32_t b){return a* b;},2,"*" },
  {[](uint32_t a,uint32_t b){return a>>b;},2,">>"},
  {[](uint32_t a,uint32_t b){return a<<b;},2,"<<"},
  {[](uint32_t a,uint32_t b){return   ~a;},1,"~" },
  {[](uint32_t a,uint32_t b){return a^ b;},2,"^" },
  {[](uint32_t a,uint32_t b){return a& b;},2,"&" },
  {[](uint32_t a,uint32_t b){return a| b;},2,"|" },
};

struct Program{
  std::vector<Instruction>instructions;
  std::vector<uint32_t>memory;
  std::vector<uint32_t>initialData;
  uint32_t operator()(uint32_t v){
    return v;
  }
  void init(){
  }
};

std::vector<Instruction>genInstructions(std::vector<CALL>const&calls,size_t nofRegs,size_t nofConsts){
  std::vector<Instruction>res;
  for(auto const&call:calls){//for all instruction type
    auto nArgs = std::get<1>(call);

    size_t memS = nofRegs+nofConsts;
    bool argsHasReg = false;

    uint32_t D=nArgs+1;//dimension
    std::vector<uint32_t>I(D);//n-dimensional index
    std::vector<uint32_t>S(D,memS);//sizes

    auto isValid = [&](std::vector<uint32_t>const&I){
      if(I[0]>=nofRegs)return false;
      bool hasReg = false;
      for(uint32_t i=1;i<I.size();++i)
        if(I[i]<nofRegs){
          hasReg = true;
          break;
        }
      return hasReg;
    };

    for(;;){
      if(isValid(I)){//valid instruction
        Instruction inst;
        for(uint32_t i=0;i<D;++i)inst.reg[i] = I[i];
        inst.call = call;
        res.push_back(inst);
      }
      uint32_t i=0;
      bool last = false;
      for(;;){
        if(i==D){
          last = true;
          break;
        }
        if(I[i]+1==S[i]){
          I[i]=0;
          ++i;
        }else{
          I[i]++;
          break;
        }
      }
      if(last)break;
    }
  }
  return res;
}

Program generateProgram(size_t id,size_t nofRegs,size_t nofConts){
  Program res;
  res.memory.resize(nofRegs+nofConts);
  return res;
}

bool test(MORTON const&a,MORTON const&ref,uint32_t n){
  for(uint32_t i=0;i<n;++i){
    if(a(i) != ref(i))return false;
  }
  return true;
}

bool testProgram(Program&prg,uint32_t bits,uint32_t s){
  uint32_t n = 1u<<bits;
  return test(
      prg,
      [&](uint32_t x){return scatter(x,s);},
      n);
}

bool findProgram(Program&prg,uint32_t bits,uint32_t s,size_t N){
  for(size_t i=0;i<N;++i){
    prg = generateProgram(i,2,6);
    if(testProgram(prg,bits,s))return true;
  }
  return false;
}

uint32_t scatter_4_2(uint32_t v){
  //          dcba
  //       d_c_b_a
  //  
  // ^        dcba
  //        dcba
  //        dcxxba
  //  
  //        dcxxba
  // &      11__11
  //        dc__ba
  //  
  //        dc__ba
  //       dc__ba
  //       dxc_bxa
  //
  //       d_c_b_a
  // &
  //   f__ed_c__ba
  //  
  // ^ f__ed_c__ba
  //  f__ed_c__ba
  //  ff_exdcc_bxa
  //   
  //  ff_exdcc_bxa
  //  _1_1_1_1_1_1
  //  _f_e_d_c_b_a
  
  v = (v ^ (v<<2u)) & 0b00001110011;
  v = (v ^ (v<<1u)) & 0b00001010101;

  return v;
}



uint32_t scatter_6_2(uint32_t v){
  //        fedcba
  //   f_e_d_c_b_a
  //  
  // ^      fedcba
  //     fedcba
  //     fedxxxcba
  //  
  //     fedxxxcba
  // &   111___111
  //     fed___cba
  //  
  // ^   fed___cba
  //   fed___cba
  //   fexed_cbxba
  // &
  //   f__ed_c__ba
  //  
  // ^ f__ed_c__ba
  //  f__ed_c__ba
  //  ff_exdcc_bxa
  //   
  //  ff_exdcc_bxa
  //  _1_1_1_1_1_1
  //  _f_e_d_c_b_a
  
  v = (v ^ (v<<3u)) & 0b00111000111;
  v = (v ^ (v<<2u)) & 0b10011010011;
  v = (v ^ (v<<1u)) & 0b10101010101;

  return v;
}

uint32_t scatter_6_2_inst(uint32_t v){
  uint32_t mem[8];
  mem[0] = v;
  mem[1] = 0;
  mem[2] = 3u;
  mem[3] = 2u;
  mem[4] = 1u;
  mem[5] = 0b00111000111;
  mem[6] = 0b10011010011;
  mem[7] = 0b10101010101;

  mem[1] = mem[0]<<mem[2];
  mem[0] = mem[0]^ mem[1];
  mem[0] = mem[0]& mem[5];

  mem[1] = mem[0]<<mem[3];
  mem[0] = mem[0]^ mem[1];
  mem[0] = mem[0]& mem[6];

  mem[1] = mem[0]<<mem[4];
  mem[0] = mem[0]^ mem[1];
  mem[0] = mem[0]& mem[7];

  //9 instructions
  //6 const
  //2 reg

  return mem[0];
}

uint32_t scatter_16_2_xor(uint32_t x){
  uint32_t a;
  a = x << 8u;
  x = a ^  x;
  x = x &  0x00ff00ffu;
  a = x << 4u;
  x = a ^  x;
  x = x &  0x0f0f0f0fu;
  a = x << 2u;
  x = a ^  x;
  x = x &  0x33333333u;
  a = x << 1u;
  x = a ^  x;
  x = x &  0x55555555u;
  return x;
}


uint32_t scatter_16_2_power(uint32_t x){
  uint32_t a;
  uint32_t b;
  a = x & 0x00009249u;
  float z=(*(float*)&a) * (*(float*)&a);
  a=*(uint32_t*)&z;
  //a = a * a;
  a = a & 0x41041041u;
  b = x & 0x00002492u;
  b = b * b;
  b = b & 0x04104104u;
  a = a | b;
  x = x & 0x00004924u;
  x = x * x;
  x = x & 0x10410410u;
  x = x | a;
  return x;
}

// dcba*0111
//    dcba
//   dcba
//  dcba 
// -------
// 6543210
// 
// 0 a                  = a
// 1 a^b                = a^b
// 2 c^b^a^(ba)         = c^(a+b)
// 3 d^c^b^(ab!c+ca+cb) = 
// 4 c^d^()
//
//
// abcdefgh
//
// a_b_c_d_e_f_g_h
//
// a___c___e___g__
// __b___d___f___h
//
//         b_d_f_h (&*&*&)
//     b_d_f_h
//     b_dxxxx_f_h
//
//     b_d_____f_h
//
//     b_d_____f_h
//   b_d_____f_h
//   bxx_d___fxx_h
//   b___d___f___h
//
//
// abcdefghi
// a_b_c_d_e_f_g_h_i
// ____c_____f_____i
// __b_____e_____h__
// a_____d_____g____
//
//
// ____c_____f_____i
//           cdefghi
//     cdefghi
//
//


int main(){
  auto insts = genInstructions(calls,2,6);


  //std::cerr << "|insts| = " << insts.size() << std::endl;
  //return 0;

  printMorton(4);
  //return 0;

  if(test(scatter_6_2,[](uint32_t x){return scatter(x,2);},1<<6))
    std::cerr << "scatter_6_2 is good" << std::endl;
  else
    std::cerr << "scatter_6_2 is bad" << std::endl;

  if(test(scatter_6_2_inst,[](uint32_t x){return scatter(x,2);},1<<6))
    std::cerr << "scatter_6_2_inst is good" << std::endl;
  else
    std::cerr << "scatter_6_2_inst is bad" << std::endl;

  if(test(scatter_16_2_xor,[](uint32_t x){return scatter(x,2);},1<<16))
    std::cerr << "scatter_16_2_xor is good" << std::endl;
  else
    std::cerr << "scatter_16_2_xor is bad" << std::endl;

  if(test(scatter_16_2_power,[](uint32_t x){return scatter(x,2);},1<<16))
    std::cerr << "scatter_16_2_power is good" << std::endl;
  else
    std::cerr << "scatter_16_2_power is bad" << std::endl;

}
