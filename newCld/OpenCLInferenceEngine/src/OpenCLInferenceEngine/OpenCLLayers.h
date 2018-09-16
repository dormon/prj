#pragma once

#include<vector>
#include<set>
#include<memory>

#include<Tensor/Tensor.h>
#include<NetworkConfiguration/NetworkConfiguration.h>

namespace ocl{
  class DAGNode{
    public:
      virtual ~DAGNode(){}
      std::vector<std::shared_ptr<DAGNode>>inputs;
      std::set<DAGNode*>outputs;
  };

  class Layer: public DAGNode{
    public:
      enum Type{
        INPUT                ,
        CONV_2D              ,
        BIAS                 ,
        BATCH_NORMALIZATION  ,
        ACTIVATION           ,
        MAX_POOLING_2D       ,
        GLOBAL_MAX_POOLING_2D,
        DENSE                ,
        SOFTMAX_NORMALIZATION,
      };
      Layer(Type t,std::string const&n):type(t),name(n){}
      virtual ~Layer(){}
      Type               type      ;
      std::vector<size_t>outputSize;
      std::string        name      ;
      std::string        sourceName;
  };
  class Input : public Layer{
    public:
      Input(std::string const&n):Layer(Layer::INPUT,n){}
  };
  class Conv2D: public Layer{
    public:
      Conv2D(std::string const&n):Layer(Layer::CONV_2D,n){}
      Tensor<float>           weights   ;
  };
  class Bias: public Layer{
    public:
      Bias(std::string const&n):Layer(Layer::BIAS,n){}
      Tensor<float>biases;
  };
  class BatchNormalization: public Layer{
    public:
      BatchNormalization(std::string const&n):Layer(Layer::BATCH_NORMALIZATION,n){}
      Tensor<float>parameters;
      float        epsilon   ;
  };
  class Activation: public Layer{
    public:
      Activation(std::string const&n):Layer(Layer::ACTIVATION,n){}
      cnnConf::ActivationType activation;
  };
  class MaxPooling: public Layer{
    public:
      MaxPooling(std::string const&n):Layer(Layer::MAX_POOLING_2D,n){}
      size_t strideX;
      size_t strideY;
  };
  class GlobalMaxPooling: public Layer{
    public:
      GlobalMaxPooling(std::string const&n):Layer(Layer::GLOBAL_MAX_POOLING_2D,n){}
  };
  class Dense: public Layer{
    public:
      Dense(std::string const&n):Layer(Layer::DENSE,n){}
      Tensor<float>           weights;
      Tensor<float>           biases ;
      cnnConf::ActivationType activation;
  };
  class SoftmaxNormalization: public Layer{
    public:
      SoftmaxNormalization(std::string const&n):Layer(Layer::SOFTMAX_NORMALIZATION,n){}
  };

  class Network{
    public:
      std::set<std::shared_ptr<Layer>>inputs ;
      std::set<std::shared_ptr<Layer>>outputs;
      std::map<std::string,std::shared_ptr<Layer>>allLayers;
  };


  std::shared_ptr<ocl::Layer>translateInputLayer(
      std::shared_ptr<cnnConf::Layer>const&layer);
  std::shared_ptr<ocl::Layer>translateConv2DLayer(
      std::shared_ptr<cnnConf::Layer>const&layer);
  std::shared_ptr<ocl::Layer>translateBiasLayer(
      std::shared_ptr<cnnConf::Layer>const&layer);
  std::shared_ptr<ocl::Layer>translateMaxPooling(
      std::shared_ptr<cnnConf::Layer>const&layer);
  std::shared_ptr<ocl::Layer>translateGlobalMaxPooling(
      std::shared_ptr<cnnConf::Layer>const&layer);
  std::shared_ptr<ocl::Layer>translateBatchNormalization(
      std::shared_ptr<cnnConf::Layer>const&layer);
  void insertLayersAsInputs(
      std::shared_ptr<ocl::Layer>             const&layer ,
      std::vector<std::shared_ptr<ocl::Layer>>const&inputs);
  std::shared_ptr<ocl::Layer>createDense(
      std::shared_ptr<cnnConf::Layer>         const&layer ,
      std::vector<std::shared_ptr<ocl::Layer>>const&inputs);
  std::shared_ptr<ocl::Layer>translateDenseSoftMax(
      std::shared_ptr<cnnConf::Layer>         const&layer ,
      std::vector<std::shared_ptr<ocl::Layer>>const&inputs);
  std::shared_ptr<ocl::Layer>translateDense(
      std::shared_ptr<cnnConf::Layer>         const&layer ,
      std::vector<std::shared_ptr<ocl::Layer>>const&inputs);
  std::shared_ptr<ocl::Layer>translateActivation(
      std::shared_ptr<cnnConf::Layer>const&layer);
  std::shared_ptr<ocl::Layer>translateSimpleLayer(
      std::shared_ptr<cnnConf::Layer>         const&layer ,
      std::vector<std::shared_ptr<ocl::Layer>>const&inputs);
  std::shared_ptr<ocl::Layer>translateLayer(
      std::shared_ptr<cnnConf::Layer>         const&layer ,
      std::vector<std::shared_ptr<ocl::Layer>>const&inputs);
  std::shared_ptr<ocl::Layer>translateLayer(
      std::map<std::string,std::shared_ptr<ocl::Layer>>     &processedLayers,
      std::string                                      const&name           ,
      std::shared_ptr<cnnConf::Network>                const&net            );
  void translateLayers(
      std::shared_ptr<ocl    ::Network>const&outputNet,
      std::shared_ptr<cnnConf::Network>const&inputNet );
  void insertLayerAsOutputToItsInputs(
      std::shared_ptr<ocl::Layer>const&layer);
  void fillLayerOutputs(
      std::shared_ptr<ocl::Layer>const&layer);
  void fillLayersOutputs(
      std::shared_ptr<ocl::Network>const&net);
  void fillNetworkInputs(
      std::shared_ptr<ocl::Network>const&net  ,
      std::shared_ptr<ocl::Layer  >const&layer);
  void fillNetworkInputs(
      std::shared_ptr<ocl::Network>const&net);
  std::shared_ptr<ocl::Network>translateNetwork(
      std::shared_ptr<cnnConf::Network>const&net);
}
