#include<iostream>
#include<memory>
#include<functional>
#include<type_traits>
#include<cassert>
#include<vector>
#include<map>
#include<set>
#include<tuple>

#include<CL/cl.hpp>

int main(){
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  assert(platforms.size() > 0);
  auto const platform = platforms.at(0);

  std::vector<cl::Device>devices;
  platform.getDevices(CL_DEVICE_TYPE_GPU,&devices);
  assert(devices.size() > 0);
  auto const device = devices.at(0);

  cl_int contextStatus;
  cl::Context context(device,nullptr,nullptr,nullptr,&contextStatus);
  assert(contextStatus == CL_SUCCESS);


  cl::Image2DArray imageData{context,CL_MEM_READ_WRITE,{CL_R,CL_FLOAT},1 ,10,10,0,0,nullptr,nullptr};




	return 0;
}
