/*!
 * @file 
 * @brief This file contains the GlobalMaxPooling class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<NetworkConfiguration/Layer.h>

/**
 * @brief This class represent configuration of GlobalMaxPooling.
 */
class cnnConf::GlobalMaxPooling: public Layer{
  public:
    GlobalMaxPooling(
        std::string             const&name   ,
        std::vector<std::string>const&inputs ,
        std::set   <std::string>const&outputs);
};

/**
 * @brief The constructor of GlobalMaxPooling class.
 *
 * @param name the name of this layer.
 * @param inputs the vector of names of layers that are inputs of this layer
 * @param outputs the set of names of layers that have this layer as input
 */
inline cnnConf::GlobalMaxPooling::GlobalMaxPooling(
    std::string             const&name   ,
    std::vector<std::string>const&inputs ,
    std::set   <std::string>const&outputs):
      Layer(Layer::GLOBAL_MAX_POOLING_2D,name,inputs,outputs)
{
}
