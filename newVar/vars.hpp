#ifndef __VARS__
#define __VARS__

#include <string>
#include <cstdint>
#include <functional>
#include <set>
#include <map>

namespace vars{
}

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

template<typename CLASS,typename...ARGS>
uint8_t*construct(ARGS&&...args){
  auto ptr = allocateVar<CLASS>();
  callConstructor<CLASS>(ptr,args...);
  return ptr;
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
    using Type             = std::type_info       ;
    using SignalCallback   = std::function<void()>;
    using Stamp            = size_t               ;
    template<typename CLASS,typename...ARGS>Var(tag<CLASS>,ARGS&&...args);
    template<typename CLASS                >Var&operator=(CLASS&&value);
    template<typename CLASS                >operator CLASS const&()const;
    template<typename CLASS                >operator CLASS&();
    template<typename CLASS,typename...ARGS>CLASS      &reCreate(ARGS&&...args);
    template<typename CLASS                >CLASS      &get     (             );
    template<typename CLASS                >CLASS const&get     (             )const;
    template<typename T                    >bool        is      (             )const;

    Var(
        uint8_t*   const data      ,
        Destructor const&destructor,
        Type       const&type      );
    ~Var();
    void updateStamp();
    void signal();
    void setSignal(SignalCallback const&callback);
    SignalCallback getOnChange()const;
    Stamp stamp()const;
  protected:
    bool signalIsEmpty()const;
    uint8_t*               data             = nullptr;
    Destructor             destructor       = nullptr;
    Stamp                  timeStamp        = 0      ;
    Type             const&type                      ;
    SignalCallback         signalCallback   = nullptr;

};


template<typename CLASS,typename...ARGS>
Var::Var(tag<CLASS>,ARGS&&...args):Var(construct<CLASS>(args...),getDestructor<CLASS>(),typeid(CLASS)){
}

template<typename CLASS>
Var&Var::operator=(CLASS&&value){
  get<CLASS>() = value;
  updateStamp();
  return*this;
}

template<typename CLASS,typename...ARGS>
CLASS&Var::reCreate(ARGS&&...args){
  destructor(data);

  if(typeid(CLASS) != type){
    auto const oldStamp = stamp();
    deallocateVar(data);
    data = allocateVar<CLASS>();
    new(this)Var(data,getDestructor<CLASS>(),typeid(CLASS));
    timeStamp = oldStamp;
  }else
    callConstructor<CLASS>(data,args...);

  updateStamp();
  return get<CLASS>();
}
template<typename CLASS>
Var::operator CLASS const&()const{
  return get<CLASS>();
}
template<typename CLASS>
Var::operator CLASS&(){
  return get<CLASS>();
}
template<typename CLASS>
CLASS&Var::get(){
  return *reinterpret_cast<CLASS*>(data);
}
template<typename CLASS>
CLASS const&Var::get()const{
  return *reinterpret_cast<CLASS*>(data);
}
template<typename T>
bool Var::is()const{return typeid(T) == type;}

template<typename T,typename...ARGS>
Var var(ARGS&&...args){
  return Var(construct<T>(args...),getDestructor<T>(),typeid(T));
}


class Tag{
  public:
    std::string name;
};

class Vars{
  public:
    using Tag          = std::string   ;
    using VarName      = std::string   ;
    using VarSelection = std::set<Var*>;
    using Tags         = std::set<Tag >;
    template<typename CLASS,typename...ARGS>
    CLASS&add(std::string const&name,ARGS&&...args){
      auto ptr = construct<CLASS>(args...);
      auto res = name2Var.emplace(std::piecewise_construct,
          std::forward_as_tuple(name),
          std::forward_as_tuple(ptr,getDestructor<CLASS>(),typeid(CLASS))
          );
      Var&var      = std::get<0>(res)->second;
      return var.get<CLASS>();
    }
    template<typename CLASS,typename...ARGS>
    CLASS&addOrGet(std::string const&name,ARGS&&...args){
      auto ptr = construct<CLASS>(args...);
      auto res = name2Var.emplace(std::piecewise_construct,
          std::forward_as_tuple(name),
          std::forward_as_tuple(ptr,getDestructor<CLASS>(),typeid(CLASS))
          );
      Var&var      = std::get<0>(res)->second;
      return var.get<CLASS>();
    }
    template<typename CLASS,typename...ARGS>
    CLASS&get(std::string const&name){
      return name2Var.at(name).get<CLASS>();
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
    Var* get       (std::string const&name);
    void remove    (std::string const&name);
    Tags tags      (std::string const&var)const;
    bool varInTags (std::string const&var,Tags const&tags)const;
    void foreach   (std::function<void(Var*)>const&fce,Tags const&tags);
    void addTag    (std::string const&varName,Tags const&tags);
    void removeTag (std::string const&varName,Tags const&tags);
    bool notChanged(void*fce,char const*fceName);
    float&f32   (std::string const&varName            );
    float&addf32(std::string const&varName,float d=0.f);
    float&f32r  (std::string const&varName,float d=0.f);
    float&f32a  (std::string const&varName,float d=0.f);
    void exitFunction();
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
    void record(Var*var);
    class FunctionData{
      public:
        bool shouldRecord = true;
        std::map<Var*,size_t>inputs;
        char const*name;
        bool signalized = false;
        std::set<Var*>recordedInputs;
        FunctionData(Vars&vars,char const*fceName);
        bool notChanged();
        void record(Var*var);
        void updateSeenStamps(decltype(inputs)::iterator it);
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

#ifdef  __VARS_IMPLEMENTATION__

namespace vars{
}


Var::Var(
    uint8_t*   const data      ,
    Destructor const&destructor,
    Type       const&type      )
  :data      (data      )
  ,destructor(destructor)
  ,type      (type      )
  {}

Var::~Var(){
  if(!destructor || !data)return;
  destructor(data);
  deallocateVar(data);
}

void Var::updateStamp(){
  ++timeStamp;
}
void Var::signal(){
  if(signalIsEmpty())return;
  signalCallback();
}

void Var::setSignal(SignalCallback const&callback){
  signalCallback = callback;
}
Var::SignalCallback Var::getOnChange()const{
  return signalCallback;
}
Var::Stamp Var::stamp()const{return timeStamp;}

bool Var::signalIsEmpty()const{
  return signalCallback == nullptr;
}


Var*Vars::get(std::string const&name){
  auto it = name2Var.find(name);
  if(it == std::end(name2Var))return nullptr;
  record(it->second);
  return it->second;
}
void Vars::remove(std::string const&name){
  name2Var.erase(name);
  removeTag(name,var2Tags.at(name));
  var2Tags.erase(name);
}


Vars::Tags Vars::tags(std::string const&var)const{
  return var2Tags.at(var);
}

bool Vars::varInTags(std::string const&var,Tags const&tags)const{
  auto const&varTags = var2Tags.at(var);
  for(auto const&tag:tags)
    if(!varTags.count(tag))return false;
  return true;
}

void Vars::foreach(std::function<void(Var*)>const&fce,Tags const&tags){
  for(auto &var:name2Var){
    if(varInTags(var.first,tags))
      fce(&var.second);
  }
}

void Vars::addTag(std::string const&varName,Tags const&tags){
  auto&var = name2Var.at(varName);
  for(auto const&tag:tags){
    tagToVars[tag].insert(&var);
    var2Tags[varName].insert(tag);
  }
}

void Vars::removeTag(std::string const&varName,Tags const&tags){
  auto&var = name2Var.at(varName);
  for(auto const&tag:tags){
    tagToVars.at(tag).erase(&var);
    var2Tags[varName].erase(tag);
  }
}

bool Vars::notChanged(void*fce,char const*fceName){
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

void Vars::exitFunction(){
  callstackTop--;
}

void Vars::record(Var*var){
  for(size_t i=0;i<callstackTop;++i)
    callstack.at(i)->record(var);
}

float&Vars::f32(std::string const&varName){
  return get<float>(varName);
}

float&Vars::addf32(std::string const&varName,float d){
  return add<float>(varName,d);
}

float&Vars::f32r  (std::string const&varName,float d){
  return reCreate<float>(varName,d);
}

float&Vars::f32a  (std::string const&varName,float d){
  return addOrGet<float>(varName,d);
}


Vars::FunctionData::FunctionData(Vars&vars,char const*fceName):name(fceName){}
bool Vars::FunctionData::notChanged(){
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
void Vars::FunctionData::record(Var*var){
  if(shouldRecord)
    recordedInputs.insert(var);
}
void Vars::FunctionData::updateSeenStamps(decltype(inputs)::iterator it){
  for(;it!=std::end(inputs);++it)
    std::get<1>(*it) = std::get<0>(*it)->stamp();
}

#endif//__VARS_IMPLEMENTATION__

#endif//__VARS__
