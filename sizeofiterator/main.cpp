#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<geCore/TypeRegister.h>

class Key{
  public:
    float a;
    int b;
    char c;
};

int main(){
  std::cout<<sizeof(std::map<std::string,float>::iterator)<<std::endl;
  std::cout<<sizeof(std::map<std::vector<std::string>,std::vector<std::string>>::iterator)<<std::endl;
  std::cout<<sizeof(std::map<Key,std::vector<std::string>>::iterator)<<std::endl;
  return 0;
}


