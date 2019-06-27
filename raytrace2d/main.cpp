#include<iostream>
#include<vector>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>
#include<string>
#include<type_traits>
#include<sstream>
#include<typeinfo>
#include<utility>
#include<limits>
#include <glm/glm.hpp>

class Edge{
  public:
    Edge(glm::vec2 const&a,glm::vec2 const&b):a(a),b(b){}
    glm::vec2 a,b;
};

class Ray{
  public:
    Ray(glm::vec2 const&o,glm::vec2 const&d):o(o),d(d){}
    glm::vec2 o,d;
};

class AABB{
  public:
    AABB(glm::vec2 const&a = glm::vec2(1.f),glm::vec2 const&b = glm::vec2(-1.f)):a(a),b(b){}
    void extend(glm::vec2 const&p){
      a = glm::min(a,p);
      b = glm::max(b,p);
    }
    void extend(Edge const&e){
      extend(e.a);
      extend(e.b);
    }
    void extend(AABB const&aabb){
      extend(aabb.a);
      extend(aabb.b);
    }
    glm::vec2 a;
    glm::vec2 b;
};

class OctreeNode{
  public:
    OctreeNode*childs[8];
    std::vector<Edge>edges;
};

class Node{
  public:
    uint32_t edgeStart;
    uint32_t edgeNum;
    uint32_t childrenStart;
    uint32_t childrenNum;
};

float colission(Edge const&e,Ray const&r){
  auto const&a=e.a;
  auto const&b=e.b;
  auto const&o=r.o;
  auto const&d=r.d;
    
  float det = (b.x-a.x) * d.y - (b.y-a.y) * d.x;
  if(det == 0)return std::numeric_limits<float>::infinity();
  float ldet = (a.x-o.x) * d.y - (a.y-o.y) * d.x;
  float l = ldet / det;
  if(l < 0 || l > 1)return std::numeric_limits<float>::infinity();
  float tdet = (a.y-o.y)*(a.x-b.x) - (a.x-o.x)*(a.y-b.y);
  float t = tdet / det;
  if(t<0)return std::numeric_limits<float>::infinity();
  return t;
}

float colission(AABB const&aabb,Ray const&ray){
  auto const&o=ray.o;
  auto const&d=ray.d;
  auto const&a=aabb.a;
  auto const&b=aabb.b;
  //o.x+t*d.x > a.x
  //o.x+t*d.x < b.x
  //o.y+t*d.y > a.y
  //o.y+t*d.y < b.y
  //
  //t > (a.x-o.x) / d.x
  //t < (b.x-o.x) / d.x
  //t > (a.y-o.y) / d.y
  //t < (b.y-o.y) / d.y
  float tmin = 0;
  float tmax = std::numeric_limits<float>::max();
  for(int i=0;i<2;++i){
    if(d[i] > 0){
      tmin = glm::max(tmin,(a[i]-o[i])/d[i]);
      tmax = glm::min(tmax,(b[i]-o[i])/d[i]);
    }
    if(d[i] < 0){
      tmax = glm::min(tmax,(a[i]-o[i])/d[i]);
      tmin = glm::max(tmin,(b[i]-o[i])/d[i]);
    }
  }
  if(tmax < tmin)return std::numeric_limits<float>::infinity();
  return tmin;
}

int main(){

  return 0;
}
