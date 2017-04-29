#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>

class Blb{
  public:
    static void a(){
      std::cout<<__FUNCTION__<<std::endl;
    }
};

int main(){
  Blb::a();
  return 0;
}





