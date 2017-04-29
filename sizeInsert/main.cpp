#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

int main(){
  std::set<unsigned>a;
  a.insert(a.size());
  a.insert(a.size());
  a.insert(a.size());
  a.insert(a.size());
  for(auto x:a)
    std::cout<<x<<std::endl;

  std::map<std::string,unsigned>b;
  b["a"]=b.size();
  b["b"]=b.size();
  b["c"]=b.size();
  for(auto x:b)
    std::cout<<x.first<<":"<<x.second<<std::endl;

  return 0;
}
