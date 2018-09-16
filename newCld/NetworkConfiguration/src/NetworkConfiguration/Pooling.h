/*!
 * @file 
 * @brief This file contains the implementation of pooling configuration class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<Tensor/Tensor.h>
#include<NetworkConfiguration/Layer.h>
#include<NetworkConfiguration/PoolingType.h>
#include<NetworkConfiguration/PaddingType.h>

/**
 * @brief This class represents pooling layer configuration.
 * This layer converts block of values to single value.
 *
 * @tparam TYPE base type of parameters
 */
class cnnConf::Pooling: public Layer{
  public:
    Pooling(
        std::string             const&name   ,
        std::vector<std::string>const&inputs ,
        std::set   <std::string>const&outputs,
        PoolingType                   pooling,
        std::vector<size_t>     const&size   ,
        std::vector<size_t>     const&stride ,
        PaddingType                   padding);
    std::vector<size_t>const&getSize   ()const;
    std::vector<size_t>const&getStride ()const;
    PaddingType              getPadding()const;
    PoolingType              getPooling()const;
  protected:
    PoolingType        poolingType;
    std::vector<size_t>poolSize   ;
    std::vector<size_t>poolStride ;    
    PaddingType        paddingType;
};


/**
 * @brief Constructor
 *
 * @param name the name of layer
 * @param inputs the vector of input layers names
 * @param outputs the set of output layers names
 * @param pooling pooling type
 * @param size pool size
 * @param stride pool stride
 * @param padding padding type
 */
inline cnnConf::Pooling::Pooling(
    std::string             const&name   ,
    std::vector<std::string>const&inputs ,
    std::set   <std::string>const&outputs,
    PoolingType                   pooling,
    std::vector<size_t>     const&size  ,
    std::vector<size_t>     const&stride,
    PaddingType                   padding):
  Layer      (Layer::POOLING,name,inputs,outputs),
  poolingType(pooling                           ),
  poolSize   (size                              ),
  poolStride (stride                            ),
  paddingType(padding                           )
{
  if(poolStride.size() != poolSize.size())
    throw std::invalid_argument("poolSize has to have same dimension as poolStride");
}

/**
 * @brief This function returns pool size.
 *
 * @return pool size
 */
inline std::vector<size_t>const&cnnConf::Pooling::getSize   ()const{
  return poolSize;
}

/**
 * @brief This function return pool stride.
 *
 * @return pool stride
 */
inline std::vector<size_t>const&cnnConf::Pooling::getStride ()const{
  return poolStride;
}

/**
 * @brief This function return padding type.
 *
 * @return padding type
 */
inline cnnConf::PaddingType cnnConf::Pooling::getPadding()const{
  return paddingType;
}

/**
 * @brief This function return pooling type.
 *
 * @return pooling type.
 */
inline cnnConf::PoolingType cnnConf::Pooling::getPooling()const{
  return poolingType;
}
