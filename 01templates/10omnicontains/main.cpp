#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* This file implements contains function.
 * This function find item in std container using the fastes implementation.
 * If container contains function count, the contain function will use it.
 * WARNING! this is really dark magic
 */

template<typename...>struct voider{using type=void;};
template<typename...Ts>using void_t=typename voider<Ts...>::type;


template<typename...>struct types{using type=types;};

namespace details {
  template<template<typename...>class CONTAINER, typename types, typename=void>
    struct can_apply : std::false_type {};

  template<template<typename...>class CONTAINER, typename...Ts>
    struct can_apply< CONTAINER, types<Ts...>, void_t< CONTAINER<Ts...> > >: std::true_type {};
}

template<template<typename...>class CONTAINER, typename...Ts>
using can_apply = details::can_apply<CONTAINER,types<Ts...>>;

template<typename CONTAINER, typename VALUE>
using dot_count_type = decltype( std::declval<CONTAINER>().count(std::declval<VALUE>()) );

template<typename CONTAINER, typename VALUE>
using has_dot_count = can_apply<dot_count_type, CONTAINER, VALUE>;

namespace details {
  template<typename CONTAINER, typename VALUE>
    bool contains(std::false_type, CONTAINER const& c, VALUE const& i) {
      for(auto&& x:c) {
        if(x == i) { return true; }
      }
      return false;
    }
  template<typename CONTAINER, typename VALUE>
    bool contains(std::true_type, CONTAINER const& c, VALUE const& i) {
      return c.count(i) != 0;
    }
}

template<typename CONTAINER, typename VALUE>
bool contains( CONTAINER const& c, VALUE const& i ) {
  return details::contains( has_dot_count<CONTAINER const&,VALUE const&>{}, c, i );
}

int main(){
  std::vector<int>asd;
  asd.push_back(10);
  asd.push_back(8);
  asd.push_back(1);
  asd.push_back(7);
  if(contains(asd,7)){std::cout<<"obsahuje"<<std::endl;}

  std::set<float>vsa;
  vsa.insert(23.f);
  vsa.insert(9.f);
  vsa.insert(10.f);
  if(contains(vsa,10.f)){std::cout<<"obsahuje"<<std::endl;}

  return 0;
}
