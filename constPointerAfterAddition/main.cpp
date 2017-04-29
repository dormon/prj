#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>
#include<cassert>
#include<functional>
#include<typeinfo>
#include<typeindex>

int main(){
  float*data = new float[2];
  data[0] = 42;
  data[1] = 100;
  float const*const ptr0 = data;
  auto ptr1 = ptr0 + 1;
  ptr1 = ptr0 + 2;
  std::cout << *ptr1 << std::endl;
  //*ptr1 = 3;
  auto const ptr3 = static_cast<float const*const>(data);
  ptr3 = data;
  std::cout << *ptr3 <<std::endl;
  std::cout << *ptr0 <<std::endl;
  delete[]data;
}
