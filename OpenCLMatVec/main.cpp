#include <CL/cl.hpp>
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <cassert>
#include <memory>

#include<CLQuick.h>

std::string static const kernel_code =
R".(
void kernel oneThreadPerOutput(
  global const float*matrix    ,
  global const float*input     ,
  global       float*output    ,
         const uint  inputSize ,
         const uint  outputSize){
  uint outputIndex = get_global_id(0);
  if(outputIndex >= outputSize)return;

  float result = 0.f;

  for(uint inputIndex = 0; inputIndex < inputSize; ++inputIndex)
    result += matrix[inputIndex + outputIndex*inputSize] * input[inputIndex];
    
  output[outputIndex] = result;
}


#define WGS 64u
void kernel oneThreadPerOutputLocal(
  global const float*matrix    ,
  global const float*input     ,
  global       float*output    ,
         const uint  inputSize ,
         const uint  outputSize){
  local float localInputPart[WGS];
  uint offset = 0;
  uint outputIndex = get_global_id(0);
  float result = 0.f;
  do{
    barrier(CLK_LOCAL_MEM_FENCE);
    if(get_local_id(0) + offset < inputSize)
      localInputPart[get_local_id(0)] = input[get_local_id(0) + offset];
    uint activeThreads = min(WGS,inputSize - offset);
    barrier(CLK_LOCAL_MEM_FENCE);
    for(uint t = 0; t < activeThreads; ++t)
      result += matrix[offset+t + outputIndex*inputSize] * localInputPart[t];
    offset += WGS;
  }while(offset < inputSize);
  if(outputIndex >= outputSize)return;
  output[outputIndex] = result;
}

).";


float measureKernel(CLKernel const&kernel,cl::NDRange const&global,cl::NDRange const&local,size_t nofMeasurements){
  finish(kernel.clb);
  Timer<float>t;
  for(size_t i=0;i<nofMeasurements;++i){
    enqueueKernel(kernel,global,local);
    finish(kernel.clb);
  }
  return t.elapsedFromStart()/static_cast<float>(nofMeasurements);
}

void prepareKernelParameters(CLBase const&clb,KernelParameters&params,size_t inputSize,size_t outputSize){
  addKernelParameter(params,0,createBuffer(clb, inputSize*outputSize*sizeof(float)));
  addKernelParameter(params,1,createBuffer(clb, inputSize*sizeof(float)));
  addKernelParameter(params,2,createBuffer(clb, outputSize*sizeof(float)));
  addKernelParameter(params,3,static_cast<cl_uint>(inputSize));
  addKernelParameter(params,4,static_cast<cl_uint>(outputSize));
}

float oneThreadPerOutput(CLBase const&clb,size_t inputSize,size_t outputSize,size_t nofMeasurements){
  KernelParameters params;
  prepareKernelParameters(clb,params,inputSize,outputSize);

  auto kernel = createKernel(clb,"oneThreadPerOutput",kernel_code);
  setKernelParameters(kernel,params);

  return measureKernel(kernel,cl::NDRange(outputSize),cl::NDRange(64),nofMeasurements);
}

float oneThreadPerOutputLocal(CLBase const&clb,size_t inputSize,size_t outputSize,size_t nofMeasurements){
  KernelParameters params;
  prepareKernelParameters(clb,params,inputSize,outputSize);

  auto kernel = createKernel(clb,"oneThreadPerOutputLocal",kernel_code);
  setKernelParameters(kernel,params);

  return measureKernel(kernel,cl::NDRange(outputSize),cl::NDRange(64),nofMeasurements);
}

void measureKernel(
    CLBase             const&clb            ,
    std::vector<size_t>const&inputSizes     ,
    std::vector<size_t>const&outputSizes    ,
    std::vector<size_t>const&batchSizes     ,
    size_t                   nofMeasurements){
  static_cast<void>(batchSizes);
  sqlite3*db;
  std::string const database = "measurement";
  int rc = sqlite3_open(database.c_str(), &db);
  if(rc){
    std::cerr << "Can't open database: " << database << ": " << sqlite3_errmsg(db);
    return;
  }
  sqlite3_exec(db,"create table oneThreadPerOutput",nullptr,0,nullptr);
  for(size_t inputSize:inputSizes)
    for(size_t outputSize:outputSizes)
        oneThreadPerOutput(clb,inputSize,outputSize,nofMeasurements);
  sqlite3_close(db);
}

int main() {
  size_t nofMeasurements = 10;
  size_t inputSize = 1024*4;
  size_t outputSize = 1024*4;

  auto clb     = createCtx();

  std::cout << "oneThreadPerOutput     : " << oneThreadPerOutput(clb,inputSize,outputSize,nofMeasurements) << std::endl;
  std::cout << "oneThreadPerOutputLocal: " << oneThreadPerOutputLocal(clb,inputSize,outputSize,nofMeasurements) << std::endl;


  return 0;
}
