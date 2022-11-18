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

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

struct Exp;

Exp optimize(Exp const&e);

struct Exp{
  enum Type{CONST=0,VAR,LOG,SIN,COS,ADD,MUL,DIV,POW}type = CONST;
  float c;
  std::string name = "";
  std::vector<size_t>dims;
  std::vector<Exp>op;
  bool isOptimized=false;
  Exp(){}
  Exp(std::string const&n):type(VAR),name(n){}
  Exp(float c):type(CONST),c(c){}
  Exp(Exp const&a,Type type):type(type){
    op.push_back(a);
  }
  Exp(Exp const&a,Exp const&b,Type type):type(type){
    op.push_back(a);
    op.push_back(b);
  }
  Exp const&operator[](size_t i)const{return op[i];}
  Exp swap()const{return Exp(op[1],op[0],type);}
};

Exp var      (std::string const&a    ){return Exp(a);}
Exp log      (Exp const&a            ){return Exp(a,Exp::LOG);}
Exp sin      (Exp const&a            ){return Exp(a,Exp::SIN);}
Exp cos      (Exp const&a            ){return Exp(a,Exp::COS);}
Exp operator+(Exp const&a,Exp const&b){return Exp(a,b,Exp::ADD);}
Exp operator*(Exp const&a,Exp const&b){return Exp(a,b,Exp::MUL);}
Exp operator-(Exp const&a,Exp const&b){return Exp(a,-1.f*b,Exp::ADD);}
Exp operator/(Exp const&a,Exp const&b){return Exp(a,b,Exp::DIV);}
Exp operator^(Exp const&a,Exp const&b){return Exp(a,b,Exp::POW);}
Exp operator-(Exp const&a            ){return -1.f*a;}

std::string toStr(Exp const&e,bool addB=false){
  std::stringstream ss;
  switch(e.type){
    case Exp::CONST:ss << e.c                                                        ;break;
    case Exp::VAR  :ss << e.name                                                     ;break;
    case Exp::LOG  :ss << "log("<<toStr(e[0])<<")"                                   ;break;
    case Exp::SIN  :ss << "sin("<<toStr(e[0])<<")"                                   ;break;
    case Exp::COS  :ss << "cos("<<toStr(e[0])<<")"                                   ;break;
    case Exp::ADD  :ss << (addB?"(":"")<<toStr(e[0])<<"+"<<toStr(e[1])<<(addB?")":"");break;
    case Exp::MUL  :ss << toStr(e[0],true)<<"*"<<toStr(e[1],true)                    ;break;
    case Exp::DIV  :ss << toStr(e[0],true)<<"/"<<toStr(e[1],true)                    ;break;
    case Exp::POW  :ss << toStr(e[0],true)<<"^"<<toStr(e[1],true)                    ;break;
    default        :                                                                  break;
  }
  return ss.str();
}

bool operator<(Exp const&a,Exp const&b){
  auto av=static_cast<uint32_t>(a.type);
  auto bv=static_cast<uint32_t>(b.type);
  if(av<bv)return true;
  if(av>bv)return false;
  switch(a.type){
    case Exp::CONST:return a.c < b.c;
    case Exp::VAR  :return a.name < b.name;
    case Exp::LOG  :return a.op[0] < b.op[0];
    case Exp::SIN  :return a.op[0] < b.op[0];
    case Exp::COS  :return a.op[0] < b.op[0];
    case Exp::ADD  :
    case Exp::MUL  :
    case Exp::DIV  :
    case Exp::POW  :return a.op[0]<b.op[0]?true:(b.op[0]<a.op[0]?false:a.op[1]<b.op[1]);
    default        :return true;
  }
}

Exp sort(Exp const&e){
  if(e.type != Exp::ADD || e.type != Exp::MUL)return e;
  if(e.op[1] < e.op[0])return e.swap();
  return e;
}

Exp optimizeAdd(Exp const&e){
  auto a = optimize(e.op[0]);
  auto b = optimize(e.op[1]);
  if(a.type==Exp::CONST && b.type==Exp::CONST)return a.c+b.c;
  if(a.type==Exp::VAR && b.type==Exp::VAR && a.name == b.name)return 2.f*a;
  return (a+b).swap();
}

Exp optimizeMul(Exp const&e){
  auto a = optimize(e.op[0]);
  auto b = optimize(e.op[1]);
  if(a.type==Exp::CONST && a.c == 1.f)return b;
  if(b.type==Exp::CONST && b.c == 1.f)return a;
  if(a.type==Exp::CONST && a.c == 0.f)return 0.f;
  if(b.type==Exp::CONST && b.c == 0.f)return 0.f;
  return (a*b).swap();
}

Exp optimizeRec(Exp const&e){
  //std::cerr << "optimizing: " << toStr(e) << std::endl;
  switch(e.type){
    case Exp::CONST:return e;
    case Exp::VAR  :return e;
    case Exp::LOG  :return e; 
    case Exp::SIN  :return e; 
    case Exp::COS  :return e; 
    case Exp::ADD  :return optimizeAdd(e);
    case Exp::MUL  :return optimizeMul(e); 
    case Exp::DIV  :return e; 
    case Exp::POW  :return e; 
    default        :return e; 
  }
}

Exp optimize(Exp const&e){
  if(e.isOptimized)return e;
  auto a = optimizeRec(e);
  a = optimizeRec(a);
  a = optimizeRec(a);
  a = optimizeRec(a);
  a = optimizeRec(a);
  a.isOptimized=true;
  return a;
}

Exp diff(Exp const&e,std::string const&v){
  Exp res;
  switch(e.type){
    case Exp::CONST:res = 0                                                  ;break;
    case Exp::VAR  :res = e.name == v?1.f:0.f                                ;break;
    case Exp::LOG  :res = diff(e[0],v)/e[0]                                  ;break;
    case Exp::SIN  :res = cos(e[0])                                          ;break;
    case Exp::COS  :res = -sin(e[0])                                         ;break;
    case Exp::ADD  :res = diff(e[0],v)+diff(e[1],v)                          ;break;
    case Exp::MUL  :res =  diff(e[0],v)*e[1] + e[0]*diff(e[1],v)             ;break;
    case Exp::DIV  :res = (diff(e[0],v)*e[1] - e[0]*diff(e[1],v))/(e[1]*e[1]);break;
    case Exp::POW  :res = e*(diff(e[1],v)*log(e[0])+e[1]*diff(e[0],v)/e[0])  ;break;
    default        :res = 0.f                                                ;break;
  }
  return optimize(res);
}



int main(){
  auto a = var("a");
  auto b = var("b");
  auto c = (a+b+2.)*a;
  std::cerr << toStr(c) << std::endl;
  std::cerr << toStr(diff(c,"a")) << std::endl;
  return 0;
}


