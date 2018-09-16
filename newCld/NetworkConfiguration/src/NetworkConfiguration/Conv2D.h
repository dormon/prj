/*!
 * @file 
 * @brief This file contains the implementation of Conv2D class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<Tensor/Tensor.h>

#include<NetworkConfiguration/Layer.h>
#include<NetworkConfiguration/PaddingType.h>

/**
 * @brief This class represents Conv2D layer configuration.
 *
 * @tparam TYPE base type of parameters
 */
template<typename TYPE>
class cnnConf::Conv2D: public Layer{
  public:
    Conv2D(
        std::string             const&name   ,
        std::vector<std::string>const&inputs ,
        std::set   <std::string>const&outputs,
        Tensor<TYPE>            const&weights,
        std::vector<size_t>     const&stride ,
        PaddingType             const&padding);
    size_t                   getWidth           ()const;
    size_t                   getHeight          ()const;
    size_t                   getNofInputChannels()const;
    size_t                   getNofFilters      ()const;
    Tensor<TYPE>       const&getWeights         ()const;
    std::vector<size_t>const&getStride          ()const;
    PaddingType              getPadding         ()const;
    struct WEIGHTS{
      constexpr static size_t DIMENSION = 4;
      struct AXES{
        constexpr static size_t WIDTH   = 0;
        constexpr static size_t HEIGHT  = 1;
        constexpr static size_t CHANNEL = 2;
        constexpr static size_t FILTER  = 3;
      };
    };
    struct BIASES{
      constexpr static size_t DIMENSION = 1;
      struct AXES{
        constexpr static size_t FILTER = 0;
      };
    };
  protected:
    Tensor<TYPE>       weights    ;
    Tensor<TYPE>       biases     ;
    std::vector<size_t>stride     ;
    PaddingType        paddingType;
};

/**
 * @brief This is the constructor of Conv2D class.
 *
 * @param name the name of this layer
 * @param inputs The vector of names of layers that are inputs to this layer.
 * @param outputs The set of names of layers that have this layer as input.
 * @param weights weights of convolution kernel
 * @param biases biases 
 * @param activation the activation type
 */
template<typename TYPE>
inline cnnConf::Conv2D<TYPE>::Conv2D(
    std::string             const&name      ,
    std::vector<std::string>const&inputs    ,
    std::set   <std::string>const&outputs   ,
    Tensor<TYPE>            const&weights   ,
    std::vector<size_t>     const&stride    ,
    PaddingType             const&padding   ):
  Layer      (Layer::CONV_2D,name,inputs,outputs),
  weights    (weights                           ),
  stride     (stride                            ),
  paddingType(padding                           )
{
  if(inputs.size() != 1){
    std::stringstream ss;
    ss << "Conv2D layer (" << name << ")";
    ss << " has to have exactly one input layer, inputs: ";
    for(auto const&x:inputs)
      ss << x << " ";
    throw std::invalid_argument(ss.str());
  }
  auto const weightsDimension = getDimension(weights);
  if(weightsDimension != WEIGHTS::DIMENSION){
    std::stringstream ss;
    ss << "Conv2D layer (" << name << ")";
    ss << " has to have weights with dimension equals to: " << WEIGHTS::DIMENSION;
    ss << " , given weights has dimension: " << weightsDimension;
    throw std::invalid_argument(ss.str());
  }
}

/**
 * @brief This function returns the width of convolution kernel.
 *
 * @return the width of convolution kernel
 */
template<typename TYPE>
inline size_t cnnConf::Conv2D<TYPE>::getWidth()const{
  assert(getDimension(weights) == WEIGHTS::DIMENSION);
  return weights.size.at(WEIGHTS::AXES::WIDTH);
}

/**
 * @brief This function returns the height of convolution kernel.
 *
 * @return the height of convolution kernel
 */
template<typename TYPE>
inline size_t cnnConf::Conv2D<TYPE>::getHeight()const{
  assert(getDimension(weights) == WEIGHTS::DIMENSION);
  return weights.size.at(WEIGHTS::AXES::HEIGHT);
}

/**
 * @brief This function returns the number of channels of convolution kernel.
 *
 * @return the number of channels of convolution kernel.
 */
template<typename TYPE>
inline size_t cnnConf::Conv2D<TYPE>::getNofInputChannels()const{
  assert(getDimension(weights) == WEIGHTS::DIMENSION);
  return weights.size.at(WEIGHTS::AXES::CHANNEL);
}

/**
 * @brief This function returns the number of convolution kernels.
 *
 * @return the number of convolution kernels.
 */
template<typename TYPE>
inline size_t cnnConf::Conv2D<TYPE>::getNofFilters()const{
  assert(getDimension(weights) == WEIGHTS::DIMENSION);
  return weights.size.at(WEIGHTS::AXES::FILTER);
}

/**
 * @brief This function returns weights of convolution kernels as 4D tensor.
 *
 * @return 4D tensor of convolution kernels weights
 */
template<typename TYPE>
inline Tensor<TYPE>const&cnnConf::Conv2D<TYPE>::getWeights()const{
  return weights;
}

/**
 * @brief This function returns the strides.
 *
 * @return the strides
 */
template<typename TYPE>
std::vector<size_t>const&      cnnConf::Conv2D<TYPE>::getStride    ()const{
  return stride;
}

/**
 * @brief This function returns the padding type.
 *
 * @return the padding type
 */
template<typename TYPE>
inline cnnConf::PaddingType    cnnConf::Conv2D<TYPE>::getPadding   ()const{
  return paddingType;
}
