#include<OpenCLInferenceEngine/OpenCLLayers.h>

template<typename TYPE>
TYPE*sharedPointerToRawPointer(std::shared_ptr<TYPE>const&p){
  return &(*p);
}

std::shared_ptr<ocl::Layer>ocl::translateInputLayer(
    std::shared_ptr<cnnConf::Layer>const&layer){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::Input>(layer);
  assert(typedLayer != nullptr);
  auto result = std::make_shared<ocl::Input>(layer->name);
  assert(result != nullptr);
  result->outputSize = typedLayer->getSize();
  return result;
}

std::shared_ptr<ocl::Layer>ocl::translateConv2DLayer(
    std::shared_ptr<cnnConf::Layer>const&layer){  
  assert(layer != nullptr);                       
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::Conv2D<float>>(layer);
  assert(typedLayer != nullptr);
  auto result = std::make_shared<ocl::Conv2D>(layer->name);
  assert(result != nullptr);
  result->weights    = typedLayer->getWeights();
  return result;
}

std::shared_ptr<ocl::Layer>ocl::translateBiasLayer(
    std::shared_ptr<cnnConf::Layer>const&layer){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::Bias<float>>(layer);
  assert(typedLayer != nullptr);
  auto result = std::make_shared<ocl::Bias>(layer->name);
  assert(result != nullptr);
  result->biases = typedLayer->getBiases();
  return result;
}

std::shared_ptr<ocl::Layer>ocl::translateMaxPooling(
    std::shared_ptr<cnnConf::Layer>const&layer){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::Pooling>(layer);
  assert(typedLayer != nullptr);
  auto result = std::make_shared<ocl::MaxPooling>(layer->name);
  assert(result != nullptr);
  auto const&stride = typedLayer->getStride();
  result->strideX = stride.at(0);
  result->strideY = stride.at(1);
  return result;
}

std::shared_ptr<ocl::Layer>ocl::translateGlobalMaxPooling(
    std::shared_ptr<cnnConf::Layer>const&layer){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::GlobalMaxPooling>(layer);
  assert(typedLayer != nullptr);
  auto result = std::make_shared<ocl::GlobalMaxPooling>(layer->name);
  assert(result != nullptr);
  return result;
}

std::shared_ptr<ocl::Layer>ocl::translateBatchNormalization(
    std::shared_ptr<cnnConf::Layer>const&layer){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::BatchNormalization<float>>(layer);
  assert(typedLayer != nullptr);
  auto result = std::make_shared<ocl::BatchNormalization>(layer->name);
  assert(result != nullptr);
  result->parameters = typedLayer->getParameters();
  result->epsilon    = typedLayer->getEpsilon();
  return result;
}

void ocl::insertLayersAsInputs(
    std::shared_ptr<ocl::Layer>             const&layer ,
    std::vector<std::shared_ptr<ocl::Layer>>const&inputs){
  for(auto const&i:inputs)
    layer->inputs.push_back(i);
}

std::shared_ptr<ocl::Layer>ocl::createDense(
    std::shared_ptr<cnnConf::Layer>         const&layer ,
    std::vector<std::shared_ptr<ocl::Layer>>const&inputs){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::Dense<float>>(layer);
  assert(typedLayer != nullptr);

  auto result = std::make_shared<ocl::Dense>(layer->name);
  assert(result != nullptr);
  result->weights    = typedLayer->getWeights();
  result->biases     = typedLayer->getBiases ();
  result->activation = typedLayer->getActivation();

  insertLayersAsInputs(result,inputs);

  return result;
}

std::shared_ptr<ocl::Layer>ocl::translateDenseSoftMax(
    std::shared_ptr<cnnConf::Layer>         const&layer ,
    std::vector<std::shared_ptr<ocl::Layer>>const&inputs){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::Dense<float>>(layer);
  assert(typedLayer != nullptr);

  auto dense = createDense(layer,inputs);

  auto softmax = std::make_shared<ocl::SoftmaxNormalization>(layer->name+"_softmaxNormalization");
  softmax->inputs.push_back(dense);

  return softmax;
}

std::shared_ptr<ocl::Layer>ocl::translateDense(
    std::shared_ptr<cnnConf::Layer>         const&layer ,
    std::vector<std::shared_ptr<ocl::Layer>>const&inputs){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::Dense<float>>(layer);
  assert(typedLayer != nullptr);
  if(typedLayer->getActivation() == cnnConf::SOFTMAX)
    return translateDenseSoftMax(layer,inputs);
 
  return createDense(layer,inputs);
}

std::shared_ptr<ocl::Layer>ocl::translateActivation(
    std::shared_ptr<cnnConf::Layer>const&layer){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<cnnConf::Activation>(layer);
  assert(typedLayer != nullptr);
  auto result = std::make_shared<ocl::Activation>(layer->name);
  assert(result != nullptr);
  result->activation = typedLayer->getActivation();
  return result;
}

std::shared_ptr<ocl::Layer>ocl::translateSimpleLayer(
    std::shared_ptr<cnnConf::Layer>         const&layer ,
    std::vector<std::shared_ptr<ocl::Layer>>const&inputs){
  std::shared_ptr<ocl::Layer>result;
  switch(layer->type){
    case cnnConf::Layer::INPUT                :
      result = translateInputLayer(layer);
      break;
    case cnnConf::Layer::CONV_2D              :
      result = translateConv2DLayer(layer);
      break;
    case cnnConf::Layer::BIAS                 :
      result = translateBiasLayer(layer);
      break;
    case cnnConf::Layer::POOLING              :
      result = translateMaxPooling(layer);
      break;
    case cnnConf::Layer::GLOBAL_MAX_POOLING_2D:
      result = translateGlobalMaxPooling(layer);
      break;
    case cnnConf::Layer::BATCH_NORMALIZATION  :
      result = translateBatchNormalization(layer);
      break;
    case cnnConf::Layer::ACTIVATION           :
      result = translateActivation(layer);
      break;
    default:
      assert(false);
      break;
  }

  assert(result != nullptr);
  insertLayersAsInputs(result,inputs);

  return result;
}

std::shared_ptr<ocl::Layer>ocl::translateLayer(
    std::shared_ptr<cnnConf::Layer>         const&layer ,
    std::vector<std::shared_ptr<ocl::Layer>>const&inputs){
  std::shared_ptr<ocl::Layer>result;
  switch(layer->type){
    case cnnConf::Layer::INPUT                :
    case cnnConf::Layer::CONV_2D              :
    case cnnConf::Layer::BIAS                 :
    case cnnConf::Layer::POOLING              :
    case cnnConf::Layer::GLOBAL_MAX_POOLING_2D:
    case cnnConf::Layer::BATCH_NORMALIZATION  :
    case cnnConf::Layer::ACTIVATION           :
      result = translateSimpleLayer(layer,inputs);
      break;
    case cnnConf::Layer::DENSE                :
      result = translateDense(layer,inputs);
      break;
    case cnnConf::Layer::CONCATENATE          :
      throw std::runtime_error("OpenCL concatenate layer is not yes implemented");
      break;
  }
  result->sourceName = layer->name;
  return result;
}

std::shared_ptr<ocl::Layer>ocl::translateLayer(
    std::map<std::string,std::shared_ptr<ocl::Layer>>     &processedLayers,
    std::string                                      const&name           ,
    std::shared_ptr<cnnConf::Network>                const&net            ){
  assert(net != nullptr);

  if(processedLayers.count(name) > 0)return processedLayers.at(name);

  auto const layer = net->getLayer(name);

  std::vector<std::shared_ptr<ocl::Layer>>inputs;

  for(auto const&i:layer->inputs){
    auto const inputLayer = translateLayer(processedLayers,i,net);
    assert(inputLayer != nullptr);
    inputs.push_back(inputLayer);
  }

  auto const translatedLayer = translateLayer(layer,inputs);
  processedLayers[name] = translatedLayer;
  return translatedLayer;
}

void ocl::translateLayers(
    std::shared_ptr<ocl    ::Network>const&outputNet,
    std::shared_ptr<cnnConf::Network>const&inputNet ){
  assert(outputNet != nullptr);
  assert(inputNet  != nullptr);
  auto const outputNames = inputNet->getOutputLayerNames();

  std::map<std::string,std::shared_ptr<ocl::Layer>>processedLayers;
  for(auto const&outputLayerName:outputNames){
    assert(processedLayers.count(outputLayerName) == 0);
    auto outputLayer = translateLayer(processedLayers,outputLayerName,inputNet);
    assert(outputLayer != nullptr);
    outputNet->outputs.insert(outputLayer);
  }
}

void ocl::insertLayerAsOutputToItsInputs(
    std::shared_ptr<ocl::Layer>const&layer){
  assert(layer != nullptr);

  for(auto const&input:layer->inputs){
    assert(input != nullptr);
    auto const&inputLayer = std::dynamic_pointer_cast<ocl::Layer>(input);
    assert(inputLayer != nullptr);
    auto const layerAsDAGNode = std::dynamic_pointer_cast<ocl::DAGNode>(layer);
    assert(layerAsDAGNode != nullptr);
    auto const layerAsRawDAGNodePointer = sharedPointerToRawPointer(layerAsDAGNode);
    assert(layerAsRawDAGNodePointer != nullptr);
    inputLayer->outputs.insert(layerAsRawDAGNodePointer);
  }
}

void ocl::fillLayerOutputs(
    std::shared_ptr<ocl::Layer>const&layer){
  assert(layer != nullptr);

  insertLayerAsOutputToItsInputs(layer);

  for(auto const&input:layer->inputs)
    fillLayerOutputs(std::dynamic_pointer_cast<ocl::Layer>(input));
}

void ocl::fillLayersOutputs(
    std::shared_ptr<ocl::Network>const&net){
  for(auto const&layer:net->outputs)
    fillLayerOutputs(layer);
}

void ocl::fillNetworkInputs(
    std::shared_ptr<ocl::Network>const&net  ,
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(net   != nullptr);
  assert(layer != nullptr);

  if(layer->type == Layer::INPUT){
    net->inputs.insert(layer);
    return;
  }

  for(auto const&input:layer->inputs){
    assert(input != nullptr);
    auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(input);
    assert(inputLayer != nullptr);
    fillNetworkInputs(net,inputLayer);
  }
}

void ocl::fillNetworkInputs(
    std::shared_ptr<ocl::Network>const&net){
  assert(net != nullptr);
  for(auto const&output:net->outputs)
    fillNetworkInputs(net,output);
}

void computeLayerOutputSize(
    std::shared_ptr<ocl::Network>const&net  ,
    std::shared_ptr<ocl::Layer  >const&layer);

std::vector<size_t>computeInputOutputSize(
    std::shared_ptr<ocl::Network>const&     ,
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::Input>(layer);
  assert(typedLayer != nullptr);
  return typedLayer->outputSize;
}

#define ___ std::cout << __LINE__ << std::endl

void computeLayerOutputSizeForLayerThatDoesNotChangeSize(
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(layer != nullptr);
  assert(layer->inputs.size() == 1);
  auto const input = layer->inputs.at(0);
  assert(input != nullptr);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(input);
  assert(inputLayer != nullptr);
  auto const&inputSize = inputLayer->outputSize;
  assert(!inputSize.empty());
  layer->outputSize = inputSize;
}

void computeConv2DOutputSize(
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(layer != nullptr);
  assert(layer->inputs.size() == 1);
  auto const input = layer->inputs.at(0);
  assert(input != nullptr);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(input);
  assert(inputLayer != nullptr);
  auto const&inputSize = inputLayer->outputSize;
  assert(inputSize.size() == 3);
  auto const conv2DLayer = std::dynamic_pointer_cast<ocl::Conv2D>(layer);
  assert(conv2DLayer != nullptr);
  assert(conv2DLayer->weights.size.size() == 4);
  layer->outputSize = {inputSize.at(0),inputSize.at(1),conv2DLayer->weights.size.at(3)};
}


void computeMaxPoolingOutputSize(
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(layer != nullptr);
  assert(layer->inputs.size() == 1);
  auto const input = layer->inputs.at(0);
  assert(input != nullptr);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(input);
  assert(inputLayer != nullptr);
  auto inputSize = inputLayer->outputSize;
  assert(!inputSize.empty());

  auto const typedLayer = std::dynamic_pointer_cast<ocl::MaxPooling>(layer);
  assert(typedLayer != nullptr);

  auto const divRoundUp = [](size_t v,size_t divisor){
    return (v/divisor) + static_cast<size_t>((v%divisor) != 0);
  };

  assert(inputSize.size() == 3);
  inputSize[0] = divRoundUp(inputSize[0],typedLayer->strideX);
  inputSize[1] = divRoundUp(inputSize[1],typedLayer->strideX);

  layer->outputSize = inputSize;
}

void computeGlobalMaxPoolingOutputSize(
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(layer != nullptr);
  assert(layer->inputs.size() == 1);
  auto const input = layer->inputs.at(0);
  assert(input != nullptr);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(input);
  assert(inputLayer != nullptr);

  auto const inputSize = inputLayer->outputSize;
  assert(!inputSize.empty());

  layer->outputSize = {inputSize.at(2)};
}

void computeDenseOutputSize(
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(layer != nullptr);
  auto const denseLayer = std::dynamic_pointer_cast<ocl::Dense>(layer);
  assert(denseLayer != nullptr);
  assert(!denseLayer->biases.size.empty());
  layer->outputSize = {denseLayer->biases.size.at(0)};
}

void computeLayerInputsOutputSizes(
    std::shared_ptr<ocl::Network>const&net  ,
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(net != nullptr);
  assert(layer != nullptr);

  for(auto const&i:layer->inputs){
    assert(i != nullptr);
    auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(i);
    assert(inputLayer != nullptr);
    computeLayerOutputSize(net,inputLayer);
  }
}

void computeLayerOutputSize(
    std::shared_ptr<ocl::Network>const&net  ,
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(net != nullptr);
  assert(layer != nullptr);

  if(!layer->outputSize.empty())
    return;

  computeLayerInputsOutputSizes(net,layer);

  switch(layer->type){
    case ocl::Layer::INPUT                :
      computeInputOutputSize(net,layer);
      break;
    case ocl::Layer::BATCH_NORMALIZATION  :
    case ocl::Layer::ACTIVATION           :
    case ocl::Layer::SOFTMAX_NORMALIZATION:
    case ocl::Layer::BIAS                 :
      computeLayerOutputSizeForLayerThatDoesNotChangeSize(layer);
      break;
    case ocl::Layer::CONV_2D              :
      computeConv2DOutputSize(layer);
      break;
    case ocl::Layer::MAX_POOLING_2D       :
      computeMaxPoolingOutputSize(layer);
      break;
    case ocl::Layer::GLOBAL_MAX_POOLING_2D:
      computeGlobalMaxPoolingOutputSize(layer);
      break;
    case ocl::Layer::DENSE                :
      computeDenseOutputSize(layer);
      break;
  }
}

void computeLayersOutputSizes(
    std::shared_ptr<ocl::Network>const&net){
  assert(net != nullptr);
  for(auto const&x:net->outputs)
    computeLayerOutputSize(net,x);
}

void addLayerToListOfAllLayers(
    std::shared_ptr<ocl::Network>const&net  ,
    std::shared_ptr<ocl::Layer  >const&layer){
  assert(net != nullptr);
  assert(layer != nullptr);
  net->allLayers[layer->name] = layer;

  for(auto const&i:layer->inputs){
    assert(i != nullptr);
    auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(i);
    assert(inputLayer != nullptr);
    addLayerToListOfAllLayers(net,inputLayer);
  }
}


void fillListOfAllLayers(std::shared_ptr<ocl::Network>const&net){
  assert(net != nullptr);
  for(auto const&x:net->outputs)
    addLayerToListOfAllLayers(net,x);
}

std::shared_ptr<ocl::Network>ocl::translateNetwork(
    std::shared_ptr<cnnConf::Network>const&net){

  auto result = std::make_shared<ocl::Network>();

  translateLayers(result,net);

  fillLayersOutputs(result);

  fillNetworkInputs(result);

  computeLayersOutputSizes(result);

  fillListOfAllLayers(result);

  return result;
}


