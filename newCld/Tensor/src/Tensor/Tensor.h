#pragma once

#include<set>
#include<sstream>
#include<numeric>

#include<NIndex/NIndex.h>


template<typename TYPE>
class Tensor{
  public:
    Tensor(){};
    Tensor(std::vector<size_t>const&s,std::vector<TYPE>const&d = {});
    void reshape(
        std::vector<size_t>const&newAxisSizes      ,
        std::vector<size_t>      axisOrderings = {},
        std::vector<bool  >      axisFlips     = {});
    static size_t nofElements(std::vector<size_t>const&s);
    std::vector<size_t>size;
    std::vector<TYPE  >data;
};

template<typename TYPE>
size_t getDimension(Tensor<TYPE>const&t){
  return t.size.size();
}

template<typename TYPE>
bool isEmpty(Tensor<TYPE>const&t){
  return getDimension(t) == 0;
}

template<typename TYPE>
Tensor<TYPE>::Tensor(std::vector<size_t>const&s,std::vector<TYPE>const&d):size(s),data(d){
  if(d.empty()){
    auto const nofElements = std::accumulate(s.begin(),s.end(),1,std::multiplies<typename std::decay<decltype(s)>::type::value_type>());
    data.resize(nofElements);
  }
}

template<typename TYPE>
void Tensor<TYPE>::reshape(
    std::vector<size_t>const&newAxisSizes ,
    std::vector<size_t>      axisOrderings,
    std::vector<bool  >      axisFlips    ){
  if(nofElements(size) != nofElements(newAxisSizes))
    throw std::string("tensor reshaping cannot be done due to different number of elements");
  if(axisOrderings.empty())
    for(size_t i=0;i<size.size();++i)axisOrderings.push_back(i);
  else{
    if(axisOrderings.size() != size.size())
      throw std::string("tensor reshaping cannot be done due to different size of order vector");
    for(auto const&x:axisOrderings)
      if(x >= size.size())
        throw std::string("tensor reshaping cannot be done due to invalid values in order vector");
    std::set<size_t>uniqueOrderValue;
    for(auto const&x:axisOrderings)uniqueOrderValue.insert(x);
    if(uniqueOrderValue.size() != size.size())
      throw std::string("tensor reshaping cannot be done due to duplicate values in order vector");
  }

  NIndex fromIndex(size);
  size_t toIndex1D = 0;
  auto dataCopy = data;

  fromIndex.setMin();
  do{
    auto const fromIndex1D = fromIndex.get1DIndex(axisFlips);
    data[toIndex1D++] = dataCopy[fromIndex1D];
  }while(fromIndex.inc(axisOrderings));

  size = newAxisSizes;
}

template<typename TYPE>
size_t Tensor<TYPE>::nofElements(std::vector<size_t>const&s){
  size_t result = 1;
  for(auto const&x:s)result *= x;
  return result;
}

