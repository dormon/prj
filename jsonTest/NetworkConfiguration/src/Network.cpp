#include<cassert>

#include<NetworkConfiguration/Network.h>

void cnnConf::Network::init(size_t nofLayers){
  layers.resize(nofLayers);
}

void cnnConf::Network::setLayer(size_t i,std::shared_ptr<Layer>const&l){
  assert(i < layers.size());
  layers[i] = l;
}

std::shared_ptr<cnnConf::Layer>cnnConf::Network::getLayer(size_t i)const{
  assert(i < layers.size());
  return layers[i];
}

size_t cnnConf::Network::getNofLayers()const{
  return layers.size();
}
