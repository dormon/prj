#pragma once

#include<InferenceEngine/Engine.h>
#include<CPUInferenceEngine/cpuinferenceengine_export.h>

class CPUInferenceEngineImpl;
class CPUINFERENCEENGINE_EXPORT CPUInferenceEngine: public InferenceEngine{
  public:
    CPUInferenceEngine(std::shared_ptr<cnnConf::Network>const&network);
    virtual ~CPUInferenceEngine();
    virtual void operator()(
        std::map<std::string,Tensor<float>>     &output,
        Tensor<float>                      const&input )override;
    Tensor<float>const&getLayerOutput(std::string const&name)const;
  protected:
    friend class CPUInferenceEngineImpl;
    std::unique_ptr<CPUInferenceEngineImpl>pimpl;
};
