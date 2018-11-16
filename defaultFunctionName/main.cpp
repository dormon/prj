#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>

std::string get(size_t i,std::string const&n = __FUNCTION__){
  if(i == 0)return "ahoj";
  return n;
}

int main(int,char*[]) {
  std::cerr << "w: " << get(0) << std::endl;
  std::cerr << "w: " << get(1) << std::endl;

  return 0;
}
