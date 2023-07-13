#include<iostream>
#include<vector>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>
#include<string>
#include<type_traits>
#include<sstream>
#include<typeinfo>
#include<utility>


template<typename T>struct is_basic                    :std::false_type{};
template<          >struct is_basic<void              >:std::true_type {};
template<          >struct is_basic<bool              >:std::true_type {};
template<          >struct is_basic<char              >:std::true_type {};
template<          >struct is_basic<signed char       >:std::true_type {};
template<          >struct is_basic<unsigned char     >:std::true_type {};
template<          >struct is_basic<short             >:std::true_type {};
template<          >struct is_basic<unsigned short    >:std::true_type {};
template<          >struct is_basic<int               >:std::true_type {};
template<          >struct is_basic<unsigned int      >:std::true_type {};
template<          >struct is_basic<long              >:std::true_type {};
template<          >struct is_basic<unsigned long     >:std::true_type {};
template<          >struct is_basic<long long         >:std::true_type {};
template<          >struct is_basic<unsigned long long>:std::true_type {};
template<          >struct is_basic<float             >:std::true_type {};
template<          >struct is_basic<double            >:std::true_type {};
template<          >struct is_basic<long double       >:std::true_type {};
template<          >struct is_basic<wchar_t           >:std::true_type {};
template<          >struct is_basic<char16_t          >:std::true_type {};
template<          >struct is_basic<char32_t          >:std::true_type {};



template<typename T,typename std::enable_if<is_basic<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_function<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_member_function_pointer<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_array<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_class<T>::value && !std::is_const<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<!std::is_pointer<T>::value &&  std::is_const<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if< std::is_pointer<T>::value &&  std::is_const<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if< std::is_pointer<T>::value && !std::is_const<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_reference<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_array<T>::value,unsigned>::type = 0>
std::string extents();

template<typename T,typename std::enable_if<!std::is_array<T>::value,unsigned>::type = 0>
std::string extents(){
  return "";
}





template<typename...ARGS,typename std::enable_if<sizeof...(ARGS)==0,unsigned>::type = 0>
std::string argPrinter(){
  return"";
}

template<typename HEAD,typename...ARGS,typename std::enable_if<sizeof...(ARGS)==0,unsigned>::type = 0>
std::string argPrinter(){
  return keyword<HEAD>();
}

template<typename HEAD,typename...ARGS,typename std::enable_if<(sizeof...(ARGS)>0),unsigned>::type = 0>
std::string argPrinter(){
  return keyword<HEAD>()+","+argPrinter<ARGS...>();
}

template<typename TTT,std::size_t...I>
std::string para(std::index_sequence<I...>){
  return argPrinter<typename std::tuple_element<I,TTT>::type...>();
}

template<typename T>
inline std::string parabase(){
  return para<T>(std::make_index_sequence<std::tuple_size<T>::value>{});
}


template<typename...ARGS>
std::string printArgs(std::tuple<ARGS...>const&){
  return argPrinter<ARGS...>();
}

template<typename FCE>struct FceArgType;
template<typename OUTPUT,typename... ARGS>
struct FceArgType<OUTPUT(ARGS...)>{
  using type = std::tuple<ARGS...>;
};

template<typename FCE>struct FceReturnType;
template<typename OUTPUT,typename... ARGS>
struct FceReturnType<OUTPUT(ARGS...)>{
  using type = OUTPUT;
};



template<typename FCE>struct MemFceArgType;
template<typename OUTPUT,typename CLASS,typename... ARGS>
struct MemFceArgType<OUTPUT(CLASS::*)(ARGS...)>{
  using type = std::tuple<ARGS...>;
};

template<typename FCE>struct MemFceClassType;
template<typename OUTPUT,typename CLASS,typename...ARGS>
struct MemFceClassType<OUTPUT(CLASS::*)(ARGS...)>{
  using type = CLASS;
};

template<typename FCE>struct MemFceReturnType;
template<typename OUTPUT,typename CLASS,typename...ARGS>
struct MemFceReturnType<OUTPUT(CLASS::*)(ARGS...)>{
  using type = OUTPUT;
};

template<typename T,typename std::enable_if<std::is_function<T>::value,unsigned>::type>
std::string keyword(){
  return "("+
    parabase<typename FceArgType<T>::type>()
    //printArgs(typename FceArgType<T>::type())
    +")->"+keyword<typename FceReturnType<T>::type>();
}

template<typename T,typename std::enable_if<std::is_member_function_pointer<T>::value,unsigned>::type>
std::string keyword(){
  return keyword<typename MemFceClassType<T>::type>()+"::("+
    parabase<typename MemFceArgType<T>::type>()
    //printArgs(typename MemFceArgType<T>::type())
    +")->"+keyword<typename MemFceReturnType<T>::type>();
}

template<typename T,typename std::enable_if<std::is_array<T>::value,unsigned>::type>
std::string extents(){
  std::stringstream ss;
  ss<<"[";
  ss<<std::extent<T>::value;
  ss<<"]";
  ss<<extents<typename std::remove_extent<T>::type>();
  return ss.str();
}

template<typename T,typename std::enable_if<std::is_array<T>::value,unsigned>::type>
inline std::string keyword(){
  std::stringstream ss;
  ss<<keyword<typename std::remove_all_extents<T>::type>();
  ss<<extents<T>();
  return ss.str();
}

template<typename T,typename std::enable_if< std::is_pointer<T>::value && !std::is_const<T>::value,unsigned>::type>
inline std::string keyword(){return keyword<typename std::remove_pointer<T>::type>()+"*";}

template<typename T,typename std::enable_if<!std::is_pointer<T>::value &&  std::is_const<T>::value,unsigned>::type>
inline std::string keyword(){return keyword<typename std::remove_const<T>::type>()+" const";}

template<typename T,typename std::enable_if< std::is_pointer<T>::value &&  std::is_const<T>::value,unsigned>::type>
inline std::string keyword(){return keyword<typename std::remove_const<T>::type>()+"const";}

template<typename T,typename std::enable_if<std::is_reference<T>::value,unsigned>::type>
inline std::string keyword(){return keyword<typename std::remove_reference<T>::type>()+"&";}

template<>inline std::string keyword<void              >(){return "void"              ;}
template<>inline std::string keyword<bool              >(){return "bool"              ;}
template<>inline std::string keyword<char              >(){return "char"              ;}
template<>inline std::string keyword<signed char       >(){return "signed char"       ;}
template<>inline std::string keyword<unsigned char     >(){return "unsigned char"     ;}
template<>inline std::string keyword<short             >(){return "short"             ;}
template<>inline std::string keyword<unsigned short    >(){return "unsigned short"    ;}
template<>inline std::string keyword<int               >(){return "int"               ;}
template<>inline std::string keyword<unsigned int      >(){return "unsigned int"      ;}
template<>inline std::string keyword<long              >(){return "long"              ;}
template<>inline std::string keyword<unsigned long     >(){return "unsigned long"     ;}
template<>inline std::string keyword<long long         >(){return "long long"         ;}
template<>inline std::string keyword<unsigned long long>(){return "unsigned long long";}
template<>inline std::string keyword<float             >(){return "float"             ;}
template<>inline std::string keyword<double            >(){return "double"            ;}
template<>inline std::string keyword<long double       >(){return "long double"       ;}
template<>inline std::string keyword<wchar_t           >(){return "wchar_t"           ;}
template<>inline std::string keyword<char16_t          >(){return "char16_t"          ;}
template<>inline std::string keyword<char32_t          >(){return "char32_t"          ;}




class Auto{};
struct Buto{};

class TestClass{
  public:
    int add(int a,int b){return a+b;}
};




template<>inline std::string keyword<Auto>(){return "auto";}
template<>inline std::string keyword<Buto>(){return "buto";}
template<>inline std::string keyword<TestClass>(){return "TestClass";}
template<>inline std::string keyword<std::string>(){return "string";}

int main(){
  //if(std::is_same<decltype(&A::add),int(A::*)(int,int)>::value)std::cout<<"jojo"<<std::endl;
  //return 0;
  std::cout<<keyword<int32_t(int32_t const&)>()<<std::endl;
  std::cout<<keyword<int32_t>()<<std::endl;
  std::cout<<keyword<int32_t[3]>()<<std::endl;
  std::cout<<keyword<int32_t[1][2][3][4]>()<<std::endl;
  std::cout<<keyword<int32_t*>()<<std::endl;
  std::cout<<keyword<int32_t**>()<<std::endl;
  std::cout<<keyword<int32_t***>()<<std::endl;
  std::cout<<keyword<int32_t&>()<<std::endl;
  std::cout<<keyword<int32_t const&>()<<std::endl;
  std::cout<<keyword<int32_t const*>()<<std::endl;
  std::cout<<keyword<int32_t&>()<<std::endl;
  std::cout<<keyword<int32_t const**>()<<std::endl;
  std::cout<<keyword<int32_t const*const*>()<<std::endl;
  std::cout<<keyword<int32_t *const&>()<<std::endl;
  std::cout<<keyword<int32_t const*const&>()<<std::endl;
  std::cout<<keyword<Auto>()<<std::endl;
  std::cout<<keyword<Auto*>()<<std::endl;
  std::cout<<keyword<Auto&>()<<std::endl;
  std::cout<<keyword<Auto const&>()<<std::endl;
  std::cout<<keyword<Buto>()<<std::endl;
  std::cout<<keyword<std::string>()<<std::endl;
  std::cout<<keyword<int32_t(int32_t,int32_t)>()<<std::endl;
  std::cout<<keyword<int32_t()>()<<std::endl;
  std::cout<<keyword<int32_t(float,int32_t,int8_t)>()<<std::endl;
  std::cout<<keyword<decltype(&TestClass::add)>()<<std::endl;
  /*
  std::cout<<typeid(int32_t).name()<<std::endl;
  std::cout<<typeid(int64_t).name()<<std::endl;
  std::cout<<typeid(Buto).name()<<std::endl;
  std::cout<<typeid(int32_t const&).name()<<std::endl;
  */
  return 0;
}
