#include<iostream>
#include<memory>
#include<functional>
#include<type_traits>
#include<cassert>
#include<vector>
#include<map>
#include<set>
#include<tuple>
#include<functional>

#include<CL/opencl.h>

class A{
  public:
    std::function<void()>clb = nullptr;
    A(std::function<void()>const&c):clb(c){}
    void operator()(){this->clb();}
};

void printInt(int a){
  std::cout<<a<<std::endl;
}

int main(){
  A a(std::bind(printInt,123));
  a();

  cl_int status;
  cl_uint num_platforms;
  std::cerr<<"A"<<std::endl;
  status = clGetPlatformIDs(0,nullptr,&num_platforms);
  std::cerr<<status<<std::endl;
  std::cerr<<num_platforms<<std::endl;
	return 0;
}
