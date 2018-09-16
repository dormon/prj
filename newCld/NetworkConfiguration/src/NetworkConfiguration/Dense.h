/** 
 * @file
 * @brief This file contains dense layer configuration.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<Tensor/Tensor.h>

#include<NetworkConfiguration/Layer.h>
#include<NetworkConfiguration/ActivationType.h>

/**
 * @brief This class represents Dense layer.
 *
 * @tparam TYPE base type of parameters.
 */
template<typename TYPE>
class cnnConf::Dense: public Layer{
  public:
    Dense(
        std::string             const&name      ,
        std::vector<std::string>const&inputs    ,
        std::set   <std::string>const&outputs   ,
        Tensor<TYPE>            const&weights   ,
        Tensor<TYPE>            const&biases    ,
        ActivationType          const&activation);
    size_t getInputSize()const;
    size_t getOutputSize()const;
    Tensor<TYPE>const&getWeights()const;
    Tensor<TYPE>const&getBiases()const;
    ActivationType getActivation()const;
    struct WEIGHTS{
      constexpr static size_t DIMENSION = 2;
      struct AXES{
        constexpr static size_t INPUT  = 0;
        constexpr static size_t OUTPUT = 1;
      };
    };
    struct BIASES{
      constexpr static size_t DIMENSION = 1;
      struct AXES{
        constexpr static size_t OUTPUT = 0;
      };
    };
  protected:
    Tensor<TYPE>   weights   ;
    Tensor<TYPE>   biases    ;
    ActivationType activation;
};

/**
 * @brief This is the constructor of Dense class.
 *
 * @param name The name of layer
 * @param inputs The vector of names of layers that are inputs to this layer.
 * @param outputs The set of names of layers that have this layer as input.
 * @param weights weights (2D tensor)
 * @param biases biases (1D tensor)
 * @param activation the activation type
 */
template<typename TYPE>
inline cnnConf::Dense<TYPE>::Dense(
    std::string             const&name      ,
    std::vector<std::string>const&inputs    ,
    std::set   <std::string>const&outputs   ,
    Tensor<TYPE>            const&weights   ,
    Tensor<TYPE>            const&biases    ,
    ActivationType          const&activation):
  Layer     (Layer::DENSE,name,inputs,outputs),
  weights   (weights                         ),
  biases    (biases                          ),
  activation(activation                      )
{
  auto const weightsDimension = getDimension(weights);
  if(weightsDimension != WEIGHTS::DIMENSION){
    std::stringstream ss;
    ss << "cnnConf::Dense failed - ";
    ss << "weights parameter does not have corret dimension. ";
    ss << "Expected: " << WEIGHTS::DIMENSION;
    ss << " Given: " << weightsDimension;
    throw std::invalid_argument(ss.str());
  }
}

/**
 * @brief This function returns the size of input tensor.
 *
 * @return the size of input tensor
 */
template<typename TYPE>
inline size_t cnnConf::Dense<TYPE>::getInputSize()const{
  assert(WEIGHTS::AXES::INPUT < weights.size.size());
  return weights.size[WEIGHTS::AXES::INPUT];
}

/**
 * @brief This function returns the size of output tensor.
 *
 * @return the size of output tensor
 */
template<typename TYPE>
inline size_t cnnConf::Dense<TYPE>::getOutputSize()const{
  assert(WEIGHTS::AXES::OUTPUT < weights.size.size());
  return weights.size[WEIGHTS::AXES::OUTPUT];
}

/**
 * @brief This function returns weights (2D tensor).
 *
 * @return weights (2D tensor)
 */
template<typename TYPE>
inline Tensor<TYPE>const&cnnConf::Dense<TYPE>::getWeights()const{
  return weights;
}

/**
 * @brief This function returns biases (1D tensor).
 *
 * @return biases (1D tensor)
 */
template<typename TYPE>
inline Tensor<TYPE>const&cnnConf::Dense<TYPE>::getBiases()const{
  return biases;
}

/**
 * @brief This function returns the activation type.
 *
 * @return the activation type
 */
template<typename TYPE>
inline cnnConf::ActivationType cnnConf::Dense<TYPE>::getActivation()const{
  return activation;
}
