#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>



int main(){
  std::string a="\"ahoj\"";
  std::cout<<a.substr(1,a.size()-2)<<std::endl;
  return 0;
}
