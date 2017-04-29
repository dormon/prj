#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>
#include<cassert>
#include<functional>
#include<typeinfo>
#include<typeindex>

struct Base{
  float* data = nullptr;
  size_t size = 0      ;
  Base(size_t const&s):size(s){
    assert(this!=nullptr);
    this->data = new float[this->size];
    assert(this->data!=nullptr);
  }
  ~Base(){
    assert(this!=nullptr);
    delete[]this->data;
    this->data = nullptr;
  }
};

int main(){
  Base a(10);
  return 0;
}
