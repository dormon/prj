#pragma once

#include<vector>

template<typename T=float>
struct MeanVariance{
  T mean;
  T variance;
};

template<typename T=float>
T computeMean(std::vector<T>const&v){
  T mean = 0;
  for(auto const&x:v)
    mean += x;
  return mean / static_cast<T>(v.size());
}

template<typename T=float>
T computeVariance(std::vector<T>const&v,T const&mean){
  T variance = 0;
  for(auto const&x:v){
    auto d = (mean - x);
    d*=d;
    variance += d;
  }
  return variance /= static_cast<T>(v.size());
}

template<typename T=float>
MeanVariance<T> computeMeanVariance(std::vector<T>const&v){
  MeanVariance<T> r;
  r.mean = computeMean(v);
  r.variance = computeVariance(v,r.mean);
  return r;
}

