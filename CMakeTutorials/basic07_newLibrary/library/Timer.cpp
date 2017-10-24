#include<Timer.h>

Timer::Timer(){
  reset();
}

void Timer::reset(){
  start = std::chrono::high_resolution_clock::now();
  last  = start;
}

float Timer::getTimeFromLast(){
  auto newTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = newTime - last;
  last = newTime;
  return elapsed.count();
}

float Timer::getTimeFromStart(){
  auto newTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = newTime - start;
  this->last = newTime;
  return elapsed.count();
}
