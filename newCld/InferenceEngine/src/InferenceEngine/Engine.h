#pragma once

#include<map>

#include<NetworkConfiguration/NetworkConfiguration.h>
#include<InferenceEngine/inferenceengine_export.h>

class INFERENCEENGINE_EXPORT InferenceEngine{
  public:
    InferenceEngine(std::shared_ptr<cnnConf::Network>const&network);
    virtual ~InferenceEngine();
    virtual void operator()(
        std::map<std::string,Tensor<float>>     &output,
        Tensor<float>                      const&input ) = 0;
    std::vector<size_t>getInputSize()const;
    std::vector<std::string>getOutputLayerNames()const;
  protected:
    std::shared_ptr<cnnConf::Network>network;
};

