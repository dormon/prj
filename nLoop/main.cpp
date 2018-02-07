#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<sstream>
#include<chrono>

#include"forBody.h"



//
//void forLoop(std::vector<size_t>const&maxVals,std::function<void()>const&f){
//  forLoopRec(maxVals.size(),maxVals,f);
//}

void forLoop(
    size_t dim,
    std::vector  <ssize_t>     &ii      ,
    std::vector  <ssize_t>const&mins    ,
    std::vector  <ssize_t>const&maxs    ,
    std::vector  <ssize_t>const&offset  ,
    std::vector  <ssize_t>const&stride  ,
    std::vector  <size_t >const&ordering,
    std::function<void() >const&body){
  size_t axis = dim;
  if(!ordering.empty())axis = ordering[dim];
  for(ii[axis]=offset[axis];ii[axis]<=maxs[axis]&&ii[axis]>=mins[axis];ii[axis]+=stride[axis]){
    if(dim == 0)body();
    else forLoop(dim-1,ii,mins,maxs,offset,stride,ordering,body);
  }
}

template<typename T>
T measure(std::function<void()>const&f){
  auto startTime = std::chrono::high_resolution_clock::now();
  f();
  auto endTime   = std::chrono::high_resolution_clock::now();
  std::chrono::duration<T>const elapsed = endTime - startTime;
  return elapsed.count();
}

void measureAndPrint(std::string const&what,size_t power,std::function<void(size_t)>const&f){
  std::cout << what << " " << (1<<power) << " : ";
  std::cout << measure<float>(std::bind(f,power));
  std::cout << std::endl;
}



template<size_t DIM,typename...FARGS,typename...ARGS,typename std::enable_if_t<(DIM==0),char> = 0>
inline void forLoopRec(
    std::vector  <size_t              >const&    ,
    void(*f)(FARGS const&...),
    //std::function<void(FARGS const&...)>f,
    ARGS                               const&... );

template<size_t DIM,typename...FARGS,typename...ARGS,typename std::enable_if_t<(DIM >0),char> = 0>
inline void forLoopRec(
    std::vector  <size_t                            >const&   maxVals,
    void(*f)(FARGS const&...),
    //std::function<void(FARGS const&...)>f,
    ARGS                                             const&...args   ){
  for(size_t i=0;i<maxVals.at(DIM-1);++i)
    forLoopRec<DIM-1>(maxVals,f,args...,i);
}

template<size_t DIM,typename...FARGS,typename...ARGS,typename std::enable_if_t<(DIM==0),char>>
inline void forLoopRec(
    std::vector  <size_t              >const&       ,
    void(*f)(FARGS const&...),
    //std::function<void(FARGS const&...)>f,
    ARGS                               const&...args){
  f(args...);
}

template<size_t DIM,typename...FARGS,typename...ARGS,typename std::enable_if_t<(DIM==0),char>>
inline void forLoopRec(
    std::vector<size_t>const&,
    auto(*f)(FARGS const&...)->void,
    ARGS                    const&...args){
  f(args...);
}

void asd(size_t const&i,size_t const&j){
  std::cout << i << " " << j << std::endl;
}

int main(){
  //forLoopRec<2>(std::vector<size_t>{3,3},asd);
  forLoopRec<2>(std::vector<size_t>{3,3},+[](size_t const&i,size_t const&j){std::cout << i << " " << j << std::endl;});
  return 0;

  auto const cppFor1D = [](size_t const power){
    size_t acc = 0;
    size_t const N0 = 1 << power;
    for(size_t i0=0;i0<N0;++i0){
      forBody(acc,i0,i0,i0,i0,i0,i0,i0,i0);
    }
    if(acc == 123)std::cout << std::endl;
    //std::cout << acc << std::endl;
  };
  auto const cppFor2D = [](size_t const power){
    size_t acc = 0;
    size_t const N0 = 1 << (        (power/2));
    size_t const N1 = 1 << (power - (power/2));
    for(size_t i1=0;i1<N1;++i1)
      for(size_t i0=0;i0<N0;++i0)
        forBody(acc,i0,i1,i1,i1,i1,i1,i1,i1);
    if(acc == 123)std::cout << std::endl;
    //std::cout << acc << std::endl;
  };
  auto const cppFor3D = [](size_t const power){
    size_t acc = 0;
    size_t const N0 = 1 << (          (power/3));
    size_t const N1 = 1 << (          (power/3));
    size_t const N2 = 1 << (power - 2*(power/3));
    for(size_t i2=0;i2<N2;++i2)
      for(size_t i1=0;i1<N1;++i1)
        for(size_t i0=0;i0<N0;++i0)
          forBody(acc,i0,i1,i2,i2,i2,i2,i2,i2);
    if(acc == 123)std::cout << std::endl;
    //std::cout << acc << std::endl;
  };
  auto const cppFor4D = [](size_t const power){
    size_t acc = 0;
    size_t const N0 = 1 << (          (power/4));
    size_t const N1 = 1 << (          (power/4));
    size_t const N2 = 1 << (          (power/4));
    size_t const N3 = 1 << (power - 3*(power/4));
    for(size_t i3=0;i3<N3;++i3)
      for(size_t i2=0;i2<N2;++i2)
        for(size_t i1=0;i1<N1;++i1)
          for(size_t i0=0;i0<N0;++i0)
            forBody(acc,i0,i1,i2,i3,i3,i3,i3,i3);
    if(acc == 123)std::cout << std::endl;
    //std::cout << acc << std::endl;
  };
  auto const cppFor5D = [](size_t const power){
    size_t acc = 0;
    size_t const N0 = 1 << (          (power/5));
    size_t const N1 = 1 << (          (power/5));
    size_t const N2 = 1 << (          (power/5));
    size_t const N3 = 1 << (          (power/5));
    size_t const N4 = 1 << (power - 4*(power/5));
    for(size_t i4=0;i4<N4;++i4)
      for(size_t i3=0;i3<N3;++i3)
        for(size_t i2=0;i2<N2;++i2)
          for(size_t i1=0;i1<N1;++i1)
            for(size_t i0=0;i0<N0;++i0)
              forBody(acc,i0,i1,i2,i3,i4,i4,i4,i4);
    if(acc == 123)std::cout << std::endl;
    //std::cout << acc << std::endl;
  };
  auto const cppFor6D = [](size_t const power){
    size_t acc = 0;
    size_t const N0 = 1 << (          (power/6));
    size_t const N1 = 1 << (          (power/6));
    size_t const N2 = 1 << (          (power/6));
    size_t const N3 = 1 << (          (power/6));
    size_t const N4 = 1 << (          (power/6));
    size_t const N5 = 1 << (power - 5*(power/6));
    for(size_t i5=0;i5<N5;++i5)
      for(size_t i4=0;i4<N4;++i4)
        for(size_t i3=0;i3<N3;++i3)
          for(size_t i2=0;i2<N2;++i2)
            for(size_t i1=0;i1<N1;++i1)
              for(size_t i0=0;i0<N0;++i0)
                forBody(acc,i0,i1,i2,i3,i4,i5,i5,i5);
    if(acc == 123)std::cout << std::endl;
    //std::cout << acc << std::endl;
  };
  auto const cppFor7D = [](size_t const power){
    size_t acc = 0;
    size_t const N0 = 1 << (          (power/7));
    size_t const N1 = 1 << (          (power/7));
    size_t const N2 = 1 << (          (power/7));
    size_t const N3 = 1 << (          (power/7));
    size_t const N4 = 1 << (          (power/7));
    size_t const N5 = 1 << (          (power/7));
    size_t const N6 = 1 << (power - 6*(power/7));
    for(size_t i6=0;i6<N6;++i6)
      for(size_t i5=0;i5<N5;++i5)
        for(size_t i4=0;i4<N4;++i4)
          for(size_t i3=0;i3<N3;++i3)
            for(size_t i2=0;i2<N2;++i2)
              for(size_t i1=0;i1<N1;++i1)
                for(size_t i0=0;i0<N0;++i0)
                  forBody(acc,i0,i1,i2,i3,i4,i5,i6,i6);
    if(acc == 123)std::cout << std::endl;
    //std::cout << acc << std::endl;
  };
  auto const cppFor8D = [](size_t const power){
    size_t acc = 0;
    size_t const N0 = 1 << (          (power/8));
    size_t const N1 = 1 << (          (power/8));
    size_t const N2 = 1 << (          (power/8));
    size_t const N3 = 1 << (          (power/8));
    size_t const N4 = 1 << (          (power/8));
    size_t const N5 = 1 << (          (power/8));
    size_t const N6 = 1 << (          (power/8));
    size_t const N7 = 1 << (power - 7*(power/8));
    for(size_t i7=0;i7<N7;++i7)
      for(size_t i6=0;i6<N6;++i6)
        for(size_t i5=0;i5<N5;++i5)
          for(size_t i4=0;i4<N4;++i4)
            for(size_t i3=0;i3<N3;++i3)
              for(size_t i2=0;i2<N2;++i2)
                for(size_t i1=0;i1<N1;++i1)
                  for(size_t i0=0;i0<N0;++i0)
                    forBody(acc,i0,i1,i2,i3,i4,i5,i6,i7);
    if(acc == 123)std::cout << std::endl;
    //std::cout << acc << std::endl;
  };

  measureAndPrint("c++ 1D loop",26,cppFor1D);
  measureAndPrint("c++ 2D loop",26,cppFor2D);
  measureAndPrint("c++ 3D loop",26,cppFor3D);
  measureAndPrint("c++ 4D loop",26,cppFor4D);
  measureAndPrint("c++ 5D loop",26,cppFor5D);
  measureAndPrint("c++ 6D loop",26,cppFor6D);
  measureAndPrint("c++ 7D loop",26,cppFor7D);
  measureAndPrint("c++ 8D loop",26,cppFor8D);
  measureAndPrint("c++ 1D loop",27,cppFor1D);
  measureAndPrint("c++ 2D loop",27,cppFor2D);
  measureAndPrint("c++ 3D loop",27,cppFor3D);
  measureAndPrint("c++ 4D loop",27,cppFor4D);
  measureAndPrint("c++ 5D loop",27,cppFor5D);
  measureAndPrint("c++ 6D loop",27,cppFor6D);
  measureAndPrint("c++ 7D loop",27,cppFor7D);
  measureAndPrint("c++ 8D loop",27,cppFor8D);
  measureAndPrint("c++ 1D loop",28,cppFor1D);
  measureAndPrint("c++ 2D loop",28,cppFor2D);
  measureAndPrint("c++ 3D loop",28,cppFor3D);
  measureAndPrint("c++ 4D loop",28,cppFor4D);
  measureAndPrint("c++ 5D loop",28,cppFor5D);
  measureAndPrint("c++ 6D loop",28,cppFor6D);
  measureAndPrint("c++ 7D loop",28,cppFor7D);
  measureAndPrint("c++ 8D loop",28,cppFor8D);

  return 0;
};
