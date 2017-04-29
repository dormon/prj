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

template <typename... Args>
struct variadic_typedef
{
  //this single type represents a collection of types,
  // as the template arguments it took to define it
};

template <typename... Args>
struct convert_in_tuple
{
  // base case, nothing special,
  // just use the arguments directly
  // however they need to be used
  typedef std::tuple<Args...> type;
};

template <typename... Args>
struct convert_in_tuple<variadic_typedef<Args...>>
{
  // expand the variadic_typedef back into
  // its arguments, via specialization
  // (doesn't rely on functionality to be provided
  // by the variadic_typedef struct itself, generic)
  typedef typename convert_in_tuple<Args...>::type type;
};

typedef variadic_typedef<int, float> myTypes;
typedef convert_in_tuple<myTypes>::type int_float_tuple;

/*
int main()
{}
*/

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

template<typename OUTPUT,typename...ARGS,std::size_t...I>
void lala_call(OUTPUT*o,std::tuple<ARGS...>const&args,OUTPUT(Provider::*f)(ARGS...),std::index_sequence<I...>){
  auto prov = &*getDefaultProvider();
  *o = (prov->*f)(std::get<I>(args)...);
}


template<typename OUTPUT,typename...ARGS>
std::shared_ptr<Command>sharedCommand(OUTPUT(Provider::*fce)(ARGS...),OUTPUT*output,ARGS...args){
  class Comm: public Command{
    public:
      OUTPUT*_output;
      std::tuple<ARGS...>_args;
      OUTPUT(Provider::*_impl)(ARGS...);
      Comm(OUTPUT(Provider::*f)(ARGS...),OUTPUT*o,ARGS...ar){
        this->_output = o;
        this->_args = std::tuple<ARGS...>(ar...);
        this->_impl = f;
      }
      virtual void operator()(){
        lala_call(this->_output,this->_args,this->_impl,std::index_sequence_for<ARGS...>{});
      }
  };
  auto ret = std::make_shared<Comm>(fce,output,args...);
  return std::dynamic_pointer_cast<Command>(ret);
}

template<typename...ARGS,std::size_t...I>
void lala_call(std::tuple<ARGS...>const&args,void(Provider::*f)(ARGS...),std::index_sequence<I...>){
  auto prov = &*getDefaultProvider();
  (prov->*f)(std::get<I>(args)...);
}

template<typename...ARGS>
std::shared_ptr<Command>sharedCommand(void(Provider::*fce)(ARGS...),ARGS...args){
  class Comm: public Command{
    public:
      std::tuple<ARGS...>_args;
      void(Provider::*_impl)(ARGS...);
      Comm(void(Provider::*f)(ARGS...),ARGS...ar){
        this->_args = std::tuple<ARGS...>(ar...);
        this->_impl = f;
      }
      virtual void operator()(){
        lala_call(this->_args,this->_impl,std::index_sequence_for<ARGS...>{});
      }
  };
  auto ret = std::make_shared<Comm>(fce,args...);
  return std::dynamic_pointer_cast<Command>(ret);
}




int main(){
  defaultProvider = std::make_shared<Provider>();
  int32_t ret;
  auto cmd = sharedCommand(&Provider::incr,&ret,12);
  (*cmd)();
  auto cmd2 = sharedCommand(&Provider::useProgram,100);
  (*cmd2)();
  return 0;
}
