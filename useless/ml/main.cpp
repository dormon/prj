#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>

class Vars{
  public:
    Vars(size_t i,size_t o,size_t h):inputSize(i),outputSize(o),hiddenSize(h),vars(i+o+h){}
  protected:
    size_t inputSize;
    size_t outputSize;
    size_t hiddenSize;
    std::vector<float>vars;
    std::vector<float>parameters;
};

int main(){
  return 0;
}
