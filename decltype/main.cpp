#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

char getA(){
  return 'a';
}

int main(){
  std::cout<<sizeof(decltype(getA()))<<std::endl;
  return 0;
}
