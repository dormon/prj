#include<iostream>
#pragma GCC system_header
#define call(fce,i,...)\
  fce(i,##__VA_ARGS__)
 

void test0(int){}
void test1(int,float){}


int main()
{
  call(test0,1);
  call(test1,1,2.f);

  return 0;
}
