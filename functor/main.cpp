#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class While{
  public:
    void operator()(){
      std::cout<<"ahoj"<<std::endl;
    }
};


int main(){
  While asd;
  asd();
  return 0;
}
