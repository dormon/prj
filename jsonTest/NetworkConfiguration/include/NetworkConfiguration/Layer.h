#pragma once

#include<iostream>

#include<NetworkConfiguration/Fwd.h>
#include<NetworkConfiguration/networkconfiguration_export.h>

class NETWORKCONFIGURATION_EXPORT cnnConf::Layer{
  public:
    enum Type{
      INPUT                 = 0,
      CONV_2D               = 1,
      MAX_POOLING_2D        = 2,
      GLOBAL_MAX_POOLING_2D = 3,
      DROPOUT               = 4,
      DENSE                 = 5,
    }type;
    Layer(Type const&t);
    virtual ~Layer();
    static Type translateLayerNameToLayerType(std::string const&name);
};

