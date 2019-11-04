#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include "magic_enum.hpp"

enum Test{
  POINT,
  LINE,
  FILL
};

int main(){
  Test a = FILL;
  std::cerr << magic_enum::enum_name(a) << std::endl;
  return 0;
}





