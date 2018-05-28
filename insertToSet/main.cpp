#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <tuple>

#include<TypeSet.h>

int main() {
  static_assert( typeSet::size<typeSet::Set<         >>() == 0);
  static_assert( typeSet::size<typeSet::Set<float    >>() == 1);
  static_assert( typeSet::size<typeSet::Set<float,int>>() == 2);
  static_assert(!typeSet::contain<float                  ,typeSet::Set<              >>());
  static_assert( typeSet::contain<float                  ,typeSet::Set<float         >>());
  static_assert(!typeSet::contain<float                  ,typeSet::Set<int,char      >>());
  static_assert( typeSet::contain<float                  ,typeSet::Set<int,char,float>>());
  static_assert( typeSet::contain<float                  ,typeSet::Set<int,float,char>>());
  static_assert( typeSet::contain<float                  ,typeSet::Set<float,char,int>>());
  static_assert( typeSet::subset <typeSet::Set<              >,typeSet::Set<              >>());
  static_assert( typeSet::subset <typeSet::Set<              >,typeSet::Set<float         >>());
  static_assert( typeSet::subset <typeSet::Set<float         >,typeSet::Set<float         >>());
  static_assert( typeSet::subset <typeSet::Set<float         >,typeSet::Set<float,int     >>());
  static_assert( typeSet::subset <typeSet::Set<float,int     >,typeSet::Set<float,int     >>());
  static_assert( typeSet::subset <typeSet::Set<float,int,char>,typeSet::Set<float,int,char>>());
  static_assert(!typeSet::subset <typeSet::Set<float,int,char>,typeSet::Set<float,int     >>());
  static_assert(!typeSet::subset <typeSet::Set<float         >,typeSet::Set<              >>());
  static_assert( typeSet::equal  <typeSet::Set<              >,typeSet::Set<              >>());
  static_assert(!typeSet::equal  <typeSet::Set<              >,typeSet::Set<float         >>());
  static_assert(!typeSet::equal  <typeSet::Set<float         >,typeSet::Set<              >>());
  static_assert(!typeSet::equal  <typeSet::Set<float,int     >,typeSet::Set<              >>());
  static_assert( typeSet::equal  <typeSet::Set<float         >,typeSet::Set<float         >>());
  static_assert( typeSet::equal  <typeSet::Set<float,int     >,typeSet::Set<float,int     >>());
  static_assert(!typeSet::equal  <typeSet::Set<              >,typeSet::Set<float,int     >>());
  static_assert(!typeSet::equal  <typeSet::Set<float         >,typeSet::Set<float,int     >>());
  static_assert( typeSet::equal  <typeSet::Set<int,float     >,typeSet::Set<float,int     >>());
  static_assert(std::is_same_v<typeSet::Insert    <float                  ,typeSet::Set<                         >>,typeSet::Set<float                >>);
  static_assert(std::is_same_v<typeSet::Insert    <float                  ,typeSet::Set<int                      >>,typeSet::Set<int,float            >>);
  static_assert(std::is_same_v<typeSet::Insert    <float                  ,typeSet::Set<float,int                >>,typeSet::Set<float,int            >>);
  static_assert(std::is_same_v<typeSet::Insert    <int                    ,typeSet::Set<float,int                >>,typeSet::Set<float,int            >>);
  static_assert(std::is_same_v<typeSet::Union     <typeSet::Set<              >,typeSet::Set<                         >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Union     <typeSet::Set<              >,typeSet::Set<int                      >>,typeSet::Set<int                  >>);
  static_assert(std::is_same_v<typeSet::Union     <typeSet::Set<float         >,typeSet::Set<                         >>,typeSet::Set<float                >>);
  static_assert(std::is_same_v<typeSet::Union     <typeSet::Set<              >,typeSet::Set<int,float                >>,typeSet::Set<int,float            >>);
  static_assert(std::is_same_v<typeSet::Union     <typeSet::Set<float         >,typeSet::Set<int                      >>,typeSet::Set<float,int            >>);
  static_assert(std::is_same_v<typeSet::Union     <typeSet::Set<float,double  >,typeSet::Set<                         >>,typeSet::Set<float,double         >>);
  static_assert(std::is_same_v<typeSet::Union     <typeSet::Set<float,double  >,typeSet::Set<int,char                 >>,typeSet::Set<float,double,int,char>>);
  static_assert(std::is_same_v<typeSet::Intersect <typeSet::Set<              >,typeSet::Set<                         >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Intersect <typeSet::Set<float         >,typeSet::Set<                         >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Intersect <typeSet::Set<              >,typeSet::Set<float                    >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Intersect <typeSet::Set<float         >,typeSet::Set<float                    >>,typeSet::Set<float                >>);
  static_assert(std::is_same_v<typeSet::Intersect <typeSet::Set<float,int     >,typeSet::Set<float                    >>,typeSet::Set<float                >>);
  static_assert(std::is_same_v<typeSet::Intersect <typeSet::Set<float,int     >,typeSet::Set<float,int                >>,typeSet::Set<float,int            >>);
  static_assert(std::is_same_v<typeSet::Intersect <typeSet::Set<float,int,char>,typeSet::Set<double,int16_t,char,float>>,typeSet::Set<char,float           >>);
  static_assert(std::is_same_v<typeSet::Remove    <float                  ,typeSet::Set<                         >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Remove    <float                  ,typeSet::Set<int                      >>,typeSet::Set<int                  >>);
  static_assert(std::is_same_v<typeSet::Remove    <float                  ,typeSet::Set<float,int                >>,typeSet::Set<int                  >>);
  static_assert(std::is_same_v<typeSet::Remove    <int                    ,typeSet::Set<float,int                >>,typeSet::Set<float                >>);
  static_assert(std::is_same_v<typeSet::Difference<typeSet::Set<>              ,typeSet::Set<                         >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Difference<typeSet::Set<>              ,typeSet::Set<int                      >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Difference<typeSet::Set<>              ,typeSet::Set<int,float                >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Difference<typeSet::Set<int>           ,typeSet::Set<                         >>,typeSet::Set<int                  >>);
  static_assert(std::is_same_v<typeSet::Difference<typeSet::Set<int>           ,typeSet::Set<int                      >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Difference<typeSet::Set<int>           ,typeSet::Set<int,float                >>,typeSet::Set<                     >>);
  static_assert(std::is_same_v<typeSet::Difference<typeSet::Set<int,float>     ,typeSet::Set<                         >>,typeSet::Set<int,float                >>);
  static_assert(std::is_same_v<typeSet::Difference<typeSet::Set<int,float>     ,typeSet::Set<int                      >>,typeSet::Set<float                >>);
  static_assert(std::is_same_v<typeSet::Difference<typeSet::Set<int,float>     ,typeSet::Set<int,float                >>,typeSet::Set<                     >>);
  return 0; 
}
