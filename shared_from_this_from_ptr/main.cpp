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
#include<utility>
#include<cassert>

class WTF: public std::enable_shared_from_this<WTF>{
  public:
    int a;
    void printThis(){
      std::cout<<this<<std::endl;
    };
    WTF*getThis(){return this;}
};

int main(){
  auto r = std::make_shared<WTF>();
  auto p = r->getThis();
  auto rr = p->shared_from_this();
  std::cout<<r<<std::endl;
  std::cout<<p<<std::endl;
  std::cout<<rr<<std::endl;
  assert(r==rr);

  return 0;
}
