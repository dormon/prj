#pragma once

#include<type_traits>
#include<cstring>

template<typename TYPE,std::size_t DIMENSION,std::size_t N>
struct Simplex;

template<typename TYPE,std::size_t DIMENSION>
struct Simplex<TYPE,DIMENSION,0>{
  TYPE elements[DIMENSION];
  void clear();
  template<typename INPUT_TYPE,std::size_t INPUT_DIMENSION,typename std::enable_if_t<std::is_convertible<INPUT_TYPE,TYPE>::value,int> = 0>
  Simplex<TYPE,DIMENSION,0>&operator=(Simplex<INPUT_TYPE,INPUT_DIMENSION,0>const&s);
  Simplex();
  template<typename INPUT_TYPE,std::size_t INPUT_DIMENSION,typename std::enable_if_t<std::is_convertible<INPUT_TYPE,TYPE>::value,int> = 0>
  Simplex(Simplex<INPUT_TYPE,INPUT_DIMENSION,0> const&s);
};

template<typename TYPE,std::size_t DIMENSION>
using Vertex      = Simplex<TYPE,DIMENSION,0>;

template<typename TYPE,std::size_t DIMENSION,size_t N>
struct Simplex{
  Vertex<TYPE,DIMENSION> vertices[N+1];
  void clear();
  template<
    typename    INPUT_TYPE     ,
    std::size_t INPUT_DIMENSION,
    std::size_t INPUT_N        ,
    typename std::enable_if_t<
      std::is_convertible<INPUT_TYPE,TYPE>::value &&
      DIMENSION >= INPUT_DIMENSION                &&
      N         >= INPUT_N,int> = 0>
  Simplex<TYPE,DIMENSION,N>&operator=(Simplex<INPUT_TYPE,INPUT_DIMENSION,INPUT_N>const&s);
  Simplex();
  template<
    typename    INPUT_TYPE     ,
    std::size_t INPUT_DIMENSION,
    std::size_t INPUT_N        ,
    typename std::enable_if_t<
      std::is_convertible<INPUT_TYPE,TYPE>::value &&
      (DIMENSION >= INPUT_DIMENSION)              &&
      (N         >= INPUT_N),int> = 0>
  Simplex(Simplex<INPUT_TYPE,INPUT_DIMENSION,INPUT_N> const&s);
};

template<typename TYPE,std::size_t DIMENSION>
using Line        = Simplex<TYPE,DIMENSION,1>;

template<typename TYPE,std::size_t DIMENSION>
using Triangle    = Simplex<TYPE,DIMENSION,2>;

template<typename TYPE,std::size_t DIMENSION>
using Tetrahedron = Simplex<TYPE,DIMENSION,3>;

template<typename TYPE>using Vertex1D      = Vertex     <TYPE,1>;
template<typename TYPE>using Vertex2D      = Vertex     <TYPE,2>;
template<typename TYPE>using Vertex3D      = Vertex     <TYPE,3>;
template<typename TYPE>using Vertex4D      = Vertex     <TYPE,4>;
template<typename TYPE>using Line1D        = Line       <TYPE,1>;
template<typename TYPE>using Line2D        = Line       <TYPE,2>;
template<typename TYPE>using Line3D        = Line       <TYPE,3>;
template<typename TYPE>using Line4D        = Line       <TYPE,4>;
template<typename TYPE>using Triangle1D    = Triangle   <TYPE,1>;
template<typename TYPE>using Triangle2D    = Triangle   <TYPE,2>;
template<typename TYPE>using Triangle3D    = Triangle   <TYPE,3>;
template<typename TYPE>using Triangle4D    = Triangle   <TYPE,4>;
template<typename TYPE>using Tetrahedron1D = Tetrahedron<TYPE,1>;
template<typename TYPE>using Tetrahedron2D = Tetrahedron<TYPE,2>;
template<typename TYPE>using Tetrahedron3D = Tetrahedron<TYPE,3>;
template<typename TYPE>using Tetrahedron4D = Tetrahedron<TYPE,4>;

using Vertex1Df      = Vertex1D     <float>;
using Vertex2Df      = Vertex2D     <float>;
using Vertex3Df      = Vertex3D     <float>;
using Vertex4Df      = Vertex4D     <float>;
using Line1Df        = Line1D       <float>;
using Line2Df        = Line2D       <float>;
using Line3Df        = Line3D       <float>;
using Line4Df        = Line4D       <float>;
using Triangle1Df    = Triangle1D   <float>;
using Triangle2Df    = Triangle2D   <float>;
using Triangle3Df    = Triangle3D   <float>;
using Triangle4Df    = Triangle4D   <float>;
using Tetrahedron1Df = Tetrahedron1D<float>;
using Tetrahedron2Df = Tetrahedron2D<float>;
using Tetrahedron3Df = Tetrahedron3D<float>;
using Tetrahedron4Df = Tetrahedron4D<float>;

template<typename TYPE,std::size_t DIMENSION>
void Simplex<TYPE,DIMENSION,0>::clear(){
  std::memset(elements,0,sizeof(TYPE)*DIMENSION);
}

template<typename TYPE,std::size_t DIMENSION>
template<typename INPUT_TYPE,std::size_t INPUT_DIMENSION,typename std::enable_if_t<std::is_convertible<INPUT_TYPE,TYPE>::value,int>>
Simplex<TYPE,DIMENSION,0>&Simplex<TYPE,DIMENSION,0>::operator=(Simplex<INPUT_TYPE,INPUT_DIMENSION,0>const&s){
  for(std::size_t i=0;i<INPUT_DIMENSION;++i)
    elements[i] = s.elements[i];
  return *this;
}

template<typename TYPE,std::size_t DIMENSION>
Simplex<TYPE,DIMENSION,0>::Simplex(){
  clear();
}

template<typename TYPE,std::size_t DIMENSION>
template<typename INPUT_TYPE,std::size_t INPUT_DIMENSION,typename std::enable_if_t<std::is_convertible<INPUT_TYPE,TYPE>::value,int>>
Simplex<TYPE,DIMENSION,0>::Simplex(Simplex<INPUT_TYPE,INPUT_DIMENSION,0> const&s){
  *this = s;
}

template<typename TYPE,std::size_t DIMENSION,size_t N>
void Simplex<TYPE,DIMENSION,N>::clear(){
  for(std::size_t i=0;i<=N;++i)
    vertices[i].clear();
}

template<typename TYPE,std::size_t DIMENSION,size_t N>
template<
  typename    INPUT_TYPE     ,
  std::size_t INPUT_DIMENSION,
  std::size_t INPUT_N        ,
  typename std::enable_if_t<
    std::is_convertible<INPUT_TYPE,TYPE>::value &&
    DIMENSION >= INPUT_DIMENSION                &&
    N         >= INPUT_N,int>>
Simplex<TYPE,DIMENSION,N>&Simplex<TYPE,DIMENSION,N>::operator=(Simplex<INPUT_TYPE,INPUT_DIMENSION,INPUT_N>const&s){
  for(std::size_t i=0;i<INPUT_N;++i)
    vertices[i] = s.vertices[i];
}

template<typename TYPE,std::size_t DIMENSION,size_t N>
Simplex<TYPE,DIMENSION,N>::Simplex(){
  clear();
}

template<typename TYPE,std::size_t DIMENSION,size_t N>
template<
  typename    INPUT_TYPE     ,
  std::size_t INPUT_DIMENSION,
  std::size_t INPUT_N        ,
  typename std::enable_if_t<
    std::is_convertible<INPUT_TYPE,TYPE>::value &&
    (DIMENSION >= INPUT_DIMENSION)              &&
    (N         >= INPUT_N),int>>
Simplex<TYPE,DIMENSION,N>::Simplex(Simplex<INPUT_TYPE,INPUT_DIMENSION,INPUT_N> const&s):Simplex(){
  *this = s;
}

template<
  typename    OUTPUT_TYPE     ,
  std::size_t OUTPUT_DIMENSION,
  typename    INPUT_TYPE      ,
  std::size_t INPUT_DIMENSION ,
  typename std::enable_if_t<
    std::is_convertible<INPUT_TYPE,OUTPUT_TYPE>::value &&
    OUTPUT_DIMENSION == INPUT_DIMENSION + 1
    ,int> = 0>
void createHomogenous(Vertex<OUTPUT_TYPE,OUTPUT_DIMENSION>&output,Vertex<INPUT_TYPE,INPUT_DIMENSION>const&input){
  output = input;
  output.elements[OUTPUT_DIMENSION-1] = 1;
}

template<
  typename    OUTPUT_TYPE     ,
  std::size_t OUTPUT_DIMENSION,
  std::size_t N               ,
  typename    INPUT_TYPE      ,
  std::size_t INPUT_DIMENSION ,
  typename std::enable_if_t<
    (N > 0) &&
    std::is_convertible<INPUT_TYPE,OUTPUT_TYPE>::value &&
    OUTPUT_DIMENSION == INPUT_DIMENSION + 1
    ,int> = 0>
void createHomogenous(Simplex<OUTPUT_TYPE,OUTPUT_DIMENSION,N>&output,Simplex<INPUT_TYPE,INPUT_DIMENSION,N>const&input){
  for(std::size_t v=0;v<=N;++v)
    createHomogenous(output.vertices[v],input.vertices[v]);
}
