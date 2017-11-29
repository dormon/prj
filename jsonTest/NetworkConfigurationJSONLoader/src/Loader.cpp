#include<cassert>
#include<map>

#include<NetworkConfiguration/NetworkConfiguration.h>

#include<NetworkConfigurationJSONLoader/json.hpp>
#include<NetworkConfigurationJSONLoader/Loader.h>

#define ___ std::cerr<<__FILE__<<" "<<__LINE__<<std::endl

std::vector<std::string>static const layerNames = {
  "input"               ,
  "conv2d"              ,
  "max_pooling2d"       ,
  "global_max_pooling2d",
  "dense"               ,
};

std::string getLayerName(nlohmann::json const&j){
  return j.at("name").get<std::string>();
}

bool isLayerValid(nlohmann::json const&j){
  auto const layerName = getLayerName(j);
  for(size_t i=0;i<layerNames.size();++i)
    if(layerName.find(layerNames[i]) == 0)
      return true;
  return false;
}

size_t getNofAllLayers(nlohmann::json const&j){
  return j.size();
}

nlohmann::json getLayerJSON(nlohmann::json const&j,size_t layerId){
  return j.at(layerId);
}

size_t getNofValidLayers(nlohmann::json const&j){
  size_t const nofAllLayers = getNofAllLayers(j);
  size_t nofValidLayers = 0;
  for(size_t layerId = 0; layerId < nofAllLayers; ++layerId)
    if(isLayerValid(getLayerJSON(j,layerId)))
      ++nofValidLayers;
  return nofValidLayers;
}

cnnConf::Layer::Type getLayerType(nlohmann::json const&j){
  auto const layerName = getLayerName(j);
  for(size_t i=0;i<layerNames.size();++i)
    if(layerName.find(layerNames[i]) == 0)
      return static_cast<cnnConf::Layer::Type>(static_cast<size_t>(cnnConf::Layer::INPUT)+i);
  return cnnConf::Layer::INPUT;
}

std::shared_ptr<cnnConf::Layer>loadInput(nlohmann::json const&j){
  auto const shape = j.at("config").at("batch_input_shape");
  size_t width    = shape.at(1).get<size_t>();
  size_t height   = shape.at(2).get<size_t>();
  size_t channels = shape.at(3).get<size_t>();
  auto result = std::make_shared<cnnConf::Input>(width,height,channels);
  return std::dynamic_pointer_cast<cnnConf::Layer>(result);
}

Tensor<float>loadTensor(nlohmann::json j){
  auto const getJsonTensorSize = [&](nlohmann::json j){
    std::vector<size_t>result;
    while(j.is_array()){
      result.push_back(j.size());
      j = j.at(0);
    }
    return result;
  };
  auto const getJsonTensorElement = [&](nlohmann::json j,std::vector<size_t>const&index){
    for(auto const&x:index)
      j = j.at(x);
    return static_cast<float>(j);
  };

  auto const getJsonTensor = [&](nlohmann::json j){
    auto tensorSize = getJsonTensorSize(j);
    NIndex n(tensorSize);
    std::vector<size_t>ordering(n.getDimension());
    std::iota(ordering.begin(),ordering.end(),0);
    std::reverse(ordering.begin(),ordering.end());
    n.zero();

    std::vector<float>data;
    do{
      data.push_back(getJsonTensorElement(j,n.getParts()));
    }while(n.inc(ordering));

    std::reverse(tensorSize.begin(),tensorSize.end());
    Tensor<float>result(data,tensorSize);
    return result;
  };
  return getJsonTensor(j);
}

std::shared_ptr<cnnConf::Layer>loadConv2D(nlohmann::json const&j){
  auto const reorderConv2dToWidthHeightChannelsFilters = [&](Tensor<float>&t){
    t.reshape({t.size[2],t.size[3],t.size[0],t.size[1]},{2,3,0,1});
  };

  auto weights = loadTensor(j.at("weights").at(0));
  reorderConv2dToWidthHeightChannelsFilters(weights);
  auto biases  = loadTensor(j.at("weights").at(1));
  auto result = std::make_shared<cnnConf::Conv2D>(weights,biases);
  return std::dynamic_pointer_cast<cnnConf::Layer>(result);
}

std::shared_ptr<cnnConf::Layer>loadMaxPooling(nlohmann::json const&j){
  auto const config = j.at("config");
  std::array<size_t,2>const size({{
    config.at("pool_size").at(0),
    config.at("pool_size").at(1),
  }});
  std::array<size_t,2>const stride({{
    config.at("strides"  ).at(0),
    config.at("strides"  ).at(1),
  }});
  auto result = std::make_shared<cnnConf::MaxPooling>(size,stride);
  return std::dynamic_pointer_cast<cnnConf::Layer>(result);
}

std::shared_ptr<cnnConf::Layer>loadGlobalMaxPooling(nlohmann::json const&){
  auto result = std::make_shared<cnnConf::GlobalMaxPooling>();
  return std::dynamic_pointer_cast<cnnConf::Layer>(result);
}

std::shared_ptr<cnnConf::Layer>loadDense           (nlohmann::json const&j){
  auto const data = j.at("weights");

  auto const transpose = [&](Tensor<float>&t){
    t.reshape({t.size[1],t.size[0]},{1,0});
  };

  auto       weights = loadTensor(data.at(0));
  transpose(weights);
  auto const biases  = loadTensor(data.at(1));
  auto result = std::make_shared<cnnConf::Dense>(weights,biases);
  return std::dynamic_pointer_cast<cnnConf::Layer>(result);
}


std::shared_ptr<cnnConf::Layer>loadLayer(nlohmann::json const&j){
  auto const layerType = getLayerType(j);
  using LoadLayerFunction = std::shared_ptr<cnnConf::Layer>(*)(nlohmann::json const&);
  std::vector<LoadLayerFunction>const loadLayerFunctions = {
    loadInput           ,
    loadConv2D          ,
    loadMaxPooling      ,
    loadGlobalMaxPooling,
    loadDense           ,
  };

  auto const loadLayerFunctionId = static_cast<size_t>(layerType) - static_cast<size_t>(cnnConf::Layer::INPUT);
  auto const loadLayerFunction = loadLayerFunctions.at(loadLayerFunctionId);
  return loadLayerFunction(j);
}


/**
 * @brief This function loads network configuration using json source
 * @param jsonSource json source i.e. content of json file
 *
 * @return network configuration
 */
std::shared_ptr<cnnConf::Network>cnnConf::loadJSON(std::string const&jsonSource){
  auto const j = nlohmann::json::parse(jsonSource);

  auto result = std::make_shared<cnnConf::Network>();

  size_t nofValidLayers = getNofValidLayers(j);
  result->init(nofValidLayers);

  size_t nofAllLayers = getNofAllLayers(j);
  size_t validLayerCounter = 0;
  for(size_t layerId = 0; layerId < nofAllLayers; ++layerId){
    auto layerJson = getLayerJSON(j,layerId);
    if(!isLayerValid(layerJson))continue;

    result->setLayer(validLayerCounter++,loadLayer(layerJson));
  }

  return result;
}
