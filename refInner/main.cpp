#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Accessor{
  public:
    void* _data = nullptr;
    template<typename T>
    operator T&(){
      return *((T*)this->_data);
    }
    template<typename T>
    operator T*(){
      return (T*)this->_data;
    }
    template<typename T>
    operator T**(){
      return (T**)&this->_data;
    }

};

class Data{
  public:
    float _a = 1.1f;
    float _b = 2.2f;
    float _c = 3.3f;
};

void alloc(Data**ptr){
  *ptr=new Data();
}

int main(){
  Accessor a;
  alloc(a);
  ((Data&)a)._a=42.42f;
  ((Data*)a)->_c=17.17f;

  std::cout<<((Data*)a._data)->_a<<std::endl;
  std::cout<<((Data*)a._data)->_b<<std::endl;
  std::cout<<((Data*)a._data)->_c<<std::endl;
  return 0;
}
