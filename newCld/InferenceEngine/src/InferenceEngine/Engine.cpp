#include<cassert>

#include<NetworkConfiguration/Network.h>

#include<InferenceEngine/Engine.h>

InferenceEngine::InferenceEngine(
    std::shared_ptr<cnnConf::Network>const&network):
  network(network)
{
}

InferenceEngine::~InferenceEngine(){}

std::vector<size_t>InferenceEngine::getInputSize()const{
  assert(network != nullptr);
  return network->getInputSize();
}

std::vector<std::string>InferenceEngine::getOutputLayerNames()const{
  assert(network != nullptr);
  return network->getOutputLayerNames();
}
