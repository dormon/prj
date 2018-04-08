#pragma once

#include <CL/cl.hpp>

namespace clq {
std::vector<cl::Platform> getPlatforms();
std::vector<cl::Device> getDevices(cl::Platform const &platform);
cl::Context createContext(std::vector<cl::Device> const &devs);
cl::CommandQueue createCommandQueue(cl::Context const &context,
                                    cl::Device const & device);
cl::Program createProgram(cl::Context const &             context,
                          std::vector<cl::Device> const & devices,
                          std::vector<std::string> const &sources,
                          std::string const &             options = "");
cl::Kernel createKernel(cl::Program const &program, std::string const &name);
void checkSetArg(cl_int err, cl::Kernel const &kernel, cl_uint i);
template <typename T>
void setArg(cl::Kernel &kernel, cl_uint i, T const &data);
cl::Buffer createBuffer(cl::Context const &context, size_t size,
                        void *       data  = nullptr,
                        cl_mem_flags flags = CL_MEM_READ_WRITE);
cl::Image2DArray createImage2DArray(cl::Context const &    context,
                                    cl::ImageFormat const &format,
                                    size_t nofLayers, size_t width,
                                    size_t height, void *data = nullptr,
                                    cl_mem_flags flags = CL_MEM_READ_WRITE,
                                    size_t rowPitch = 0, size_t slicePitch = 0);
}

template <typename T>
void clq::setArg(cl::Kernel &kernel, cl_uint i, T const &data) {
  cl_int err = kernel.setArg<T>(i, data);
  checkSetArg(err, kernel, i);
}
