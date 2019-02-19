#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>
#include <cstdlib>
#include <memory>

#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

#include <sstream>
#include "sink.h"

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

template <typename T           > struct is_map                : std::false_type{};
template <typename K,typename V> struct is_map<std::map<K,V>> : std::true_type {};

template <typename T           > struct is_unordered_map                          : std::false_type{};
template <typename K,typename V> struct is_unordered_map<std::unordered_map<K,V>> : std::true_type {};

template <typename T              > struct is_container                                   : std::false_type{};
template <typename T,std::size_t N> struct is_container<std::array             <T,N    >> : std::true_type {};
template <typename T              > struct is_container<std::vector            <T      >> : std::true_type {};
template <typename T              > struct is_container<std::deque             <T      >> : std::true_type {};
template <typename T              > struct is_container<std::forward_list      <T      >> : std::true_type {};
template <typename T              > struct is_container<std::list              <T      >> : std::true_type {};
template <typename...ARGS         > struct is_container<std::set               <ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::map               <ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::multiset          <ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::multimap          <ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::unordered_set     <ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::unordered_map     <ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::unordered_multiset<ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::unordered_multimap<ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::stack             <ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::queue             <ARGS...>> : std::true_type {};
template <typename...ARGS         > struct is_container<std::priority_queue    <ARGS...>> : std::true_type {};

template<typename T,typename std::enable_if<std::is_same<std::string,T>::value,unsigned>::type = 0>
std::string classKeyword(){
  return "std::string";
}


template<typename T>
std::string mapKeyword();

template<typename T>
std::string unordered_mapKeyword();

template<typename T,typename std::enable_if<is_map<T>::value,unsigned>::type = 0>
std::string classKeyword(){
  return mapKeyword<T>();
}

template<typename T,typename std::enable_if<is_unordered_map<T>::value,unsigned>::type = 0>
std::string classKeyword(){
  return unordered_mapKeyword<T>();
}

template<typename T,typename std::enable_if<is_basic<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_function<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_member_function_pointer<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_array<T>::value,unsigned>::type = 0>
std::string keyword();

template<typename T,typename std::enable_if<std::is_class<T>::value && !std::is_const<T>::value,unsigned>::type = 0>
std::string keyword(){
  return classKeyword<T>();
}

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

template<typename MAP>struct MapKey;
template<typename K,typename V>struct MapKey<std::map<K,V>>{using type = K;};
template<typename MAP>struct MapValue;
template<typename K,typename V>struct MapValue<std::map<K,V>>{using type = V;};

template<typename MAP>struct UnorderedMapKey;
template<typename K,typename V>struct UnorderedMapKey<std::unordered_map<K,V>>{using type = K;};
template<typename MAP>struct UnorderedMapValue;
template<typename K,typename V>struct UnorderedMapValue<std::unordered_map<K,V>>{using type = V;};

template<typename T>
std::string mapKeyword(){
  std::stringstream ss;
  ss << "std::map<" << keyword<typename MapKey<T>::type>() << "," << keyword<typename MapValue<T>::type>() << ">";
  return ss.str();
}

template<typename T>
std::string unordered_mapKeyword(){
  std::stringstream ss;
  ss << "std::unordered_map<" << keyword<typename UnorderedMapKey<T>::type>() << "," << keyword<typename UnorderedMapValue<T>::type>() << ">";
  return ss.str();
}



void measure(std::string const&name,std::function<void()>const&f){
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end   = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = end - start;
  std::cout << name << ": " << elapsed.count() << std::endl;
}

template<typename T>
std::string typeName();

template<>std::string typeName<uint32_t   >(){return "uint32_t"   ;}
template<>std::string typeName<size_t     >(){return "size_t"     ;}
template<>std::string typeName<std::string>(){return "std::string";}
//template<>std::string typeName<std::map<K,V>>(){std::string ss;ss<<"std::map<"<<typename<K>()<<","<<typename<V>()<<">";return ss.str();}

template<typename KEY>
inline constexpr KEY construct(size_t i);

template<>inline size_t      construct<size_t     >(size_t i){return i;}
template<>inline uint32_t    construct<uint32_t   >(size_t i){return i;}
template<>inline std::string construct<std::string>(size_t i){std::stringstream ss;ss<<i;return ss.str();}

template<typename KEY,typename VALUE,template<typename...>class CONTAINER = std::map>
void fill(CONTAINER<KEY,VALUE>&data,size_t N){
  for(size_t i=0;i<N;++i)
    data[construct<KEY>(i)] = construct<VALUE>(i);
}

template<typename KEY,typename VALUE,template<typename...>class CONTAINER = std::map>
void measureInsert(size_t N){
  CONTAINER<KEY,VALUE>data;
  std::stringstream ss;
  ss << keyword<CONTAINER<KEY,VALUE>>() << "::insert [0,1,2,...," << N << ")";
  measure(ss.str(),[&]{
    fill(data,N);
  });
  if(data.at(construct<KEY>(N-1)) != construct<VALUE>(N-1))
    std::cerr << "jo" << std::endl;
};

template<typename KEY,typename VALUE,template<typename...>class CONTAINER = std::map>
void measureAt(size_t N){
  CONTAINER<KEY,VALUE>data;
  std::stringstream ss;
  ss << keyword<CONTAINER<KEY,VALUE>>() << "::at " << N;
  fill(data,N);
  measure(ss.str(),[&]{
    for(size_t i=0;i<N;++i)
      sink(data.at(construct<KEY>(i)));
  });
};

template<size_t N>
class Element{
  public:
    Element(Element&&e){
      bytes = e.bytes;
    }
    uint8_t bytes[N];
};

/*
template <typename T           > struct is_map                : std::false_type{};
template <typename K,typename V> struct is_map<std::map<K,V>> : std::true_type {};
*/

template <typename                                         T> struct is_template             : std::false_type{};
template <typename   ...ARGS,template<typename   ...>class T> struct is_template<T<ARGS...>> : std:: true_type{};
template <std::size_t...ARGS,template<std::size_t...>class T> struct is_template<T<ARGS...>> : std:: true_type{};
template <typename A,std::size_t...ARGS,template<typename,std::size_t...>class T> struct is_template<T<A,ARGS...>> : std:: true_type{};
template <typename A,template<typename>class T> struct is_template<T<A>> : std:: true_type{};

int main(int argc,char*[]) {

  std::cerr << typeid(std::string).name() << std::endl;
  if(is_template<std::vector<int>>::value )std::cerr << "std::vector<int> je template" << std::endl;
  if(is_template<int>::value              )std::cerr << "int je template" << std::endl;
  if(is_template<std::array<int,4>>::value)std::cerr << "std::array<int,4> je template" << std::endl;

  return 0;
  {
    size_t acc=0;
    for(size_t i=0;i<1000000+argc;++i)
      acc += i;
    if(acc == 1337)
      std::cerr << "leet warmup" << std::endl;
  }

  auto const mapInsertAsc = [&]{
    std::map<uint32_t,uint32_t>data;
    measure("std::map<uint32_t,uint32_t>::insert [0,1,2,...,1000000)",[&]{
      for(uint32_t i=0;i<999999+argc;++i)
        data[i] = i;
    });
    if(data.at(999999) != 999999)
      std::cerr << "jo" << std::endl;
  };

  auto const mapInsertDesc = [&]{
    std::map<uint32_t,uint32_t>data;
    measure("std::map<uint32_t,uint32_t>::insert (1000000,999999,...0]",[&]{
      for(uint32_t i=0;i<999999+argc;++i)
        data[i] = 999999-i;
    });
    if(data.at(999999) == 999999)
      std::cerr << "jo" << std::endl;
  };

  auto const mapAt = [&]{
    std::map<uint32_t,uint32_t>data;
    for(uint32_t i=0;i<1000000;++i)
      data[i] = i;
    measure("std::map<uint32_t,uint32_t>::at 1000000",[&]{
      for(uint32_t i=0;i<1000000;++i)
        data.at(i);
    });
  };

  auto const unorderedInsertAsc = [&]{
    std::unordered_map<uint32_t,uint32_t>data;
    measure("std::unordered_map<uint32_t,uint32_t>::insert [0,1,2,...,1000000)",[&]{
      for(uint32_t i=0;i<999999+argc;++i)
        data[i] = i;
    });
    if(data.at(999999) != 999999)
      std::cerr << "jo" << std::endl;
  };

  auto const unorderedInsertDesc = [&]{
    std::map<uint32_t,uint32_t>data;
    measure("std::unordered_map<uint32_t,uint32_t>::insert (1000000,999999,...0]",[&]{
      for(uint32_t i=0;i<999999+argc;++i)
        data[i] = 999999-i;
    });
    if(data.at(999999) == 999999)
      std::cerr << "jo" << std::endl;
  };

  auto const unorderedAt = [&]{
    std::map<uint32_t,uint32_t>data;
    for(uint32_t i=0;i<1000000;++i)
      data[i] = i;
    measure("std::unordered_map<uint32_t,uint32_t>::at 1000000",[&]{
      for(uint32_t i=0;i<1000000;++i)
        data.at(i);
    });
  };


  mapInsertAsc();
  mapInsertAsc();
  mapInsertAsc();
  mapInsertAsc();
  unorderedInsertAsc();
  unorderedInsertAsc();
  unorderedInsertAsc();
  unorderedInsertAsc();

  mapInsertDesc();
  mapInsertDesc();
  mapInsertDesc();
  mapInsertDesc();
  unorderedInsertDesc();
  unorderedInsertDesc();
  unorderedInsertDesc();
  unorderedInsertDesc();

  mapAt();
  mapAt();
  mapAt();
  mapAt();
  unorderedAt();
  unorderedAt();
  unorderedAt();
  unorderedAt();

  std::cerr << "################################" << std::endl;

  size_t N = 1000000;

  measureInsert<uint32_t   ,uint32_t,std::map          >(N);
  measureInsert<uint32_t   ,uint32_t,std::map          >(N);
  measureInsert<uint32_t   ,uint32_t,std::map          >(N);
  measureInsert<uint32_t   ,uint32_t,std::map          >(N);

  measureInsert<uint32_t   ,uint32_t,std::unordered_map>(N);
  measureInsert<uint32_t   ,uint32_t,std::unordered_map>(N);
  measureInsert<uint32_t   ,uint32_t,std::unordered_map>(N);
  measureInsert<uint32_t   ,uint32_t,std::unordered_map>(N);

  measureInsert<std::string,uint32_t,std::map          >(N);
  measureInsert<std::string,uint32_t,std::map          >(N);
  measureInsert<std::string,uint32_t,std::map          >(N);
  measureInsert<std::string,uint32_t,std::map          >(N);

  measureInsert<std::string,uint32_t,std::unordered_map>(N);
  measureInsert<std::string,uint32_t,std::unordered_map>(N);
  measureInsert<std::string,uint32_t,std::unordered_map>(N);
  measureInsert<std::string,uint32_t,std::unordered_map>(N);

  measureAt<uint32_t   ,uint32_t,std::map          >(N);
  measureAt<uint32_t   ,uint32_t,std::map          >(N);
  measureAt<uint32_t   ,uint32_t,std::map          >(N);
  measureAt<uint32_t   ,uint32_t,std::map          >(N);

  measureAt<uint32_t   ,uint32_t,std::unordered_map>(N);
  measureAt<uint32_t   ,uint32_t,std::unordered_map>(N);
  measureAt<uint32_t   ,uint32_t,std::unordered_map>(N);
  measureAt<uint32_t   ,uint32_t,std::unordered_map>(N);

  measureAt<std::string,uint32_t,std::map          >(N);
  measureAt<std::string,uint32_t,std::map          >(N);
  measureAt<std::string,uint32_t,std::map          >(N);
  measureAt<std::string,uint32_t,std::map          >(N);

  measureAt<std::string,uint32_t,std::unordered_map>(N);
  measureAt<std::string,uint32_t,std::unordered_map>(N);
  measureAt<std::string,uint32_t,std::unordered_map>(N);
  measureAt<std::string,uint32_t,std::unordered_map>(N);
  return 0;
}
