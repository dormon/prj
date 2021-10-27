#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <sstream>
#include <map>
#include <cmath>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

struct E{
  uint32_t op ;
  uint32_t a  ;
  uint32_t b  ;
  uint32_t aux;
};

struct Exp{
  enum Type{FRACT,VAR,ADD,MUL,SUB,DIV,POW}type;
  Exp(int32_t u,int32_t d=1);
  Exp():type(FRACT){}
  Exp(std::string const&n):type(VAR),name(n){}
  Exp(Exp const&a,Exp const&b,Type t):type(t){
    arg.push_back(a);
    arg.push_back(b);
  }
  int32_t up   = 1;  
  int32_t down = 1;
  std::vector<Exp>arg;
  std::string name;
};

Exp var(std::string const&n){return Exp(n);}

Exp operator+(Exp const&a,Exp const&b){return Exp(a,b,Exp::ADD);}
Exp operator*(Exp const&a,Exp const&b){return Exp(a,b,Exp::MUL);}
Exp operator-(Exp const&a,Exp const&b){return Exp(a,b,Exp::SUB);}
Exp operator/(Exp const&a,Exp const&b){return Exp(a,b,Exp::DIV);}
Exp operator^(Exp const&a,Exp const&b){return Exp(a,b,Exp::POW);}

std::string braces(std::string const&a,Exp::Type in,Exp::Type out){
}

std::string str(Exp const&v){
  std::stringstream ss;
  if(v.type==Exp::FRACT){
    ss << v.up;
    if(v.down>1)ss << "/" << v.down;
  }
  if(v.type==Exp::VAR)ss<<v.name;
  if(v.type==Exp::ADD)ss << str(v.arg.at(0)) << "+" << str(v.arg.at(1));
  if(v.type==Exp::MUL)ss << str(v.arg.at(0)) << "*" << str(v.arg.at(1));
  if(v.type==Exp::SUB)ss << str(v.arg.at(0)) << "-" << str(v.arg.at(1));
  if(v.type==Exp::DIV)ss << str(v.arg.at(0)) << "/" << str(v.arg.at(1));
  if(v.type==Exp::POW)ss << str(v.arg.at(0)) << "^" << str(v.arg.at(1));
  return ss.str();
}


Exp smallest(Exp f){
  for(int32_t i=2;i<1000;++i)
    if((f.up%i)==0 && (f.down%i)==0){
      f.up/=i;
      f.down/=i;
    }
  if(f.down < 0){f.up*=-1;f.down*=-1;}
  return f;
}

Exp::Exp(int32_t u,int32_t d):type(FRACT),up(u),down(d){
  auto fs=smallest(*this);
  up=fs.up;
  down=fs.down;
}


int main(){
  auto a = var("a");
  auto b = var("b");
  auto c = var("c");
  auto z = c*(a+b);

  std::cerr << str(z) << std::endl;

  return 0;
}


