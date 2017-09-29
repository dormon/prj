#include<cstdlib>
#include<iostream>
#include<vector>
#include<CL/cl.hpp>

const size_t BLOCKS_PER_WORKGROUP    = 8   ;
const size_t BLOCK_STRIDE            = 64  ;//8192;
const size_t BLOCK_SIZE              = 64  ;
const size_t WORKGROUP_SIZE          = 64  ;

const size_t ITERATIONS              = 50                                                                  ;
const size_t BYTES_PER_FLOAT         = sizeof(float)                                                       ;
const size_t NANOSECONDS_IN_SECOND   = 1000000000                                                          ;
const size_t GIGA                    = 1 << 30                                                             ;
const size_t MEGA                    = 1 << 20                                                             ;
const size_t COMPUTE_UNITS           = 20                                                                  ;
const size_t GROUPS_PER_COMPUTE_UNIT = 40*32                                                               ;
const size_t NOF_WORKGROUPS          = COMPUTE_UNITS * GROUPS_PER_COMPUTE_UNIT                             ;

const size_t READ_SIZE               = BYTES_PER_FLOAT * BLOCK_SIZE * BLOCKS_PER_WORKGROUP * NOF_WORKGROUPS;
const size_t BUFFER_SIZE             = (READ_SIZE * BLOCK_STRIDE) / BLOCK_SIZE                             ;

int main(int argc,char*argv[]){
  static_cast<void>(argc);
  static_cast<void>(argv);

  try{
    std::vector<cl::Platform>platforms;
    cl::Platform::get(&platforms);
    if(platforms.empty())
      throw "there is no available OpenCL platform";
  
    size_t selectedPlatformId = 0;
    auto platform = platforms.at(selectedPlatformId);

    std::vector<cl::Device>devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU,&devices);

    std::vector<size_t>selectedDeviceIds = {0};
    std::vector<cl::Device>selectedDevices;
    for(auto const&x:selectedDeviceIds)selectedDevices.emplace_back(devices.at(x));

    cl_int contextStatus = CL_SUCCESS;
    cl::Context context(
        selectedDevices,
        nullptr        ,
        nullptr        ,
        nullptr        ,
        &contextStatus );

    if(contextStatus != CL_SUCCESS)
      throw "context creation error";

    cl::Buffer buffer(context,0,BUFFER_SIZE);

    cl::Program program(R".(
      __kernel void readBuffer(__global uint*data){
        data[get_global_id(0)] = get_global_id(0);
      }
      ).",true);

    cl::Kernel kernel(program,"readBuffer");
    kernel.setArg(0,buffer);
    cl_int commandQueueStatus;
    cl::CommandQueue queue(
        context                  ,
        selectedDevices.at(0)    ,
        CL_QUEUE_PROFILING_ENABLE,
        &commandQueueStatus      );
    if(commandQueueStatus != CL_SUCCESS)
      throw "command queue creation error";

    queue.enqueueNDRangeKernel(kernel,{0},{NOF_WORKGROUPS*WORKGROUP_SIZE},{WORKGROUP_SIZE});


  }catch(std::string const&e){
    std::cerr << "ERROR: " << e << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
