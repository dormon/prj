#pragma once

#include<array>

#include<NetworkConfiguration/Layer.h>
#include<Tensor/Tensor.h>

class NETWORKCONFIGURATION_EXPORT cnnConf::MaxPooling: public Layer{
  public:
    MaxPooling(
        std::array<size_t,2>const&size  ,
        std::array<size_t,2>const&stride);
    size_t getSizeX  ()const;
    size_t getSizeY  ()const;
    size_t getStrideX()const;
    size_t getStrideY()const;
    std::array<size_t,2>getSize  ()const;
    std::array<size_t,2>getStride()const;
  protected:
    std::array<size_t,2>size  ;
    std::array<size_t,2>stride;
};

