/** 
 * @file
 * @brief This file contains implementation of Concatenate configuration class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<NetworkConfiguration/Layer.h>

/**
 * @brief This class represents concatenate layer configuration.
 */
class cnnConf::Concatenate: public Layer{
  public:
    Concatenate(
        std::string             const&name   ,
        std::vector<std::string>const&inputs ,
        std::set   <std::string>const&outputs,
        size_t                  const&axis   );
    size_t getAxis()const;
  protected:
    size_t axis;
};


/**
 * @brief This is constructor of the class.
 *
 * @param name the name of layer
 * @param inputs the vector of input layer names
 * @param outputs the set of output layer names
 * @param axis concatenation axis
 */
inline cnnConf::Concatenate::Concatenate(
    std::string             const&name   ,
    std::vector<std::string>const&inputs ,
    std::set   <std::string>const&outputs,
    size_t                  const&axis   ):
  Layer(cnnConf::Layer::CONCATENATE,name,inputs,outputs),
  axis (axis                                           )
{
}

/**
 * @brief This function returns concatenation axis.
 *
 * @return concatenation axis
 */
inline size_t cnnConf::Concatenate::getAxis()const{
  return axis;
}
