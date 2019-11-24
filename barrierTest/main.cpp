#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <sstream>
#include <map>
#include <Vars/Vars.h>
#include <FunctionPrologue.h>
#include <Timer.h>
#include "costring.h"

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl


#define FUNCTION(N,...)\
void call##N(vars::Vars&vars){\
  FCE_PRLG("obj",__VA_ARGS__);\
  vars.reCreate<uint32_t>("v",10);\
}

#define FCE_PRLG(...) FUNCTION_PROLOGUE(__VA_ARGS__)

FUNCTION(0);
FUNCTION(1,"a0");
FUNCTION(2,"a0","a1");
FUNCTION(3,"a0","a1","a2");
FUNCTION(4,"a0","a1","a2","a3");
FUNCTION(5,"a0","a1","a2","a3","a4");
FUNCTION(6,"a0","a1","a2","a3","a4","a5");

#undef FCE_PRLG

#define GUNCTION(N,names)\
void gall##N(vars::Vars&vars){\
  if(notChanged(vars,names))return;\
  FUNCTION_CALLER();\
  vars.reCreate<uint32_t>("v",10);\
}


GUNCTION(0,"obj gall0");
GUNCTION(1,"obj gall1 a0");
GUNCTION(2,"obj gall2 a0 a1");
GUNCTION(3,"obj gall3 a0 a1 a2");
GUNCTION(4,"obj gall4 a0 a1 a2 a3");
GUNCTION(5,"obj gall5 a0 a1 a2 a3 a4");
GUNCTION(6,"obj gall6 a0 a1 a2 a3 a4 a5");

#define HUNCTION(N,names)\
void hall##N(vars::Vars&vars){\
  if(notChanged2(vars,names))return;\
  FUNCTION_CALLER();\
  vars.reCreate<uint32_t>("v",10);\
}


HUNCTION(0,"obj gall0");
HUNCTION(1,"obj gall1 a0");
HUNCTION(2,"obj gall2 a0 a1");
HUNCTION(3,"obj gall3 a0 a1 a2");
HUNCTION(4,"obj gall4 a0 a1 a2 a3");
HUNCTION(5,"obj gall5 a0 a1 a2 a3 a4");
HUNCTION(6,"obj gall6 a0 a1 a2 a3 a4 a5");


#define JUNCTION(N,...)\
void jall##N(vars::Vars&vars){\
  if(notChanged2(vars,getName(__VA_ARGS__).c_str()))return;\
  FUNCTION_CALLER();\
  vars.reCreate<uint32_t>("v",10);\
}


JUNCTION(0,"obj","gall0");
JUNCTION(1,"obj","gall1","a0");
JUNCTION(2,"obj","gall2","a0","a1");
JUNCTION(3,"obj","gall3","a0","a1","a2");
JUNCTION(4,"obj","gall4","a0","a1","a2","a3");
JUNCTION(5,"obj","gall5","a0","a1","a2","a3","a4");
JUNCTION(6,"obj","gall6","a0","a1","a2","a3","a4","a5");

using FCE = void(*)(vars::Vars&);



int main(){
  vars::Vars vars;
  for(size_t i=0;i<10;++i){
    std::stringstream ss;
    ss << "a" << i;
    vars.addUint32(ss.str(),1);
  }
  size_t N = 1000000;

  auto align = [](size_t a,size_t n = 20){
    if(a>n)return a;
    return n-a;
  };
  auto measure = [&](std::string const&name,FCE const&f){
    auto timer = Timer<float>();
    timer.reset();
    for(size_t i=0;i<N;++i)
      f(vars);
    auto const time = timer.elapsedFromStart();

    std::cerr << name;

    auto toFill = align(name.length());
    for(size_t i=0;i<toFill;++i)
      std::cerr << " ";

    std::cerr << ": " << time << std::endl;
  };

  measure("call0",call0);
  measure("call1",call1);
  measure("call2",call2);
  measure("call3",call3);
  measure("call4",call4);
  measure("call5",call5);
  measure("call6",call6);
  std::cerr << std::endl;

  measure("gall0",gall0);
  measure("gall1",gall1);
  measure("gall2",gall2);
  measure("gall3",gall3);
  measure("gall4",gall4);
  measure("gall5",gall5);
  measure("gall6",gall6);
  std::cerr << std::endl;

  measure("hall0",hall0);
  measure("hall1",hall1);
  measure("hall2",hall2);
  measure("hall3",hall3);
  measure("hall4",hall4);
  measure("hall5",hall5);
  measure("hall6",hall6);
  std::cerr << std::endl;

  measure("jall0",jall0);
  measure("jall1",jall1);
  measure("jall2",jall2);
  measure("jall3",jall3);
  measure("jall4",jall4);
  measure("jall5",jall5);
  measure("jall6",jall6);
  std::cerr << std::endl;
  return 0;
}


