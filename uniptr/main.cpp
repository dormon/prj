#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

class P{
  public:
    float*data = nullptr;
    P(){
      std::cout<<"P::P()"<<std::endl;
      this->data = new float[100];
    }
    ~P(){
      std::cout<<"P::~P()"<<std::endl;
      delete[]this->data;
    }
};

int main(){

  std::unique_ptr<int>p0=std::unique_ptr<int>(new int);
  std::shared_ptr<int>p1=std::unique_ptr<int>(new int);
  std::weak_ptr<int>p2 = p1;
  *p0=32;

  auto uptr = std::unique_ptr<P>(new P);
  (void)uptr;

  return 0;
}
