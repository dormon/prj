#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstring>
#include <functional>
#include <typeinfo>
#include <set>
#include <chrono>
#include <stack>
#include <thread>

using namespace std;

template<class...>struct types{using type=types;};
template<class T>struct tag{using type=T;};
template<class Tag>using type_t=typename Tag::type;

template<typename CLASS>
uint8_t*allocateVar(){
  return new uint8_t[sizeof(CLASS)];
}

template<typename CLASS,typename...ARGS>
void callConstructor(uint8_t*ptr,ARGS&&...args){
  new(reinterpret_cast<CLASS*>(ptr))CLASS(args...);
}

void deallocateVar(uint8_t*p){
  delete [] p;
}


using Destructor       = std::function<void(void*)>;

template<typename CLASS>
Destructor getDestructor(){
  return [](void*p){reinterpret_cast<CLASS*>(p)->~CLASS();};
}

template<typename CLASS>
class Wrapper{
  public:
    Wrapper(CLASS&&d):data(d){}
    operator CLASS const&()const{return data;}
    operator CLASS      &()     {return data;}
  protected:
    CLASS data;
};

class Var{
  public:
    using Type             = std::type_info            ;
    using SignalCallback   = std::function<void()>     ;
    using Stamp            = size_t                    ;
    Var(
        uint8_t*   const data      ,
        Destructor const&destructor,
        Type       const&type      )
      :data      (data      )
      ,destructor(destructor)
      ,type      (type      )
      {}
    template<typename CLASS,typename...ARGS>
    Var(tag<CLASS>,ARGS&&...args):Var(allocateVar<CLASS>(),getDestructor<CLASS>(),typeid(CLASS)){
      callConstructor<CLASS>(data,args...);
    }
    ~Var(){
      if(!destructor || !data)return;
      destructor(data);
      deallocateVar(data);
    }
    template<typename CLASS>
      Var&operator=(CLASS&&value){
        get<CLASS>() = value;
        updateStamp();
        return*this;
      }
    template<typename CLASS,typename...ARGS>
      CLASS&reCreate(ARGS&&...args){
        destructor(data);

        if(typeid(CLASS) != type){
          deallocateVar(data);
          auto const oldStamp = stamp();
          data = allocateVar<CLASS>();
          new(this)Var(data,getDestructor<CLASS>(),typeid(CLASS));
          timeStamp = oldStamp;
        }else
          callConstructor<CLASS>(data,args...);

        updateStamp();
        return get<CLASS>();
      }
    template<typename CLASS>
      operator CLASS const&()const{
        return get<CLASS>();
      }
    template<typename CLASS>
      operator CLASS&(){
        return get<CLASS>();
      }
    void updateStamp(){
      ++timeStamp;
    }
    void signal(){
      if(signalIsEmpty())return;
      signalCallback();
    }
    template<typename CLASS>
    CLASS&get(){
      return *reinterpret_cast<CLASS*>(data);
    }
    template<typename CLASS>
    CLASS const&get()const{
      return *reinterpret_cast<CLASS*>(data);
    }
    void seSignal(SignalCallback const&callback){
      signalCallback = callback;
    }
    SignalCallback getOnChange()const{
      return signalCallback;
    }
    template<typename T>
    bool is()const{return typeid(T) == type;}
    Stamp stamp()const{return timeStamp;}
  protected:
    bool signalIsEmpty()const{
      return signalCallback == nullptr;
    }
    uint8_t*               data             = nullptr;
    Destructor             destructor       = nullptr;
    Stamp                  timeStamp        = 0      ;
    Type             const&type                      ;
    SignalCallback         signalCallback   = nullptr;

};

template<typename T,typename...ARGS>
Var var(ARGS&&...args){
  auto ptr = allocateVar<T>();
  callConstructor<T>(ptr,args...);
  return Var(ptr,getDestructor<T>(),typeid(T));
}


class Tag{
  public:
    std::string name;
};

std::vector<std::string>split(std::string txt,std::string const&splitter){
  if(txt == "")return {};
  std::vector<std::string>res;

  size_t pos;
  do{
    pos = txt.find(splitter);
    if(pos == std::string::npos){
      res.push_back(txt);
      break;
    }else{
      res.push_back(txt.substr(0,pos));
      txt = txt.substr(pos+splitter.size());
    }
  }while(true);

  return res;
}

class Vars{
  public:
    using Tag          = std::string   ;
    using VarName      = std::string   ;
    using VarSelection = std::set<Var*>;
    using Tags         = std::set<Tag >;
    template<typename CLASS,typename...ARGS>
    CLASS&addOrGet(std::string const&name,ARGS&&...args){
      return *std::get<0>(name2Var.emplace(name,args...));
    }
    Var*get(std::string const&name){
      auto it = name2Var.find(name);
      if(it == std::end(name2Var))return nullptr;
      record(it->second);
      return it->second;
    }
    void remove(std::string const&name){
      name2Var.erase(name);
      removeTag(name,var2Tags.at(name));
      var2Tags.erase(name);
    }
    template<typename CLASS,typename...ARGS>
    CLASS&reCreate(std::string const&name,ARGS&&...args){
      auto res = name2Var.emplace(std::piecewise_construct,
          std::forward_as_tuple(name),
          std::forward_as_tuple(tag<CLASS>{},args...));
      auto firstAdd = std::get<1>(res);
      Var&var      = std::get<0>(res)->second;
      if(firstAdd)return var;
      return var.reCreate<CLASS>(args...);
    }
    Tags tags(std::string const&var)const{
      return var2Tags.at(var);
    }
    bool varInTags(std::string const&var,Tags const&tags)const{
      auto const&varTags = var2Tags.at(var);
      for(auto const&tag:tags)
        if(!varTags.count(tag))return false;
      return true;
    }
    void foreach(std::function<void(Var*)>const&fce,Tags const&tags){
      for(auto &var:name2Var){
        if(varInTags(var.first,tags))
          fce(&var.second);
      }
    }
    void addTag(std::string const&varName,Tags const&tags){
      auto&var = name2Var.at(varName);
      for(auto const&tag:tags){
        tagToVars[tag].insert(&var);
        var2Tags[varName].insert(tag);
      }
    }
    void removeTag(std::string const&varName,Tags const&tags){
      auto&var = name2Var.at(varName);
      for(auto const&tag:tags){
        tagToVars.at(tag).erase(&var);
        var2Tags[varName].erase(tag);
      }
    }
    bool notChanged(void*fce,char const*fceName){
      auto res = fceData.emplace(std::piecewise_construct,
          std::forward_as_tuple(fce),
          std::forward_as_tuple(*this,fceName));
      auto firstAdd = std::get<1>(res);
      auto&data = std::get<0>(res)->second;
      data.shouldRecord = firstAdd;
      if(firstAdd){
        callstack.at(callstackTop++) = &data;
        return false;
      }
      auto notChanged = std::get<0>(res)->second.notChanged();
      if(notChanged)
        callstack.at(callstackTop++) = &data;


      return notChanged;
    }
    void exitFunction(){
    }
    class Caller{
      public:
        Caller(Vars&vars):vars(vars){
        }
        ~Caller(){
          vars.exitFunction();
        }
        Vars&vars;
    };
  protected:
    void record(Var*var){
      for(size_t i=0;i<callstackTop;++i)
        callstack.at(i)->record(var);
    }
    bool shouldRecord = false;
    class FunctionData{
      public:
        bool shouldRecord = true;
        FunctionData(Vars&vars,char const*fceName):name(fceName){
          //auto ins = split(std::string(conf),",");
          //for(auto const&i:ins){
          //  auto var = vars.get(i);
          //  throwIfVarDoesNotExists(var,i);
          //  inputs.emplace_back(var);
          //  seenStamps.emplace_back(var->stamp());
          //}
        }
        void throwIfVarDoesNotExists(Var*var,std::string const&n){
          if(var)return;
          throw std::runtime_error("var: \""+n+"\" does not exists!");
        }
        bool notChanged(){
          if(signalized){
            signalized = false;
            updateSeenStamps(std::begin(inputs));
            return false;
          }
          bool notCh = true;
          auto it = std::begin(inputs);
          for(;it != std::end(inputs);it++){
            auto&var = std::get<0>(*it);
            auto&stamp = std::get<1>(*it);
            if(var->stamp() > stamp){
              notCh = false;
              break;
            }
          }

          updateSeenStamps(it);
          return notCh;
        }
        void record(Var*var){
          if(shouldRecord)
            recordedInputs.insert(var);
        }
        std::set<std::tuple<Var*,size_t>>inputs;
        void updateSeenStamps(decltype(inputs)::iterator it){
          //for(;it!=std::end(inputs);++it)
          //  std::get<1>(*it) = std::get<0>(*it)->stamp();
        }
        char const*name;
        bool signalized = false;
        std::set<Var*>recordedInputs;
    };
    std::vector<FunctionData*>callstack;
    size_t callstackTop = 0;
    std::map<void*  ,FunctionData>fceData  ;
    std::map<Tag    ,VarSelection>tagToVars;
    std::map<VarName,Var         >name2Var ;
    std::map<VarName,Tags        >var2Tags ;
};

#define VARS_PROLOGUE(fce)\
  if(vars.notChanged((void*)fce,__PRETTY_FUNCTION__))return;\
  Vars::Caller{vars}

void createProgram(Vars&vars){
  VARS_PROLOGUE(createProgram);
  vars.reCreate<float>("prg",0.f);
}

class Ctx{
  public:
    bool notChanged(void*f,char const*fn){
      auto it = fces.emplace(f,fn);
      if(std::get<1>(it)){
        return true;
      }
      auto&fce = std::get<0>(it)->second;
      return false;
    }
    struct Log{
      Log(Ctx&c):c(c){}
      ~Log(){}
      Ctx&c;
    };
    struct Fce{
      Fce(char const*fn):fn(fn){}
      char const*fn;
    };

    using Des = std::function<void(uint8_t*p)>;
    template<typename C>auto dest(){return [](uint8_t*p){((C*)p)->~C();};}
    template<typename C>
    uint8_t*alloc(){return new uint8_t[sizeof(C)];}
    void free(uint8_t*d){delete[]d;}
    struct Res{
      Res(uint8_t*d,size_t s,Des&&ds):data(d),stamp(s),ds(ds){}
      ~Res(){ds(data);delete[]data;}
      uint8_t*data;
      size_t stamp;
      Des ds;
    };
    template<typename C,typename...ARGS>
    C&addorGet(std::string const&);
    std::unordered_map<void*,Fce>fces;
    std::unordered_map<std::string,Res>vars;
};

void work(Ctx&ctx){
  if(ctx.notChanged((void*)work,__PRETTY_FUNCTION__))return;
  Ctx::Log{ctx};

}

class A{
  public:
    A(){std::cerr << "A::A()" << std::endl;}
    A(A const&){std::cerr << "A::A(A const&)" << std::endl;}
    A(A&&){std::cerr << "A::A(A&&)" << std::endl;}
    ~A(){std::cerr << "A::~A()" << std::endl;}
};

template<typename T>
class MyVec{
  public:
    template<typename...ARGS>
    void emplace_back(ARGS&&...args){
      resizeIfNeeded();

      new(((T*)data)+size)T(args...);

      size++;
    }
    ~MyVec(){
      for(size_t i=0;i<size;++i)
        (((T*)data)+i)->~T();
      free(data);
    }
  protected:
    void resizeIfNeeded(){
      if(data == nullptr){
        data = malloc(sizeof(T));
        reserved = 1;
      }else{
        if(size >= reserved)
        reserved *= 2;
        data = realloc(data,sizeof(T)*reserved);
      }
    }
    
    void*  data = nullptr;
    size_t size = 0      ;
    size_t reserved = 0  ;
};

auto measure(std::function<void()>const&f){
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> dur = end-start;
  auto t = dur.count();
  return t;
}

class T{
  public:
    template<typename C>
      T(tag<C>){}
};

int main(){

  //auto th = std::thread([](){std::cerr<<"a"<<std::endl;});
  //return 0;

  //T{tag<float>()};
  //size_t N = 1000000;
  //{
  //  auto t = measure([&](){
  //    MyVec<float>myvec;
  //    for(size_t i=0;i<N;++i)
  //      myvec.emplace_back();
  //  });
  //  std::cerr << "time: " << t << std::endl;
  //}
  //{
  //  auto t = measure([&](){
  //    std::vector<float>myvec;
  //    for(size_t i=0;i<N;++i)
  //      myvec.emplace_back();
  //  });
  //  std::cerr << "time: " << t << std::endl;
  //}

  //return 0;
  //std::vector<A>vec;
  //
  //vec.emplace_back();
  //vec.emplace_back();
  //vec.emplace_back();
  //vec.emplace_back();
  //vec.emplace_back();

  //return 0;
  auto a = var<float>();
  std::cerr << a.stamp() << std::endl;
  a = 3.3f;
  std::cerr << a.stamp() << std::endl;
  float b = a;
  return 0;
}
