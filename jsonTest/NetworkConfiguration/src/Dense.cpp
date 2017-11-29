#include<NetworkConfiguration/Dense.h>

cnnConf::Dense::Dense(
    Tensor<float>const&weights,
    Tensor<float>const&biases ):
  Layer(Layer::DENSE),
  weights(weights),
  biases(biases)
{
}

size_t cnnConf::Dense::getInputSize()const{
  return weights.size[0];
}

size_t cnnConf::Dense::getOutputSize()const{
  return weights.size[1];
}

Tensor<float>const&cnnConf::Dense::getWeights()const{
  return weights;
}

Tensor<float>const&cnnConf::Dense::getBiases()const{
  return biases;
}
