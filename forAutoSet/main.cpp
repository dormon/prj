#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>


int main(){
  std::vector<std::set<std::string>>a;
  a.push_back(std::set<std::string>());
  a.push_back(std::set<std::string>());
  a.push_back(std::set<std::string>());
  a[2].insert("B");
  a[2].insert("AA");
  a[2].insert("BB");
  a[2].insert("CCC");
  auto last=a[a.size()-1];
  last.insert("AAA");
  last.insert("A");
  std::string name="";
  for(auto x:last)
    name+=" "+x;
  std::cout<<"#"<<name<<"#"<<std::endl;

  return 0;
}
