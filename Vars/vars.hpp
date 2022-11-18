#ifndef __VARS__
#define __VARS__

#include <string>
#include <cstdint>
#include <functional>
#include <set>
#include <map>

namespace vars{
}

//template<class...  >struct types{using type=types;};
template<class    T>struct Class{using type=T    ;};
//template<class Tag>using type_t=typename Tag::type;

using Destructor       = std::function<void(void*)>;

class Var{
  public:
    using Type             = std::type_info       ;
    using SignalCallback   = std::function<void()>;
    using Stamp            = size_t               ;
    template<typename CLASS,typename...ARGS>Var(Class<CLASS>,ARGS&&...args);
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

class Vars{
  public:
    using Tag          = std::string   ;
    using VarName      = std::string   ;
    using VarSelection = std::set<Var*>;
    using Tags         = std::set<Tag >;
    template<typename CLASS,typename...ARGS>
    CLASS&var(std::string const&name,ARGS&&...args);
    template<typename CLASS,typename...ARGS>
    CLASS&reVar(std::string const&name,ARGS&&...args);
    Var* get       (std::string const&name);
    void remove    (std::string const&name);
    Tags tags      (std::string const&var)const;
    bool varInTags (std::string const&var,Tags const&tags)const;
    void foreach   (std::function<void(Var*)>const&fce,Tags const&tags);
    void addTag    (std::string const&varName,Tags const&tags);
    void removeTag (std::string const&varName,Tags const&tags);
    bool dontExecuteFunction(void*fce,char const*fceName);
    Vars&dir       (std::string const&dirName);

    bool       &b    (std::string const&varName,bool              d=false);
    int8_t     &i8   (std::string const&varName,int8_t            d=0    );
    int16_t    &i16  (std::string const&varName,int16_t           d=0    );
    int32_t    &i32  (std::string const&varName,int32_t           d=0    );
    int64_t    &i64  (std::string const&varName,int64_t           d=0    );
    uint8_t    &u8   (std::string const&varName,uint8_t           d=0u   );
    uint16_t   &u16  (std::string const&varName,uint16_t          d=0u   );
    uint32_t   &u32  (std::string const&varName,uint32_t          d=0u   );
    uint64_t   &u64  (std::string const&varName,uint64_t          d=0u   );
    float      &f32  (std::string const&varName,float             d=0.f  );
    double     &f64  (std::string const&varName,double            d=0.   );
    size_t     &sizeT(std::string const&varName,size_t            d=0    );
    std::string&s    (std::string const&varName,std::string const&d=""   );

    bool       &br    (std::string const&varName,bool              d=false);
    int8_t     &i8r   (std::string const&varName,int8_t            d=0    );
    int16_t    &i16r  (std::string const&varName,int16_t           d=0    );
    int32_t    &i32r  (std::string const&varName,int32_t           d=0    );
    int64_t    &i64r  (std::string const&varName,int64_t           d=0    );
    uint8_t    &u8r   (std::string const&varName,uint8_t           d=0u   );
    uint16_t   &u16r  (std::string const&varName,uint16_t          d=0u   );
    uint32_t   &u32r  (std::string const&varName,uint32_t          d=0u   );
    uint64_t   &u64r  (std::string const&varName,uint64_t          d=0u   );
    float      &f32r  (std::string const&varName,float             d=0.f  );
    double     &f64r  (std::string const&varName,double            d=0.   );
    size_t     &sizeTr(std::string const&varName,size_t            d=0    );
    std::string&sr    (std::string const&varName,std::string const&d=""   );

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

void deallocateVar(uint8_t*p);

template<typename CLASS>
Destructor getDestructor(){
  return [](void*p){reinterpret_cast<CLASS*>(p)->~CLASS();};
}

template<typename CLASS,typename...ARGS>
Var::Var(Class<CLASS>,ARGS&&...args):Var(construct<CLASS>(args...),getDestructor<CLASS>(),typeid(CLASS)){
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



template<typename CLASS,typename...ARGS>
CLASS&Vars::var(std::string const&name,ARGS&&...args){
  auto res = name2Var.try_emplace(name,Class<CLASS>{},args...);
  Var& var = std::get<0>(res)->second;
  return var.get<CLASS>();
}

template<typename CLASS,typename...ARGS>
CLASS&Vars::reVar(std::string const&name,ARGS&&...args){
  auto res      = name2Var.try_emplace(name,Class<CLASS>{},args...);
  auto firstAdd = std::get<1>(res);
  Var& var      = std::get<0>(res)->second;
  if(firstAdd)return var;
  return var.reCreate<CLASS>(args...);
}

#define VARS_PROLOGUE(fce)\
  if(vars.dontExecuteFunction((void*)fce,__PRETTY_FUNCTION__))return;\
  Vars::Caller{vars}






#ifdef  __VARS_IMPLEMENTATION__

namespace vars{
}

void deallocateVar(uint8_t*p){
  delete [] p;
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

#define ___ std::cerr << __FILE__ << "/" << __LINE__ << std::endl

bool Vars::dontExecuteFunction(void*fce,char const*fceName){
  ___;
  auto res = fceData.emplace(std::piecewise_construct,
      std::forward_as_tuple(fce),
      std::forward_as_tuple(*this,fceName));
  ___;
  auto firstCall    = std::get<1>(res);
  ___;
  auto&data         = std::get<0>(res)->second;
  ___;
  data.shouldRecord = firstCall;
  ___;
  if(firstCall){
  ___;
    if(callstack.size() <= callstackTop){
      callstack.push_back(&data);
      callstackTop++;
    }else{
      callstack.at(callstackTop++) = &data;
    }
  ___;
    return false;
  ___;
  }
  ___;
  auto notChanged = std::get<0>(res)->second.notChanged();
  ___;
  if(notChanged){
    if(callstack.size() <= callstackTop){
      callstack.push_back(&data);
      callstackTop++;
    }else{
      callstack.at(callstackTop++) = &data;
    }
  }
  ___;


  return notChanged;
}

void Vars::exitFunction(){
  callstackTop--;
}

void Vars::record(Var*var){
  for(size_t i=0;i<callstackTop;++i)
    callstack.at(i)->record(var);
}

#define DEF_BASE(NAME,NAME2,PARAM,TYPE)\
TYPE&Vars::NAME(std::string const&varName,PARAM d){\
  return var<TYPE>(varName,d);\
}\
TYPE&Vars::NAME2(std::string const&varName,PARAM d){\
  return reVar<TYPE>(varName,d);\
}

DEF_BASE(b    ,br    ,bool              ,bool       )
DEF_BASE(i8   ,i8r   ,int8_t            ,int8_t     )
DEF_BASE(i16  ,i16r  ,int16_t           ,int16_t    )
DEF_BASE(i32  ,i32r  ,int32_t           ,int32_t    )
DEF_BASE(i64  ,i64r  ,int64_t           ,int64_t    )
DEF_BASE(u8   ,u8r   ,uint8_t           ,uint8_t    )
DEF_BASE(u16  ,u16r  ,uint16_t          ,uint16_t   )
DEF_BASE(u32  ,u32r  ,uint32_t          ,uint32_t   )
DEF_BASE(u64  ,u64r  ,uint64_t          ,uint64_t   )
DEF_BASE(f32  ,f32r  ,float             ,float      )
DEF_BASE(f64  ,f64r  ,double            ,double     )
DEF_BASE(sizeT,sizeTr,size_t            ,size_t     )
DEF_BASE(s    ,sr    ,std::string const&,std::string)

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
