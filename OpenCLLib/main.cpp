#include <CL/cl.hpp>
#include <iostream>
#include <sstream>
#include <cassert>
#include <memory>
#include <set>

#include<CLQuick.h>

std::string static const kernel_code =
R".(
kernel void matrixTimesVector(
  global float      *output,
  global float const*matrix,
  global float const*input ,
  uint inputSize ,
  uint outputSize){
  get_local_size(0)
}
).";

using Arguments = std::map<std::string,std::string>;

size_t getArgument(Arguments&args,int argc,char*argv[],std::string const&arg,size_t def = 0){
  std::stringstream ss;
  ss << arg << " " << def;
  args[arg] = ss.str();
  for(int i=0;i<argc;++i)
    if(std::string(argv[i]) == arg)
      if(i+1 < argc)
        return static_cast<size_t>(atoi(argv[1]));
  return def;
}

bool isArgumentPresent(Arguments&args,int argc,char*argv[],std::string const&arg){
  std::stringstream ss;
  ss << arg;
  args[arg] = ss.str();
  for(int i=0;i<argc;++i)
    if(std::string(argv[i]) == arg)
      return true;
  return false;
}

int main(int argc,char*argv[]) {
  try{
  Arguments args;
  size_t const inputSize = getArgument(args,argc,argv,"--inputSize",1024);
  size_t const outputSize = getArgument(args,argc,argv,"--outputSize",1024);
  size_t selectedPlatformId = getArgument(args,argc,argv,"--platform",0);
  size_t selectedDeviceId = getArgument(args,argc,argv,"--device",0);
  bool printHelp = isArgumentPresent(args,argc,argv,"--help");
  bool printPlatforms = isArgumentPresent(args,argc,argv,"--printPlatforms");
  bool printDevices = isArgumentPresent(args,argc,argv,"--printDevices");

  if(printHelp){
    for(auto const&x:args)
      std::cout << x.second << std::endl;
    return EXIT_SUCCESS;
  }

  auto const platforms = clq::getPlatforms();

  if(printPlatforms){
    for(auto const&platform:platforms)
      std::cout << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
    return EXIT_SUCCESS;
  }

  auto const platform = platforms.at(selectedPlatformId);
  auto const devices = clq::getDevices(platform);

  if(printDevices){
    for(auto const&device:devices)
      std::cout << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    return EXIT_SUCCESS;
  }

  auto const device = devices.at(selectedDeviceId);
  auto const context = clq::createContext({device});
  auto queue = clq::createCommandQueue(context,device);
  auto program = clq::createProgram(context,{device},{kernel_code});
  auto kernel = clq::createKernel(program,"matrixTimesVector");
  auto inputBuffer = clq::createBuffer(context,sizeof(float)*inputSize);
  auto outputBuffer = clq::createBuffer(context,sizeof(float)*outputSize);
  auto matrixBuffer = clq::createBuffer(context,sizeof(float)*inputSize*outputSize);
  clq::setArg(kernel,0,outputBuffer);
  clq::setArg(kernel,1,matrixBuffer);
  clq::setArg(kernel,2,inputBuffer);
  clq::setArg(kernel,3,static_cast<cl_uint>(inputSize));
  clq::setArg(kernel,4,static_cast<cl_uint>(outputSize));
  }catch(std::runtime_error&e){
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
