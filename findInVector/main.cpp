#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<glm/glm.hpp>
#include<algorithm>

int main(){
  std::vector<std::string>args = {"asd","b","a"};
  auto ii = std::find(args.begin(),args.end(),"b");
  std::cout<<*ii<<std::endl;

  return 0;
}
