#include<cstdlib>
#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include <iostream>
 
int r(int a,float const& b,float const& c,float const& d,float const& e,float const& f,float const& g,float const& h,float const& i,float const& j,float const& k,float const& l,float const& m){
  if(b*c*d*e*f*g*h*i*j*k*l*m>.3)return a*2;
  return a*3;
}

int call(int a,float*v,void*fce){
  return ((int(*)(int,float const&,float const&,float const&,float const&,float const&,float const&,float const&,float const&,float const&,float const&,float const&,float const&))fce)(a,v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10],v[11]);
}

int main(){
  int target = 1;//-12
  float data[]={.1,.1,2,.1,2.,3.,1.,.5,2.3,10.1,2.,3.3};
  asm("nop");
  target = call(10,data,(void*)&r);
  asm("nop");
  std::cout<<target<<std::endl;
  return 0;
}


