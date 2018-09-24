#include <iostream>
#include <CL/cl.hpp>

int main(){
    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }

    std::cout << "platforms: " << std::endl;
    for(auto const&p:all_platforms)
      std::cout << p.getInfo<CL_PLATFORM_NAME>() << std::endl;
    std::cout << std::endl;

    cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }

    std::cout << "devices: " << std::endl;
    for(auto const&d:all_devices)
      std::cout << d.getInfo<CL_DEVICE_NAME>() << std::endl;
    std::cout << std::endl;

    cl::Device default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";


    cl::Context context({default_device});

    cl::Program::Sources sources;

    std::string kernel_code=
    R".(
    void kernel simple_add(global float* A){
      A[get_global_id(0)] = (float)get_global_id(0);
    }                                                                               
    ).";
    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);

    if(program.build({default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }


    // create buffers on the device
    cl::Buffer buffer_A(context,CL_MEM_READ_WRITE,sizeof(int)*10);

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,default_device);


    //run the kernel
    //cl::make_kernel<cl::Buffer&, cl::Buffer&, cl::Buffer&> simple_add(cl::Kernel(program,"simple_add"));
    //cl::EnqueueArgs eargs(queue,cl::NullRange,cl::NDRange(10),cl::NullRange);
    //simple_add(eargs, buffer_A,buffer_B,buffer_C).wait();


    //alternative way to run the kernel
    cl::Kernel kernel_add=cl::Kernel(program,"simple_add");
    kernel_add.setArg(0,buffer_A);
    queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
    queue.finish();

    int C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_A,CL_TRUE,0,sizeof(int)*10,C);

    std::cout<<" result: \n";
    for(int i=0;i<10;i++){
      std::cout << C[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
