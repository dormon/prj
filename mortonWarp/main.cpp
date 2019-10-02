#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdexcept>
#include <string.h>
#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <limits>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ___ std::cerr << __FILE__ << " : " << __LINE__ << std::endl

class AABB{
  public:
    AABB(std::vector<glm::vec2>const&p):AABB(){
      for(auto const&x:p)
        add(x);
    }
    AABB():minPoint(std::numeric_limits<float>::max()),maxPoint(-std::numeric_limits<float>::max()){}
    void add(glm::vec2 const&p){
      minPoint = glm::min(minPoint,p);
      maxPoint = glm::max(maxPoint,p);
    }
    glm::vec2 minPoint;
    glm::vec2 maxPoint;
};

uint32_t requiredBits(uint32_t a){
  return (uint32_t)glm::ceil(glm::log2(a));
}

glm::uvec2 requiredBits(glm::uvec2 const&a){
  return glm::uvec2(requiredBits(a.x),requiredBits(a.y));
}

uint32_t morton(glm::vec2 const&p,AABB const&aabb,glm::uvec2 const& div){
  auto np = (p-aabb.minPoint) / (aabb.maxPoint - aabb.minPoint);
  auto inp = glm::uvec2(np*glm::vec2(div));
  uint32_t res = 0;
  auto const bits = requiredBits(div);
  for(size_t d=0;d<2;++d)
    for(size_t b=0;b<bits[d];++b){
      res |= ((inp[d]>>b)&1)<<(b*2+d);
    }
  return res;
}

class Space{
  public:
    Space(std::vector<glm::vec2>const&p):data(p),aabb(p){
      for(size_t i=0;i<data.size();++i){
        auto mor = morton(data.at(i),aabb,glm::uvec2(128,128));
        mortons.push_back(mor);
        morton2Id[mor] = i;
      }
      std::sort(std::begin(mortons),std::end(mortons));
    }
    std::vector<glm::vec2>data;
    std::vector<uint32_t>mortons;
    std::map<uint32_t,uint32_t>morton2Id;
    AABB aabb;
};

class Mortons{
  public:
    std::vector<uint32_t>data;
};


int main(void)
{
  return EXIT_SUCCESS;
}
