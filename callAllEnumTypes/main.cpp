#include<iostream>
#include<tuple>
#include <type_traits>

template<typename T>
void tests(){
  std::cerr << "doing test with type: " << typeid(T).name()  << std::endl;
}

enum TestType{
  FLOAT = 0,
  INT      ,
  CHAR     ,
  DOUBLE   ,
};

template<typename...TAIL>
typename std::enable_if_t<sizeof...(TAIL)==0,void>call(int,TestType){}

template<typename H,typename...TAIL>
void call(int n,TestType type){
  if(n==type)tests<H>();
  call<TAIL...>(n+1,type);
}

void megacall(TestType t){
  call<float,int,char,double>(0,t);
}

int main(int argc,char*argv[]){

  megacall(FLOAT);
  megacall(INT);
  megacall(CHAR);
  megacall(DOUBLE);

  return 0;
}
