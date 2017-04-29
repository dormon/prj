#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>
#include<cassert>
#include<functional>
#include<typeinfo>
#include<typeindex>

template<typename TYPE,size_t DIMENSION>
class Vertex{
  public:
    TYPE components[DIMENSION];
};

template<typename TYPE,size_t DIMENSION>
class Hyperplane{
  public:
    TYPE components[DIMENSION];
};

template<typename TYPE,size_t DIMENSION>
TYPE multiply(Vertex<TYPE,DIMENSION>const&v,Hyperplane<TYPE,DIMENSION>const&p){
  TYPE result = 0;
  for(size_t i=0;i<DIMENSION;++i)
    result += v.components[i] * p.components[i];
  return result;
}

template<typename TYPE,size_t DIMENSION>
TYPE multiply(Hyperplane<TYPE,DIMENSION>const&p,Vertex<TYPE,DIMENSION>const&v){
  TYPE result = 0;
  for(size_t i=0;i<DIMENSION;++i)
    result += v.components[i] * p.components[i];
  return result;
}

template<typename TYPE,size_t DIMENSION>
bool onPlane(Hyperplane<TYPE,DIMENSION>const&p,Vertex<TYPE,DIMENSION>const&v){
  return multiply(p,v)==0;
}

template<typename TYPE,size_t DIMENSION>
bool inFrontOfPlane(Hyperplane<TYPE,DIMENSION>const&p,Vertex<TYPE,DIMENSION>const&v){
  return multiply(p,v)>0;
}

template<typename TYPE,size_t DIMENSION>
bool behindPlane(Hyperplane<TYPE,DIMENSION>const&p,Vertex<TYPE,DIMENSION>const&v){
  return multiply(p,v)<0;
}

template<typename TYPE,size_t DIMENSION>
class Polytope{
  public:
    std::vector<Hyperplane<TYPE,DIMENSION>>planes;;
};

bool all(){return true;}

template<typename BOOL>
bool all(BOOL b){
  return b;
}

template<typename FIRST_BOOL,typename... REST_BOOLS>
bool all(FIRST_BOOL fb,REST_BOOLS ...bools){
  return fb && all(bools...);
}

bool any(){return false;}

template<typename BOOL>
bool any(BOOL b){
  return b;
}

template<typename FIRST_BOOL,typename... REST_BOOLS>
bool any(FIRST_BOOL fb,REST_BOOLS... bools){
  return fb || any(bools...);
}

class All{
  public:

};
 
// multiply(x,y) := for(r=0,i=0;i<x;++i)r+=y;return r;
// multiply(x,y) == multiply(y,x)
//
//
// forall x € X: x*x € X
// exist  x € X: x/x € X
//
//
//
//

/*
 *
 *
using Point      = Simplex<0>;
using Segment    = Simplex<1>;
using Triangle   = Simplex<2>;
using Tetrahedra = Simplex<3>;

using Line  = Hyperplane<2>;
using Plane = Hyperplane<3>;

using Vertex1D = Vertex<1>;
using Vertex2D = Vertex<2>;
using Vertex3D = Vertex<3>;
using Vertex4D = Vertex<4>;


template<typename PRIMITIVE>
constexpr size_t verticesPer();

template<>constexpr size_t verticesPer<Point     >(){return 1;}
template<>constexpr size_t verticesPer<Segment   >(){return 2;}
template<>constexpr size_t verticesPer<Triangle  >(){return 3;}
template<>constexpr size_t verticesPer<Tetrahedra>(){return 4;}

template<typename SUBSPACE>
constexpr size_t componentsPer();

template<>constexpr size_t componentsPer<Line >(){return 3;}
template<>constexpr size_t componentsPer<Plane>(){return 4;}

template<>constexpr size_t componentsPer<Vertex1D>(){return 1;}
template<>constexpr size_t componentsPer<Vertex2D>(){return 2;}
template<>constexpr size_t componentsPer<Vertex3D>(){return 3;}
template<>constexpr size_t componentsPer<Vertex4D>(){return 4;}
// */

int main(){
  //verticesPer<Triangle>();
  return 0;
}
