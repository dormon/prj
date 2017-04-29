#include<iostream>
#include<memory>

// hack, MSVS does not work with std::integer_sequences
namespace ge{
  namespace core{
    template <class T, T... I> struct integer_sequence
    {
      template <T N> using append = integer_sequence<T, I..., N>;
      static std::size_t size() { return sizeof...(I); }
      using next = append<sizeof...(I)>;
      using type = T;
    };

    template <class T, T Index, std::size_t N>
      struct sequence_generator
      {
        using type = typename sequence_generator<T, Index - 1, N - 1>::type::next;
      };

    template <class T, T Index>
      struct sequence_generator<T, Index, 0ul> { using type = integer_sequence<T>; };

    template <std::size_t... I>
      using index_sequence = integer_sequence<std::size_t, I...>;

    template <class T, T N>
      struct make_integer_sequence
      {
        typedef typename sequence_generator<T, N, N>::type type;
      };

    template <std::size_t N>
      using make_index_sequence = make_integer_sequence<std::size_t, N>;

    template<class... T>
      using index_sequence_for = make_index_sequence<sizeof...(T)>;
  }
}

//OpenGL function provider
class Provider{
  public:
    void useProgram(int32_t id){
      std::cout<<"useProgram("<<id<<")"<<std::endl;
    }
    int32_t incr(int32_t a){
      std::cout<<"incr("<<a<<")"<<std::endl;
      return a+1;
    }
    int32_t add(int32_t a,int32_t b){
      std::cout<<"add("<<a<<","<<b<<")"<<std::endl;
      return a+b;
    }
};

thread_local std::shared_ptr<Provider>defaultProvider = nullptr;

std::shared_ptr<Provider>getDefaultProvider(){
  return defaultProvider;
}

//Abstract command
class Command{
  public:
    virtual ~Command(){}
    virtual void operator()() = 0;
};

template<typename TO,typename FROM>
inline TO convertTo(FROM const&);

//function with non void return value
template<typename OUTPUT_TO,typename...ARGS_TO,typename OUTPUT_FROM,typename... ARGS_FROM,std::size_t...I,typename std::enable_if<!std::is_same<OUTPUT_TO,void>::value,unsigned>::type = 0>
void lala_call(OUTPUT_FROM o,std::tuple<ARGS_FROM...>const&args,OUTPUT_TO(Provider::*f)(ARGS_TO...),ge::core::index_sequence<I...>){
  auto prov = &*getDefaultProvider();
  *o = (OUTPUT_TO)(prov->*f)(convertTo<ARGS_TO,ARGS_FROM>(std::get<I>(args))...);
}

template<typename OUTPUT_TO,typename...ARGS_TO,typename OUTPUT_FROM,typename... ARGS_FROM,typename std::enable_if<!std::is_same<OUTPUT_TO,void>::value,unsigned>::type = 0>
std::shared_ptr<Command>sharedCommand(OUTPUT_TO(Provider::*fce)(ARGS_TO...),OUTPUT_FROM output,ARGS_FROM...args){
  class FunctionCommand: public Command{
    public:
      OUTPUT_FROM _output;
      std::tuple<ARGS_FROM...>_args;
      OUTPUT_TO(Provider::*_impl)(ARGS_TO...);
      FunctionCommand(OUTPUT_TO(Provider::*f)(ARGS_TO...),OUTPUT_FROM o,ARGS_FROM...ar){
        this->_output = o;
        this->_args = std::tuple<ARGS_FROM...>(ar...);
        this->_impl = f;
      }
      virtual void operator()(){
        lala_call(this->_output,this->_args,this->_impl,typename ge::core::make_index_sequence<sizeof...(ARGS_FROM)>::type{});
      }
  };
  auto ret = std::make_shared<FunctionCommand>(fce,output,args...);
  return std::dynamic_pointer_cast<Command>(ret);
}

//void functions
template<typename...TO,typename...FROM,std::size_t...I>
void lala_call(std::tuple<FROM...>const&args,void(Provider::*f)(TO...),ge::core::index_sequence<I...>){
  auto prov = &*getDefaultProvider();
  (prov->*f)(convertTo<TO,FROM>(std::get<I>(args))...);
}

template<typename...TO,typename...FROM>
std::shared_ptr<Command>sharedCommand(void(Provider::*fce)(TO...),FROM...args){
  class FunctionCommand: public Command{
    public:
      std::tuple<FROM...>_args;
      void(Provider::*_impl)(TO...);
      FunctionCommand(void(Provider::*f)(TO...),FROM...ar){
        this->_args = std::tuple<FROM...>(ar...);
        this->_impl = f;
      }
      virtual void operator()(){
        lala_call(this->_args,this->_impl,typename ge::core::make_index_sequence<sizeof...(FROM)>::type{});
      }
  };
  auto ret = std::make_shared<FunctionCommand>(fce,args...);
  return std::dynamic_pointer_cast<Command>(ret);
}



template<>
inline int32_t convertTo(int32_t const&a){
  return a;
}

template<>
inline int32_t convertTo(std::shared_ptr<int32_t> const&a){
  return *a;
}


int main(){
  //vytvorime function provider (neco jako OpenGL function provider v nasledujicim nahrazeni glew v geGL)
  defaultProvider = std::make_shared<Provider>();
  int32_t ret;

  //vytvorime command ktery vola "OpenGL" function incr 
  auto cmd = sharedCommand(&Provider::incr,&ret,12);
  (*cmd)();

  //vytvorime command, ktery vola "OpenGL" funkci useProgram, program id 100
  auto cmd2 = sharedCommand(&Provider::useProgram,100);
  (*cmd2)();

  //funguje to i se sdilenymi parametry, pokud existuje convertTo specializace
  auto pp = std::make_shared<int32_t>(120);
  auto cmd3 = sharedCommand(&Provider::useProgram,pp);
  (*cmd3)();

  //vice parametru, kombinace shared a non shared
  auto spp = std::make_shared<int32_t>(50);
  auto cmd4 = sharedCommand(&Provider::add,&ret,10,spp);
  (*cmd4)();
  std::cout<<ret<<std::endl;

  return 0;
}
