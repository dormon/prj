#include<iostream>

struct A{
  float a = 1.32f;
  char b = 'a';
  int c = 32;
};

struct B{
  short b = 3;
  char  x = 'b';
};

union U{
  A a;
  B b;
};


int main(int argc,char*argv[]){
  U u;

  std::cerr << "A: " << u.a.a << std::endl;
  std::cerr << "B: " << u.b.b << std::endl;

  return 0;
}
