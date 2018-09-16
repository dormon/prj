#pragma once

#include<CL/cl.hpp>

#include<OpenCLLists/Lists.h>

#include<sstream>

namespace clh{
  class getPlatform{
    public:
      getPlatform&givenPlatformId(size_t id){
        selectedPlatform = id;
        return *this;
      }
      cl::Platform operator()()const{
        std::vector<cl::Platform>platforms;
        cl::Platform::get(&platforms);
        if(platforms.empty())
          throw std::string("No platforms found. Check OpenCL installation!");
        if(selectedPlatform >= platforms.size()){
          std::stringstream ss;
          ss << "Cannot getPlatform, selected platform id: ";
          ss << selectedPlatform << " >= " << platforms.size();
          ss << " is to large!";
          throw ss.str();
        }
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
      getDevice&givenPlatform(cl::Platform p){
        platform = p;
        return *this;
      }
      cl::Device operator()()const{
        std::vector<cl::Device>devices;
        platform.getDevices(deviceType, &devices);
        if(devices.empty()){
          std::stringstream ss;
          ss << "Cannot getDevice, no device found for platform: ";
          ss << platform.getInfo<CL_PLATFORM_NAME>();
          throw ss.str();
        }
        if(selectedDevice >= devices.size()){
          std::stringstream ss;
          ss << "Cannot getDevice, selected device id: ";
          ss << selectedDevice << " >= " << devices.size();
          ss << " is to large!";
          throw ss.str();
        }
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
      cl::Context operator()()const{
        return cl::Context(devices);
      }
    protected:
      std::vector<cl::Device>devices;
  };

  class getQueue{
    public:
      getQueue&givenDevice(cl::Device const&d){
        device = d;
        return *this;
      }
      getQueue&givenContext(cl::Context const&ctx){
        context = ctx;
        return *this;
      }
      cl::CommandQueue operator()(){
        cl_int err;
        cl::CommandQueue result(context,device,0,&err);
        if(err != CL_SUCCESS){
          std::stringstream ss;
          ss<< "Cannot create command queue: " << clErrorCodes.at(err) << std::endl;
          ss<< "device: " <<device.getInfo<CL_DEVICE_NAME>();
          throw ss.str();
        }
        return result;
      }
    protected:
      cl::Device  device ;
      cl::Context context;

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
      cl::Program operator()()const{
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
      cl::Image2DArray operator()()const{
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

  class createBuffer{
    public:
      createBuffer&givenContext(cl::Context const&ctx){
        context = ctx;
        return *this;
      }
      createBuffer&givenFlags(cl_mem_flags f){
        flags = f;
        return *this;
      }
      createBuffer&givenSize(size_t s){
        size = s;
        return *this;
      }
      createBuffer&givenData(void*d){
        host_ptr = d;
        return *this;
      }
      cl::Buffer operator()()const{
        cl_int err;
        cl::Buffer result(context,flags,size,host_ptr,&err);
        if(err != CL_SUCCESS){
          std::stringstream ss;
          ss << "Cannot create buffer: " << std::endl;
          ss << "size: " << size << std::endl;
          ss << "host_ptr: " << host_ptr << std::endl;
          ss << "flags: ";
          for(auto const&x:clMemoryFlags)
            if((x.first & flags) == x.first)
              ss << x.second << " ";
          ss << std::endl;
          ss << clErrorCodes.at(err);
          throw ss.str();
        }
        return result;
      }
    protected:
      cl::Context  context                     ;
      cl_mem_flags flags    = CL_MEM_READ_WRITE;
      size_t       size     = 4                ;
      void*        host_ptr = nullptr          ;
  };

  class createKernel{
    public:
      createKernel&givenProgram(cl::Program const&p){
        program = p;
        return *this;
      }
      createKernel&givenName(std::string const&n){
        name = n;
        return *this;
      }
      cl::Kernel operator()()const{
        cl_int err;
        auto result = cl::Kernel(program,name.c_str(),&err);
        if(err != CL_SUCCESS){
          std::stringstream ss;
          ss << "Cannot create kernel: ";
          ss << "name: " << name << std::endl;
          throw ss.str();
        }
        return result;
      }
    protected:
      cl::Program program;
      std::string name   ;
  };

  template<typename T>
    void setArg(cl::Kernel &k,cl_uint i,T const&d){
      cl_int err = k.setArg<T>(i,d);
      if(err != CL_SUCCESS){
        std::stringstream ss;
        ss << "Cannot set argument: " << i << " for kernel: ";
        ss << k.getInfo<CL_KERNEL_FUNCTION_NAME>() << " - ";
        ss << clErrorCodes.at(err) << std::endl;
        auto const nofArgs = k.getInfo<CL_KERNEL_NUM_ARGS>();
        ss << "number of kernel attributes: " << nofArgs  << std::endl;
        for(std::decay<decltype(nofArgs)>::type i=0;i<nofArgs;++i){
          cl_int argNameErr;
          auto argName = k.getArgInfo<CL_KERNEL_ARG_NAME>(i,&argNameErr);
          if(argNameErr != CL_SUCCESS)argName = clErrorCodes.at(argNameErr);
          cl_int argTypeErr;
          auto argType = k.getArgInfo<CL_KERNEL_ARG_TYPE_NAME>(i,&argTypeErr);
          if(argTypeErr != CL_SUCCESS)argType = clErrorCodes.at(argNameErr);
          ss << argName << " - " << argType  << " " << std::endl;
        }
        throw ss.str();
      }
    }
}
