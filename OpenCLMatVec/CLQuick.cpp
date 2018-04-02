#include <CLQuick.h>

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
    ss << "device: " << getDeviceName(device) << std::endl;
    ss << getProgramBuildLog(program, device);
  }
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
  throw ss.str();
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


CLBase createCtx() {
  // get all platforms (drivers)
  auto const   all_platforms    = clq::getPlatforms();
  cl::Platform default_platform = all_platforms[0];
  std::cout << "Using platform: "
            << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

  // get default device of the default platform
  auto const all_devices = clq::getDevices(default_platform);
  CLBase     result;
  result.dev = all_devices[0];
  std::cout << "Using device: " << result.dev.getInfo<CL_DEVICE_NAME>() << "\n";

  result.ctx = clq::createContext({all_devices[0]});

  cl_int qerr;
  result.queue = cl::CommandQueue(result.ctx, result.dev, 0, &qerr);
  if (qerr != CL_SUCCESS) {
    std::cerr << "Cannot create queue" << std::endl;
    exit(0);
  }
  return result;
}

void checkCreateProgram(cl_int err, cl::Program const &p, cl::Device const &d) {
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << " Error building: " << p.getBuildInfo<CL_PROGRAM_BUILD_LOG>(d);
  throw std::runtime_error(ss.str());
}

CLProgram createProgram(CLBase const &     clb,
                        std::string const &src,
                        std::string const &options) {
  CLProgram            result(clb);
  cl::Program::Sources sources;

  // kernel calculates for each element C=A+B
  sources.push_back({src.c_str(), src.length()});

  result.program = cl::Program(clb.ctx, sources);

  cl_int err = result.program.build({clb.dev}, options.c_str());
  checkCreateProgram(err, result.program, clb.dev);
  return result;
}

void checkCreateBuffer(cl_int err) {
  if (err == CL_SUCCESS) return;
  std::stringstream ss;
  ss << "Cannot create buffer:" << clErrorCodes.at(err);
  throw std::runtime_error(ss.str());
}

CLBuffer createBuffer(CLBase const &clb, size_t size, cl_mem_flags flags) {
  CLBuffer result(clb);
  cl_int   err;
  result.buffer = cl::Buffer(clb.ctx, flags, size, nullptr, &err);
  checkCreateBuffer(err);
  return result;
}

cl::Event writeBuffer(CLBuffer const &              buffer,
                      void const *                  ptr,
                      size_t                        size,
                      size_t                        offset,
                      cl_bool                       blocking,
                      std::vector<cl::Event> const &events) {
  cl::Event resultEvent;
  cl_int    err = buffer.clb.queue.enqueueWriteBuffer(
      buffer.buffer, blocking, offset, size, ptr, &events, &resultEvent);
  if (err != CL_SUCCESS) {
    std::cerr << "Cannot write buffer: " << clErrorCodes.at(err) << std::endl;
    exit(0);
  }
  return resultEvent;
}
