#pragma once

#include<CL/cl.hpp>

#include<sstream>

class getPlatform{
  public:
    getPlatform&givenPlatformId(size_t id){
      selectedPlatform = id;
      return *this;
    }
    cl::Platform operator()(){
      std::vector<cl::Platform>platforms;
      cl::Platform::get(&platforms);
      if(platforms.empty())
        throw std::string("No platforms found. Check OpenCL installation!");
      if(selectedPlatform >= platforms.size())
        throw std::string("Selected platform is invalid!");
      return platforms.at(selectedPlatform);
    }
  protected:
    size_t selectedPlatform = 0;
};

class getDevice{
  public:
    getDevice&givenDeviceType(cl_device_type type){
      deviceType = type;
      return *this;
    }
    getDevice&givenDeviceId(size_t id){
      selectedDevice = id;
      return *this;
    }
    getDevice&givenPlatform(cl::Platform const&p){
      platform = p;
      return *this;
    }
    cl::Device operator()(){
      std::vector<cl::Device>devices;
      platform.getDevices(deviceType, &devices);
      if(devices.empty())
        throw std::string("No devices found. Check OpenCL installation!");
      if(selectedDevice >= devices.size())
        throw std::string("Selected device is invalid!");
      return devices.at(selectedDevice);
    }
  protected:
    cl::Platform   platform                           ;
    cl_device_type deviceType     = CL_DEVICE_TYPE_ALL;
    size_t         selectedDevice = 0                 ;
};

class getContext{
  public:
    getContext&givenDevices(std::vector<cl::Device>const&devs){
      devices = devs;
      return *this;
    }
    cl::Context operator()(){
      return cl::Context(devices);
    }
  protected:
    std::vector<cl::Device>devices;
};

class buildProgram{
  public:
    buildProgram&givenContext(cl::Context const&ctx){
      context = ctx;
      return *this;
    }
    buildProgram&givenDevices(std::vector<cl::Device>const&devs){
      devices = devs;
      return *this;
    }
    buildProgram&givenSources(std::vector<std::string>const&s){
      sources = s;
      return *this;
    }
    buildProgram&givenOptions(std::string const&o){
      options = o;
      return *this;
    }
    cl::Program operator()(){
      cl::Program::Sources srcs;
      for(auto const&x:sources)
        srcs.push_back({x.c_str(),x.length()});
      cl_int errStatus;
      cl::Program result(context,srcs,&errStatus);

      if(errStatus != CL_SUCCESS)
        throw std::string("Cannot create program!");

      auto buildStatus = result.build(devices,options.c_str());

      if(buildStatus != CL_SUCCESS){
        std::stringstream ss;
        ss << "Program building failed with following errors:" << std::endl; 
        for(auto const&d:devices){
          cl_int err;
          ss << "device: " << d.getInfo<CL_DEVICE_NAME>(&err) << std::endl;
          if(err != CL_SUCCESS)
            throw std::string("Program building failed, also we are unable to obtain name of one or more devices!");
          ss << result.getBuildInfo<CL_PROGRAM_BUILD_LOG>(d,&err);
          if(err != CL_SUCCESS)
            throw std::string("Program building failed, also we are unable to obtain build log for one or more devices!");
        }
        throw std::string(ss.str());
      }

      return result;
    }
  protected:
    cl::Context             context;
    std::vector<cl::Device> devices;
    std::vector<std::string>sources;
    std::string options;
};

class createImage2DArray{
  public:
    createImage2DArray&givenContext(cl::Context const&ctx){
      context = ctx;
      return *this;
    }
    createImage2DArray&givenFlags(cl_mem_flags f){
      flags = f;
      return *this;
    }
    createImage2DArray&givenFormat(cl::ImageFormat const&f){
      format = f;
      return *this;
    }
    createImage2DArray&givenNofLayers(size_t n){
      nofLayers = n;
      return *this;
    }
    createImage2DArray&givenWidth(size_t w){
      width = w;
      return *this;
    }
    createImage2DArray&givenHeight(size_t h){
      height = h;
      return *this;
    }
    createImage2DArray&givenRowPitch(size_t r){
      rowPitch = r;
      return *this;
    }
    createImage2DArray&givenSlicePitch(size_t s){
      slicePitch = s;
      return *this;
    }
    createImage2DArray&givenData(void*ptr){
      host_ptr = ptr;
      return *this;
    }
    cl::Image2DArray operator()(){
      cl_int err;
      cl::Image2DArray result(context,flags,format,nofLayers,width,height,rowPitch,slicePitch,host_ptr,&err);
      if(err != CL_SUCCESS)
        throw std::string("Cannot create Image2DArray");
      return result;
    }
  protected:
    cl::Context context;
    cl_mem_flags flags = CL_MEM_READ_WRITE;
    cl::ImageFormat format = {CL_R,CL_FLOAT};
    size_t nofLayers = 1;
    size_t width = 1;
    size_t height = 1;
    size_t rowPitch = 0;
    size_t slicePitch = 0;
    void*  host_ptr = nullptr;
};
