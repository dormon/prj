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

class WTF{
  public:
    WTF(){
      std::cout<<"WTF::WTF()"<<std::endl;
    }
    ~WTF(){
      std::cout<<"WTF::~WTF()"<<std::endl;
    }
    void method1(){
      std::cout<<"WTF::method1()"<<std::endl;
    }
    void method2(){
      std::cout<<"WTF::method2()"<<std::endl;
    }
};

WTF        get        (std::shared_ptr<WTF>const&w){return *w ;}
WTF&       getRef     (std::shared_ptr<WTF>const&w){return *w ;}
WTF const& getConstRef(std::shared_ptr<WTF>const&w){return *w ;}
WTF*       getPtr     (std::shared_ptr<WTF>const&w){return &*w;}

int main(){
  auto r = std::make_shared<WTF>();
  using MemPtr = void(WTF::*)();
  MemPtr ptr = &WTF::method1;

  ((getRef(r)).*(ptr))();


  return 0;
}
