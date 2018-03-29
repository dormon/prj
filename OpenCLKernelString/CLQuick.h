#pragma once

#include <CL/cl.hpp>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

/**
 * This function splits string using splitter string
 * @param txt string that will be splitted
 * @param splitter splitter string
 *
 * @return list of substring, for string "a,b,c" and splitter "," ["a","b","c"]
 */
std::vector<std::string> splitString(std::string const &txt,
                                     std::string const &splitter) {
  std::vector<std::string> result;
  size_t                   endPosition  = 0;
  size_t                   lastPosition = 0;
  size_t const             splitterSize = splitter.size();
  while ((endPosition = txt.find(splitter, lastPosition)) !=
         std::string::npos) {
    result.push_back(txt.substr(lastPosition, endPosition - lastPosition));
    lastPosition = endPosition + splitterSize;
  }
  result.push_back(txt.substr(lastPosition, txt.size() - lastPosition));
  return result;
}

/**
 * @brief This function removes white spaces from string
 *
 * @param txt input string
 *
 * @return string without with spaces
 */
std::string removeSpaces(std::string const &txt) {
  std::string result = "";
  for (auto const &x : txt)
    if (!std::isspace(x)) result += x;
  return result;
}

/**
 * @brief This function loads text file.
 *
 * @param fileName file name
 *
 * @return content of the file or empty string
 */
std::string loadTextFile(std::string const &fileName) {
  std::ifstream f(fileName.c_str());
  if (!f.is_open()) return "";
  std::string str((std::istreambuf_iterator<char>(f)),
                  std::istreambuf_iterator<char>());
  f.close();
  return str;
}

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

DEFINE_LIST(cl_platform_info, clPlatformParams, CL_PLATFORM_EXTENSIONS,
            CL_PLATFORM_NAME, CL_PLATFORM_PROFILE, CL_PLATFORM_VENDOR,
            CL_PLATFORM_VERSION);

DEFINE_LIST(cl_device_info, clDeviceParams, CL_DEVICE_BUILT_IN_KERNELS,
            CL_DEVICE_EXTENSIONS, CL_DEVICE_MAX_WORK_ITEM_SIZES, CL_DEVICE_NAME,
            CL_DEVICE_OPENCL_C_VERSION, CL_DEVICE_PARTITION_PROPERTIES,
            CL_DEVICE_PARTITION_TYPE, CL_DEVICE_PROFILE, CL_DEVICE_VENDOR,
            CL_DEVICE_VERSION, CL_DRIVER_VERSION);

DEFINE_LIST(cl_device_partition_property, clDeviceParatitionProperty,
            CL_DEVICE_PARTITION_EQUALLY, CL_DEVICE_PARTITION_BY_COUNTS,
            CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN);

DEFINE_LIST(cl_device_type, clDeviceTypes, CL_DEVICE_TYPE_CPU,
            CL_DEVICE_TYPE_GPU, CL_DEVICE_TYPE_ACCELERATOR,
            CL_DEVICE_TYPE_CUSTOM, CL_DEVICE_TYPE_DEFAULT, CL_DEVICE_TYPE_ALL);

DEFINE_LIST(
    cl_int, clErrorCodes, CL_SUCCESS, CL_DEVICE_NOT_FOUND,
    CL_DEVICE_NOT_AVAILABLE, CL_COMPILER_NOT_AVAILABLE,
    CL_MEM_OBJECT_ALLOCATION_FAILURE, CL_OUT_OF_RESOURCES,
    CL_OUT_OF_HOST_MEMORY, CL_PROFILING_INFO_NOT_AVAILABLE, CL_MEM_COPY_OVERLAP,
    CL_IMAGE_FORMAT_MISMATCH, CL_IMAGE_FORMAT_NOT_SUPPORTED,
    CL_BUILD_PROGRAM_FAILURE, CL_MAP_FAILURE, CL_MISALIGNED_SUB_BUFFER_OFFSET,
    CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST, CL_COMPILE_PROGRAM_FAILURE,
    CL_LINKER_NOT_AVAILABLE, CL_LINK_PROGRAM_FAILURE,
    CL_DEVICE_PARTITION_FAILED, CL_KERNEL_ARG_INFO_NOT_AVAILABLE,
    CL_INVALID_VALUE, CL_INVALID_DEVICE_TYPE, CL_INVALID_PLATFORM,
    CL_INVALID_DEVICE, CL_INVALID_CONTEXT, CL_INVALID_QUEUE_PROPERTIES,
    CL_INVALID_COMMAND_QUEUE, CL_INVALID_HOST_PTR, CL_INVALID_MEM_OBJECT,
    CL_INVALID_IMAGE_FORMAT_DESCRIPTOR, CL_INVALID_IMAGE_SIZE,
    CL_INVALID_SAMPLER, CL_INVALID_BINARY, CL_INVALID_BUILD_OPTIONS,
    CL_INVALID_PROGRAM, CL_INVALID_PROGRAM_EXECUTABLE, CL_INVALID_KERNEL_NAME,
    CL_INVALID_KERNEL_DEFINITION, CL_INVALID_KERNEL, CL_INVALID_ARG_INDEX,
    CL_INVALID_ARG_VALUE, CL_INVALID_ARG_SIZE, CL_INVALID_KERNEL_ARGS,
    CL_INVALID_WORK_DIMENSION, CL_INVALID_WORK_GROUP_SIZE,
    CL_INVALID_WORK_ITEM_SIZE, CL_INVALID_GLOBAL_OFFSET,
    CL_INVALID_EVENT_WAIT_LIST, CL_INVALID_EVENT, CL_INVALID_OPERATION,
    CL_INVALID_GL_OBJECT, CL_INVALID_BUFFER_SIZE, CL_INVALID_MIP_LEVEL,
    CL_INVALID_GLOBAL_WORK_SIZE, CL_INVALID_PROPERTY,
    CL_INVALID_IMAGE_DESCRIPTOR, CL_INVALID_COMPILER_OPTIONS,
    CL_INVALID_LINKER_OPTIONS, CL_INVALID_DEVICE_PARTITION_COUNT);

DEFINE_LIST(cl_context_info, clContextParams, CL_CONTEXT_REFERENCE_COUNT,
            CL_CONTEXT_NUM_DEVICES, CL_CONTEXT_DEVICES, CL_CONTEXT_PROPERTIES);

DEFINE_LIST(cl_context_properties, clContextProperties, CL_CONTEXT_PLATFORM,
            CL_CONTEXT_INTEROP_USER_SYNC);

DEFINE_LIST(cl_mem_flags, clMemoryFlags, CL_MEM_READ_WRITE, CL_MEM_WRITE_ONLY,
            CL_MEM_READ_ONLY, CL_MEM_USE_HOST_PTR, CL_MEM_ALLOC_HOST_PTR,
            CL_MEM_COPY_HOST_PTR, CL_MEM_HOST_WRITE_ONLY, CL_MEM_HOST_READ_ONLY,
            CL_MEM_HOST_NO_ACCESS);

DEFINE_LIST(cl_mem_object_type, clMemoryBaseImageTypes, CL_MEM_OBJECT_IMAGE2D,
            CL_MEM_OBJECT_IMAGE3D);

DEFINE_LIST(cl_mem_object_type, clMemoryTypes, CL_MEM_OBJECT_BUFFER,
            CL_MEM_OBJECT_IMAGE2D, CL_MEM_OBJECT_IMAGE3D,
            CL_MEM_OBJECT_IMAGE2D_ARRAY, CL_MEM_OBJECT_IMAGE1D,
            CL_MEM_OBJECT_IMAGE1D_ARRAY, CL_MEM_OBJECT_IMAGE1D_BUFFER);

DEFINE_LIST(cl_channel_order, clChannelOrder, CL_R, CL_A, CL_RG, CL_RA, CL_RGB,
            CL_RGBA, CL_BGRA, CL_ARGB, CL_INTENSITY, CL_LUMINANCE, CL_Rx,
            CL_RGx, CL_RGBx, CL_DEPTH, CL_DEPTH_STENCIL);

DEFINE_LIST(cl_channel_type, clChannelType, CL_SNORM_INT8, CL_SNORM_INT16,
            CL_UNORM_INT8, CL_UNORM_INT16, CL_UNORM_SHORT_565,
            CL_UNORM_SHORT_555, CL_UNORM_INT_101010, CL_SIGNED_INT8,
            CL_SIGNED_INT16, CL_SIGNED_INT32, CL_UNSIGNED_INT8,
            CL_UNSIGNED_INT16, CL_UNSIGNED_INT32, CL_HALF_FLOAT, CL_FLOAT,
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
  CLKernel(CLBase const &c) : clb(c) {}
};

inline CLBase createContext() {
  // get all platforms (drivers)
  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);
  if (all_platforms.empty()) {
    std::cout << " No platforms found. Check OpenCL installation!\n";
    exit(1);
  }
  cl::Platform default_platform = all_platforms[0];
  std::cout << "Using platform: "
            << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

  // get default device of the default platform
  std::vector<cl::Device> all_devices;
  default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
  if (all_devices.empty()) {
    std::cout << " No devices found. Check OpenCL installation!\n";
    exit(1);
  }
  CLBase result;
  result.dev = all_devices[0];
  std::cout << "Using device: " << result.dev.getInfo<CL_DEVICE_NAME>() << "\n";

  result.ctx = cl::Context({result.dev});

  cl_int qerr;
  result.queue = cl::CommandQueue(result.ctx, result.dev, 0, &qerr);
  if (qerr != CL_SUCCESS) {
    std::cerr << "Cannot create queue" << std::endl;
    exit(0);
  }
  return result;
}

inline CLProgram createProgram(CLBase const &clb, std::string const &src) {
  CLProgram            result(clb);
  cl::Program::Sources sources;

  // kernel calculates for each element C=A+B
  sources.push_back({src.c_str(), src.length()});

  result.program = cl::Program(clb.ctx, sources);

  if (result.program.build({clb.dev}) != CL_SUCCESS) {
    std::cout << " Error building: "
              << result.program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(clb.dev)
              << "\n";
    exit(1);
  }
  return result;
}

inline CLBuffer createBuffer(CLBase const &clb, size_t size,
                             cl_mem_flags flags = CL_MEM_READ_WRITE) {
  CLBuffer result(clb);
  cl_int   err;
  result.buffer = cl::Buffer(clb.ctx, flags, size, nullptr, &err);
  if (err != CL_SUCCESS) {
    std::cerr << "Cannot create buffer:" << clErrorCodes.at(err) << std::endl;
    std::cerr << size << std::endl;
    exit(0);
  }
  return result;
}

inline cl::Event writeBuffer(CLBuffer const &buffer, void const *ptr, size_t size,
                        size_t offset = 0, cl_bool blocking = CL_TRUE,std::vector<cl::Event>const&events = {}) {
  cl::Event resultEvent;
  cl_int err = buffer.clb.queue.enqueueWriteBuffer(buffer.buffer, blocking,
                                                   offset, size, ptr,&events,&resultEvent);
  if (err != CL_SUCCESS) {
    std::cerr << "Cannot write buffer: " << clErrorCodes.at(err) << std::endl;
    exit(0);
  }
  return resultEvent;
}

template <typename T>
inline cl::Event writeBuffer(CLBuffer const &buffer, std::vector<T> const &data,
                        size_t offset = 0, cl_bool blocking = CL_TRUE,std::vector<cl::Event>const&events = {}) {
  return writeBuffer(buffer, data.data(), data.size() * sizeof(T), offset, blocking,events);
}

inline cl::Event readBuffer(CLBuffer const &buffer, void *ptr, size_t size,
                       size_t offset = 0, cl_bool blocking = CL_TRUE,std::vector<cl::Event>const&events = {}) {
  cl::Event resultEvent;
  cl_int err = buffer.clb.queue.enqueueReadBuffer(buffer.buffer, blocking,
                                                  offset, size, ptr,&events,&resultEvent);
  if (err != CL_SUCCESS) {
    std::cerr << "Cannot read buffer: " << clErrorCodes.at(err) << std::endl;
    exit(0);
  }
  return resultEvent;
}

template <typename T>
inline cl::Event readBuffer(CLBuffer const &buffer, std::vector<T> &data,
                       size_t offset = 0, cl_bool blocking = CL_TRUE,std::vector<cl::Event>const&events = {}) {
  return readBuffer(buffer, data.data(), data.size() * sizeof(T), offset, blocking,events);
}

template <typename T>
inline CLBuffer createBuffer(CLBase const &clb, std::vector<T> const &data,
                             cl_mem_flags flags = CL_MEM_READ_WRITE) {
  auto buffer = createBuffer(clb, data.size() * sizeof(T), flags);
  writeBuffer(buffer, data);
  return buffer;
}

inline cl::NDRange createRange(size_t dim) {
  if (dim == 1) return cl::NDRange(0);
  if (dim == 2) return cl::NDRange(0, 0);
  return cl::NDRange(0, 0, 0);
}

inline CLKernel createKernel(CLProgram const &  program,
                             std::string const &name) {
  CLKernel result(program.clb);
  cl_int   err;
  result.kernel = cl::Kernel(program.program, name.c_str(), &err);
  if (err != CL_SUCCESS) {
    std::cerr << "Cannot create kernel: " << clErrorCodes.at(err) << std::endl;
    exit(0);
  }
  return result;
}

inline cl::Event enqueueKernel(CLKernel const &kernel, cl::NDRange const &global,
                          cl::NDRange const &local  = cl::NullRange,
                          cl::NDRange const &offset = cl::NullRange,std::vector<cl::Event>const&events = {}) {
  cl::Event resultEvent;
  cl_int err = kernel.clb.queue.enqueueNDRangeKernel(kernel.kernel, offset,
                                                     global, local,&events,&resultEvent);
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

template <typename T, typename... ARGS,
          typename std::enable_if_t<sizeof...(ARGS) != 0, int> = 0>
inline void setArg(CLKernel &kernel, cl_int i, T const &t0,
                   ARGS const &... args) {
  setArg(kernel, i, t0);
  setArg(kernel, i + 1, args...);
}

template <typename... ARGS>
inline void setArgs(CLKernel &kernel, ARGS const &... args) {
  setArg(kernel, 0, args...);
}

inline void finish(CLBase const &clb) { clb.queue.finish(); }
