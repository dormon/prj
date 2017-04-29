#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>


template<typename KEY,typename VALUE>
class bimap: public std::map<KEY,VALUE>, std::map<VALUE,KEY>{
  public:

};

int main(){
  bimap<int,std::string>bm;
  return 0;
}
