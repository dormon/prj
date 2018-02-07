#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<sstream>

void print(std::vector<std::reference_wrapper<std::vector<float>>>const&refData){
  for(auto &r:refData){
    for(auto &x:r.get())
      x+=1000.f;
  }
}

std::function<void()>createCommand(std::map<std::string,std::vector<float>>const&data){
  std::vector<std::reference_wrapper<std::vector<float>>>refData;
  for(auto const&a:data)
    refData.emplace_back(a.second);
  return std::bind(print,refData);
}

int main(){
  std::map<std::string,std::vector<float>>data;
  data["a"] = {1.f,2.f,3.f};
  data["b"] = {5.f,10.f};
  data["c"] = {};
  data["d"] = {0.1f,-23.f,12.f,32.f};

  std::vector<std::reference_wrapper<std::vector<float>>>refData;
  refData.emplace_back(data.at("a"));
  refData.emplace_back(data.at("b"));
  refData.emplace_back(data.at("c"));
  refData.emplace_back(data.at("d"));

  data.at("c").push_back(13.1313f);


  for(auto const&x:data){
    std::cout << x.first << " : ";
    for(auto const&y:x.second)
      std::cout << y << " ";
    std::cout << std::endl;
  }

  return 0;
};
