#pragma once

#include<tuple>
#include<type_traits>

template<typename...TYPES>
struct TypeList{
  using types = std::tuple<TYPES...>;
  template<size_t ID>
    using type = typename std::tuple_element<ID,types>::type;
  static const size_t nofTypes = std::tuple_size<types>::value;
  
  template<typename     ,size_t ID = 0,typename std::enable_if_t<(ID>=nofTypes),char> = 0>
    constexpr static size_t toId(){
      return ID;
    }

  template<typename TYPE,size_t ID = 0,typename std::enable_if_t<(ID< nofTypes),char> = 0>
    constexpr static size_t toId(){
      if(std::is_same<TYPE,type<ID>>::value)return ID;
      return toId<TYPE,ID+1>();
    }

  template<typename TYPE>
    constexpr static bool contains(){
      return toId<TYPE>()<nofTypes;
    }
};

