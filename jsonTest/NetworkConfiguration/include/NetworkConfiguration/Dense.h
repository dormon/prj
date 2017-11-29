#pragma once

#include<NetworkConfiguration/Layer.h>
#include<Tensor/Tensor.h>

class NETWORKCONFIGURATION_EXPORT cnnConf::Dense: public Layer{
  public:
    Dense(
        Tensor<float>const&weights,
        Tensor<float>const&biases );
    size_t getInputSize()const;
    size_t getOutputSize()const;
    Tensor<float>const&getWeights()const;
    Tensor<float>const&getBiases()const;
  protected:
    Tensor<float>weights;
    Tensor<float>biases ;
};

