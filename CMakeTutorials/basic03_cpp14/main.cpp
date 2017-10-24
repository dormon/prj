#include<iostream>

#include<MyClass.h>

constexpr int getValue(){
  return 23;
}

int main(int,char*[]){
  mylib::MyClass c;
  c.a = 1;
  return 0;
}
