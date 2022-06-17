#include<iostream>
#include<functional>
#include<chrono>

float measure(std::function<void()>const&f){
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto const newTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float>const elapsed = newTime - start;
  return elapsed.count();
}

void linear(uint32_t n){
  auto d=malloc(sizeof(uint32_t)*1);
  for(uint32_t i=2;i<=n;++i)
    d=realloc(d,sizeof(uint32_t)*i);
  free(d);
}

void exponential(uint32_t n){
  auto d=malloc(sizeof(uint32_t)*1);
  uint32_t reserved=1;
  for(uint32_t i=2;i<=n;++i){
    if(i>reserved){
      reserved*=2;
      d=realloc(d,sizeof(uint32_t)*reserved);
    }
  }
  free(d);
}

int main(){
  uint32_t N=1024*1024*16;
  std::cerr << "exponetial: " << measure([&](){exponential(N);}) << std::endl;
  std::cerr << "linear    : " << measure([&](){linear     (N);}) << std::endl;
  return 0;
}
