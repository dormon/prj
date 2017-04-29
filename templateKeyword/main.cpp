#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<tuple>
#include<string>
#include<cassert>
#include<type_traits>
#include<sstream>

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

template<typename T,typename std::enable_if<std::is_array<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<is_basic<T>::value,unsigned>::type = 0>
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

std::string toStr(size_t i){
  std::stringstream ss;
  ss<<i;
  return ss.str();
}

template<typename T,typename std::enable_if<std::is_array<T>::value,unsigned>::type>
inline std::string keyword(){return keyword<typename std::remove_extent<T>::type>()+"["+toStr(std::extent<T>::value)+"]";}

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


int main(){
  std::cout<<keyword<int[4][3]>()<<std::endl;
  std::cout<<keyword<int const*const>()<<std::endl;
  std::cout<<keyword<int const&>()<<std::endl;
  std::cout<<keyword<int [3][4][4][1] >() << std::endl;
  return 0;
}





