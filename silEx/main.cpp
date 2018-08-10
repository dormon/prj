#include <iostream>
#include <tuple>
#include <sstream>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <iomanip>
#include <map>
#include <array>
#include <glm/glm.hpp>
#include <limits>

class MaskData{
  public:
    uint8_t  mask    ;
    std::vector<uint32_t>edges;
};

size_t const nofOctreeChildren = 8;

class Node{
  public:
    std::array<Node*,nofOctreeChildren>children;
    std::vector<uint32_t>silEdges;
    std::vector<uint32_t>potEdges;
};

using Plane3D = glm::vec4;
using Point3D = glm::vec3;

class AABB{
  public:
    Point3D corner[2];
};

size_t const nofOppositeVertices = 2;
size_t const pointsPerEdge       = 2;

class EdgeInput{
  public:
    Point3D point   [pointsPerEdge      ];
    Point3D opposize[nofOppositeVertices];
};

class EdgePlanes{
  public:
    Plane3D planes[nofOppositeVertices];
};

EdgePlanes getEdgePlanes(EdgeInput const&i){
  EdgePlanes result;
  return result;
}

AABB getAABB(std::vector<EdgeInput>const&input){
  AABB result = {
    Point3D(+std::numeric_limits<float>::max()),
    Point3D(-std::numeric_limits<float>::min()),
  };
  for(auto const&x:input)
    for(size_t i=0;i<2;++i){
      result.corner[0] = glm::min(result.corner[0],x.point[i]);
      result.corner[1] = glm::max(result.corner[1],x.point[i]);
    }
  return result;
}

AABB scaleAABB(AABB const&aabb,float scale){
  auto const center               = (aabb.corner[0] + aabb.corner[1]) / 2.f;
  auto const diagonal             = (aabb.corner[1] - aabb.corner[0]);
  auto const maxDiagonalComponent = glm::max(glm::max(diagonal.x,diagonal.y),diagonal.z);
  auto const newDiagonal          = glm::vec3(maxDiagonalComponent*scale/2.f);
  AABB const result               = {center - newDiagonal,center + newDiagonal};
  return result;
}

AABB getScaledAABB(std::vector<EdgeInput>const&input,float scale){
  return scaleAABB(getAABB(input),scale);
}

size_t const nofCubeCorner = 8;

glm::vec3 getAABBCorner(AABB const&box,size_t id){
  auto const&a = box.corner[0];
  auto const&b = box.corner[1];
  glm::vec3 result;
  for(size_t i=0;i<3;++i){
    auto const selectB = (id>>i)&1;
    if(selectB)result[i] = b[i];
    else       result[i] = a[i];
  }
  return result;
}

int planePointConfiguration(glm::vec4 const&plane,glm::vec3 const&point){
  return static_cast<int>(glm::sign(glm::dot(plane,glm::vec4(point,1.f))));
}


int planeAABBConfiguration(glm::vec4 const&plane,AABB const&box){
  bool isAnyCornerInfront = false;
  bool isAnyCornerBehind  = false;
  for(size_t i=0;i<nofCubeCorner;++i){
    auto const corner        = getAABBCorner(box,i);
    auto const configuration = planePointConfiguration(plane,corner);
    isAnyCornerInfront |= configuration > 0;
    isAnyCornerBehind  |= configuration < 0;
    if(isAnyCornerBehind && isAnyCornerInfront)return 0;
  }
  if(isAnyCornerInfront)return 1;
  return -1;
}

bool operator<=(Point3D const&a,Point3D const&b){
  for(size_t i=0;i<3;++i)
    if(a[i] > b[i])return false;
  return true;
}

Plane3D constructPlaneOrdered(Point3D const&a,Point3D const&b,Point3D const&c){
  auto const n = glm::cross(b-a,c-a);
  return Plane3D(n,glm::dot(n,a));
}

Plane3D flipPlaneNormal(Plane3D const&a){
  return Plane3D(-a.x,-a.y,-a.z,a.w);
}

Plane3D constructPlane(Point3D const&a,Point3D const&b,Point3D const&c){
  if(a<=b && b<=c)return                (constructPlaneOrdered(a,b,c));
  if(a<=c && c<=b)return flipPlaneNormal(constructPlaneOrdered(a,c,b));
  if(b<=a && a<=c)return flipPlaneNormal(constructPlaneOrdered(b,a,c));
  if(b<=c && c<=a)return                (constructPlaneOrdered(b,c,a));
  if(c<=a && a<=b)return                (constructPlaneOrdered(c,a,b));
                  return flipPlaneNormal(constructPlaneOrdered(c,b,a));
}

Node*createGraph(std::vector<EdgeInput>const&input,size_t levels,float scale){
  auto aabb = getScaledAABB(input,scale);
  return nullptr;
}

int main() {
  //auto m = glm::min(glm::vec3(0.f,1.f,2.f),glm::vec3(1.f,0.f,3.f));
  //std::cout << m.x << " " << m.y << " " << m.z << std::endl;
  //

  return 0;
}
