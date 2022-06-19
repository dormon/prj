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

struct Exp{
  enum Type{CONST,VAR,ADD,MUL,DIV,POW}type = CONST;
  ssize_t up = 1;
  ssize_t down = 1;
  std::string name = "";
  std::vector<Exp>op;
  Exp();
  Exp(std::string const&n);
  Exp(ssize_t u,ssize_t d=1);
  Exp(Exp const&a,Exp const&b,Type type);
};

void factorize(ssize_t&up,ssize_t&down){
  for(ssize_t i=2;i<1000&&up>=i&&down>=i;++i)
    if((up%i)==0&&(down%i)==0){
      up/=i;down/=i;
    }
}

Exp::Exp(){}
Exp::Exp(std::string const&n):type(VAR),name(n){}
Exp::Exp(ssize_t u,ssize_t d):type(CONST),up(u),down(d){
  if(down<0){down*=-1;up*=-1;}
  factorize(up,down);
}
Exp::Exp(Exp const&a,Exp const&b,Type t):type(t){
  op.push_back(a);
  op.push_back(b);
}

Exp var(std::string const&a){
  return Exp(a);
}

Exp operator+(Exp const&a,Exp const&b);

Exp constAdd(Exp const&a,Exp const&b){
  switch(b.type){
    case Exp::CONST:
      return Exp(a.up*b.down+b.up*a.down,a.down*b.down);
    case Exp::ADD:
      return (a+b.op[0])+b.op[1];
    default:
      return Exp(a,b,Exp::ADD);
  }
}

Exp varAdd(Exp const&a,Exp const&b){
  switch(b.type){
  }
}

bool smaller(Exp const&a,Exp const&b){
  switch(a.type){
    case Exp::CONST:
      return b.type!=Exp::CONST;
    case Exp::VAR:
      if(b.type==Exp::CONST)return false;
      if(b.type==Exp::VAR  )return a.name<b.name;
      return true;
    case Exp::ADD:
    default:
      return false;
  }
}

Exp operator+(Exp const&a,Exp const&b){
  if(smaller(b,a))return b+a;
  switch(a.type){
    case Exp::CONST:
      return constAdd(a,b);
    case Exp::VAR:
      return varAdd(a,b);
      break;
    default:
      break;
  }
  if(a.type==Exp::CONST){
    if(b.type==Exp::CONST)
      return Exp(a.up*b.down+b.up*a.down,a.down*b.down);
    if(b.type==Exp::ADD){
    }
  }
  if(a.type==Exp::VAR){
  }
}

int main(){
  return 0;
}


