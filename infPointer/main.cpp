#include<iostream>

void(*get())(){
  return (void(*)())&get;
}

int main(){
  void*p=&p;
  std::cout<<(void*)**********(size_t**********)p<<std::endl;
  return 0;
}
