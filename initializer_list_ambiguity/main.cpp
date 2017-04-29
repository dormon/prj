#include<iostream>
#include<vector>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>

void fce(std::vector<std::string>const&a){
  (void)a;
}

void fce(std::vector<int>const&a){
  (void)a;
}

int main(){
  fce({"asd","asd"});
  return 0;
}
