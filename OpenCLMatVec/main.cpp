#include <CL/cl.hpp>
#include <sqlite3.h>
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
  auto kernel = createKernel(clb,src);
  KernelParameters params;
  auto data = allocateOpenCLTaskData(clb,size);
  prepareKernelParameters(params,size,data);
  setKernelParameters(kernel,params);
  return measureKernel(kernel,cl::NDRange(size.output),cl::NDRange(64),nofMeasurements);
}

MeanVariance<float> oneThreadPerOutput(CLBase const&clb,TaskSize const&size,size_t nofMeasurements){
  std::string const static src = R".(
void kernel main(
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

MeanVariance<float> sgemm7(){
  std::string const static src = R".(

    // Wider loads combined with 2D register blocking
    __kernel void myGEMM7(const int M, const int N, const int K,
                          const __global floatX* A,
                          const __global floatX* B,
                          __global float* C) {

        
        // Thread identifiers
        const int tidm = get_local_id(0); // Local row ID (max: TSM/WPTM)
        const int tidn = get_local_id(1); // Local col ID (max: TSN/WPTN)
        const int offsetM = TSM*get_group_id(0); // Work-group offset
        const int offsetN = TSN*get_group_id(1); // Work-group offset
     
        // Local memory to fit a tile of A and B
        __local float Asub[TSK][TSM];
        __local float Bsub[TSK][TSN];
     
        // Allocate register space
        float Areg;
        float Breg[WPTN];
        float acc[WPTM][WPTN];
     
        // Initialise the accumulation registers
        for (int wm=0; wm<WPTM; wm++) {
            for (int wn=0; wn<WPTN; wn++) {
                acc[wm][wn] = 0.0f;
            }
        }
        
        // Loop over all tiles
        int numTiles = K/TSK;
        for (int t=0; t<numTiles; t++) {
     
            // Load one tile of A and B into local memory
            for (int la=0; la<LPTA/WIDTH; la++) {
                int tid = tidn*RTSM + tidm;
                int id = la*RTSN*RTSM + tid;
                int row = id % (TSM/WIDTH);
                int col = id / (TSM/WIDTH);
     
                // Load the values (wide vector load)
                int tiledIndex = TSK*t + col;
                floatX vecA = A[tiledIndex*(M/WIDTH) + offsetM/WIDTH + row];
                floatX vecB = B[tiledIndex*(N/WIDTH) + offsetN/WIDTH + row];
     
                // Store the loaded vectors into local memory
                #if WIDTH == 1
                    Asub[col][row] = vecA;
                    Asub[col][row] = vecA;
                #elif WIDTH == 2
                    Asub[col][WIDTH*row + 0] = vecA.x;
                    Asub[col][WIDTH*row + 1] = vecA.y;
                #elif WIDTH == 4
                    Asub[col][WIDTH*row + 0] = vecA.x;
                    Asub[col][WIDTH*row + 1] = vecA.y;
                    Asub[col][WIDTH*row + 2] = vecA.z;
                    Asub[col][WIDTH*row + 3] = vecA.w;
                #endif
                #if WIDTH == 1
                    Bsub[col][row] = vecB;
                    Bsub[col][row] = vecB;
                #elif WIDTH == 2
                    Bsub[col][WIDTH*row + 0] = vecB.x;
                    Bsub[col][WIDTH*row + 1] = vecB.y;
                #elif WIDTH == 4
                    Bsub[col][WIDTH*row + 0] = vecB.x;
                    Bsub[col][WIDTH*row + 1] = vecB.y;
                    Bsub[col][WIDTH*row + 2] = vecB.z;
                    Bsub[col][WIDTH*row + 3] = vecB.w;
                #endif
            }
            
            // Synchronise to make sure the tile is loaded
            barrier(CLK_LOCAL_MEM_FENCE);
     
            // Loop over the values of a single tile
            for (int k=0; k<TSK; k++) {
     
                // Cache the values of Bsub in registers
                for (int wn=0; wn<WPTN; wn++) {
                    int col = tidn + wn*RTSN;
                    Breg[wn] = Bsub[k][col];
                }
     
                // Perform the computation
                for (int wm=0; wm<WPTM; wm++) {
                    int row = tidm + wm*RTSM;
                    Areg = Asub[k][row];
                    for (int wn=0; wn<WPTN; wn++) {
                        acc[wm][wn] += Areg * Breg[wn];
                    }
                }
            }
     
            // Synchronise before loading the next tile
            barrier(CLK_LOCAL_MEM_FENCE);
        }
     
        // Store the final results in C
        for (int wm=0; wm<WPTM; wm++) {
            int globalRow = offsetM + tidm + wm*RTSM;
            for (int wn=0; wn<WPTN; wn++) {
                int globalCol = offsetN + tidn + wn*RTSN;
                C[globalCol*M + globalRow] = acc[wm][wn];
            }
        }
    }


  ).";

}

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
      for(size_t batchSize:batchSizes)
        oneThreadPerOutput(clb,TaskSize{inputSize,outputSize,batchSize},nofMeasurements);
  sqlite3_close(db);
}

size_t getArg(int argc,char*argv[],int a,size_t d=0){
  if(a>=argc)return d;
  return std::atoi(argv[a]);
}

int main(int argc,char*argv[]) {
  size_t nofMeasurements = getArg(argc,argv,4,10);
  TaskSize taskSize{
    getArg(argc,argv,1,128),
    getArg(argc,argv,2,128),
    getArg(argc,argv,3,1024),
  };

  auto clb     = createCtx();

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
  //std::cout << "oneThreadPerOutput     : " << oneThreadPerOutput(clb,inputSize,outputSize,nofMeasurements) << std::endl;
  //std::cout << "oneThreadPerOutputLocal: " << oneThreadPerOutputLocal(clb,inputSize,outputSize,nofMeasurements) << std::endl;

  return 0;
}
