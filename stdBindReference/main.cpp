#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<functional>

struct Tensor{
  float data;
};

void print(Tensor const&t){
  std::cout << t.data << std::endl;
}

int main(){
  std::map<std::string,Tensor>data;
  data["input"] = {10};

  auto const fce = std::bind(print,std::ref<Tensor>(data.at("input")));

  data["input"].data = 100;
  fce();
  return 0;
}
