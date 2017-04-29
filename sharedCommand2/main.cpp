#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>
#include<utility>

class Provider{
  public:
    void useProgram(int32_t id){
      std::cout<<"useProgram("<<id<<")"<<std::endl;
    }
    int32_t incr(int32_t a){
      std::cout<<"incr("<<a<<")"<<std::endl;
      return a+1;
    }
};

std::shared_ptr<Provider>defaultProvider = nullptr;

std::shared_ptr<Provider>getDefaultProvider(){
  return defaultProvider;
}

class Command{
  public:
    virtual ~Command(){}
    virtual void operator()() = 0;
};

template<typename TO,typename FROM>
inline TO convert(FROM const&);

template<typename OUTPUT_TO,typename...ARGS_TO,typename OUTPUT_FROM,typename... ARGS_FROM,std::size_t...I,typename std::enable_if<!std::is_same<OUTPUT_TO,void>::value,unsigned>::type = 0>
void lala_call(OUTPUT_FROM o,std::tuple<ARGS_FROM...>const&args,OUTPUT_TO(Provider::*f)(ARGS_TO...),std::index_sequence<I...>){
  auto prov = &*getDefaultProvider();
  *o = (OUTPUT_TO)(prov->*f)(convert<ARGS_TO,ARGS_FROM>(std::get<I>(args))...);
}


template<typename OUTPUT_TO,typename...ARGS_TO,typename OUTPUT_FROM,typename... ARGS_FROM,typename std::enable_if<!std::is_same<OUTPUT_TO,void>::value,unsigned>::type = 0>
std::shared_ptr<Command>sharedCommand(OUTPUT_TO(Provider::*fce)(ARGS_TO...),OUTPUT_FROM output,ARGS_FROM...args){
  class Comm: public Command{
    public:
      OUTPUT_FROM _output;
      std::tuple<ARGS_FROM...>_args;
      OUTPUT_TO(Provider::*_impl)(ARGS_TO...);
      Comm(OUTPUT_TO(Provider::*f)(ARGS_TO...),OUTPUT_FROM o,ARGS_FROM...ar){
        this->_output = o;
        this->_args = std::tuple<ARGS_FROM...>(ar...);
        this->_impl = f;
      }
      virtual void operator()(){
        lala_call(this->_output,this->_args,this->_impl,std::index_sequence_for<ARGS_FROM...>{});
      }
  };
  auto ret = std::make_shared<Comm>(fce,output,args...);
  return std::dynamic_pointer_cast<Command>(ret);
}

template<typename...TO,typename...FROM,std::size_t...I>
void lala_call(std::tuple<FROM...>const&args,void(Provider::*f)(TO...),std::index_sequence<I...>){
  auto prov = &*getDefaultProvider();
  (prov->*f)(convert<TO,FROM>(std::get<I>(args))...);
}

template<typename...TO,typename...FROM>
std::shared_ptr<Command>sharedCommand(void(Provider::*fce)(TO...),FROM...args){
  class Comm: public Command{
    public:
      std::tuple<FROM...>_args;
      void(Provider::*_impl)(TO...);
      Comm(void(Provider::*f)(TO...),FROM...ar){
        this->_args = std::tuple<FROM...>(ar...);
        this->_impl = f;
      }
      virtual void operator()(){
        lala_call(this->_args,this->_impl,std::index_sequence_for<FROM...>{});
      }
  };
  auto ret = std::make_shared<Comm>(fce,args...);
  return std::dynamic_pointer_cast<Command>(ret);
}

template<>
inline int32_t convert(int32_t const&a){
  return a;
}

template<>
inline int32_t convert(std::shared_ptr<int32_t> const&a){
  return *a;
}


int main(){
  defaultProvider = std::make_shared<Provider>();
  int32_t ret;
  auto cmd = sharedCommand(&Provider::incr,&ret,12);
  (*cmd)();
  auto cmd2 = sharedCommand(&Provider::useProgram,100);
  (*cmd2)();

  auto pp = std::make_shared<int32_t>(120);
  auto cmd3 = sharedCommand(&Provider::useProgram,pp);
  (*cmd3)();
  return 0;
}
