#include <iostream>
#include <plane.h>
#include <stdlib.h>
#include <chrono>
#include <cassert>
#include <functional>
#include <planeSink.h>

template<typename T = float>
T measure(std::function<void()>const&f){
  auto const start = std::chrono::high_resolution_clock::now();
  f();
  auto const end   = std::chrono::high_resolution_clock::now();
  std::chrono::duration<T>const elapsed = end - start;
  return elapsed.count();
}

float randomf(){
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

glm::vec3 generate(){
  return glm::vec3(randomf(),randomf(),randomf());
}

int main(int,char*[]){
  size_t N=500;
  glm::vec3*points = new glm::vec3[N];
  for(size_t i=0;i<N;++i)
    points[i] = generate();


  auto const timeBase = measure([&](){
  for(size_t i=0;i<N-2;++i)
    for(size_t j=i+1;j<N-1;++j)
      for(size_t k=j+i;k<N;++k)
        planeSink(plane(points[i],points[j],points[k]));
  });

  std::cerr << "timeBase: " << timeBase << std::endl;

  auto const timeBase2 = measure([&](){
  for(size_t i=0;i<N-2;++i)
    for(size_t j=i+1;j<N-1;++j)
      for(size_t k=j+i;k<N;++k)
        planeSink(plane2(points[i],points[j],points[k]));
  });

  std::cerr << "timeBase2: " << timeBase2 << std::endl;

  auto const timeNonDeter = measure([&](){
  for(size_t i=0;i<N-2;++i)
    for(size_t j=i+1;j<N-1;++j)
      for(size_t k=j+i;k<N;++k)
        planeSink(planeSorted(points[i],points[j],points[k]));
  });

  std::cerr << "timeNonDeter: " << timeNonDeter << std::endl;

  delete[]points;
  return 0;
}
