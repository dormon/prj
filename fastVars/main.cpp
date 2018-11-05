#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>

using FCE = int(*)(int,int);

int add(int a,int b){
  return a+b;
}

class Table{
  public:
    std::function<int(int,int)> fce = [&](int a,int b){
      std::cerr << "start" << std::endl;
      fce = add;
      return add(a,b);
    };
};

using Destructor = std::function<void(void*)>;

class Resource{
  public:
    Resource(){}
    Resource(void*d,Destructor const&dst):data(d),destructor(dst){}
    ~Resource(){destructor(data);destructor = nullptr;data = nullptr;time = 1;}
    void*reCreate(void*d,Destructor const&dst){
      destructor(data);
      data = d;
      destructor = dst;
      time++;
      return d;
    }
    void*data = nullptr;
    Destructor destructor = nullptr;
    size_t time = 1;
};

class Vars{
  public:
    template<typename CLASS,typename...ARGS>
    CLASS*add(std::string const&n,ARGS&&...args){
      auto d = new CLASS(args...);
      auto dst = [](void*v){delete (CLASS*)v;};
      return add(d,dst);
    }
    void*add(std::string const&n,void*d,Destructor const&dst){
      auto r = std::make_shared<Resource>(d,dst);
      resources[n] = r;
      return d;
    }
    template<typename CLASS>
    CLASS*get(std::string const&n)const{
      return (CLASS*)resources.at(n)->data;
    }
    template<typename CLASS,typename...ARGS>
    CLASS*reCreate(std::string const&n,ARGS&&...args){
      if(resources.count(n) == 0)
        return add<CLASS>(n,args...);
      auto d = new CLASS(args...);
      auto dst = [](void*v){delete (CLASS*)v;};
      return (CLASS*)resources.at(n)->reCreate(d,dst);
    }
    std::shared_ptr<Resource>getResource(std::string const&n)const{
      return resources.at(n);
    }
    std::map<std::string,std::shared_ptr<Resource>>resources;
};

void computeNofLevels(Vars&vars){
  static std::function<bool(Vars&)>notChanged = [&](Vars&v){
    vars.add<decltype(notChanged)*>("computeNofLevelsNotChanged",&notChanged);
    return false;
  };
  if(notChanged(vars))return;
  static size_t widthTime   = 0;
  static size_t heightTime  = 0;
  static size_t warpTime    = 0;
  static size_t*widthRTime  = nullptr;
  static size_t*heightRTime = nullptr;
  static size_t*warpRTime   = nullptr;

  auto widthR  = vars.getResource("width" );
  auto heightR = vars.getResource("height");
  auto warpR   = vars.getResource("warp"  );
  if(
      widthTime  >= *widthRTime  &&
      heightTime >= *heightRTime &&
      warpTime   >= *warpRTime
     )
    return;
  widthTime  = widthR ->time;
  heightTime = heightR->time;
  warpTime   = warpR  ->time;



  auto const width  = *(size_t*)widthR ->data;
  auto const height = *(size_t*)heightR->data;
  auto const warp   = *(size_t*)warpR  ->data;

  auto pixs = width * height;
  size_t n = 0;
  while(pixs > 0){
    n++;
    pixs /= warp;
  }

  vars.reCreate<size_t>("nofLevels",n);
}

int main(){

  Table a;
  std::cerr << a.fce(10,23) << std::endl;
  std::cerr << a.fce(132,23) << std::endl;


  return 0;
}
