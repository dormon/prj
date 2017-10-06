#include<iostream>
#include<istream>
#include<fstream>
#include"json.hpp"

using json = nlohmann::json;

std::string loadTextFile(std::string const&fileName){
  std::ifstream f(fileName.c_str());
  if(!f.is_open())
    return "";
  std::string str((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  f.close();
  return str;
}

class LayerConfiguration{
  public:
    enum Type{
      INPUT                 = 0,
      CONV_2D               = 1,
      MAX_POOLING_2D        = 2,
      GLOBAL_MAX_POOLING_2D = 3,
      DROPOUT               = 4,
      DENSE                 = 5,
    }type;
    LayerConfiguration(Type const&t):type(t){}
    static Type translateLayerNameToLayerType(std::string const&name){
      std::vector<std::string>layerNames = {
        "input"               ,
        "conv2d"              ,
        "max_pooling2d"       ,
        "global_max_pooling2d",
        "dropout"             ,
        "dense"               ,
      };
      for(size_t i=0;i<layerNames.size();++i)
        if(name.find(layerNames.at(i)) == 0)return static_cast<Type>(static_cast<size_t>(INPUT)+i);
      throw std::string("unknown layer name: "+name);
    }
};

class InputLayerConfiguration: public LayerConfiguration{
  public:
    InputLayerConfiguration(
        size_t w,
        size_t h,
        size_t c):LayerConfiguration(LayerConfiguration::INPUT),width(w),height(h),channels(c){}
    size_t getNofChannels()const{
      return channels;
    }
    size_t getWidth()const{
      return width;
    }
    size_t getHeight()const{
      return height;
    }
  protected:
    size_t width   ;
    size_t height  ;
    size_t channels;
};

class Conv2DLayerConfiguration: public LayerConfiguration{
  public:
  protected:
    std::vector<float>biases;
};

class NetworkConfiguration{
  public:
    NetworkConfiguration(std::string const&jsonSource);
    ~NetworkConfiguration();
    void loadConfiguration(nlohmann::json const&j);
    std::shared_ptr<InputLayerConfiguration>getInputLayer()const;
  protected:
    std::shared_ptr<LayerConfiguration>loadLayer(nlohmann::json const&j);
    std::shared_ptr<LayerConfiguration>loadInputLayer(nlohmann::json const&j);
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
    &NetworkConfiguration::loadInputLayer,
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

int main(){

  std::ifstream i("/home/dormon/Desktop/model_simple_00.json");
  json j;
  i >> j;
  std::cout << j.size() << std::endl;
  auto jj = j.at(0);
  std::cout << j.at(0).at("config").at("batch_input_shape").at(1).get<size_t>() << std::endl;

  std::cout << j.at(1).at("weights").is_array() << std::endl;
  std::vector<float> t = j.at(1).at("weights").at(0);
  for(auto const&x:t)
    std::cout << x << " ";
  std::cout << std::endl;

  return 0;
}
