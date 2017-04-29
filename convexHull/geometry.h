#pragma once

#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<glm/glm.hpp>

class AdjacentPlane;

class AdjacentPoint: glm::vec3{
  protected:
    std::set<AdjacentPlane>_adjacentPlanes;
  public:
    AdjacentPoint(){}
    AdjacentPoint(AdjacentPlane a,AdjacentPlane b,AdjacentPlane c);
    bool operator==(AdjacentPoint const&other)const;
    bool operator< (AdjacentPoint const&other)const;
};

class AdjacentPlane: glm::vec4{
  protected:
    std::set<AdjacentPoint>_adjacentPoints;
  public:
    AdjacentPlane(){}
    AdjacentPlane(AdjacentPoint a,AdjacentPoint b,AdjacentPoint c);
    bool operator==(AdjacentPlane const&other)const;
    bool operator< (AdjacentPlane const&other)const;
    int  adjacency(AdjacentPoint const&point);
};

