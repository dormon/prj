#include<iostream>

#include<Timer.h>

int main(int,char*[]){
  Timer timer;

  timer.reset();
  for(size_t i=0;i<40000000;++i);
  std::cout << timer.getTimeFromLast() << std::endl;


  return 0;
}
