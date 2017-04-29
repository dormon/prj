#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>

#define COPY2(   fce,name,number,...)          fce(name##0,number+0,__VA_ARGS__)          fce(name##1,number+1   ,__VA_ARGS__)
#define COPY4(   fce,name,number,...) COPY2   (fce,name##0,number+0,__VA_ARGS__) COPY2   (fce,name##1,number+2   ,__VA_ARGS__)
#define COPY8(   fce,name,number,...) COPY4   (fce,name##0,number+0,__VA_ARGS__) COPY4   (fce,name##1,number+4   ,__VA_ARGS__)
#define COPY16(  fce,name,number,...) COPY8   (fce,name##0,number+0,__VA_ARGS__) COPY8   (fce,name##1,number+8   ,__VA_ARGS__)
#define COPY32(  fce,name,number,...) COPY16  (fce,name##0,number+0,__VA_ARGS__) COPY16  (fce,name##1,number+16  ,__VA_ARGS__)
#define COPY64(  fce,name,number,...) COPY32  (fce,name##0,number+0,__VA_ARGS__) COPY32  (fce,name##1,number+32  ,__VA_ARGS__)
#define COPY128( fce,name,number,...) COPY64  (fce,name##0,number+0,__VA_ARGS__) COPY64  (fce,name##1,number+64  ,__VA_ARGS__)
#define COPY256( fce,name,number,...) COPY128 (fce,name##0,number+0,__VA_ARGS__) COPY128 (fce,name##1,number+128 ,__VA_ARGS__)
#define COPY512( fce,name,number,...) COPY256 (fce,name##0,number+0,__VA_ARGS__) COPY256 (fce,name##1,number+256 ,__VA_ARGS__)
#define COPY1024(fce,name,number,...) COPY512 (fce,name##0,number+0,__VA_ARGS__) COPY512 (fce,name##1,number+512 ,__VA_ARGS__)
#define COPY2048(fce,name,number,...) COPY1024(fce,name##0,number+0,__VA_ARGS__) COPY1024(fce,name##1,number+1024,__VA_ARGS__)
#define COPY4096(fce,name,number,...) COPY2048(fce,name##0,number+0,__VA_ARGS__) COPY2048(fce,name##1,number+2048,__VA_ARGS__)

#define DEF_IMPL(name,number,...)void name##_impl(int a)const{return this->gl_call(&Table::name##_impl,number,a);}
//#define DEF_IMPL(name,number,...)void name##_impl(int a)const{return ((void(*)(int))this->ptrs[number])(a);}

#define DEF_CONSTRUCTOR(name,number,...)this->memptrs[number] = (MEMBERFCEPTR)&Table::name##_impl;

//54.977
#define DEF_CALL(name,number,...)void name(int val)const{return(this->*(decltype(&Table::name)(this->memptrs[number])))(val);}
//55.315
//#define DEF_CALL(name,number,...)void name(int val)const{return(this->*((void(Table::*)(int)const)(this->memptrs[number])))(val);}

#define DEF_CALL_FCE(name,number,...)table.name(number);

#define NUMBER 2048



class Table{
  public:
    using FCEPTR = void(*)();
    using MEMBERFCEPTR = void(Table::*)();
    FCEPTR ptrs[NUMBER];
    MEMBERFCEPTR memptrs[NUMBER];
    template<typename OUTPUT,typename...ARGS>
      OUTPUT gl_call(OUTPUT(Table::*)(ARGS...)const,size_t id,ARGS...args)const{
        return ((OUTPUT(*)(ARGS...))this->ptrs[id])(args...);
      }
    COPY2048(DEF_IMPL,baseName,0)
    Table(){
      COPY2048(DEF_CONSTRUCTOR,baseName,0)
    }
    COPY2048(DEF_CALL,baseName,0)
};

int main(){
  Table table;
  //COPY2048(DEF_CALL_FCE,baseName,0)
  return 0;
}
