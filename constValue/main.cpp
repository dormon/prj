#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>

int main(){
  int a;(void)a;
  int*a0 = &a;
  const int b = 3;(void)b;
  //a=7;// - a is constant
  const int*c = &a;(void)c;
  c = &b;
  //*c = 10;// - c points to constant
  int*const d = &a;(void)d;
  *d = 10;
  //*d = &a;// - d is constant pointer
  int&e = a;(void)e;
  e=10;
  int const&f = a;(void)f;
  //f=12;// - f is reference to constant
  //int &const g = a;(void)g;// - there is no such thing as constant reference
  int const*const h = &a;(void)h;// constant pointer to constant
  int *&i = a0;(void)i;//reference to pointer
  *i = 1000;
  //int &* j = e;//pointer to reference is not possible

  std::cout<<a<<std::endl;
  return 0;
}
