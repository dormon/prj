#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

class Base{
  public:
    float a;
    Base(float const&a = 0.f):a(a){}  
    Base(Base const&) = delete;
};

class Child: public Base{
  public:
    int s;
    Child(int s = 0):s(s){}
};

int main(){
  Child a;
  //copy constructor is deleted
  //Child b = a;
  return 0;
}


