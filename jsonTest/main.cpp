#include<iostream>
#include<istream>
#include<fstream>

#include<NetworkConfigurationJSONLoader/Loader.h>

#include"NetworkConfigurationJSONLoader/include/NetworkConfigurationJSONLoader/json.hpp"

std::string loadTextFile(std::string const&fileName){
  std::ifstream f(fileName.c_str());
  if(!f.is_open())
    return "";
  std::string str((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  f.close();
  return str;
}

int main(){
  std::string jsonSource = loadTextFile("/home/dormon/git/prj/jsonTest/model_simple_00.json");
  ////std::cout << jsonSource << std::endl;
  //auto j = nlohmann::json::parse(jsonSource);
  //std::cout << j.size() << std::endl;

  ////std::ifstream i("/home/dormon/git/prj/jsonTest/model_simple_00.json");
  //nlohmann::json jj;
  //jj.parse(jsonSource);
  ////i >> jj;
  //std::cout << jj.size() << std::endl;
  
  auto config = cnnConf::loadJSON(jsonSource);
  std::cout << config->getNofLayers() << std::endl;
  auto input = std::dynamic_pointer_cast<cnnConf::Input>(config->getLayer(0));
  std::cout << input->getWidth() << std::endl;

  auto conv = std::dynamic_pointer_cast<cnnConf::Conv2D>(config->getLayer(1));
  for(auto const&x:conv->getWeights().data)
    std::cout << x << std::endl;

  auto dense = std::dynamic_pointer_cast<cnnConf::Dense>(config->getLayer(12));
  std::cout << "lastDenseInput : " << dense->getInputSize () << std::endl;
  std::cout << "lastDenseOutput: " << dense->getOutputSize() << std::endl;
  for(auto const&x:dense->getWeights().data)
    std::cout << x << std::endl;
  return 0;
}

#if 0
using json = nlohmann::json;

std::string loadTextFile(std::string const&fileName){
  std::ifstream f(fileName.c_str());
  if(!f.is_open())
    return "";
  std::string str((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  f.close();
  return str;
}

//padding: same, zero
//stride
//pool_size
//
//

class NetworkConfiguration{
  public:
    NetworkConfiguration(std::string const&jsonSource);
    ~NetworkConfiguration();
    void loadConfiguration(nlohmann::json const&j);
    std::shared_ptr<InputLayerConfiguration>getInputLayer()const;
  protected:
    std::shared_ptr<LayerConfiguration>loadLayer                (nlohmann::json const&j);
    std::shared_ptr<LayerConfiguration>loadInputLayer           (nlohmann::json const&j);
    std::shared_ptr<LayerConfiguration>loadConv2DLayer          (nlohmann::json const&j);
    std::shared_ptr<LayerConfiguration>loadMaxPoolingLayer      (nlohmann::json const&j);
    std::shared_ptr<LayerConfiguration>loadGlobalMaxPoolingLayer(nlohmann::json const&j);
    std::shared_ptr<LayerConfiguration>loadDropoutLayer         (nlohmann::json const&j);
    std::shared_ptr<LayerConfiguration>loadDenseLayer           (nlohmann::json const&j);
    Tensor<float>loadTensor(nlohmann::json j);
    std::vector<std::shared_ptr<LayerConfiguration>>layers;
};


void NetworkConfiguration::loadConfiguration(nlohmann::json const&j){
  size_t const nofLayers = j.size();
  for(size_t layerId = 0; layerId < nofLayers; ++layerId)
    layers.push_back(loadLayer(j.at(layerId)));
}

std::shared_ptr<LayerConfiguration>NetworkConfiguration::loadLayer(nlohmann::json const&j){
  auto const layerName = j.at("name").get<std::string>();
  auto const layerType = LayerConfiguration::translateLayerNameToLayerType(layerName);
  using LoadLayer = std::shared_ptr<LayerConfiguration>(NetworkConfiguration::*)(nlohmann::json const&);
  std::vector<LoadLayer> loadLayerFunctions = {
    &NetworkConfiguration::loadInputLayer           ,
    &NetworkConfiguration::loadConv2DLayer          ,
    &NetworkConfiguration::loadMaxPoolingLayer      ,
    &NetworkConfiguration::loadGlobalMaxPoolingLayer,
    &NetworkConfiguration::loadDropoutLayer         ,
    &NetworkConfiguration::loadDenseLayer           ,
  };
  auto const loadLayerFunctionId = static_cast<size_t>(layerType) - static_cast<size_t>(LayerConfiguration::INPUT);
  auto const loadLayerFunction = loadLayerFunctions.at(loadLayerFunctionId);
  
  return (this->*loadLayerFunction)(j);
}

std::shared_ptr<LayerConfiguration>NetworkConfiguration::loadInputLayer(nlohmann::json const&j){
  auto const shape = j.at("config").at("batch_input_shape");
  size_t width    = shape.at(1).get<size_t>();
  size_t height   = shape.at(2).get<size_t>();
  size_t channels = shape.at(3).get<size_t>();
  auto result = std::make_shared<InputLayerConfiguration>(width,height,channels);
  return std::dynamic_pointer_cast<LayerConfiguration>(result);
}

Tensor<float>NetworkConfiguration::loadTensor(nlohmann::json j){
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

std::shared_ptr<LayerConfiguration>NetworkConfiguration::loadConv2DLayer(nlohmann::json const&j){
  auto const reorderConv2dToWidthHeightChannelsFilters = [&](Tensor<float>&t){
    t.reshape({t.size[2],t.size[3],t.size[0],t.size[1]},{2,3,0,1});
  };

  auto weights = loadTensor(j.at("weights").at(0));
  reorderConv2dToWidthHeightChannelsFilters(weights);
  auto biases  = loadTensor(j.at("weights").at(1));
  auto result = std::make_shared<Conv2DLayerConfiguration>(weights,biases);
  return std::dynamic_pointer_cast<LayerConfiguration>(result);
}

std::shared_ptr<LayerConfiguration>NetworkConfiguration::loadMaxPoolingLayer(nlohmann::json const&j){
  auto const config = j.at("config");
  std::array<size_t,2>const size({{
    config.at("pool_size").at(0),
    config.at("pool_size").at(1),
  }});
  std::array<size_t,2>const stride({{
    config.at("strides"  ).at(0),
    config.at("strides"  ).at(1),
  }});
  auto result = std::make_shared<MaxPoolingLayerConfiguration>(size,stride);
  return std::dynamic_pointer_cast<LayerConfiguration>(result);
}

std::shared_ptr<LayerConfiguration>NetworkConfiguration::loadGlobalMaxPoolingLayer(nlohmann::json const&){
  auto result = std::make_shared<GlobalMaxPoolingLayerConfiguration>();
  return std::dynamic_pointer_cast<LayerConfiguration>(result);
}

std::shared_ptr<LayerConfiguration>NetworkConfiguration::loadDropoutLayer         (nlohmann::json const&j){
  auto const config = j.at("config");
  float const rate = config.at("rate");
  auto result = std::make_shared<DropoutLayerConfiguration>(rate);
  return std::dynamic_pointer_cast<LayerConfiguration>(result);
}

std::shared_ptr<LayerConfiguration>NetworkConfiguration::loadDenseLayer           (nlohmann::json const&j){
  auto const data = j.at("weights");

  Tensor<float>weights({},{});
  Tensor<float>biases ({},{});
  auto result = std::make_shared<DenseLayerConfiguration>(weights,biases);
  return std::dynamic_pointer_cast<LayerConfiguration>(result);
}

int main(){

  std::ifstream i("/home/dormon/git/prj/jsonTest/model_simple_00.json");
  json j;
  i >> j;
  std::cout << j.size() << std::endl;
  auto jj = j.at(0);
  std::cout << j.at(0).at("config").at("batch_input_shape").at(1).get<size_t>() << std::endl;

  auto const getJsonTensorSize = [&](nlohmann::json j){
    std::vector<size_t>result;
    while(j.is_array()){
      result.push_back(j.size());
      j = j.at(0);
    }
    return result;
  };
  auto const getJsonTensorElement = [&](nlohmann::json j,std::vector<size_t>index){
    for(auto const&x:index)
      j = j.at(x);
    return static_cast<float>(j);
  };


  auto const getJsonTensorData = [&](nlohmann::json j){
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
    return Tensor<float>(data,tensorSize);
  };

  auto kernel = getJsonTensorData(j.at(1).at("weights").at(0));
  kernel.reshape({3,3,3,4},{1,2,3,0});

  for(auto const&x:kernel.size)
    std::cout << x << " ";
  std::cout << std::endl;


  for(auto const&x:kernel.data)
    std::cout << x << std::endl;



  return 0;
}
#endif
