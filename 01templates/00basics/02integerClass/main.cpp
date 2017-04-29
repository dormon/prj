#include<iostream>
#include<cassert>
#include<memory>
#include<set>
#include<vector>
#include<map>

template<size_t DIMENSION>
class Vector{
  public:
    Vector(){
      assert(this != nullptr);
      for(size_t i = 0; i < DIMENSION; ++i)
        this->_data[i] = 0.f;
    }
    void set(size_t const&index,float const&value){
      assert(this != nullptr);
      assert(index < DIMENSION);
      this->_data[index] = value;
    }
    float get(size_t const&index)const{
      assert(this != nullptr);
      assert(index < DIMENSION);
      return this->_data[index];
    }
  protected:
    float _data[DIMENSION];
};

int main(){
  Vector<10>a;
  a.set(2,32.f);
  return 0;
}
