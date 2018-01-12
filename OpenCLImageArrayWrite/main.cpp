#include<iostream>
#include<array>

#include"OpenCLHelper.h"

// kernel calculates for each element C=A+B
std::string static const kernel_code = R".(
void kernel setImageToOne(
  write_only image2d_array_t       output   ,
             uint            const nofLayers,
             uint2           const imageSize){
  int2 const pos = {get_global_id(0),get_global_id(1)};

  if(pos.x > imageSize.x || pos.y > imageSize.y)return;

  for(uint layer = 0; layer < nofLayers; ++layer){
    write_imagef(output,(int4)(pos,layer,0),1.f);
  }
}                                                                               
).";

int main(){
  try{
    //get platform
    auto const platform = getPlatform()();
    //std::cout << "Using platform: "<<platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get device
    auto const device = getDevice().givenPlatform(platform)();
    //std::cout<< "Using device: "<<device.getInfo<CL_DEVICE_NAME>()<<"\n";

    //get context
    auto const context = getContext().givenDevices({device})();

    //get program
    auto program = buildProgram().givenContext(context).givenDevices({device}).givenSources({kernel_code})();


    cl_uint const nofLayers   = 3;
    std::array<cl_uint,2>const imageSize{{10,10}};

    auto outputImage = createImage2DArray()
      .givenContext  (context        )
      .givenNofLayers(nofLayers      )
      .givenWidth    (imageSize.at(0))
      .givenHeight   (imageSize.at(1))();

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,device);

    //alternative way to run the kernel
    cl::Kernel kernel_setImageToOne=cl::Kernel(program,"setImageToOne");
    kernel_setImageToOne.setArg(0,outputImage);
    kernel_setImageToOne.setArg(1,nofLayers  );
    kernel_setImageToOne.setArg(2,imageSize  );

    auto const roundToClosestMultipleOf = [](size_t value,size_t multiplier){
      return (value/multiplier)*multiplier + multiplier*static_cast<size_t>((value%multiplier)!=0);
    };

    cl::NDRange const workGroupSize(8,8);
    cl::NDRange const globalSize(roundToClosestMultipleOf(imageSize.at(0),workGroupSize[0]),roundToClosestMultipleOf(imageSize.at(1),workGroupSize[1]));
    queue.enqueueNDRangeKernel(
        kernel_setImageToOne,
        cl::NullRange,//offset
        globalSize,
        workGroupSize);
    queue.finish();

    float data[nofLayers*imageSize.at(0)*imageSize.at(1)];
    cl::size_t<3>origin;
    origin[0] = 0;
    origin[1] = 0;
    origin[2] = 0;
    cl::size_t<3>region;
    region[0] = imageSize.at(0);
    region[1] = imageSize.at(1);
    region[2] = nofLayers;

    queue.enqueueReadImage(
        outputImage,
        CL_TRUE,//blocking read
        origin,
        region,
        0,//row pitch
        0,//slice pitch
        data,
        nullptr,
        nullptr);


    //std::cout<<" result: \n";

    size_t errorCounter = 0;
    for(std::decay<decltype(nofLayers)>::type l=0;l<nofLayers;++l){
      //std::cout << "layer: " << l << std::endl;
      for(std::decay<decltype(imageSize)::value_type>::type y = 0; y < imageSize.at(1); ++y){
        //std::cout << "row: " << y << std::endl;
        for(std::decay<decltype(imageSize)::value_type>::type x = 0; x < imageSize.at(0); ++x){
          if(data[l * imageSize.at(1) * imageSize.at(0) + y * imageSize.at(0) + x] != 1.f)
            errorCounter++;
          //std::cout << x << " : " << data[l * imageSize.at(1) * imageSize.at(0) + y * imageSize.at(0) + x]<< std::endl;
        }
      }
    }

    std::cerr << "Number of errors: " << errorCounter << std::endl;

  }catch(std::string const&e){
    std::cerr << "ERROR: " << std::endl;
    std::cerr << e; 
  }
  return 0;
}
