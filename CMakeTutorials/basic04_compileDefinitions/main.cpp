#include<iostream>

#include<MyClass.h>

constexpr int getValue(){
  return 23;
}

#ifndef NOF_STUDENTS
#define NOF_STUDENTS 0
#endif//NOF_STUDENTS

int main(int,char*[]){
  mylib::MyClass c;
  c.a = 1;
  std::cout << "number of students: " << NOF_STUDENTS << std::endl;
  return 0;
}
