#include <iostream>
#include <vector>
#include <functional>
#include <map>

using Destructor = std::function<void(void*)>;

class Res{
  public:
    Res(){}
    Res(void*d,Destructor const&dst):data(d),destructor(dst){}
    void create(void*d,Destructor const&dst){
      data       = d  ;
      destructor = dst;
    }
    void free(){
      if(data)destructor(data);
      data = nullptr;
      destructor = nullptr;
    }
    bool isFree()const{return data == nullptr;}
    void*data = nullptr;
    Destructor destructor = nullptr;
};

class Vars{
  public:
    template<typename CLASS,typename...ARGS>
    CLASS& add(std::string const&n,ARGS &&... args){
      auto d = new CLASS(args...);
      auto dst = [](void*d){delete (CLASS*)d;};
      if(!empty.empty()){
        auto where =empty.back();
        empty.pop_back();
        resources.at(where).create(d,dst);
        name2Id[n] = where;
        id2Name[where] = n;
        return *(CLASS*)d;
      }else{
        resources.emplace_back(d,dst);
        name2Id[n] = resources.size()-1;
        id2Name[resources.size()-1] = n;
        return *(CLASS*)d;
      }
    }
    void rem(size_t w){
      if(w>=resources.size())return;
      if(resources.at(w).isFree())return;
      resources.at(w).free();
      empty.push_back(w);
    }
    std::vector<Res>resources;
    std::vector<size_t>empty;
    std::map<std::string,size_t>name2Id;
    std::map<size_t,std::string>id2Name;
};

int main()
{
  return 0;
}
