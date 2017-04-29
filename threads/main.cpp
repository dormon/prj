#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>

int data = 0;

std::mutex mtx;

void thread_a(){
  std::lock_guard<std::mutex>guard(mtx);
  for(int i=0;i<100;++i)
    data += 1;
}

void thread_b(){
  std::lock_guard<std::mutex>guard(mtx);
  for(int i=0;i<100;++i)
    data += 2;
}

int main(){
  std::thread a(thread_a);
  std::thread b(thread_b);
  a.join();
  b.join();

  std::cout<<"main thread ends"<<std::endl;
  std::cout<<data<<std::endl;
  return 0;
}
