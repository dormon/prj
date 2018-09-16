/*!
 * @file 
 * @brief This file contains the implementation of Activation class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<NetworkConfiguration/Layer.h>
#include<NetworkConfiguration/ActivationType.h>

/**
 * @brief This class represent activation layer configuration.
 */
class cnnConf::Activation: public Layer{
  public:
    Activation(
        std::string             const&name      ,
        std::vector<std::string>const&inputs    ,
        std::set   <std::string>const&outputs   ,
        ActivationType          const&activation);
    ActivationType getActivation()const;
  protected:
    ActivationType activation;
};

/**
 * @brief This is the constructor of Activation layer.
 *
 * @param name the layer name
 * @param inputs the vector of names of layers that are inputs to this layer.
 * @param outputs the set of names of layers that has this layer as input.
 * @param activation the activation type
 */
inline cnnConf::Activation::Activation(
    std::string             const&name      ,
    std::vector<std::string>const&inputs    ,
    std::set   <std::string>const&outputs   ,
    ActivationType          const&activation):
  Layer     (ACTIVATION,name,inputs,outputs),
  activation(activation                    )
{
}

/**
 * @brief This function returns activation type.
 *
 * @return the activation type
 */
inline cnnConf::ActivationType cnnConf::Activation::getActivation()const{
  return activation;
}
