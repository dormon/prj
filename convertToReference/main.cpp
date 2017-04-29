#include<iostream>
#include<vector>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>
#include<string>
#include<type_traits>
#include<sstream>
#include<typeinfo>
#include<cassert>

#define ___ std::cerr<<__FILE__<<" "<<__LINE__<<std::endl

template<typename INNERTYPE>
class Resource{
  public:
    void*data;
    Resource(){
      assert(this!=nullptr);
      this->data = new uint8_t[sizeof(INNERTYPE)];
      assert(this->data!=nullptr);
    }
    ~Resource(){
      assert(this!=nullptr);
      delete[](uint8_t*)this->data;
      this->data = nullptr;
    }

    template<typename T>
    operator T&(){
      assert(this!=nullptr);
      return *(T*)this->data;
    }


};

int main(){
  auto a = new Resource<int32_t>();
  (int32_t&)*a = 100;
  std::cout<<(int32_t const&)*a<<std::endl;
  delete a;
  return 0;
}
