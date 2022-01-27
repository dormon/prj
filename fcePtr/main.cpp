#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstring>
#include <functional>
#include <typeinfo>
#include <set>
#include <chrono>
#include <stack>
#include <thread>
#include <cassert>

using PTR  = void (*)(int);
using PTR2 = void (&)(int);

void print(int a){  std::cerr << a << std::endl;}
void print2(int a){  std::cerr << a << std::endl;}

int main(){
  std::function<void(int)>ptr;
  PTR p = print;
  p(32);
  (*p)(123);

  return 0;
}
