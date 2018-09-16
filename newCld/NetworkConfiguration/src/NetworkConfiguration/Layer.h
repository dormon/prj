/*!
 * @file 
 * @brief This file contains the implementation of layer class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<vector>
#include<set>
#include<iostream>

#include<NetworkConfiguration/Fwd.h>

/**
 * @brief This class represents layer configuration.
 */
class cnnConf::Layer{
  public:
    /**
     * @brief Type of layer
     */
    enum Type{
      INPUT                ,
      CONV_2D              ,
      BIAS                 ,
      POOLING              ,
      GLOBAL_MAX_POOLING_2D,
      BATCH_NORMALIZATION  ,
      DENSE                ,
      ACTIVATION           ,
      CONCATENATE          ,
    }type;
    std::string             name   ;
    std::vector<std::string>inputs ;
    std::set<std::string>outputs;
    Layer(
        Type                    const&t      ,
        std::string             const&name   ,
        std::vector<std::string>const&inputs ,
        std::set   <std::string>const&outputs);
    virtual ~Layer();
};

/**
 * Constructor of the class instance
 * @param t type of layer
 * @param name name of this layer
 * @param inputs vector of layer names that are inputs for this layer
 * @param outputs set of layer names that have this layer as input
 */
inline cnnConf::Layer::Layer(
    Type                    const&t      ,
    std::string             const&name   ,
    std::vector<std::string>const&inputs ,
    std::set   <std::string>const&outputs):
  type   (t      ),
  name   (name   ),
  inputs (inputs ),
  outputs(outputs)
{
}

/**
 * @brief Destructor of the class instance
 */
inline cnnConf::Layer::~Layer(){}
 
