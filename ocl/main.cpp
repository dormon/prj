#include<iostream>
#include<memory>
#include<functional>
#include<type_traits>
#include<cassert>
#include<vector>
#include<map>
#include<set>
#include<tuple>

#include<CL/opencl.h>

int main(){
  cl_int status;
  cl_uint num_platforms;
  std::cerr<<"A"<<std::endl;
  status = clGetPlatformIDs(0,nullptr,&num_platforms);
  std::cerr<<status<<std::endl;
  std::cerr<<num_platforms<<std::endl;
	return 0;
}
