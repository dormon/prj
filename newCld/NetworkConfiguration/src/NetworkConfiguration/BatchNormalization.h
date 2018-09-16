/*!
 * @file 
 * @brief This file contains the BatchNormalization class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<Tensor/Tensor.h>

#include<NetworkConfiguration/Layer.h>

/**
 * @brief This class represent BatchNormalization layer configuration.
 *
 * @tparam TYPE base type of parameters
 */
template<typename TYPE>
class cnnConf::BatchNormalization: public Layer{
  public:
    BatchNormalization(
        std::string             const&name      ,
        std::vector<std::string>const&inputs    ,
        std::set   <std::string>const&outputs   ,
        Tensor<TYPE>            const&parameters,
        TYPE                          epsilon   );
    Tensor<TYPE>const&getParameters()const;
    TYPE getEpsilon()const;
    struct PARAMETERS{
      constexpr static size_t DIMENSION = 2;
      struct AXES{
        constexpr static size_t CHANNEL   = 0;
        constexpr static size_t PARAMETER = 1;
      };
      constexpr static size_t GAMMA    = 0;
      constexpr static size_t BETA     = 1;
      constexpr static size_t MEAN     = 2;
      constexpr static size_t VARIANCE = 3;
    };
  protected:
    Tensor<TYPE>parameters;
    TYPE epsilon;
};

/**
 * @brief This is the constructor of BatchNormalization.
 *
 * @param name the layer name
 * @param inputs the vector of names of layers that are inputs to this layer
 * @param outputs the set of names of layers that have this layer as input
 * @param parameters weights of batch normalization (gamma, beta, mean, variance)
 * @param epsilon epsilon factor of batch normalization
 */
template<typename TYPE>
inline cnnConf::BatchNormalization<TYPE>::BatchNormalization(
    std::string             const&name      ,
    std::vector<std::string>const&inputs    ,
    std::set   <std::string>const&outputs   ,
    Tensor<TYPE>            const&parameters,
    TYPE                          epsilon   ):
  Layer     (Layer::BATCH_NORMALIZATION,name,inputs,outputs),
  parameters(parameters                                    ),
  epsilon   (epsilon                                       )
{
}

/**
 * @brief This function returns weights.
 */
template<typename TYPE>
inline Tensor<TYPE>const&cnnConf::BatchNormalization<TYPE>::getParameters()const{
  return parameters;
}

/**
 * @brief This function returns epsilon factor of batch normalization.
 *
 * @return the epsilon factor
 */
template<typename TYPE>
inline TYPE cnnConf::BatchNormalization<TYPE>::getEpsilon()const{
  return epsilon;
}
