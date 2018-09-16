/*!
 * @file 
 * @brief This file contains the implementation of bias class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<Tensor/Tensor.h>
#include<NetworkConfiguration/Layer.h>

/**
 * @brief This class represents bias layer configuration.
 * This layer adds bias to input tensor.
 *
 * @tparam TYPE base type of parameters
 */
template<typename TYPE>
class cnnConf::Bias: public Layer{
  public:
    Bias(
        std::string             const&name   ,
        std::vector<std::string>const&inputs ,
        std::set   <std::string>const&outputs,
        Tensor<TYPE>            const&biases );
    Tensor<TYPE>const&getBiases()const;
  protected:
    Tensor<TYPE>biases;
};


template<typename TYPE>
cnnConf::Bias<TYPE>::Bias(
    std::string             const&name   ,
    std::vector<std::string>const&inputs ,
    std::set   <std::string>const&outputs,
    Tensor<TYPE>            const&biases ):
  Layer (Layer::BIAS,name,inputs,outputs),
  biases(biases                         )
{
}

template<typename TYPE>
Tensor<TYPE>const&cnnConf::Bias<TYPE>::getBiases()const{
  return biases;
}
