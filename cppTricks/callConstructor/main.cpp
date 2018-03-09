#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class A{
  public:
    A(float a,std::string const&b):dataa(a),datab(b){
      std::cout << "A::A()" << std::endl;
    }
    float dataa;
    std::string datab;
    ~A(){
      std::cout << "~A::A()" << std::endl;
    }
};

int main(){
  auto v = new  A(1,"");
  delete v;

  void* ptr = new uint8_t[sizeof(A)];
  new(ptr)A(1,"");


  static_cast<A*>(ptr)->~A();
  delete[](uint8_t*)ptr;

  return 0;
}





