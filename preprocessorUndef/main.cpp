#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#define Index unsigned

#pragma push_macro("Index")
#undef Index

class Index{
  public:
    unsigned id;
};

int main(){
  Index asd;
#pragma pop_macro("Index")
  Index bsd;
  asd.id=123;
  bsd=asd.id;
  std::cout<<bsd<<std::endl;
  return 0;
}
