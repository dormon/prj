#include<iostream>
#include<istream>
#include<fstream>
#include"json.hpp"

#include"NIndex.h"
#include"Tensor.h"

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

//padding: same, zero
//stride
//pool_size
//
//

class Conv2DLayerConfiguration: public LayerConfiguration{
  public:
    Conv2DLayerConfiguration(
        Tensor<float>const&weights,
        Tensor<float>const&biases ):
      LayerConfiguration(LayerConfiguration::CONV_2D),
      weights(weights),
      biases(biases){}
    size_t getWidth()const{
      return weights.size.at(0);
    }
    size_t getHeight()const{
      return weights.size.at(1);
    }
    size_t getNofInputChannels()const{
      return weights.size.at(2);
    }
    size_t getNofFilters()const{
      return weights.size.at(3);
    }
  protected:
    Tensor<float>weights;
    Tensor<float>biases;
};

class MaxPoolingLayerConfiguration: public LayerConfiguration{
  public:
    MaxPoolingLayerConfiguration(
        std::array<size_t,2>const&size  ,
        std::array<size_t,2>const&stride):
      LayerConfiguration(LayerConfiguration::MAX_POOLING_2D),
      size              (size                              ),
      stride            (stride                            ){}
    size_t getSizeX  ()const{return size  [0];}
    size_t getSizeY  ()const{return size  [1];}
    size_t getStrideX()const{return stride[0];}
    size_t getStrideY()const{return stride[1];}
    std::array<size_t,2>getSize  ()const{return size  ;}
    std::array<size_t,2>getStride()const{return stride;}
  protected:
    std::array<size_t,2>size  ;
    std::array<size_t,2>stride;
};

class GlobalMaxPoolingLayerConfiguration: public LayerConfiguration{
  public:
    GlobalMaxPoolingLayerConfiguration():
      LayerConfiguration(LayerConfiguration::GLOBAL_MAX_POOLING_2D){}
};

class DropoutLayerConfiguration: public LayerConfiguration{
  public:
    DropoutLayerConfiguration(
        float rate):
      LayerConfiguration(LayerConfiguration::DROPOUT),
      rate              (rate                       ){}
    float getRate()const{return rate;}
  protected:
    float rate;
};

class DenseLayerConfiguration: public LayerConfiguration{
  public:
    DenseLayerConfiguration(
        Tensor<float>const&weights,
        Tensor<float>const&biases ):
      LayerConfiguration(LayerConfiguration::DENSE),
      weights(weights),
      biases(biases){}
  protected:
    Tensor<float>weights;
    Tensor<float>biases ;
};

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
