#pragma once

#include <iostream>

struct uvec3{
  public:
    uint32_t      &operator[](size_t i)     {return data[i];}
    uint32_t const&operator[](size_t i)const{return data[i];}
    uint32_t      &x()     {return data[0];}
    uint32_t      &y()     {return data[1];}
    uint32_t      &z()     {return data[2];}
    uint32_t const&x()const{return data[0];}
    uint32_t const&y()const{return data[1];}
    uint32_t const&z()const{return data[2];}
    uvec3(uint32_t a,uint32_t b,uint32_t c){
      data[0] = a;
      data[1] = b;
      data[2] = c;
    }
    uvec3(uint32_t a){data[0]=a;data[1]=a;data[2]=a;}
    uvec3(){data[0]=0;data[1]=0;data[2]=0;}
    bool operator == (uvec3 const&v)const{
      if(data[0] != v.data[0])return false;
      if(data[1] != v.data[1])return false;
      if(data[2] != v.data[2])return false;
      return true;
    }
    friend std::ostream& operator<< (std::ostream& stream, const uvec3& o) {
      stream << "(" << o.data[0] << "," << o.data[1] << "," << o.data[2] << ")";
      return stream;
    }
  private:
    uint32_t data[3];
};

uint32_t clamp(uint32_t a,uint32_t b,uint32_t c){
  if(a<b)return b;
  if(a>c)return c;
  return a;
}

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

#define uint uint32_t 
