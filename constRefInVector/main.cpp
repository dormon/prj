#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<utility>
#include<cassert>
#include<limits>
#include<algorithm>
#include<array>

class Class{
  public:
    int a;
    int const&getA()const{
      return a;
    }
    std::vector<int>b;
    int const&getB(size_t x)const{
      return this->b.at(x);
    }
    std::vector<std::vector<int>>c;
    int const&getC(size_t x,size_t y)const{
      return this->c.at(x).at(y);
    }
    int const**d;
    int const* const&getD(size_t x)const{
      return d[x];
    }
    std::vector<int const*>e;
    int const* const&getE(size_t x)const{
      return e[x];
    }

    void add(){
      
    }
      /*
    std::vector<std::vector<int>>d;
    const int* const&getD(size_t x)const{
      return this->d.data()[x].data();
    }*/
};

int main(){
  Class cls;
  cls.getA();
  cls.getB(0);
  cls.getC(0,0);
  cls.getD(0);
  return 0;
}
