#include<iostream>

class Var{
  public:
    Var(float a = 0):data(a){}
    ~Var(){
      std::cerr << "Var::~Var()" << std::endl;
    }
    float data = 0;
};

Var operator+(Var const&a,Var const&b){
  return Var(a.data+b.data);
}

void add(Var&c,Var const&a,Var const&b){
  c.data = a.data + b.data;
}

int main(){
  Var a,b;

  //add(c,a,b);
  auto c = a+b;
  return 0;
}


