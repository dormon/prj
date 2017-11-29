#pragma once

#include<memory>
#include<vector>

#include<NetworkConfiguration/Layer.h>

class NETWORKCONFIGURATION_EXPORT cnnConf::Network{
  public:
    void init(size_t nofLayers);
    void setLayer(size_t i,std::shared_ptr<Layer>const&l);
    std::shared_ptr<Layer>getLayer(size_t i)const;
    size_t getNofLayers()const;
  protected:
    std::vector<std::shared_ptr<Layer>>layers;
};
