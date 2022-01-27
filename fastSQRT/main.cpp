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

/*
black        30         40
red          31         41
green        32         42
yellow       33         43
blue         34         44
magenta      35         45
cyan         36         46
white        37         47
*/

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

std::string printAsBits(float a,bool colorLastExponent = false,bool colorFirstMantisa = false){
  bool colorMantisa = false;
  std::stringstream ss;
  uint32_t u = *(uint32_t*)&a;
  for(uint32_t i=0;i<32;++i){
    if(i==1||i==9)ss << "|";
    auto b = ((u>>(32u-i-1))&1u);
    if((i==8 && b==1 && colorLastExponent) || (i==9 && b==1 && colorFirstMantisa) || (colorMantisa && i>8))
      ss << colored(b,red);
    else
      ss << b;
  }
  return ss.str();
}

std::string printAsBits(int a,bool colorLastExponent = false,bool colorFirstMantisa = false){
  return printAsBits(*(float*)&a,colorLastExponent,colorFirstMantisa);
}

std::string printAsBits(uint32_t a,bool colorLastExponent = false,bool colorFirstMantisa = false){
  return printAsBits(*(float*)&a,colorLastExponent,colorFirstMantisa);
}

void printMethod(float x){
  int magic = 0x5f3759df;
  int    i  = *(int*)&x;
  int   ii  = magic - (i>>1);
  float xx  = *(float*)&ii;
#define SCI  std::setprecision(20)
  std::cerr << colored("=====================PROCEDURE========================  ",yellow)   << std::endl;
  std::cerr << "            x     = " << SCI << colored(x,white)           << std::endl;
  std::cerr << "        int(x)    = " << printAsBits(x         ,true      ) << std::endl;
  std::cerr << "        int(x)>>1 = " << printAsBits(i>>1      ,false,true) << std::endl;
  std::cerr << "magic             = " << printAsBits(0x5f3759df           ) << std::endl;
  std::cerr << "magic             = " << std::scientific << *(float*)&magic << std::endl;
  std::cerr << "                                                        "   << std::endl;
  std::cerr << "                    " << printAsBits(0x5f3759df           ) << std::endl;
  std::cerr << "                  - " << printAsBits(i>>1      ,false,true) << std::endl;
  std::cerr << "                    ----------------------------------  "   << std::endl;
  std::cerr << "magic - int(x)>>1 = " << printAsBits(0x5f3759df - (i>>1))   << std::endl;
  std::cerr << "           y0     = " << SCI << colored(xx,cyan)            << std::endl;
  std::cerr << colored("======================================================  ",yellow)   << std::endl;
  std::cerr << std::endl;
  std::cerr << std::endl;
}

float InvSqrt(float x){
  float xhalf = 0.5f*x;
  int i = *(int*)&x;

  std::cerr << "-----" << std::endl;
  std::cerr << "magic - (i>>1)" << " = " << printAsBits(0x5f3759df) << " - " << printAsBits(i>>1) << " = ";
  i = 0x5f3759df - (i>>1);
  std::cerr << printAsBits(i) << std::endl;
  x = *(float*)&i;
  std::cerr << "guess: " << x << std::endl;
  x = x*(1.5f-xhalf*x*x);
  return x;
}

float getGuess(float x){
  int i = *(int*)&x;
  i = 0x5f3759df - (i>>1);
  x = *(float*)&i;
  return x;
}

int main(){
  for(float x=0.5;x<10;x+=0.3f){
    std::cerr << x << ";" << getGuess(x) << std::endl;
  }
  printMethod( 2.f);
  printMethod( 5.f);
  printMethod(0.999999f);
  int i=0x5f3759df;
  std::cerr << printAsBits(i) << std::endl;

  float magic = *(float*)&i;
  std::cerr << magic << std::endl;

  std::cerr << 1.f/sqrt(2.f) << std::endl;
  std::cerr <<  InvSqrt(2.f) << std::endl;
  return 0;
}
