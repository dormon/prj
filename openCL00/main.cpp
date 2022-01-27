#include<DefineList/DefineList.h>
#include<OpenCLLists/Lists.h>
#include<OpenCLHelper.h>

class OCL{
  public:
    cl::Platform platform;
    cl::Device device;
    cl::Context contex;
};

int main(){
  try{
  auto platform = clh::getPlatform()();
  auto device   = clh::getDevice().givenPlatform(platform).givenDeviceId(0)();
  auto context  = clh::getContext().givenDevices({device})();
  auto queue    = clh::getQueue().givenDevice(device).givenContext(context)();

  auto program  = clh::buildProgram()
    .givenContext(context)
    .givenDevices({device})
    .givenSources({"void kernel copy(global int*o,global int const*i){o[get_global_id(0)] = i[get_global_id(0)];}"})();



  auto output = clh::createBuffer().givenSize(10*sizeof(uint32_t)).givenContext(context)();
  auto input  = clh::createBuffer().givenSize(10*sizeof(uint32_t)).givenContext(context)();
  auto kernel = clh::createKernel().givenName("copy").givenProgram(program)();
  clh::setArg(kernel,0,output);
  clh::setArg(kernel,1,input);


  std::vector<uint32_t>A{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<uint32_t>B{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  queue.enqueueWriteBuffer(input ,CL_TRUE,0,sizeof(uint32_t)*10,A.data());
  queue.enqueueWriteBuffer(output,CL_TRUE,0,sizeof(uint32_t)*10,B.data());
  queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(10),cl::NullRange);
  queue.finish();

  std::vector<uint32_t>C(10);
  queue.enqueueReadBuffer(output,CL_TRUE,0,sizeof(uint32_t)*10,C.data());

  if(C==A)std::cerr << "same" << std::endl;
  else std::cerr << "different" << std::endl;
  }catch(std::string&e){
    std::cerr << e << std::endl;
  }

  return 0;
}
