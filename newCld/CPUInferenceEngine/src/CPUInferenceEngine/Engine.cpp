#include<cassert>
#include<limits>
#include<cmath>
#include<iostream>
#include<map>
#include<functional>

#include<CPUInferenceEngine/Engine.h>
#include<CPULayerImplementation/Layers.h>
//#include<ComputeGraph/ComputeGraph.h>

#define ___ std::cerr<<__FILE__<<" "<<__LINE__<<std::endl

#define IMPLEMENT_FAST















using namespace CPULayerImplementation;

class CPUInferenceEngineImpl{
  public:
    CPUInferenceEngineImpl(std::shared_ptr<cnnConf::Network>const&network);
    std::vector<Tensor<float>>layerData;
    std::shared_ptr<cnnConf::Network>network;

    std::map<std::string,Tensor<float>>layerOutputs;
    std::vector<std::function<void()>>commandList;
    std::vector<std::string>outputLayers;
    std::string inputLayerName;

    Tensor<float>const&getLayerOutput(std::string const&name);

    void createLayerOutput(std::string const&name);
    void createCommandList();
};

Tensor<float>const&CPUInferenceEngine::getLayerOutput(std::string const&name)const{
  assert(pimpl != nullptr);
  assert(pimpl->layerOutputs.count(name) > 0);
  return pimpl->layerOutputs.at(name);
}

size_t substractClampToZero(size_t a,size_t b){
  if(a < b)return 0;
  return a-b;
}

size_t computeOutputSize(
    size_t inputSize,
    size_t stride,
    size_t size,
    cnnConf::PaddingType type){
  auto const reducedSize = inputSize - size;

  if(type == cnnConf::PaddingType::VALID){
    if(inputSize < size)return 0;
    return reducedSize / stride + 1;
  }
  if(type == cnnConf::PaddingType::SAME){
    if(inputSize < size)return 1;
    return reducedSize / stride + 1 + static_cast<size_t>((reducedSize % stride) != 0);
  }
  return 0;
}

void createInputLayerOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const inputLayer = std::dynamic_pointer_cast<cnnConf::Input>(layer);
  assert(inputLayer != nullptr);
  auto const inputLayerName = layer->name;
  layerOutputs[layer->name] = Tensor<float>(inputLayer->getSize());
}

std::vector<size_t> getLayerOutputSize(
    std::map<std::string,Tensor<float>>const&layerOutputs,
    std::string                        const&name        ){
  assert(layerOutputs.count(name) > 0);
  return layerOutputs.at(name).size;
}



void createConv2dLayerOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const conv2DLayer = std::dynamic_pointer_cast<cnnConf::Conv2D<float>>(layer);
  assert(conv2DLayer != nullptr);
  assert(layer->inputs.size() == 1);
  layerOutputs[layer->name] = Tensor<float>({
      getLayerOutputSize(layerOutputs,layer->inputs.at(0)).at(0),
      getLayerOutputSize(layerOutputs,layer->inputs.at(0)).at(1),
      conv2DLayer->getWeights().size[cnnConf::Conv2D<float>::WEIGHTS::AXES::FILTER],
      });
}

void createBiasLayerOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const biasLayer = std::dynamic_pointer_cast<cnnConf::Bias<float>>(layer);
  assert(biasLayer != nullptr);
  assert(layer->inputs.size() == 1);
  layerOutputs[layer->name] = Tensor<float>(getLayerOutputSize(layerOutputs,layer->inputs.at(0)));
}

void createBatchNormalizationLayerOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const batchNormalizationLayer = std::dynamic_pointer_cast<cnnConf::BatchNormalization<float>>(layer);
  assert(batchNormalizationLayer != nullptr);
  assert(batchNormalizationLayer->inputs.size() == 1);
  layerOutputs[layer->name] = Tensor<float>(getLayerOutputSize(layerOutputs,layer->inputs.at(0)));
}

void createActivationLayerOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const activationLayer = std::dynamic_pointer_cast<cnnConf::Activation>(layer);
  assert(activationLayer != nullptr);
  assert(activationLayer->inputs.size() == 1);
  layerOutputs[layer->name] = Tensor<float>(getLayerOutputSize(layerOutputs,layer->inputs.at(0)));
}

void createPoolingLayerOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const poolingLayer = std::dynamic_pointer_cast<cnnConf::Pooling>(layer);
  assert(poolingLayer != nullptr);
  assert(poolingLayer->inputs.size() == 1);
  auto const inputSize = getLayerOutputSize(layerOutputs,layer->inputs.at(0));
  auto const dimension = inputSize.size();
  auto const padding   = poolingLayer->getPadding();
  auto       stride    = poolingLayer->getStride();
  auto       poolSize  = poolingLayer->getSize();
  while(stride  .size() < dimension)stride  .emplace_back(1);
  while(poolSize.size() < dimension)poolSize.emplace_back(1);
  std::vector<size_t>outputSize;
  for(size_t i=0;i<dimension;++i)
    outputSize.push_back(computeOutputSize(inputSize.at(i),poolSize.at(i),stride.at(i),padding));
  layerOutputs[layer->name] = Tensor<float>(outputSize);
}

void createGlobalMaxPoolingLayerOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const globalMaxPoolingLayer = std::dynamic_pointer_cast<cnnConf::GlobalMaxPooling>(layer);
  assert(globalMaxPoolingLayer != nullptr);
  assert(globalMaxPoolingLayer->inputs.size() == 1);
  layerOutputs[layer->name] = Tensor<float>({
      getLayerOutputSize(layerOutputs,layer->inputs.at(0)).at(2),
      });
}

void createDenseLayerOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const denseLayer = std::dynamic_pointer_cast<cnnConf::Dense<float>>(layer);
  assert(denseLayer != nullptr);
  assert(denseLayer->inputs.size() == 1);
  layerOutputs[layer->name] = Tensor<float>({
      denseLayer->getWeights().size[cnnConf::Dense<float>::WEIGHTS::AXES::OUTPUT]
      });
}

void createConcatenateLayerOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const concatenateLayer = std::dynamic_pointer_cast<cnnConf::Concatenate>(layer);
  assert(concatenateLayer != nullptr);
  size_t dimension = 0;
  std::vector<size_t>commonSize;
  size_t accumulatedSize = 0;
  for(auto const&x:concatenateLayer->inputs){
    auto const inputSize = getLayerOutputSize(layerOutputs,x);
    if(dimension == 0){
      commonSize = inputSize;
      dimension = commonSize.size();
    }else if(dimension == getLayerOutputSize(layerOutputs,x).size()){
      for(size_t i=0;i<dimension;++i){
        if(i == concatenateLayer->getAxis())continue;
        if(commonSize.at(i) != inputSize.at(i))
          throw std::string("layer: \""+x+"\" has incorrect size for concatenation");
      }
    }else{
      throw std::string("one of the inputs: \""+x+"\" to concatenate layer has difference dimension");
    }
    accumulatedSize += inputSize.at(concatenateLayer->getAxis());
  }
  commonSize.at(concatenateLayer->getAxis()) = accumulatedSize;
  layerOutputs[layer->name] = Tensor<float>(commonSize);
}

void CPUInferenceEngineImpl::createLayerOutput(std::string const&name){

  if(layerOutputs.count(name) > 0)return;

  auto const layer = network->getLayer(name);
  assert(layer != nullptr);

  //create outputs for layer inputs
  for(auto const&i:layer->inputs)
    createLayerOutput(i);

  switch(layer->type){
    case cnnConf::Layer::INPUT                :createInputLayerOutput             (layerOutputs,layer);break;
    case cnnConf::Layer::CONV_2D              :createConv2dLayerOutput            (layerOutputs,layer);break;
    case cnnConf::Layer::BIAS                 :createBiasLayerOutput              (layerOutputs,layer);break;
    case cnnConf::Layer::BATCH_NORMALIZATION  :createBatchNormalizationLayerOutput(layerOutputs,layer);break;
    case cnnConf::Layer::ACTIVATION           :createActivationLayerOutput        (layerOutputs,layer);break;
    case cnnConf::Layer::POOLING              :createPoolingLayerOutput           (layerOutputs,layer);break;
    case cnnConf::Layer::GLOBAL_MAX_POOLING_2D:createGlobalMaxPoolingLayerOutput  (layerOutputs,layer);break;
    case cnnConf::Layer::DENSE                :createDenseLayerOutput             (layerOutputs,layer);break;
    case cnnConf::Layer::CONCATENATE          :createConcatenateLayerOutput       (layerOutputs,layer);break;
  }
}

std::reference_wrapper<Tensor<float>>getInput(
    std::map<std::string,Tensor<float>>     &layerOutputs    ,
    std::shared_ptr<cnnConf::Layer>    const&layer           ,
    size_t                                   i            = 0){
  return std::ref(layerOutputs.at(layer->inputs.at(i)));
}

std::reference_wrapper<Tensor<float>>getOutput(
    std::map<std::string,Tensor<float>>     &layerOutputs    ,
    std::shared_ptr<cnnConf::Layer>    const&layer           ){
  return std::ref(layerOutputs.at(layer->name));
}


std::function<void()>createInputCommand(
    std::map<std::string,Tensor<float>>     &,
    std::shared_ptr<cnnConf::Layer>    const&){
  return nullptr;
}

std::function<void()>createConv2dCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  assert(layer->inputs.size() == 1);
  std::function<void()> command = std::bind(conv2d,getOutput(layerOutputs,layer),getInput(layerOutputs,layer),layer);
  assert(command != nullptr);
  return command;
}

std::function<void()>createBiasCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  assert(layer->inputs.size() == 1);
  std::function<void()>command = std::bind(bias,getOutput(layerOutputs,layer),getInput(layerOutputs,layer),layer);
  assert(command != nullptr);
  return command;
}

std::function<void()>createBatchNormalizationCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  assert(layer->inputs.size() == 1);
  std::function<void()>command = std::bind(batchNormalization,getOutput(layerOutputs,layer),getInput(layerOutputs,layer),layer);
  assert(command != nullptr);
  return command;
}

std::function<void()>createActivationCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const activationLayer = std::dynamic_pointer_cast<cnnConf::Activation>(layer);
  assert(layer->inputs.size() == 1);
  assert(activationLayer != nullptr);
  auto const activation = activationLayer->getActivation();
  std::function<void()>command = std::bind(activation2d,getOutput(layerOutputs,layer),getInput(layerOutputs,layer),activation);
  assert(command != nullptr);
  return command;
}

std::function<void()>createMaxPoolingCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  assert(layer->inputs.size() == 1);
  std::function<void()>command = std::bind(maxPooling,getOutput(layerOutputs,layer),getInput(layerOutputs,layer),layer);
  assert(command != nullptr);
  return command;
}

std::function<void()>createGlobalMaxPoolingCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  assert(layer->inputs.size() == 1);
  std::function<void()>command = std::bind(globalMaxPooling,getOutput(layerOutputs,layer),getInput(layerOutputs,layer),layer);
  assert(command != nullptr);
  return command;
}

std::function<void()>createDenseCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  assert(layer->inputs.size() == 1);
  std::function<void()>command = std::bind(dense,getOutput(layerOutputs,layer),getInput(layerOutputs,layer),layer);
  assert(command != nullptr);
  return command;
}

std::function<void()>createConcatenate3dCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::Concatenate>(layer);
  std::vector<std::reference_wrapper<Tensor<float>>>inputs;
  for(auto const&x:layer->inputs)
    inputs.push_back(std::ref(layerOutputs.at(x)));
  std::function<void()>command = std::bind(concatenate3d,getOutput(layerOutputs,layer),inputs,layer);
  assert(command != nullptr);
  return command;
}

std::function<void()>createConcatenateCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  auto const dimension = getDimension(layerOutputs.at(layer->inputs.at(0)));
  switch(dimension){
    case 3: return createConcatenate3dCommand(layerOutputs,layer);
    default:{
              std::stringstream ss;
              ss << "Concatenation of layers with with dimmension: ";
              ss << dimension << " is unsupported";
              throw std::runtime_error(ss.str());
            }
            return nullptr;
  }
}

std::function<void()>createCommand(
    std::map<std::string,Tensor<float>>     &layerOutputs,
    std::shared_ptr<cnnConf::Layer>    const&layer       ){
  switch(layer->type){
    case cnnConf::Layer::INPUT                :return createInputCommand             (layerOutputs,layer);
    case cnnConf::Layer::CONV_2D              :return createConv2dCommand            (layerOutputs,layer);
    case cnnConf::Layer::BIAS                 :return createBiasCommand              (layerOutputs,layer);
    case cnnConf::Layer::BATCH_NORMALIZATION  :return createBatchNormalizationCommand(layerOutputs,layer);
    case cnnConf::Layer::ACTIVATION           :return createActivationCommand        (layerOutputs,layer);
    case cnnConf::Layer::POOLING              :return createMaxPoolingCommand        (layerOutputs,layer);        
    case cnnConf::Layer::GLOBAL_MAX_POOLING_2D:return createGlobalMaxPoolingCommand  (layerOutputs,layer);
    case cnnConf::Layer::DENSE                :return createDenseCommand             (layerOutputs,layer);
    case cnnConf::Layer::CONCATENATE          :return createConcatenateCommand       (layerOutputs,layer);
  }
  return nullptr;
}

void createCommandRecurrent(
    std::set<std::string>                 &alreadyInserted,
    std::vector<std::function<void()>>    &commandList    ,
    std::shared_ptr<cnnConf::Network>const&network        ,
    std::map<std::string,Tensor<float>>   &layerOutputs   ,
    std::string                      const&name           ){
  if(alreadyInserted.count(name) > 0)return;

  auto const layer = network->getLayer(name);
  assert(layer != nullptr);

  for(auto const&i:layer->inputs)
    createCommandRecurrent(alreadyInserted,commandList,network,layerOutputs,i);

  std::function<void()>command = createCommand(layerOutputs,layer);
  if(command != nullptr)
    commandList.push_back(command);
  alreadyInserted.insert(name);
}

void CPUInferenceEngineImpl::createCommandList(){
  std::set<std::string>alreadyInserted;

  for(auto const&x:outputLayers)
    createCommandRecurrent(alreadyInserted,commandList,network,layerOutputs,x);
}

CPUInferenceEngineImpl::CPUInferenceEngineImpl(
    std::shared_ptr<cnnConf::Network>const&network):network(network){

  //compute output layers
  //output layer does not have outputs
  for(size_t i=0;i<network->getNofLayers();++i){
    auto const layer = network->getLayer(i);
    if(layer->outputs.empty())
      outputLayers.push_back(layer->name);
  }

  //create outputs of layers
  for(auto const&x:outputLayers)
    createLayerOutput(x);

  createCommandList();

  //get input layer name
  for(size_t i=0;i<network->getNofLayers();++i)
    if(network->getLayer(i)->type == cnnConf::Layer::INPUT)
      inputLayerName = network->getLayer(i)->name;

}




CPUInferenceEngine::CPUInferenceEngine(
    std::shared_ptr<cnnConf::Network>const&network):
  InferenceEngine(network)
{
  pimpl = std::make_unique<CPUInferenceEngineImpl>(network);
}

CPUInferenceEngine::~CPUInferenceEngine(){}

void CPUInferenceEngine::operator()(
    std::map<std::string,Tensor<float>>     &output,
    Tensor<float>                      const&input ){
  assert(pimpl != nullptr);
  assert(pimpl->layerOutputs.count(pimpl->inputLayerName) > 0);

  pimpl->layerOutputs.at(pimpl->inputLayerName).data = input.data;

  for(auto const&x:pimpl->commandList)
    x();

  for(auto const&x:pimpl->outputLayers){
    assert(pimpl->layerOutputs.count(x) > 0);
    output[x] = pimpl->layerOutputs.at(x);
  }

}
