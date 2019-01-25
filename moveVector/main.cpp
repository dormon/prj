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
#include <cstdlib>
#include <memory>

class What{
  public:
    What(What &&w) {
      std::cerr << "move" << std::endl;
    }
    What(What const&w)
      /*
      = delete;
      */
    //*
    {
      std::cerr << "copy" << std::endl;
    }// */
    ~What(){
      std::cerr << "destructor" << std::endl;
    }
    What(){
      std::cerr << "constructor" << std::endl;
    }
    std::string name;
    float a;
};



int main(int,char*[]) {
  std::vector<std::unique_ptr<What>>a;
  a.reserve(10);
  std::cerr << __LINE__ << std::endl;
  a.emplace_back(std::make_unique<What>());
  std::cerr << __LINE__ << std::endl;
  a.emplace_back(std::make_unique<What>());
  std::cerr << __LINE__ << std::endl;
  a.emplace_back(std::make_unique<What>());
  std::cerr << __LINE__ << std::endl;
  return 0;

}
