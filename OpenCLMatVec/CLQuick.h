#pragma once

#include <CL/cl.hpp>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>

std::vector<std::string> splitString(std::string const &txt,
                                     std::string const &splitter);

std::string removeSpaces(std::string const &txt);

std::string loadTextFile(std::string const &fileName);

template <typename TYPE>
std::map<TYPE, std::string> getDefineList(std::vector<TYPE> const &values,
                                          std::string const &names) {
  std::map<TYPE, std::string> result;
  std::vector<std::string> splitNames = splitString(names, ",");
  std::for_each(splitNames.begin(), splitNames.end(),
                [](std::string &x) { x = removeSpaces(x); });
  for (size_t i          = 0; i < splitNames.size(); ++i)
    result[values.at(i)] = splitNames.at(i);
  return result;
}

template <typename TYPE>
std::map<std::string, TYPE> getDefineListByName(std::vector<TYPE> const &values,
                                                std::string const &names) {
  std::map<std::string, TYPE> result;
  std::vector<std::string> splitNames = splitString(names, ",");
  std::for_each(splitNames.begin(), splitNames.end(),
                [](std::string &x) { x = removeSpaces(x); });
  for (size_t i              = 0; i < splitNames.size(); ++i)
    result[splitNames.at(i)] = values.at(i);
  return result;
}

#define DEFINE_LIST(type, name, ...)                         \
  std::map<type, std::string> static const name =            \
      getDefineList<type>({__VA_ARGS__}, #__VA_ARGS__);      \
  std::vector<type> static const name##List = {__VA_ARGS__}; \
  std::map<std::string, type> static const name##ByName =    \
      getDefineListByName<type>({__VA_ARGS__}, #__VA_ARGS__)

DEFINE_LIST(cl_platform_info,
            clPlatformParams,
            CL_PLATFORM_EXTENSIONS,
            CL_PLATFORM_NAME,
            CL_PLATFORM_PROFILE,
            CL_PLATFORM_VENDOR,
            CL_PLATFORM_VERSION);

DEFINE_LIST(cl_device_info,
            clDeviceParams,
            CL_DEVICE_BUILT_IN_KERNELS,
            CL_DEVICE_EXTENSIONS,
            CL_DEVICE_MAX_WORK_ITEM_SIZES,
            CL_DEVICE_NAME,
            CL_DEVICE_OPENCL_C_VERSION,
            CL_DEVICE_PARTITION_PROPERTIES,
            CL_DEVICE_PARTITION_TYPE,
            CL_DEVICE_PROFILE,
            CL_DEVICE_VENDOR,
            CL_DEVICE_VERSION,
            CL_DRIVER_VERSION);

DEFINE_LIST(cl_device_partition_property,
            clDeviceParatitionProperty,
            CL_DEVICE_PARTITION_EQUALLY,
            CL_DEVICE_PARTITION_BY_COUNTS,
            CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN);

DEFINE_LIST(cl_device_type,
            clDeviceTypes,
            CL_DEVICE_TYPE_CPU,
            CL_DEVICE_TYPE_GPU,
            CL_DEVICE_TYPE_ACCELERATOR,
            CL_DEVICE_TYPE_CUSTOM,
            CL_DEVICE_TYPE_DEFAULT,
            CL_DEVICE_TYPE_ALL);

DEFINE_LIST(cl_int,
            clErrorCodes,
            CL_SUCCESS,
            CL_DEVICE_NOT_FOUND,
            CL_DEVICE_NOT_AVAILABLE,
            CL_COMPILER_NOT_AVAILABLE,
            CL_MEM_OBJECT_ALLOCATION_FAILURE,
            CL_OUT_OF_RESOURCES,
            CL_OUT_OF_HOST_MEMORY,
            CL_PROFILING_INFO_NOT_AVAILABLE,
            CL_MEM_COPY_OVERLAP,
            CL_IMAGE_FORMAT_MISMATCH,
            CL_IMAGE_FORMAT_NOT_SUPPORTED,
            CL_BUILD_PROGRAM_FAILURE,
            CL_MAP_FAILURE,
            CL_MISALIGNED_SUB_BUFFER_OFFSET,
            CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST,
            CL_COMPILE_PROGRAM_FAILURE,
            CL_LINKER_NOT_AVAILABLE,
            CL_LINK_PROGRAM_FAILURE,
            CL_DEVICE_PARTITION_FAILED,
            CL_KERNEL_ARG_INFO_NOT_AVAILABLE,
            CL_INVALID_VALUE,
            CL_INVALID_DEVICE_TYPE,
            CL_INVALID_PLATFORM,
            CL_INVALID_DEVICE,
            CL_INVALID_CONTEXT,
            CL_INVALID_QUEUE_PROPERTIES,
            CL_INVALID_COMMAND_QUEUE,
            CL_INVALID_HOST_PTR,
            CL_INVALID_MEM_OBJECT,
            CL_INVALID_IMAGE_FORMAT_DESCRIPTOR,
            CL_INVALID_IMAGE_SIZE,
            CL_INVALID_SAMPLER,
            CL_INVALID_BINARY,
            CL_INVALID_BUILD_OPTIONS,
            CL_INVALID_PROGRAM,
            CL_INVALID_PROGRAM_EXECUTABLE,
            CL_INVALID_KERNEL_NAME,
            CL_INVALID_KERNEL_DEFINITION,
            CL_INVALID_KERNEL,
            CL_INVALID_ARG_INDEX,
            CL_INVALID_ARG_VALUE,
            CL_INVALID_ARG_SIZE,
            CL_INVALID_KERNEL_ARGS,
            CL_INVALID_WORK_DIMENSION,
            CL_INVALID_WORK_GROUP_SIZE,
            CL_INVALID_WORK_ITEM_SIZE,
            CL_INVALID_GLOBAL_OFFSET,
            CL_INVALID_EVENT_WAIT_LIST,
            CL_INVALID_EVENT,
            CL_INVALID_OPERATION,
            CL_INVALID_GL_OBJECT,
            CL_INVALID_BUFFER_SIZE,
            CL_INVALID_MIP_LEVEL,
            CL_INVALID_GLOBAL_WORK_SIZE,
            CL_INVALID_PROPERTY,
            CL_INVALID_IMAGE_DESCRIPTOR,
            CL_INVALID_COMPILER_OPTIONS,
            CL_INVALID_LINKER_OPTIONS,
            CL_INVALID_DEVICE_PARTITION_COUNT);

DEFINE_LIST(cl_context_info,
            clContextParams,
            CL_CONTEXT_REFERENCE_COUNT,
            CL_CONTEXT_NUM_DEVICES,
            CL_CONTEXT_DEVICES,
            CL_CONTEXT_PROPERTIES);

DEFINE_LIST(cl_context_properties,
            clContextProperties,
            CL_CONTEXT_PLATFORM,
            CL_CONTEXT_INTEROP_USER_SYNC);

DEFINE_LIST(cl_mem_flags,
            clMemoryFlags,
            CL_MEM_READ_WRITE,
            CL_MEM_WRITE_ONLY,
            CL_MEM_READ_ONLY,
            CL_MEM_USE_HOST_PTR,
            CL_MEM_ALLOC_HOST_PTR,
            CL_MEM_COPY_HOST_PTR,
            CL_MEM_HOST_WRITE_ONLY,
            CL_MEM_HOST_READ_ONLY,
            CL_MEM_HOST_NO_ACCESS);

DEFINE_LIST(cl_mem_object_type,
            clMemoryBaseImageTypes,
            CL_MEM_OBJECT_IMAGE2D,
            CL_MEM_OBJECT_IMAGE3D);

DEFINE_LIST(cl_mem_object_type,
            clMemoryTypes,
            CL_MEM_OBJECT_BUFFER,
            CL_MEM_OBJECT_IMAGE2D,
            CL_MEM_OBJECT_IMAGE3D,
            CL_MEM_OBJECT_IMAGE2D_ARRAY,
            CL_MEM_OBJECT_IMAGE1D,
            CL_MEM_OBJECT_IMAGE1D_ARRAY,
            CL_MEM_OBJECT_IMAGE1D_BUFFER);

DEFINE_LIST(cl_channel_order,
            clChannelOrder,
            CL_R,
            CL_A,
            CL_RG,
            CL_RA,
            CL_RGB,
            CL_RGBA,
            CL_BGRA,
            CL_ARGB,
            CL_INTENSITY,
            CL_LUMINANCE,
            CL_Rx,
            CL_RGx,
            CL_RGBx,
            CL_DEPTH,
            CL_DEPTH_STENCIL);

DEFINE_LIST(cl_channel_type,
            clChannelType,
            CL_SNORM_INT8,
            CL_SNORM_INT16,
            CL_UNORM_INT8,
            CL_UNORM_INT16,
            CL_UNORM_SHORT_565,
            CL_UNORM_SHORT_555,
            CL_UNORM_INT_101010,
            CL_SIGNED_INT8,
            CL_SIGNED_INT16,
            CL_SIGNED_INT32,
            CL_UNSIGNED_INT8,
            CL_UNSIGNED_INT16,
            CL_UNSIGNED_INT32,
            CL_HALF_FLOAT,
            CL_FLOAT,
            CL_UNORM_INT24);

template <typename TYPE>
class Timer {
  public:
  Timer() {
    assert(this != nullptr);
    this->reset();
  }
  void reset() {
    assert(this != nullptr);
    this->_start = std::chrono::high_resolution_clock::now();
    this->_last  = this->_start;
  }
  TYPE elapsedFromStart() {
    assert(this != nullptr);
    auto newTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<TYPE> elapsed = newTime - this->_start;
    this->_last                         = newTime;
    return elapsed.count();
  }
  TYPE elapsedFromLast() {
    assert(this != nullptr);
    auto newTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<TYPE> elapsed = newTime - this->_last;
    this->_last                         = newTime;
    return elapsed.count();
  }

  protected:
  std::chrono::time_point<std::chrono::high_resolution_clock> _start;
  std::chrono::time_point<std::chrono::high_resolution_clock> _last;
};

namespace clq {
  std::vector<cl::Platform> getPlatforms();

  std::vector<cl::Device> getDevices(cl::Platform const &platform);

  cl::Context createContext(std::vector<cl::Device> const &devs);

  cl::CommandQueue createCommandQueue(cl::Context const &context,
                                      cl::Device const & device);

  cl::Program createProgram(cl::Context const &             context,
                            std::vector<cl::Device> const & devices,
                            std::vector<std::string> const &sources,
                            std::string const &             options);

  cl::Kernel createKernel(cl::Program const &program, std::string const &name);

  void checkSetArg(cl_int err, cl::Kernel const &kernel, cl_uint i);
  template <typename T>
  void setArg(cl::Kernel &kernel, cl_uint i, T const &data);

  cl::Buffer createBuffer(cl::Context const &context,
                          size_t             size,
                          void *             data  = nullptr,
                          cl_mem_flags       flags = CL_MEM_READ_WRITE);

  cl::Image2DArray createImage2DArray(cl::Context const &    context,
                                      cl::ImageFormat const &format,
                                      size_t                 nofLayers,
                                      size_t                 width,
                                      size_t                 height,
                                      void *                 data = nullptr,
                                      cl_mem_flags flags    = CL_MEM_READ_WRITE,
                                      size_t       rowPitch = 0,
                                      size_t       slicePitch = 0);
}

template <typename T>
void clq::setArg(cl::Kernel &kernel, cl_uint i, T const &data) {
  cl_int err = kernel.setArg<T>(i, data);
  checkSetArg(err, kernel, i);
}

struct CLBase {
  cl::Context      ctx;
  cl::Device       dev;
  cl::CommandQueue queue;
};

struct CLBuffer {
  cl::Buffer    buffer;
  CLBase const &clb;
  CLBuffer(CLBase const &c) : clb(c) {}
};

struct CLProgram {
  cl::Program   program;
  CLBase const &clb;
  CLProgram(CLBase const &c) : clb(c) {}
};

struct CLKernel {
  cl::Kernel    kernel;
  CLBase const &clb;
  CLProgram     program;
  CLKernel(CLBase const &c, CLProgram const &p) : clb(c), program(p) {}
};

CLBase    createCtx();
CLProgram createProgram(CLBase const &     clb,
                        std::string const &src,
                        std::string const &options = "");
CLBuffer createBuffer(CLBase const &clb,
                      size_t        size,
                      cl_mem_flags  flags = CL_MEM_READ_WRITE);
cl::Event writeBuffer(CLBuffer const &              buffer,
                      void const *                  ptr,
                      size_t                        size,
                      size_t                        offset   = 0,
                      cl_bool                       blocking = CL_TRUE,
                      std::vector<cl::Event> const &events   = {});

template <typename T>
inline cl::Event writeBuffer(CLBuffer const &              buffer,
                             std::vector<T> const &        data,
                             size_t                        offset   = 0,
                             cl_bool                       blocking = CL_TRUE,
                             std::vector<cl::Event> const &events   = {}) {
  return writeBuffer(buffer, data.data(), data.size() * sizeof(T), offset,
                     blocking, events);
}

inline cl::Event readBuffer(CLBuffer const &              buffer,
                            void *                        ptr,
                            size_t                        size,
                            size_t                        offset   = 0,
                            cl_bool                       blocking = CL_TRUE,
                            std::vector<cl::Event> const &events   = {}) {
  cl::Event resultEvent;
  cl_int    err = buffer.clb.queue.enqueueReadBuffer(
      buffer.buffer, blocking, offset, size, ptr, &events, &resultEvent);
  if (err != CL_SUCCESS) {
    std::cerr << "Cannot read buffer: " << clErrorCodes.at(err) << std::endl;
    exit(0);
  }
  return resultEvent;
}

template <typename T>
inline cl::Event readBuffer(CLBuffer const &              buffer,
                            std::vector<T> &              data,
                            size_t                        offset   = 0,
                            cl_bool                       blocking = CL_TRUE,
                            std::vector<cl::Event> const &events   = {}) {
  return readBuffer(buffer, data.data(), data.size() * sizeof(T), offset,
                    blocking, events);
}

template <typename T>
inline CLBuffer createBuffer(CLBase const &        clb,
                             std::vector<T> const &data,
                             cl_mem_flags          flags = CL_MEM_READ_WRITE) {
  auto buffer = createBuffer(clb, data.size() * sizeof(T), flags);
  writeBuffer(buffer, data);
  return buffer;
}

inline void checkCreateKernel(cl_int err) {
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << "Cannot create kernel: " << clErrorCodes.at(err);
  throw std::runtime_error(ss.str());
}

inline CLKernel createKernel(CLProgram const &  program,
                             std::string const &name) {
  CLKernel result(program.clb, program);
  cl_int   err;
  result.kernel = cl::Kernel(program.program, name.c_str(), &err);
  checkCreateKernel(err);
  return result;
}

inline CLKernel createKernel(CLBase      const &clb                ,
                             std::string const &src                ,
                             std::string const &options    = ""    ,
                             std::string const &kernelName = "main"){
  auto program = createProgram(clb, src, options);
  return createKernel(program, kernelName);
}

inline cl::Event enqueueKernel(CLKernel const &   kernel,
                               cl::NDRange const &global,
                               cl::NDRange const &local  = cl::NullRange,
                               cl::NDRange const &offset = cl::NullRange,
                               std::vector<cl::Event> const &events = {}) {
  cl::Event resultEvent;
  cl_int    err = kernel.clb.queue.enqueueNDRangeKernel(
      kernel.kernel, offset, global, local, &events, &resultEvent);
  if (err != CL_SUCCESS) {
    std::cerr << "Cannot enqueue kernel: " << clErrorCodes.at(err) << std::endl;
    exit(0);
  }
  return resultEvent;
}

template <typename T,
          std::enable_if_t<!std::is_same<T, CLBuffer>::value, int> = 0>
inline void setArg(CLKernel &kernel, cl_int i, T const &d) {
  cl_int err = kernel.kernel.setArg(i, d);

  if (err != CL_SUCCESS) {
    std::stringstream ss;
    ss << "Cannot set argument: " << i << " for kernel: ";
    ss << kernel.kernel.getInfo<CL_KERNEL_FUNCTION_NAME>() << " - ";
    ss << clErrorCodes.at(err) << std::endl;
    auto const nofArgs = kernel.kernel.getInfo<CL_KERNEL_NUM_ARGS>();
    ss << "number of kernel attributes: " << nofArgs << std::endl;
    for (typename std::decay<decltype(nofArgs)>::type i = 0; i < nofArgs; ++i) {
      cl_int argNameErr;
      auto   argName =
          kernel.kernel.getArgInfo<CL_KERNEL_ARG_NAME>(i, &argNameErr);
      if (argNameErr != CL_SUCCESS) argName = clErrorCodes.at(argNameErr);
      cl_int argTypeErr;
      auto   argType =
          kernel.kernel.getArgInfo<CL_KERNEL_ARG_TYPE_NAME>(i, &argTypeErr);
      if (argTypeErr != CL_SUCCESS) argType = clErrorCodes.at(argNameErr);
      ss << argName << " - " << argType << " " << std::endl;
    }
    std::cerr << ss.str() << std::endl;
    exit(0);
  }
}

inline void setArg(CLKernel &kernel, cl_int i, CLBuffer const &b) {
  setArg(kernel, i, b.buffer);
}

template <typename T,
          typename... ARGS,
          typename std::enable_if_t<sizeof...(ARGS) != 0, int> = 0>
inline void setArg(CLKernel &kernel,
                   cl_int    i,
                   T const & t0,
                   ARGS const &... args) {
  setArg(kernel, i, t0);
  setArg(kernel, i + 1, args...);
}

template <typename... ARGS>
inline void setArgs(CLKernel &kernel, ARGS const &... args) {
  setArg(kernel, 0, args...);
}

inline void finish(CLBase const &clb) { clb.queue.finish(); }

class KernelParameter {
  public:
  KernelParameter(cl_int parameterId) : parameterId(parameterId) {}
  virtual ~KernelParameter() {}
  virtual void set(CLKernel &kernel) const = 0;

  protected:
  cl_int parameterId;
};

template <typename T>
class TypedKernelParameter : public KernelParameter {
  public:
  TypedKernelParameter(cl_int parameterId, T const &parameter)
      : KernelParameter(parameterId), parameter(parameter) {}
  virtual void set(CLKernel &kernel) const override {
    setArg(kernel, parameterId, parameter);
  }

  protected:
  T parameter;
};

using KernelParameters = std::map<size_t, std::shared_ptr<KernelParameter>>;

inline void setKernelParameters(CLKernel &              kernel,
                                KernelParameters const &parameters) {
  for (auto const &x : parameters) x.second->set(kernel);
}

template <typename T>
void addKernelParameter(KernelParameters &parameters,
                        size_t            parameterId,
                        T const &         parameter) {
  parameters[parameterId] =
      std::make_shared<TypedKernelParameter<T>>(parameterId, parameter);
}
