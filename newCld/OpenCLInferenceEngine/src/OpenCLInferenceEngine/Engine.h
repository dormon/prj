#pragma once

#include<InferenceEngine/Engine.h>

class OpenCLInferenceEngineImpl;
class OpenCLInferenceEngine: public InferenceEngine{
  public:
    struct Config{
      Config(size_t devId = 0):deviceId(devId){}
      size_t deviceId;
      std::array<size_t,2>conv2dWorkGroupSize               = {{8,4}};
      std::array<size_t,2>biasWorkGroupSize                 = {{8,4}};
      std::array<size_t,2>batchNormalizationWorkGroupSize   = {{8,4}};
      std::array<size_t,2>activationWorkGroupSize           = {{8,4}};
      std::array<size_t,2>maxPoolingWorkGroupSize           = {{8,4}};
      size_t              globalMaxPoolingWorkGroupSize     = 32     ;
      size_t              denseWorkGroupSize                = 32     ;
      size_t              softmaxNormalizationWorkGroupSize = 32     ;
    };

    OpenCLInferenceEngine(std::shared_ptr<cnnConf::Network>const&network,Config const&cfg = Config{});
    virtual ~OpenCLInferenceEngine();
    virtual void operator()(
        std::map<std::string,Tensor<float>>     &output,
        Tensor<float>                      const&input )override;
    Tensor<float>getLayerOutput(std::string const&n)const;
  protected:
    friend class OpenCLInferenceEngineImpl;
    std::unique_ptr<OpenCLInferenceEngineImpl>pimpl;
};
