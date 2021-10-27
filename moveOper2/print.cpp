#include <iostream>

class Var{
  public:
    float data;
    Var(float a = 0);
    ~Var();
    Var& operator=(Var&& other);
    Var& operator=(Var& other);
    Var& operator+(const Var& a);
};

void print(Var const&a){
  std::cerr << a.data << std::endl;
}
