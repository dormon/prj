#include <iostream>

class Var{
  public:
    Var(float a = 0):data(a){}
    ~Var(){
      std::cerr << "Var::~Var()" << std::endl;
    }

    float data = 0;
    
    Var& operator=(Var&& other)
    {
      std::cerr << "move assign" << std::endl;
      this->data = other.data;
      return *this;
    }

    Var& operator=(Var& other)
    {
      std::cerr << "copy assign" << std::endl;
      this->data = other.data;
      return *this;
    }

    Var operator+(const Var& a){
      return Var(this->data + a.data);
    }
    


};

void add(Var&c,Var const&a,Var const&b){
  c.data = a.data + b.data;
}

//Var& operator+(Var& a,Var const&b){
//  a.data+=b.data;
//  return a;
//}

void print(Var const&a);

int main(){
  Var a(1),b(2),c;
  c.data = 3;
  print(a);
  print(c);
  c = std::move(a+b);
  print(a);
  print(c);

  return 0;
}
