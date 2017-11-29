#include<NetworkConfiguration/MaxPooling.h>


cnnConf::MaxPooling::MaxPooling(
    std::array<size_t,2>const&size  ,
    std::array<size_t,2>const&stride):
  Layer (Layer::MAX_POOLING_2D),
  size  (size                 ),
  stride(stride               )
{
}

size_t cnnConf::MaxPooling::getSizeX  ()const{
  return size  [0];
}

size_t cnnConf::MaxPooling::getSizeY  ()const{
  return size  [1];
}

size_t cnnConf::MaxPooling::getStrideX()const{
  return stride[0];
}

size_t cnnConf::MaxPooling::getStrideY()const{
  return stride[1];
}

std::array<size_t,2>cnnConf::MaxPooling::getSize  ()const{
  return size  ;
}

std::array<size_t,2>cnnConf::MaxPooling::getStride()const{
  return stride;
}

