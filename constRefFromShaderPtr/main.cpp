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
    int*a;
    WTF(){
      std::cout<<"WTF::WTF()"<<std::endl;
    }
    ~WTF(){
      std::cout<<"WTF::~WTF()"<<std::endl;
    }
};

WTF        get        (std::shared_ptr<WTF>const&w){return *w ;}
WTF&       getRef     (std::shared_ptr<WTF>const&w){return *w ;}
WTF const& getConstRef(std::shared_ptr<WTF>const&w){return *w ;}
WTF*       getPtr     (std::shared_ptr<WTF>const&w){return &*w;}

int main(){
  auto r = std::make_shared<WTF>();
  auto a=getConstRef(r);
  return 0;
}
