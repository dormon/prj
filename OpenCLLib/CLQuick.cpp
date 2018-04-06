#include <CLQuick.h>
#include <map>
#include <fstream>
#include <algorithm>
#include <iostream>
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

void checkGetPlatforms(std::vector<cl::Platform> const &platforms) {
  if (!platforms.empty()) return;
  throw std::runtime_error("No platforms found. Check OpenCL installation!");
}

std::vector<cl::Platform> clq::getPlatforms() {
  std::vector<cl::Platform> result;
  cl::Platform::get(&result);
  checkGetPlatforms(result);
  return result;
}

void checkGetDevices(std::vector<cl::Device> const &devices,
                     cl::Platform const &           platform) {
  if (!devices.empty()) return;
  std::stringstream ss;
  ss << "No devices found for platform: ";
  ss << platform.getInfo<CL_PLATFORM_NAME>();
  throw std::runtime_error(ss.str());
}

std::vector<cl::Device> clq::getDevices(cl::Platform const &platform) {
  std::vector<cl::Device> result;
  platform.getDevices(CL_DEVICE_TYPE_ALL, &result);
  checkGetDevices(result, platform);
  return result;
}

void checkCreateContext(cl_int err) {
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << "Cannot create context: " << clErrorCodes.at(err);
  throw std::runtime_error(ss.str());
}

cl::Context clq::createContext(std::vector<cl::Device> const &devs) {
  cl_int err;
  auto   ctx = cl::Context(devs, nullptr, nullptr, nullptr, &err);
  checkCreateContext(err);
  return ctx;
}

void checkCreateCommandQueue(cl_int err, cl::Device const &device) {
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << "Cannot create command queue: " << clErrorCodes.at(err) << std::endl;
  ss << " device: " << device.getInfo<CL_DEVICE_NAME>();
  throw std::runtime_error(ss.str());
}

cl::CommandQueue clq::createCommandQueue(cl::Context const &context,
                                         cl::Device const & device) {
  cl_int err;
  auto   result = cl::CommandQueue{context, device, 0, &err};
  checkCreateCommandQueue(err, device);
  return result;
}

std::string getDeviceName(cl::Device const &device) {
  cl_int err;
  auto   result = device.getInfo<CL_DEVICE_NAME>(&err);
  if (err == CL_SUCCESS) return result;
  return "cannot obtain device name!";
}

std::string getProgramBuildLog(cl::Program const &program,
                               cl::Device const & device) {
  cl_int err;
  auto   result = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device, &err);
  if (err == CL_SUCCESS) return result;
  return std::string("cannot obtain build log for device: ") +
         getDeviceName(device);
}

void checkBuildProgram(cl_int                         err,
                       cl::Program const &            program,
                       std::vector<cl::Device> const &devices) {
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << "Program building failed with following errors:" << std::endl;
  for (auto const &device : devices) {
    auto errorLog = getProgramBuildLog(program, device);
    auto deviceName = getDeviceName(device);
    ss << "device: " << deviceName << std::endl;
    ss << errorLog;
  }
  //std::cerr << ss.str() << std::endl;
  throw std::runtime_error(ss.str());
}

cl::Program clq::createProgram(cl::Context const &             context,
                               std::vector<cl::Device> const & devices,
                               std::vector<std::string> const &sources,
                               std::string const &             options) {
  cl::Program::Sources srcs;
  for (auto const &x : sources) srcs.push_back({x.c_str(), x.length()});
  cl_int           err;
  cl::Program      result(context, srcs, &err);
  if (err != CL_SUCCESS) throw std::runtime_error("Cannot create program!");
  err = result.build(devices, options.c_str());
  checkBuildProgram(err, result, devices);
  return result;
}

void checkCreateBuffer(cl_int            err,
                       size_t            size,
                       void const *const data,
                       cl_mem_flags      flags) {
  if (err != CL_SUCCESS) {
    std::stringstream ss;
    ss << "Cannot create buffer: " << std::endl;
    ss << "size: " << size << std::endl;
    ss << "host_ptr: " << data << std::endl;
    ss << "flags: ";
    for (auto const &x : clMemoryFlags)
      if ((x.first & flags) == x.first) ss << x.second << " ";
    ss << std::endl;
    ss << "error is: ";
    ss << clErrorCodes.at(err);
    throw std::runtime_error(ss.str());
  }
}

cl::Buffer clq::createBuffer(cl::Context const &context,
                             size_t             size,
                             void *             data,
                             cl_mem_flags       flags) {
  cl_int     err;
  cl::Buffer result(context, flags, size, data, &err);
  checkCreateBuffer(err, size, data, flags);
  return result;
}

void checkCreateKernel(cl_int err,std::string const&name){
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << "Cannot create kernel: ";
  ss << "name: " << name << std::endl;
  throw std::runtime_error(ss.str());
}

cl::Kernel clq::createKernel(cl::Program const &program,
                             std::string const &name) {
  cl_int err;
  auto   result = cl::Kernel(program, name.c_str(), &err);
  checkCreateKernel(err,name);
  return result;
}

std::string getKernelName(cl::Kernel const&kernel){
  cl_int err;
  auto const result = kernel.getInfo<CL_KERNEL_FUNCTION_NAME>(&err);
  if(err == CL_SUCCESS)return result;
  return std::string("cannot obtain kernel function name (")+clErrorCodes.at(err)+")";
}

void clq::checkSetArg(cl_int err,cl::Kernel const&kernel,cl_uint i){
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << "Cannot set argument: " << i << " for kernel: ";
  ss << getKernelName(kernel) << " - ";
  ss << clErrorCodes.at(err) << std::endl;
  auto const nofArgs = kernel.getInfo<CL_KERNEL_NUM_ARGS>();
  ss << " number of kernel attributes: " << nofArgs << std::endl;
  for (std::decay<decltype(nofArgs)>::type i = 0; i < nofArgs; ++i) {
    cl_int argNameErr;
    auto   argName = kernel.getArgInfo<CL_KERNEL_ARG_NAME>(i, &argNameErr);
    if (argNameErr != CL_SUCCESS) argName = clErrorCodes.at(argNameErr);
    cl_int argTypeErr;
    auto   argType = kernel.getArgInfo<CL_KERNEL_ARG_TYPE_NAME>(i, &argTypeErr);
    if (argTypeErr != CL_SUCCESS) argType = clErrorCodes.at(argNameErr);
    ss << argName << " - " << argType << " " << std::endl;
  }
  throw std::runtime_error(ss.str());
}

void checkCreateImage2DArray(cl_int err){
  if(err == CL_SUCCESS)return;
  throw std::runtime_error(std::string("Cannot create Image2DArray - ")+clErrorCodes.at(err));
}

cl::Image2DArray clq::createImage2DArray(cl::Context const &    context,
                                    cl::ImageFormat const &format,
                                    size_t                 nofLayers,
                                    size_t                 width,
                                    size_t                 height,
                                    void *                 data,
                                    cl_mem_flags flags     ,
                                    size_t       rowPitch  ,
                                    size_t       slicePitch){
  cl_int err;
  auto result = cl::Image2DArray(context,flags,format,nofLayers,width,height,rowPitch,slicePitch,data,&err);
  checkCreateImage2DArray(err);
  return result;
}


void checkCreateProgram(cl_int err, cl::Program const &p, cl::Device const &d) {
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << " Error building: " << p.getBuildInfo<CL_PROGRAM_BUILD_LOG>(d);
  throw std::runtime_error(ss.str());
}

void checkCreateBuffer(cl_int err) {
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << "Cannot create buffer:" << clErrorCodes.at(err);
  throw std::runtime_error(ss.str());
}

