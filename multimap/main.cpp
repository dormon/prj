#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

int main(){
  std::multimap<const char*,int>mymm;
  mymm.insert(std::pair<const char*,int>("asd",12));
  mymm.insert(std::pair<const char*,int>("asd",13));
  mymm.insert(std::pair<const char*,int>("asd",14));
  mymm.insert(std::pair<const char*,int>("asd",14));
  mymm.insert(std::pair<const char*,int>("asd",14));
  std::cout<<mymm.count("asd")<<std::endl;
  return 0;
}
