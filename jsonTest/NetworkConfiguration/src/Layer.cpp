#include<vector>

#include<NetworkConfiguration/Layer.h>

/**
 * Constructor of the class instance
 * @param t type of layer
 */
cnnConf::Layer::Layer(
    Type const&t):type(t){}

/**
 * @brief Destructor of the class instance
 */
cnnConf::Layer::~Layer(){}
 
/**
 * @brief This function translates name of layer into layer type.
 *
 * @param name layer name
 *
 * @return layer type
 */
cnnConf::Layer::Type cnnConf::Layer::translateLayerNameToLayerType(
    std::string const&name){
  std::vector<std::string>const layerNames = {
    "input"               ,
    "conv2d"              ,
    "max_pooling2d"       ,
    "global_max_pooling2d",
    "dropout"             ,
    "dense"               ,
  };
  for(size_t i=0;i<layerNames.size();++i)
    if(name.find(layerNames.at(i)) == 0)
      return static_cast<Type>(static_cast<size_t>(INPUT)+i);
  throw std::string("unknown layer name: "+name);
}



