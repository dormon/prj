#include<DefineList/DefineList.h>
#include<OpenCLLists/Lists.h>
#include<OpenCLHelper.h>
#include<cstdlib>
#include<limits>
#include<glm/glm.hpp>

class OCL{
  public:
    cl::Platform platform;
    cl::Device device;
    cl::Context contex;
};

std::string const src = R".(

int lessEqual(float3 a,float3 b){
  if(a[0] > b[0])return false;
  if(a[1] > b[1])return false;
}

void kernel computePlanes(
  global float*o,
  global float*i,
         uint  n){
  uint gid = get_global_id(0);
  if(gid >= n)return;

  float3 A = ((global float3*)i)[gid*3+0];
  float3 B = ((global float3*)i)[gid*3+1];
  float3 C = ((global float3*)i)[gid*3+2];

  float3 norm = normalize(cross(B-A,C-A));
  float  d = -dot(n,A);

  o[gid*4+0] = norm[0];
  o[gid*4+1] = norm[1];
  o[gid*4+2] = norm[2];
  o[gid*4+3] = d;
}

).";

template<typename T>
T divRoundUp(T a,T b){
  return (a/b) + static_cast<T>((a%b) > 0);
}

template<typename T>
T align(T a,T b){
  return divRoundUp(a,b) * b;
}

class CLCtx{
 public:

  CLCtx(size_t p,size_t d = 0);
 protected:
  cl::Platform     platform;
  cl::Device       device  ;
  cl::Context      context ;
  cl::CommandQueue queue   ;
};

float randomf(){
  return std::rand() / std::numeric_limits<int>::max();
}

int main(){
  try{
  auto platform = clh::getPlatform()();
  auto device   = clh::getDevice().givenPlatform(platform).givenDeviceId(0)();
  auto context  = clh::getContext().givenDevices({device})();
  auto queue    = clh::getQueue().givenDevice(device).givenContext(context)();
  auto program  = clh::buildProgram()
    .givenContext(context)
    .givenDevices({device})
    .givenSources({src.c_str()})();
  auto kernel = clh::createKernel().givenName("computePlanes").givenProgram(program)();


  size_t nofTriangles = 1;
  size_t workGroupSize = 64;
  auto output = clh::createBuffer().givenSize(4*nofTriangles*sizeof(float)).givenContext(context)();
  auto input  = clh::createBuffer().givenSize(9*nofTriangles*sizeof(float)).givenContext(context)();
  clh::setArg(kernel,0,output);
  clh::setArg(kernel,1,input);
  clh::setArg(kernel,2,static_cast<uint32_t>(nofTriangles));

  std::vector<float>triangles(nofTriangles*9);
  std::vector<float>planes   (nofTriangles*4);

  for(auto&f:triangles)
    f = randomf();

  queue.enqueueWriteBuffer(input ,CL_TRUE,0,sizeof(float)*triangles.size(),triangles.data());
  queue.enqueueWriteBuffer(output,CL_TRUE,0,sizeof(float)*planes   .size(),planes   .data());
  queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(align(nofTriangles,workGroupSize)),cl::NDRange(workGroupSize));
  queue.finish();

  std::vector<uint32_t>C(10);
  queue.enqueueReadBuffer(output,CL_TRUE,0,sizeof(uint32_t)*10,C.data());

  }catch(std::string&e){
    std::cerr << e << std::endl;
  }

  return 0;
}
