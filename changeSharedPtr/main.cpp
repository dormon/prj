#include<iostream>
#include<memory>

int main(){
  auto p = std::shared_ptr<float>(new float[10],[](float*const&ptr){delete[]ptr;});
  auto b = p;
  p.reset(new float[100],[](float*const&ptr){delete[]ptr;});
  (&*b)[0] = 7.7f;
  (&*p)[0] = 13.3f;

  std::cout << (&*b)[0] << std::endl;
  std::cout << (&*p)[0] << std::endl;
  return 0;
}

