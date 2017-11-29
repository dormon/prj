#include<NetworkConfiguration/Conv2D.h>

cnnConf::Conv2D::Conv2D(
    Tensor<float>const&weights,
    Tensor<float>const&biases ):
  Layer(Layer::CONV_2D),
  weights(weights),
  biases(biases)
{
}

size_t cnnConf::Conv2D::getWidth()const{
  return weights.size.at(0);
}

size_t cnnConf::Conv2D::getHeight()const{
  return weights.size.at(1);
}

size_t cnnConf::Conv2D::getNofInputChannels()const{
  return weights.size.at(2);
}

size_t cnnConf::Conv2D::getNofFilters()const{
  return weights.size.at(3);
}

Tensor<float>const&cnnConf::Conv2D::getWeights()const{
  return weights;
}

Tensor<float>const&cnnConf::Conv2D::getBiases()const{
  return biases;
}
