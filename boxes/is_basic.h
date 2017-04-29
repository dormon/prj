#pragma once

#include<type_traits>
#include<tuple>
#include<vector>
#include<memory>

template <class T> struct is_shared                      : std::false_type {};
template <class T> struct is_shared<std::shared_ptr<T>>  : std::true_type {};

template <class T> struct is_vector                  : std::false_type {};
template <class T> struct is_vector<std::vector<T>>  : std::true_type {};



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


