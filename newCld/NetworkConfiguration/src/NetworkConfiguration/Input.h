/*!
 * @file 
 * @brief This file contains the implementation of input class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<NetworkConfiguration/Layer.h>

/**
 * @brief This class represents input layer configuration.
 */
class cnnConf::Input: public cnnConf::Layer{
  public:
    Input(
        std::string             const&name   ,
        std::set   <std::string>const&outputs,
        std::vector<size_t>     const&size   );
    std::vector<size_t>getSize()const;
  protected:
    std::vector<size_t>size;
};

/**
 * @brief Constructor of input layer
 *
 * @param name name of the layer
 * @param outputs set of layer names that have this input layer as input
 * @param s size of input tensor
 */
inline cnnConf::Input::Input(
    std::string             const&name   ,
    std::set   <std::string>const&outputs,
    std::vector<size_t     >const&s      ):
  Layer(Layer::INPUT,name,{},outputs),
  size (s                           )
{
}

/**
 * @brief This function returns size of input layer tensor
 *
 * @return size of input layer tensor
 */
inline std::vector<size_t>cnnConf::Input::getSize()const{
  return size;
}

