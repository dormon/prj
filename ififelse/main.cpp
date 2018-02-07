#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>



int main(){
  int a=30;
  if(a<10)
    if(a>4)
      a+=32;
    else
      a+=10;
  std::cout << a;
  return 0;
}
