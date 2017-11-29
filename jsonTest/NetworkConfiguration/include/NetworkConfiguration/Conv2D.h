#pragma once

#include<NetworkConfiguration/Layer.h>
#include<Tensor/Tensor.h>

class NETWORKCONFIGURATION_EXPORT cnnConf::Conv2D: public Layer{
  public:
    Conv2D(
        Tensor<float>const&weights,
        Tensor<float>const&biases );
    size_t getWidth()const;
    size_t getHeight()const;
    size_t getNofInputChannels()const;
    size_t getNofFilters()const;
    Tensor<float>const&getWeights()const;
    Tensor<float>const&getBiases()const;
  protected:
    Tensor<float>weights;
    Tensor<float>biases;
};

