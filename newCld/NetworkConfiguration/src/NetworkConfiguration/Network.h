/*!
 * @file 
 * @brief This file contains the implementation of network class.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<memory>
#include<vector>
#include<map>
#include<cassert>

#include<NetworkConfiguration/Layer.h>
#include<NetworkConfiguration/Input.h>

/**
 * @brief This class represents network configuration.
 */
class cnnConf::Network{
  public:
    void init(size_t nofLayers);
    void setLayer(size_t i,std::shared_ptr<Layer>const&l);
    std::shared_ptr<Layer>  getLayer           (size_t i           )const;
    std::shared_ptr<Layer>  getLayer           (std::string const&n)const;
    size_t                  getNofLayers       (                   )const;
    std::vector<size_t>     getInputSize       (                   )const;
    std::vector<std::string>getOutputLayerNames(                   )const;
  protected:
    std::vector<std::shared_ptr<Layer>>layers;
    std::map<std::string,size_t>name2LayerId;
};

/**
 * This function initializes network.
 * It allocates space for layers.
 *
 * @param nofLayers the number of layers
 *
 */
inline void cnnConf::Network::init(size_t nofLayers){
  layers.resize(nofLayers);
}

/**
 * @brief This function sets selected layer.
 *
 * @param i id of layer
 * @param l a layer
 */
inline void cnnConf::Network::setLayer(size_t i,std::shared_ptr<Layer>const&l){
  assert(i < layers.size());
  layers[i] = l;
  name2LayerId[l->name] = i;
}

/**
 * @brief This function returns selected layer.
 *
 * @param i id of layer
 *
 * @return a selected layer
 */
inline std::shared_ptr<cnnConf::Layer>cnnConf::Network::getLayer(size_t i)const{
  assert(i < layers.size());
  return layers[i];
}

/**
 * @brief This function returns selected layer.
 *
 * @param n the name of selected layer
 *
 * @return the selected layer
 */
inline std::shared_ptr<cnnConf::Layer>cnnConf::Network::getLayer(std::string const&n)const{
  assert(name2LayerId.count(n) > 0);
  auto const layerId = name2LayerId.at(n);
  assert(layerId < layers.size());
  return layers.at(layerId);
}

/**
 * @brief This function returns the number of layers.
 *
 * @return the selected layer
 */
inline size_t cnnConf::Network::getNofLayers()const{
  return layers.size();
}

/**
 * @brief This function returns the size of the input layer.
 *
 * @return the size of input layer
 */
inline std::vector<size_t>cnnConf::Network::getInputSize()const{
  for(auto const&x:layers)
    if(x->type == cnnConf::Layer::INPUT){
      auto const input = std::dynamic_pointer_cast<cnnConf::Input>(x);
      assert(input != nullptr);
      return input->getSize();
    }
  throw std::string("There is no input layer!");
  return {};
}

/**
 * @brief This function returns the vector of output layer names.
 *
 * @return the vector of output layer names
 */
inline std::vector<std::string>cnnConf::Network::getOutputLayerNames()const{
  std::vector<std::string>result;
  for(auto const&x:layers)
    if(x->outputs.empty())
      result.push_back(x->name);
  return result;
}
