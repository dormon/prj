#include <CL/cl.hpp>
//#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <cassert>
#include <memory>
#include <iomanip>

#include<CLQuick.h>

#include<cpuImplementation.h>
#include<CPUTaskData.h>
#include<OpenCLTaskData.h>
#include<MeanVariance.h>

std::unique_ptr<CPUTaskData>allocateTaskData(TaskSize const&size){
  auto result = std::make_unique<CPUTaskData>();
  result->matrix.resize(size.input * size.output);
  result->input .resize(size.input * size.batch );
  result->output.resize(size.output * size.batch);
  return result;
}

std::unique_ptr<OpenCLTaskData>allocateOpenCLTaskData(CLBase const&clb,TaskSize const&size){
  auto r = std::make_unique<OpenCLTaskData>(
    createBuffer(clb, size.input*size.output*sizeof(float)),
    createBuffer(clb, size.input*size.batch*sizeof(float)),
    createBuffer(clb, size.output*size.batch*sizeof(float))
    );
  return r;
}

using CPUImplementation = void(*)(std::unique_ptr<CPUTaskData>const&,TaskSize const&);

MeanVariance<float> measureCpuImplementation(TaskSize const&taskSize,CPUImplementation implementation,size_t nofMeasurements){
  auto data = allocateTaskData(taskSize);
  std::vector<float>m;
  for(size_t i=0;i<nofMeasurements;++i){
    Timer<float>t;
    implementation(data,taskSize);
    m.push_back(t.elapsedFromStart());
  }
  return computeMeanVariance(m);
}

MeanVariance<float> measureKernel(CLKernel const&kernel,cl::NDRange const&global,cl::NDRange const&local,size_t nofMeasurements){
  finish(kernel.clb);
  std::vector<float>m;
  for(size_t i=0;i<nofMeasurements;++i){
    Timer<float>t;
    enqueueKernel(kernel,global,local);
    finish(kernel.clb);
    m.push_back(t.elapsedFromStart());
  }
  return computeMeanVariance(m);
}

void prepareKernelParameters(KernelParameters&params,TaskSize const&size,std::unique_ptr<OpenCLTaskData>const&d){
  addKernelParameter(params,0,d->matrix);
  addKernelParameter(params,1,d->input);
  addKernelParameter(params,2,d->output);
  addKernelParameter(params,3,static_cast<cl_uint>(size.input));
  addKernelParameter(params,4,static_cast<cl_uint>(size.output));
  addKernelParameter(params,5,static_cast<cl_uint>(size.batch));
}

MeanVariance<float> startKernel(CLBase const&clb,TaskSize const&size,size_t nofMeasurements,std::string const&src){
  auto kernel = createKernel(clb,src,"","oneThreadPerOutput");
  KernelParameters params;
  auto data = allocateOpenCLTaskData(clb,size);
  prepareKernelParameters(params,size,data);
  setKernelParameters(kernel,params);
  return measureKernel(kernel,cl::NDRange(size.output),cl::NDRange(64),nofMeasurements);
}

MeanVariance<float> oneThreadPerOutput(CLBase const&clb,TaskSize const&size,size_t nofMeasurements){
  std::string const static src = R".(
void kernel oneThreadPerOutput(
  global const float*matrix    ,
  global const float*input     ,
  global       float*output    ,
         const uint  inputSize ,
         const uint  outputSize,
         const uint  batchSize ){
  uint outputIndex = get_global_id(0);
  if(outputIndex >= outputSize)return;

  float result = 0.f;

  for(uint inputIndex = 0; inputIndex < inputSize; ++inputIndex)
    result += matrix[inputIndex + outputIndex*inputSize] * input[inputIndex];
    
  output[outputIndex] = result;
}
  ).";
  return startKernel(clb,size,nofMeasurements,src);
}
/*
MeanVariance<float> sgemm7(){
  std::string const static src = R".(

#define TSOutput  128                  // The tile-size in dimension outputSize
#define TSBatch   128                  // The tile-size in dimension batchSize
#define TSInput   16                   // The tile-size in dimension inputSize
#define WPTOutput 8                    // The work-per-thread in dimension outputSize
#define WPTBatch  8                    // The work-per-thread in dimension batchSize
#define RTSOutput (TSOutput/WPTOutput) // The reduced tile-size in dimension outputSize
#define RTSBatch  (TSBatch/WPTBatch)   // The reduced tile-size in dimension batchSize
#define LPTMatrix ((TSInput*TSOutput)/(RTSOutput*RTSBatch)) // Loads-per-thread for matrix
#define LPTInput  ((TSInput*TSBatch)/(RTSOutput*RTSBatch)) // Loads-per-thread for input

// Use 2D register blocking (further increase in work per thread)
void kernel myGEMM6(
  global const float* matrix    ,
  global const float* input     ,
  global       float* output    ,
         const int    inputSize ,
         const int    outputSize,
         const int    batchSize ){
    
    // Thread identifiers
    const int tidOutput    = get_local_id(0)           ; // Local row ID (max: TSOutput/WPTOutput)
    const int tidBatch     = get_local_id(1)           ; // Local col ID (max: TSBatch/WPTBatch)
    const int offsetOutput = TSOutput * get_group_id(0); // Work-group offset
    const int offsetBatch  = TSBatch  * get_group_id(1); // Work-group offset
 
    // Local memory to fit a tile of matrix and input
    local float matrixSub[TSInput][TSOutput];
    local float inputSub [TSBatch][TSInput+2];
 
    // Allocate register space
    float matrixReg;
    float inputReg[WPTBatch];
    float acc[WPTOutput][WPTBatch];
 
    // Initialise the accumulation registers
    for (int o=0; o<WPTOutput; o++)
        for (int b=0; b<WPTBatch; b++)
            acc[o][b] = 0.0f;
    
    // Loop over all tiles
    int numTiles = inputSize/TSInput;
    for (int t=0; t<numTiles; t++) {
 
        // Load one tile of matrix and input into local memory
        for (int la=0; la<LPTMatrix; la++) {
            int tid        = tidBatch*RTSOutput + tidOutput;
            int id         = la*RTSBatch*RTSOutput + tid;
            int row        = id % TSOutput;
            int col        = id / TSOutput;
            int tiledIndex = TSInput*t + col;
            matrixSub[col][row] = matrix[tiledIndex*outputSize + offsetOutput + row];
            inputSub [row][col] = input [tiledIndex*batchSize  + offsetBatch  + row];
        }
        
        // Synchronise to make sure the tile is loaded
        barrier(CLK_LOCAL_MEM_FENCE);
 
        // Loop over the values of a single tile
        for (int k=0; k<TSInput; k++) {
 
            // Cache the values of inputSub in registers
            for (int wn=0; wn<WPTBatch; wn++) {
                int col = tidBatch + wn*RTSBatch;
                inputReg[wn] = inputSub[col][k];
            }
 
            // Perform the computation
            for (int wm=0; wm<WPTOutput; wm++) {
                int row = tidOutput + wm*RTSOutput;
                matrixReg = matrixSub[k][row];
                for (int wn=0; wn<WPTBatch; wn++) {
                    acc[wm][wn] += matrixReg * inputReg[wn];
                }
            }
        }
 
        // Synchronise before loading the next tile
        barrier(CLK_LOCAL_MEM_FENCE);
    }
 
    // Store the final results in output
    for (int wm=0; wm<WPTOutput; wm++) {
        int globalRow = offsetOutput + tidOutput + wm*RTSOutput;
        for (int wn=0; wn<WPTBatch; wn++) {
            int globalCol = offsetBatch + tidBatch + wn*RTSBatch;
            output[globalCol*outputSize + globalRow] = acc[wm][wn];
        }
    }
}
  ).";

}*/

MeanVariance<float> oneThreadPerOutputLocal(CLBase const&clb,TaskSize const&size,size_t nofMeasurements){
  std::string const static src = R".(
#define WGS 64u
void kernel main(
  global const float*matrix    ,
  global const float*input     ,
  global       float*output    ,
         const uint  inputSize ,
         const uint  outputSize,
         const uint  batchSize ){
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
  return startKernel(clb,size,nofMeasurements,src);
}

/*void measureKernel(
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
      for(size_t batchSize:batchSizes)
        oneThreadPerOutput(clb,TaskSize{inputSize,outputSize,batchSize},nofMeasurements);
  sqlite3_close(db);
}*/

size_t getArg(int argc,char*argv[],int a,size_t d=0){
  if(a>=argc)return d;
  return std::atoi(argv[a]);
}

int main(int argc,char*argv[]) {
  TaskSize taskSize{
    getArg(argc,argv,1,128),
    getArg(argc,argv,2,128),
    getArg(argc,argv,3,1024),
  };
  size_t nofMeasurements = getArg(argc,argv,4,10);
  size_t platform = getArg(argc,argv,5,0);
  size_t device   = getArg(argc,argv,6,0);

  auto clb     = createCtx(platform,device);
#if 0
  std::vector<CPUImplementation>implementations = {
    cpuImplementation0,
    cpuImplementation1,
    cpuImplementation2,
    cpuImplementation3,
    cpuImplementation4,
  };
  std::vector<std::string>names = {
    "cpu0",
    "cpu1",
    "cpu2",
    "cpu3",
    "cpu4",
  };
  size_t const nofMethods = implementations.size();
  for(size_t i=0;i<nofMethods;++i){
    auto r = measureCpuImplementation(taskSize,implementations[i],nofMeasurements);
    std::cout << taskSize.input << " " << taskSize.output << " " << taskSize.batch << " " << names[i] << " ";
    std::cout << std::setprecision(10) << std::fixed;
    std::cout << r.mean << " " << r.variance << std::endl;
  }
#endif

  auto r = oneThreadPerOutput(clb,taskSize,nofMeasurements);
  std::cout << r.mean << " " << r.variance << std::endl;
  //std::cout << "oneThreadPerOutputLocal: " << oneThreadPerOutputLocal(clb,inputSize,outputSize,nofMeasurements) << std::endl;

  return 0;
}
